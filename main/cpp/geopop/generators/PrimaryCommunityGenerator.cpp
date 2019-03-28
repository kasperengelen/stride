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
 *  Copyright 2019, Jan Broeckhove.
 */

#include "PrimaryCommunityGenerator.h"

#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/Location.h"
#include "pop/Population.h"
#include "util/Assert.h"
#include "util/RnMan.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace geopop {

using namespace std;
using namespace stride;
using namespace stride::ContactType;

void PrimaryCommunityGenerator::Apply(GeoGrid& geoGrid, const GeoGridConfig& geoGridConfig)
{
        // 1. calculate number of communities, each community has average 2000 persons
        // 2. assign communities to a location using a discrete distribution reflecting the relative number of
        //    people at that location

        const auto popCount       = geoGridConfig.input.pop_size;
        const auto communitySize  = geoGridConfig.pools.primary_community_size;
        const auto communityCount = static_cast<unsigned int>(ceil(popCount / static_cast<double>(communitySize)));

        vector<double> weights;
        for (const auto& loc : geoGrid) {
                const auto weight = static_cast<double>(loc->GetPopCount()) / static_cast<double>(popCount);
                AssertThrow(weight >= 0 && weight <= 1 && !std::isnan(weight),
                            "CommunityGenerator> Invalid weight: " + to_string(weight), m_logger);
                weights.push_back(weight);
        }

        if (weights.empty()) {
                // trng can't handle empty vectors
                return;
        }

        const auto dist = m_rn_man.GetDiscreteGenerator(weights, 0U);
        auto       pop  = geoGrid.GetPopulation();

        for (auto i = 0U; i < communityCount; i++) {
                const auto loc = geoGrid[dist()];
                AddPools(*loc, pop, geoGridConfig.pools.pools_per_primary_community);
        }
}

void PrimaryCommunityGenerator::AddPools(Location& loc, Population* pop, unsigned int number)
{
        auto& poolSys = pop->RefPoolSys();
        for (auto i = 0U; i < number; ++i) {
                const auto p = poolSys.CreateContactPool(Id::PrimaryCommunity);
                loc.RegisterPool<Id::PrimaryCommunity>(p);
        }
}

} // namespace geopop