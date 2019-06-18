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
#include "geopop/SimLocation.h"
#include "util/Assert.h"

#include <geopop/GeoGridConfig.h>
#include <map>
#include <utility>

namespace geopop {

using namespace std;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::AgeBrackets;
using namespace util;

struct WorkplaceInfo
{
        double       weight1;
        double       weight2;
        double       weight3;
        unsigned int target;
        unsigned int max_size;
};

/**
 * Initialize weights and size targets used for weighted drawing of workplace pools.
 * The weights are calculated using the defined workplace size distribution ratio's, sizes
 * and the amount of pools for each size.
 * A target size is used as a soft cap, after which the weight is lowered and thus that pool will get less people
 * assigned afterwards. The max_size represents a hard cap for all but the biggest defined workplace size.
 */
void init_workplace_weights(GeoGrid& geoGrid, const vector<double>& workplaceRatios,
                            const std::vector<std::pair<unsigned int, unsigned int>>& workplaceSizes,
                            stride::util::RnMan& rn_man, map<unsigned int, WorkplaceInfo>& workplaceInfo)
{
        unsigned int max_size_workplace = 0;
        if (not workplaceSizes.empty()) {
                max_size_workplace = workplaceSizes[0].second;
        }
        for (auto& size : workplaceSizes) {
                if (size.second > max_size_workplace)
                        max_size_workplace = size.second;
        }

        for (const auto& loc : geoGrid) {
                if (loc->GetPopCount() == 0) {
                        continue;
                }

                auto& pools = loc->RefPools(Id::Workplace);

                unsigned int amount_pools = pools.size();
                unsigned int pools_left   = amount_pools;

                unsigned int pool_counter = 0;

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
                        double weight1;
                        // Weight when target reached
                        double weight2;
                        double weight3;

                        if (w == 1) {
                                weight1 = 1;
                                weight2 = 1;
                                weight3 = 1;
                        } else {
                                // Starting weight for current size
                                weight1 = static_cast<double>(1 - w);

                                // Weight when target reached
                                weight2 = weight1 / pools_current_size;

                                if (max_size == max_size_workplace)
                                        weight3 = weight2 / pools_current_size;
                                else
                                        weight3 = 0.0;
                        }

                        // For every workplace generate a target size
                        auto target_draw = rn_man.GetUniformIntGenerator(min_size, max_size, 0U);
                        for (unsigned int i = 0; i < pools_current_size; i++) {
                                auto target = target_draw();

                                WorkplaceInfo wInfo{};

                                wInfo.weight1  = weight1;
                                wInfo.weight2  = weight2;
                                wInfo.weight3  = weight3;
                                wInfo.target   = target;
                                wInfo.max_size = max_size;

                                auto poolID = pools[pool_counter++]->GetId();

                                workplaceInfo[poolID] = wInfo;
                        }

                        pools_left -= pools_current_size;
                }
        }
}

/**
 * Gather the weights for the given workplaces while accounting for the current of each workplace.
 */
void get_weights(const map<unsigned int, WorkplaceInfo>& workplaceInfo, const vector<ContactPool*>& workplaces,
                 vector<double>& weights)
{
        bool all_zero = true;
        for (auto workplace : workplaces) {
                double       weight;
                unsigned int wSize = workplace->size();

                auto wInfo = workplaceInfo.at(workplace->GetId());

                if (wSize < wInfo.target) {
                        weight   = wInfo.weight1;
                        all_zero = false;
                } else if (wSize < wInfo.max_size) {
                        weight   = wInfo.weight2;
                        all_zero = false;
                } else {
                        weight = 0.0;
                }

                weights.emplace_back(weight);
        }

        if (all_zero) {
                for (int i = 0; i < (int)workplaces.size(); i++) {
                        weights[i] = workplaceInfo.at(workplaces[i]->GetId()).weight3;
                }
        }
}

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
        vector<SimLocation*>    commuteLocations{};

        const auto participWorkplace    = geoGridConfig.param.particpation_workplace;
        const auto popCollege           = geoGridConfig.info.popcount_college;
        const auto popWorkplace         = geoGridConfig.info.popcount_workplace;
        const auto fracCollegeCommute   = geoGridConfig.param.fraction_college_commuters;
        const auto fracWorkplaceCommute = geoGridConfig.param.fraction_workplace_commuters;
        const auto workplaceRatios      = geoGridConfig.workplaceSD.ratios;
        const auto workplaceSizes       = geoGridConfig.workplaceSD.sizes;
        auto&      rn_man               = m_rn_man;

        // Determine target workplace sizes to match workplace size and ratio information
        map<unsigned int, WorkplaceInfo> workplaceInfo;

        // Commuting fraction in student population estimated to obtain commuting fraction in
        // workplace population from commute data.
        double fracCommuteStudents = 0.0;
        if (static_cast<bool>(fracWorkplaceCommute) && popWorkplace) {
                fracCommuteStudents = (popCollege * fracCollegeCommute) / (popWorkplace * fracWorkplaceCommute);
        }

        init_workplace_weights(geoGrid, workplaceRatios, workplaceSizes, rn_man, workplaceInfo);

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
                for (const pair<SimLocation*, double>& commute : loc->CRefOutgoingCommutes()) {
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

                // --------------------------------------------------------------------------------
                // Get weights for the NearbyWorkspacePools used for weighted drawing of pools
                // --------------------------------------------------------------------------------
                vector<double> workplaceWeights;
                get_weights(workplaceInfo, nearbyWp, workplaceWeights);

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
                                                // this person commutes to the SimLocation and in particular to Pool
                                                // --------------------------------------------------------------
                                                auto& pools = commuteLocations[genCommute()]->RefPools(Id::Workplace);

                                                // Get weights for the NearbyWorkspacePools used for weighted drawing of
                                                // pools
                                                vector<double>       wWeights;
                                                vector<ContactPool*> vector_pools;
                                                vector_pools.insert(vector_pools.end(), pools.begin(), pools.end());
                                                get_weights(workplaceInfo, vector_pools, wWeights);

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
                                                if (nearbyWp[idraw]->size() ==
                                                    workplaceInfo[nearbyWp[idraw]->GetId()].target) {
                                                        workplaceWeights[idraw] =
                                                            workplaceInfo[nearbyWp[idraw]->GetId()].weight2;
                                                        genDiscreteNonCommute =
                                                            m_rn_man.GetDiscreteGenerator(workplaceWeights, 0U);
                                                } else if (nearbyWp[idraw]->size() ==
                                                           workplaceInfo[nearbyWp[idraw]->GetId()].target) {
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
