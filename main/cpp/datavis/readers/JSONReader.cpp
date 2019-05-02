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

#include "JSONReader.h"

#include "datavis/model/Locality.h"

namespace stride {
namespace datavis {

void JSONReader::ReadIntoModel(Model& datamodel) const
{
	// clear model
	datamodel.ClearTimesteps();

	// read data from JSON
	nlohmann::json js;
	*(this->GetInStream()) >> js;

	// add timesteps
	for(const auto& timestep_data : js.at("Timesteps"))
	{
		Timestep timestep{};

		for(const auto& locality_data : timestep_data)
		{
			const Locality locality = this->ReadLocality(locality_data);
			timestep.AddLocality(locality);
		}

		datamodel.AddTimestep(timestep);
	}

}

const Locality JSONReader::ReadLocality(const nlohmann::json& localityData) const
{
	// coord
	const std::vector<double> coord_vec = localityData.at("coord");
	const geopop::Coordinate coord      = {coord_vec.at(0), coord_vec.at(1)};
	const unsigned int tot_pop          = localityData.at("totalPop");

	// name
	const std::string name = localityData.at("name");

	ContactPoolStat household;
	ContactPoolStat k12_school;
	ContactPoolStat college = this->ReadPopCategory(localityData.at("collegePop"));
	ContactPoolStat workplace;
	ContactPoolStat primary_community;
	ContactPoolStat secondary_community;
	ContactPoolStat daycare = this->ReadPopCategory(localityData.at("daycarePop"));
	ContactPoolStat preschool;

	return Locality(name, coord, tot_pop, household, k12_school, college, workplace, primary_community, secondary_community, daycare, preschool);
}

const ContactPoolStat JSONReader::ReadPopCategory(const nlohmann::json& popCatData) const
{
	std::map<HealthStatus, unsigned int> health_map{};

	ContactPoolStat retval;

	retval.susceptible      = popCatData.at("susceptible");
	retval.exposed          = popCatData.at("exposed");
	retval.infectious       = popCatData.at("infectious");
	retval.symptomatic      = popCatData.at("symptomatic");
	retval.infect_and_sympt = popCatData.at("infectAndSympt");
	retval.recovered        = popCatData.at("recovered");
	retval.immune           = popCatData.at("immune");

	retval.total = retval.susceptible + retval.exposed + retval.infectious + retval.symptomatic
							  + retval.infect_and_sympt + retval.recovered + retval.immune;

	return retval;
}

}
}
