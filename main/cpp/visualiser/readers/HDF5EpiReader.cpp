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
 * Implementation file for the HDF5Reader class.
 */

#include "HDF5EpiReader.h"

#include "visualiser/model/PopData.h"
#include "visualiser/readers/EpiReaderException.h"
#include "contact/ContactType.h"

#include <H5Cpp.h>

namespace stride {
namespace visualiser {

const PopSection ReadPopSection(const H5::Group& location, const ContactType::Id& poolType);

void HDF5EpiReader::ReadIntoModel(Model& datamodel) const
{
    try {
        H5::Exception::dontPrint();
        const H5::H5File& file{this->GetPath(), H5F_ACC_RDONLY};

        std::vector<std::vector<Locality>> timesteps;

        for(int timestep_nr = 0; timestep_nr < file.getNumObjs(); timestep_nr++)
        {
        	std::vector<Locality> locations{};

        	const std::string timestep_label = std::to_string(timestep_nr);

        	// check if group is present
        	// 	if not: HDF5 file contains 75 groups but timestep #5 is not present. Timesteps 0-74 expected to be present.

        	const H5::Group& timestep = file.openGroup(timestep_label);

        	for(int loc_nr = 0; loc_nr < timestep.getNumObjs(); loc_nr++)
        	{
        		const H5::Group& location = timestep.openGroup("loc" + std::to_string(loc_nr));

        		// get name
        		// SOURCE: https://support.hdfgroup.org/ftp/HDF5/examples/misc-examples/stratt.cpp
        		const H5::Attribute& name_attr = location.openAttribute("name");
        		H5::StrType str_type(H5::PredType::C_S1, 256);
        		std::string name ("");
        		name_attr.read(str_type, name);

        		// get coordinate
        		const H5::Attribute& coord_attr = location.openAttribute("coordinate");
        		double coordinate[2];
        		location.openAttribute("coordinate").read(H5::PredType::NATIVE_DOUBLE, coordinate);
        		const geopop::Coordinate coord = {coordinate[0], coordinate[1]};

        		// get pools
        		const PopSection total     = ReadPopSection(location, ContactType::Id::Household);
        		const PopSection household = ReadPopSection(location, ContactType::Id::Household);
        		const PopSection k12school = ReadPopSection(location, ContactType::Id::K12School);
        		const PopSection college   = ReadPopSection(location, ContactType::Id::College);
        		const PopSection workplace = ReadPopSection(location, ContactType::Id::Workplace);
        		const PopSection prim_com  = ReadPopSection(location, ContactType::Id::PrimaryCommunity);
        		const PopSection sec_com   = ReadPopSection(location, ContactType::Id::SecondaryCommunity);
        		const PopSection daycare   = ReadPopSection(location, ContactType::Id::Daycare);
        		const PopSection preschool = ReadPopSection(location, ContactType::Id::PreSchool);

                const PopData population = {
                		total,
        				household,
						k12school,
        				college,
        				workplace,
        				prim_com,
        				sec_com,
        				daycare,
        				preschool
                };

                locations.push_back(Locality{name, coord, population});
        	}

        	// add timestep
        	timesteps.push_back(locations);
        }

        datamodel.SetTimesteps(timesteps);

    } catch (const std::exception& e) {
    	throw EpiReaderException(e.what());
    }
}

const PopSection ReadPopSection(const H5::Group& location, const ContactType::Id& poolType)
{
	const H5::DataSet& pop_household = location.openDataSet(ContactType::ToString(poolType));

	H5::CompType comp_type{sizeof(PopSection)};

	comp_type.insertMember("population",  HOFFSET(PopSection, pop),         H5::PredType::NATIVE_UINT);

	comp_type.insertMember("immune",      HOFFSET(PopSection, immune),      H5::PredType::NATIVE_DOUBLE);
	comp_type.insertMember("infected",    HOFFSET(PopSection, infected),    H5::PredType::NATIVE_DOUBLE);
	comp_type.insertMember("infectious",  HOFFSET(PopSection, infectious),  H5::PredType::NATIVE_DOUBLE);

	comp_type.insertMember("recovered",   HOFFSET(PopSection, recovered),   H5::PredType::NATIVE_DOUBLE);
	comp_type.insertMember("susceptible", HOFFSET(PopSection, susceptible), H5::PredType::NATIVE_DOUBLE);
	comp_type.insertMember("symptomatic", HOFFSET(PopSection, symptomatic), H5::PredType::NATIVE_DOUBLE);

	PopSection pop_section;

	pop_household.read(&pop_section, comp_type);

	return pop_section;
}

} // namespace visualiser
} // namespace stride

