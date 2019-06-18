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
#include <geopop/GeoGridConfig.h>

#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/SimLocation.h"
#include "pop/Population.h"
#include "util/Exception.h"
#include <map>

namespace geopop {

using namespace std;
using namespace stride::ContactType;

template <>
void Populator<stride::ContactType::Id::Household>::Apply(GeoGrid& geoGrid, const GeoGridConfig& geoGridConfig)
{
        m_logger->trace("Starting to populate Households");

        auto                                    person_id = 0U;
        map<unsigned int, std::function<int()>> hh_dist;
        for (const auto& id : geoGridConfig.refHH.ages) {
                hh_dist.insert(pair<unsigned int, std::function<int()>>(
                    id.first, m_rn_man.GetUniformIntGenerator(
                                  0, static_cast<int>(geoGridConfig.refHH.ages.at(id.first).size()), 0U)));
        }
        auto pop = geoGrid.GetPopulation();

        for (const shared_ptr<SimLocation>& loc : geoGrid) {
                for (auto& pool : loc->RefPools(Id::Household)) {
                        auto id = 0U;
                        if (geoGridConfig.refHH.multiHH) {
                                if (geoGridConfig.refHH.ages.count(loc->GetID())) {
                                        id = loc->GetID();
                                } else if (geoGridConfig.refHH.ages.count(loc->GetProvince())) {
                                        id = loc->GetProvince();
                                }
                        }
                        if (geoGridConfig.refHH.ages.count(id) == 0) {
                                throw stride::util::Exception("HouseholdPopulator::Apply> id's not found: " +
                                to_string(loc->GetID()) + ", " + to_string(loc->GetProvince()) + " or 0 (default).");
                        }
                        const auto hDraw = static_cast<unsigned int>(hh_dist[id]());
                        for (const auto& age : geoGridConfig.refHH.ages.at(id)[hDraw]) {
                                const auto p = pop->CreatePerson(person_id++, age, pool->GetId(), 0, 0, 0, 0, 0, 0, 0);
                                pool->AddMember(p);
                        }
                }
        }

        m_logger->trace("Done populating Households");
}

} // namespace geopop
