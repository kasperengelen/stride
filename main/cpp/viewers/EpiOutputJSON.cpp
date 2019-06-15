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
 *  Copyright 2019, ACED
 */

/**
 * @file
 * Implementation of the EpiOutputJSON class.
 */

#include "EpiOutputJSON.h"

#include "viewers/LocationPopStats.h"

#include "contact/ContactType.h"
#include "pop/Population.h"
#include "geopop/Location.h"
#include "util/FileSys.h"

#include <iomanip>

namespace stride {
namespace output {

using json = nlohmann::json;

EpiOutputJSON::EpiOutputJSON(const std::string &output_prefix) : EpiOutputFile(), m_data() {
    Initialize(output_prefix);
}

void EpiOutputJSON::Initialize(const std::string &output_prefix) {
    const auto p = util::FileSys::BuildPath(output_prefix, "EpiOutput.json");
    m_fstream.open(p.c_str(), std::ios::trunc | std::ios::out);
    m_data["Timesteps"] = json::array();
}

void EpiOutputJSON::Update(std::shared_ptr<const Population> population) {
        // Create timestep info
        json timestep = json::array();

        const geopop::GeoGrid &geogrid = population->CRefGeoGrid();
        for (const auto &location: geogrid) {
                const LocationPopData popdata{*location};

                json loc_json = json::object();
                loc_json["name"] = location->GetName();

                loc_json["coordinates"] = json::array();
                const auto coordinate = location->GetCoordinate();
                loc_json["coordinates"].push_back(coordinate.get<0>());
                loc_json["coordinates"].push_back(coordinate.get<1>());

                for (const auto &pool_type: ContactType::IdList) {
                        const PoolStats &pool_stats = popdata.GetPool(pool_type);

                        json pool_json = json::object();

                        pool_json["population"] = pool_stats.population;
                        pool_json["immune"] = pool_stats.immune;
                        pool_json["infected"] = pool_stats.infected;
                        pool_json["infectious"] = pool_stats.infectious;
                        pool_json["recovered"] = pool_stats.recovered;
                        pool_json["susceptible"] = pool_stats.susceptible;
                        pool_json["symptomatic"] = pool_stats.symptomatic;

                        loc_json[ContactType::ToString(pool_type)] = pool_json;
                }

                timestep.push_back(loc_json);
        }
        m_data["Timesteps"].push_back(timestep);
}

void EpiOutputJSON::Finish() {
    m_fstream.seekp(0); // Reset write position
    m_fstream << std::setw(4) << m_data << std::endl;
}

}
}