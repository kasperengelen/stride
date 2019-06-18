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
#include "contact/ContactType.h"
#include "visualiser/model/Locality.h"
#include "visualiser/model/PopData.h"
#include "visualiser/readers/EpiReaderException.h"

namespace stride {
namespace visualiser {

const PopSection ReadPopSection(const nlohmann::json& localityData, const ContactType::Id& poolType);
const Locality   ReadLocality(const nlohmann::json& localityData);

void JSONEpiReader::ReadIntoModel(Model& datamodel) const
{

        try {

                std::vector<std::vector<Locality>> timesteps;

                // read data from JSON
                nlohmann::json js;
                *(this->GetInStream()) >> js;

                // add timesteps
                for (const auto& timestep_data : js.at("Timesteps")) {
                        std::vector<Locality> timestep{};

                        for (const auto& locality_data : timestep_data) {
                                const Locality locality = ReadLocality(locality_data);
                                timestep.push_back(locality);
                        }

                        timesteps.push_back(timestep);
                }

                datamodel.SetTimesteps(timesteps);

        } catch (const std::exception& e) {
                // rethrow as our own exception.
                throw EpiReaderException(e.what());
        }
}

const Locality ReadLocality(const nlohmann::json& localityData)
{
        // coord
        const std::vector<double> coord_vec = localityData.at("coordinates");     // long, lat
        const geopop::Coordinate  coord     = {coord_vec.at(0), coord_vec.at(1)}; // long, lat

        // name
        const std::string name = localityData.at("name");

        // retrieve populations
        const PopSection total = ReadPopSection(
            localityData, ContactType::Id::Household); // Note: households contain the total, so we just copy it
        const PopSection household  = ReadPopSection(localityData, ContactType::Id::Household);
        const PopSection k12_school = ReadPopSection(localityData, ContactType::Id::K12School);
        const PopSection college    = ReadPopSection(localityData, ContactType::Id::College);
        const PopSection workplace  = ReadPopSection(localityData, ContactType::Id::Workplace);
        const PopSection prim_com   = ReadPopSection(localityData, ContactType::Id::PrimaryCommunity);
        const PopSection sec_com    = ReadPopSection(localityData, ContactType::Id::SecondaryCommunity);
        const PopSection daycare    = ReadPopSection(localityData, ContactType::Id::Daycare);
        const PopSection preschool  = ReadPopSection(localityData, ContactType::Id::PreSchool);

        const PopData population = {total,    household, k12_school, college,  workplace,
                                    prim_com, sec_com,   daycare,    preschool};

        return Locality(name, coord, population);
}

const PopSection ReadPopSection(const nlohmann::json& localityData, const ContactType::Id& poolType)
{
        const nlohmann::json& popsection_data = localityData.at(ContactType::ToString(poolType));

        PopSection retval;

        retval.pop         = popsection_data.at("population");
        retval.immune      = popsection_data.at("immune");
        retval.infected    = popsection_data.at("infected");
        retval.infectious  = popsection_data.at("infectious");
        retval.recovered   = popsection_data.at("recovered");
        retval.susceptible = popsection_data.at("susceptible");
        retval.symptomatic = popsection_data.at("symptomatic");

        return retval;
}

} // namespace visualiser
} // namespace stride
