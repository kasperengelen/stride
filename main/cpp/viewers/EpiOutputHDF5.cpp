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
 * Implementation of the EpiOutputHDF5 class.
 */

#include "EpiOutputHDF5.h"

#include "contact/ContactType.h"
#include "pop/Population.h"
#include "geopop/Location.h"
#include "util/FileSys.h"

namespace stride {
namespace output {

void WriteCoordinate(H5::Group& loc, const geopop::Coordinate& coordinate)
{
    hsize_t   dim[2] = {1, 2};
    H5::DataSpace dataspace(2, dim);
    H5::Attribute attribute         = loc.createAttribute("coordinate", H5::PredType::NATIVE_DOUBLE, dataspace);
    double    attribute_data[2] = {boost::geometry::get<0>(coordinate), boost::geometry::get<1>(coordinate)};
    attribute.write(H5::PredType::NATIVE_DOUBLE, attribute_data);
}

EpiOutputHDF5::EpiOutputHDF5(const std::string& output_dir) : EpiOutputFile(), m_data(), m_timestep(0)
{
    Initialize(output_dir);
}

void EpiOutputHDF5::Initialize(const std::string& output_prefix)
{
    const auto p     = util::FileSys::BuildPath(output_prefix, "EpiOutput.h5");
    H5::Exception::dontPrint();
    m_data = H5::H5File(p.c_str(), H5F_ACC_TRUNC);
}

// TODO use common code.

void EpiOutputHDF5::Update(std::shared_ptr<const Population> population)
{
    // data layout
    H5::CompType comp_type(sizeof(PoolTypeData));

    comp_type.insertMember("population",  HOFFSET(PoolTypeData, population),  H5::PredType::NATIVE_UINT);
    comp_type.insertMember("immune",      HOFFSET(PoolTypeData, immune),      H5::PredType::NATIVE_DOUBLE);
    comp_type.insertMember("infected",    HOFFSET(PoolTypeData, infected),    H5::PredType::NATIVE_DOUBLE);
    comp_type.insertMember("infectious",  HOFFSET(PoolTypeData, infectious),  H5::PredType::NATIVE_DOUBLE);
    comp_type.insertMember("recovered",   HOFFSET(PoolTypeData, recovered),   H5::PredType::NATIVE_DOUBLE);
    comp_type.insertMember("susceptible", HOFFSET(PoolTypeData, susceptible), H5::PredType::NATIVE_DOUBLE);
    comp_type.insertMember("symptomatic", HOFFSET(PoolTypeData, symptomatic), H5::PredType::NATIVE_DOUBLE);

    // Create timestep info
    const std::string timestep_name = std::to_string(m_timestep);

    H5::Group timestep = m_data.createGroup(timestep_name);
    m_timestep++;

    int loc_ctr = 0;
    const geopop::GeoGrid& geogrid = population->CRefGeoGrid();
    for(const auto& location : geogrid) {
        try {
            // init group that contains location data
            const std::string loc_name = "loc" + std::to_string(loc_ctr++);
            H5::Group loc_group = timestep.createGroup(loc_name);

            // add location name
            H5::DataSpace attr_ds = H5::DataSpace(H5S_SCALAR);
            H5::StrType str_dt(H5::PredType::C_S1, 256);
            H5::Attribute name_attr = loc_group.createAttribute("name", str_dt, attr_ds);
            name_attr.write(str_dt, location->GetName());

            // add coordinate
            WriteCoordinate(loc_group, location->GetCoordinate());

            // add population data
            const LocationPopData popdata = ProcessPopulation(*location);
            for(const auto& pool_type : ContactType::IdList) {
                const PoolTypeData& pool_stats = popdata.GetPool(pool_type);

                hsize_t   dim = 1;
                H5::DataSpace pool_ds(1, &dim);
                H5::DataSet pool = loc_group.createDataSet(ContactType::ToString(pool_type), comp_type, pool_ds);
                pool.write(&pool_stats, comp_type);
            }
        } catch (...) {
            // Ignore locations that can't be written
            continue;
        }
    }
}

void EpiOutputHDF5::Finish()
{
    m_data.close();
}

}
}