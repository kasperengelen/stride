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
 *  Copyright 2018, Jan Broeckhove and Bistromatics group.
 */

#include "WorkplaceGenerator.h"

#include "gengeopop/Workplace.h"

#include <trng/discrete_dist.hpp>

namespace gengeopop {

using namespace std;

void WorkplaceGenerator::Apply(shared_ptr<GeoGrid> geoGrid, GeoGridConfig& geoGridConfig)
{
        // 1. active people count and the commuting people count are given
        // 2. count the workplaces, each workplace has an average of 20 employees
        // 3. count the working people at each location = #residents + #incoming commuters - #outgoing commuters
        // 4. use the last information for the distribution
        // 5. assign each workplace to a location

        const auto EmployeeCount = geoGridConfig.calculated.popcount_1865_active;
        const auto WorkplacesCount =
            static_cast<unsigned int>(ceil(EmployeeCount / geoGridConfig.constants.mean_workplace_school_size));

        // = for each location #residents + #incoming commuting people - #outgoing commuting people
        vector<double> weights;
        for (const auto& loc : *geoGrid) {
                const double ActivePeopleCount =
                    (loc->GetPopulation() +
                     loc->IncomingCommutingPeople(geoGridConfig.input.fraction_active_commutingPeople) -
                     loc->OutGoingCommutingPeople(geoGridConfig.input.fraction_active_commutingPeople) *
                         geoGridConfig.input.fraction_1865_active);

                const double weight = ActivePeopleCount / EmployeeCount;
                CheckWeight("WorkplaceGenerator", weight);
                weights.push_back(weight);
        }

        if (weights.empty()) {
                // trng can't handle empty vectors
                return;
        }

        const auto dist = m_rnManager[0].variate_generator(trng::discrete_dist(weights.begin(), weights.end()));

        for (auto i = 0U; i < WorkplacesCount; i++) {
                const auto loc = (*geoGrid)[dist()];
                const auto w   = make_shared<Workplace>(geoGridConfig.generated.contact_center_count++);
                w->Fill(geoGrid);
                loc->AddContactCenter(w);
        }
}

} // namespace gengeopop