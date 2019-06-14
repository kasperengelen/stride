//
// Created by kasper on 14/06/19.
//

#include "EpiOutputJSON.h"

namespace stride {
namespace output {

EpiOutputJSON::EpiOutputJSON(const std::string &output_prefix) : EpiOutputFile(), m_data() {
    Initialize(output_prefix);
}

void EpiOutputJSON::Initialize(const std::string &output_prefix) {
    const auto p = FileSys::BuildPath(output_prefix, "EpiOutput.json");
    m_fstream.open(p.c_str(), std::ios::trunc | std::ios::out);
    m_data["Timesteps"] = json::array();
}

void EpiOutputJSON::Update(std::shared_ptr<const Population> population) {
        // Create timestep info
        json timestep = json::array();

        const geopop::GeoGrid &geogrid = population->CRefGeoGrid();
        for (const auto &location: geogrid) {
                const LocationPopData popdata = ProcessPopulation(*location);

                json loc_json = json::object();
                loc_json["name"] = location->GetName();

                loc_json["coordinates"] = json::array();
                const auto coordinate = location->GetCoordinate();
                loc_json["coordinates"].push_back(coordinate.get<0>());
                loc_json["coordinates"].push_back(coordinate.get<1>());

                for (const auto &pool_type: ContactType::IdList) {
                        const PoolTypeData &pool_stats = popdata.GetPool(pool_type);

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