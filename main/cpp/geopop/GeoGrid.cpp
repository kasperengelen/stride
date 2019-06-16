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

#include "GeoGrid.h"

#include "contact/ContactPool.h"
#include "geopop/Location.h"
#include "pop/Population.h"

#include <queue>
#include <stdexcept>
#include <utility>

namespace geopop {

using namespace std;
using stride::ContactPool;
using stride::ContactType::Id;

GeoGrid::GeoGrid(stride::Population* population) : m_population(population)
{}

vector<ContactPool*> GeoGrid::GetNearbyPools(Id id, const Location& start, double startRadius) const
{
        double               currentRadius = startRadius;
        vector<ContactPool*> pools;

        while (pools.empty()) {
                for (const auto* nearLoc : LocationsInRadius(start, currentRadius)) {
                        const auto& locPool = nearLoc->CRefPools(id);
                        pools.insert(pools.end(), locPool.begin(), locPool.end());
                }
                currentRadius *= 2;
                if (currentRadius == numeric_limits<double>::infinity()) {
                        break;
                }
        }
        return pools;
}

vector<Location*> GeoGrid::TopK(size_t k) const
{
        auto cmp = [](Location* rhs, Location* lhs) { return rhs->GetPopCount() > lhs->GetPopCount(); };

        priority_queue<Location*, vector<Location*>, decltype(cmp)> queue(cmp);
        for (const auto& loc : *this) {
                queue.push(loc.get());
                if (queue.size() > k) {
                        queue.pop();
                }
        }

        vector<Location*> topLocations;
        while (!queue.empty()) {
                auto loc = queue.top();
                topLocations.push_back(loc);
                queue.pop();
        }

        return topLocations;
}

} // namespace geopop
