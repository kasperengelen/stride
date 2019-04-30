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
	//*(this->GetInStream()) >> js;

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
	const geopop::Coordinate coord{coord_vec.at(0), coord_vec.at(1)};

	// name
	const std::string name = localityData.at("name");

	std::map<ContactType::Id, PopCategory> pop_categories{};

	pop_categories.insert({ContactType::Id::College, this->ReadPopCategory(localityData.at("collegePop"))});
	pop_categories.insert({ContactType::Id::Daycare, this->ReadPopCategory(localityData.at("daycarePop"))});

	// TODO more population categories

	return Locality(name, coord, pop_categories);
}

const PopCategory JSONReader::ReadPopCategory(const nlohmann::json& popCatData) const
{
	unsigned int popcount = popCatData.at("popCount");


	std::map<HealthStatus, unsigned int> health_map{};

	health_map.insert({HealthStatus::Susceptible,              popCatData.at("susceptible")});
	health_map.insert({HealthStatus::Exposed,                  popCatData.at("exposed")});
	health_map.insert({HealthStatus::Infectious,               popCatData.at("infectious")});
	health_map.insert({HealthStatus::Symptomatic,              popCatData.at("symptomatic")});
	health_map.insert({HealthStatus::InfectiousAndSymptomatic, popCatData.at("infectAndSympt")});
	health_map.insert({HealthStatus::Recovered,                popCatData.at("recovered")});
	health_map.insert({HealthStatus::Immune,                   popCatData.at("immune")});

	return PopCategory(popcount, health_map);
}

}
}
