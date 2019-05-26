/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, 2019, Jan Broeckhove and Bistromatics group.
 */

#include "Populator.h"

#include "contact/AgeBrackets.h"
#include "contact/ContactPool.h"
#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/Location.h"
#include "util/Assert.h"

#include <geopop/GeoGridConfig.h>
#include <utility>

namespace geopop {

using namespace std;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::AgeBrackets;
using namespace util;

template <>
void Populator<stride::ContactType::Id::Workplace>::Apply(GeoGrid& geoGrid, const GeoGridConfig& geoGridConfig)
{
        m_logger->trace("Starting to populate Workplaces");

        auto genCommute{function<int()>()};
        auto genUniformNonCommute{function<int()>()};
        auto genDiscreteNonCommute{function<int()>()};
        auto genWorkplaceDistr{function<int()>()};
        auto genWorkplaceComDistr{function<int()>()};

        vector<ContactPool*> nearbyWp{};
        vector<Location*>    commuteLocations{};

        const auto participWorkplace    = geoGridConfig.param.particpation_workplace;
        const auto popCollege           = geoGridConfig.info.popcount_college;
        const auto popWorkplace         = geoGridConfig.info.popcount_workplace;
        const auto fracCollegeCommute   = geoGridConfig.param.fraction_college_commuters;
        const auto fracWorkplaceCommute = geoGridConfig.param.fraction_workplace_commuters;
        const auto workplaceRatios      = geoGridConfig.workplaceSD.ratios;
        const auto workplaceSizes       = geoGridConfig.workplaceSD.sizes;
        auto&      rn_man               = m_rn_man;

        // Commuting fraction in student population estimated to obtain commuting fraction in
        // workplace population from commute data.
        double fracCommuteStudents = 0.0;
        if (static_cast<bool>(fracWorkplaceCommute) && popWorkplace) {
                fracCommuteStudents = (popCollege * fracCollegeCommute) / (popWorkplace * fracWorkplaceCommute);
        }

        // --------------------------------------------------------------------------------
        //  Data needed for workplace size distribution
        // --------------------------------------------------------------------------------

        // --------------------------------------------------------------------------------
        //  Cool function dude
        // --------------------------------------------------------------------------------

        auto get_weights = [&workplaceRatios, &workplaceSizes,
                            &rn_man](const vector<ContactPool*>& nearbyWp, vector<double>& weights,
                                     vector<double>&                           weights_target_reached,
                                     vector<pair<unsigned int, unsigned int>>& targets) {
                unsigned int amount_pools = nearbyWp.size();
                unsigned int pools_left   = amount_pools;

                // Iterate over each workplace size / ratio
                for (auto j = 0; j < (int)workplaceRatios.size(); j++) {
                        auto w = workplaceRatios[j];

                        // This should be expected amount of workplaces for the given size
                        auto pools_current_size = static_cast<unsigned int>(floor(w * amount_pools));
                        if (j == (int)workplaceRatios.size() - 1)
                                pools_current_size = pools_left;

                        if (pools_current_size == 0)
                                continue;

                        auto min_size = workplaceSizes[j].first;
                        auto max_size = workplaceSizes[j].second;

                        // Starting weight for current size
                        auto weight = static_cast<double>(1 - w);

                        // Weight when target reached
                        auto weight_target_reached = weight / pools_current_size;

                        // For every workplace generate a target size
                        auto target_draw = rn_man.GetUniformIntGenerator(min_size, max_size, 0U);
                        for (unsigned int i = 0; i < pools_current_size; i++) {
                                auto target = target_draw();

                                if (nearbyWp[weights.size()]->size() == max_size)
                                        weights.emplace_back(0);
                                else if ((int)nearbyWp[weights.size()]->size() >= target)
                                        weights.emplace_back(weight_target_reached);
                                else
                                        weights.emplace_back(weight);

                                weights_target_reached.emplace_back(weight_target_reached);
                                targets.emplace_back(make_pair(target, max_size));
                        }

                        pools_left -= pools_current_size;
                }
        };

        // --------------------------------------------------------------------------------
        // For every location, if populated ...
        // --------------------------------------------------------------------------------
        for (const auto& loc : geoGrid) {
                if (loc->GetPopCount() == 0) {
                        continue;
                }

                // --------------------------------------------------------------------------------
                // Find all Workplaces were employees from this location commute to
                // --------------------------------------------------------------------------------
                commuteLocations.clear();
                genCommute = function<int()>();

                vector<double> commutingWeights;
                for (const pair<Location*, double>& commute : loc->CRefOutgoingCommutes()) {
                        const auto& workplaces = commute.first->RefPools(Id::Workplace);
                        if (!workplaces.empty()) {
                                commuteLocations.push_back(commute.first);
                                const auto weight = commute.second - (commute.second * fracCommuteStudents);
                                commutingWeights.push_back(weight);
                                AssertThrow(weight >= 0.0 && weight <= 1.0 && !isnan(weight),
                                            "Invalid weight: " + to_string(weight), m_logger);
                        }
                }

                if (!commutingWeights.empty()) {
                        genCommute = m_rn_man.GetDiscreteGenerator(commutingWeights, 0U);
                }

                // --------------------------------------------------------------------------------
                // Set NearbyWorkspacePools and associated generators
                // --------------------------------------------------------------------------------
                nearbyWp             = geoGrid.GetNearbyPools(Id::Workplace, *loc);
                genUniformNonCommute = m_rn_man.GetUniformIntGenerator(0, static_cast<int>(nearbyWp.size()), 0U);

                vector<double>                           workplaceWeights;
                vector<double>                           workplaceWeightsTR;
                vector<pair<unsigned int, unsigned int>> workplaceTargets;
                get_weights(nearbyWp, workplaceWeights, workplaceWeightsTR, workplaceTargets);

                if (!workplaceWeights.empty()) {
                        genDiscreteNonCommute = m_rn_man.GetDiscreteGenerator(workplaceWeights, 0U);
                }
                // --------------------------------------------------------------------------------
                // For everyone of working age: decide between work or college (iff of College age)
                // --------------------------------------------------------------------------------
                for (auto& hhPool : loc->RefPools(Id::Household)) {
                        for (auto person : *hhPool) {

                                // NOTICE: logic below requires that CollegePopulator has already executed
                                // such that we can identify the college students.
                                // If this person is not in the age bracket for college/work/unemployed
                                // or if the perosn is in the age bracket but is a student we are done here.
                                if (!Workplace::HasAge(person->GetAge()) || (person->GetPoolId(Id::College) != 0)) {
                                        continue;
                                }

                                // We are dealing with a non-student person of the age bracket for work,
                                // flip coin to decide whether they are actually employed.
                                bool isActiveWorker = m_rn_man.MakeWeightedCoinFlip(participWorkplace);
                                if (isActiveWorker) {
                                        // ---------------------------------------------
                                        // this person is employed
                                        // ---------------------------------------------
                                        const auto isCommuter = m_rn_man.MakeWeightedCoinFlip(fracWorkplaceCommute);
                                        if (!commuteLocations.empty() && isCommuter) {
                                                // --------------------------------------------------------------
                                                // this person commutes to the Location and in particular to Pool
                                                // --------------------------------------------------------------
                                                auto& pools = commuteLocations[genCommute()]->RefPools(Id::Workplace);

                                                vector<double>                           wWeights;
                                                vector<double>                           wWeightsTR;
                                                vector<pair<unsigned int, unsigned int>> wTargets;
                                                vector<ContactPool*>                     vector_pools;
                                                vector_pools.insert(vector_pools.end(), pools.begin(), pools.end());
                                                get_weights(vector_pools, wWeights, wWeightsTR, wTargets);

                                                auto gen  = m_rn_man.GetDiscreteGenerator(wWeights, 0U);
                                                auto pool = pools[gen()];
                                                // so that's it
                                                pool->AddMember(person);
                                                person->SetPoolId(Id::Workplace, pool->GetId());
                                        } else {
                                                // ----------------------------
                                                // this person does not commute
                                                // ----------------------------
                                                const auto idraw = genDiscreteNonCommute();
                                                nearbyWp[idraw]->AddMember(person);
                                                person->SetPoolId(Id::Workplace, nearbyWp[idraw]->GetId());

                                                // update weight if target or max size reached
                                                if (nearbyWp[idraw]->size() == workplaceTargets[idraw].first) {
                                                        workplaceWeights[idraw] = workplaceWeightsTR[idraw];
                                                        genDiscreteNonCommute =
                                                            m_rn_man.GetDiscreteGenerator(workplaceWeights, 0U);
                                                } else if (nearbyWp[idraw]->size() == workplaceTargets[idraw].second) {
                                                        workplaceWeights[idraw] = 0.0;
                                                        genDiscreteNonCommute =
                                                            m_rn_man.GetDiscreteGenerator(workplaceWeights, 0U);
                                                }
                                        }
                                }
                        }
                }
        }
        m_logger->trace("Done populating Workplaces");
}

} // namespace geopop
