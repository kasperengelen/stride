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
 *  Copyright 2019, ACED.
 */

/**
 * @file
 * Implementation file for the JSONReader class.
 */

#include "JSONEpiReader.h"
#include "visualiser/model/PopData.h"
#include "visualiser/readers/EpiReaderException.h"
#include "contact/ContactType.h"



namespace stride {
namespace visualiser {

using geopop::PopStats;
using geopop::PoolStats;
using geopop::VisLocation;
using nlohmann::json;

const VisLocation ReadLocation(const json& locationData);
void ReadPoolIntoPopStats(PopStats& popStats, const json& locationData, const ContactType::Id& poolType);

void JSONEpiReader::ReadIntoModel(Model& datamodel) const
{

        try {

                std::vector<std::shared_ptr<VisGeoGrid>> timesteps;

                // read data from JSON
                json js;
                *(this->GetInStream()) >> js;

                // add timesteps
                for (const auto& timestep_data : js.at("Timesteps")) {
                        std::shared_ptr<VisGeoGrid> locations = std::make_shared<VisGeoGrid>();

                        for (const auto& locality_data : timestep_data) {
                                const VisLocation loc = ReadLocation(locality_data);
                                locations->AddLocation(std::make_shared<VisLocation>(loc));
                        }

                        locations->Finalize();
                        timesteps.push_back(locations);
                }

                datamodel.SetTimesteps(timesteps);

        } catch (const std::exception& e) {
                // rethrow as our own exception.
                throw EpiReaderException(e.what());
        }
}

const VisLocation ReadLocation(const json& locationData)
{
    // coord
    const std::vector<double> coord_vec = locationData.at("coordinates");     // long, lat
    const geopop::Coordinate  coord     = {coord_vec.at(0), coord_vec.at(1)}; // long, lat

    // name
    const std::string name = locationData.at("name");


    // population
    PopStats popstats;

    ReadPoolIntoPopStats(popstats, locationData, ContactType::Id::Household);
    ReadPoolIntoPopStats(popstats, locationData, ContactType::Id::K12School);
    ReadPoolIntoPopStats(popstats, locationData, ContactType::Id::College);
    ReadPoolIntoPopStats(popstats, locationData, ContactType::Id::Workplace);
    ReadPoolIntoPopStats(popstats, locationData, ContactType::Id::PrimaryCommunity);
    ReadPoolIntoPopStats(popstats, locationData, ContactType::Id::SecondaryCommunity);
    ReadPoolIntoPopStats(popstats, locationData, ContactType::Id::Daycare);
    ReadPoolIntoPopStats(popstats, locationData, ContactType::Id::PreSchool);

    return VisLocation{coord, name, popstats};
}

void ReadPoolIntoPopStats(PopStats& popStats, const json& locationData, const ContactType::Id& poolType)
{
    const nlohmann::json& popsection_data = locationData.at(ContactType::ToString(poolType));

    PoolStats poolstats;

    poolstats.population  = popsection_data.at("population");
    poolstats.immune      = popsection_data.at("immune");
    poolstats.infected    = popsection_data.at("infected");
    poolstats.infectious  = popsection_data.at("infectious");
    poolstats.recovered   = popsection_data.at("recovered");
    poolstats.susceptible = popsection_data.at("susceptible");
    poolstats.symptomatic = popsection_data.at("symptomatic");

    popStats.SetPool(poolType, poolstats);
}


} // namespace visualiser
} // namespace stride
