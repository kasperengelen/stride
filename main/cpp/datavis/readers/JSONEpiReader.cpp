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
#include "datavis/model/Locality.h"
#include "datavis/readers/EpiReaderException.h"

namespace stride {
namespace datavis {

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
                                const Locality locality = this->ReadLocality(locality_data);
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

const Locality JSONEpiReader::ReadLocality(const nlohmann::json& localityData) const
{
        // coord
        const std::vector<double> coord_vec = localityData.at("coordinates");     // long, lat
        const geopop::Coordinate  coord     = {coord_vec.at(0), coord_vec.at(1)}; // long, lat

        // name
        const std::string name = localityData.at("name");

        // retrieve populations
        const PopSection total      = this->ReadPopSection(localityData.at("Household")); // Note: households contain the total, so we just copy it
        const PopSection household  = this->ReadPopSection(localityData.at("Household"));
        const PopSection k12_school = this->ReadPopSection(localityData.at("K12School"));
        const PopSection college    = this->ReadPopSection(localityData.at("College"));
        const PopSection workplace  = this->ReadPopSection(localityData.at("Workplace"));
        const PopSection prim_com   = this->ReadPopSection(localityData.at("PrimaryCommunity"));
        const PopSection sec_com    = this->ReadPopSection(localityData.at("SecondaryCommunity"));
        const PopSection daycare    = this->ReadPopSection(localityData.at("Daycare"));
        const PopSection preschool  = this->ReadPopSection(localityData.at("PreSchool"));

        return Locality(name, coord, total, household, k12_school, college,
        				workplace, prim_com, sec_com, daycare, preschool);
}

const PopSection JSONEpiReader::ReadPopSection(const nlohmann::json& popCatData) const
{
        PopSection retval;

        retval.pop         = popCatData.at("population");
        retval.immune      = popCatData.at("immune");
        retval.infected    = popCatData.at("infected");
        retval.infectious  = popCatData.at("infectious");
        retval.recovered   = popCatData.at("recovered");
        retval.susceptible = popCatData.at("susceptible");
        retval.symptomatic = popCatData.at("symptomatic");

        return retval;
}

} // namespace datavis
} // namespace stride
