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
 * Implementation of the EpiOutputFile class.
 */

#include <iomanip>
#include <iostream>

#include "EpiOutputFile.h"
#include "contact/ContactType.h"
#include "pop/Population.h"
#include "util/FileSys.h"

namespace stride {
namespace output {

using namespace stride::util;
using namespace H5;
using json = nlohmann::json;

/**
 * Struct that contains information about the population that belongs to one type of contact pool.
 */
struct PoolTypeData
{
        unsigned int population  = 0;
        double       immune      = 0;
        double       infected    = 0;
        double       infectious  = 0;
        double       recovered   = 0;
        double       susceptible = 0;
        double       symptomatic = 0;
};

/**
 * Struct that contains information about the population inside a location.
 */
struct LocationPopData
{
private:
        PoolTypeData household;
        PoolTypeData k12_school;
        PoolTypeData college;
        PoolTypeData workplace;
        PoolTypeData prim_com;
        PoolTypeData sec_com;
        PoolTypeData daycare;
        PoolTypeData preschool;

public:
        LocationPopData() : household{}, k12_school{}, college{}, workplace{}, prim_com{}, sec_com{}, daycare{}, preschool{}
        {}

        const PoolTypeData& GetPool(const ContactType::Id& poolId) const
        {
                switch(poolId)
                {
                        case ContactType::Id::Household:
                                return this->household;
                        case ContactType::Id::K12School:
                                return this->k12_school;
                        case ContactType::Id::College:
                                return this->college;
                        case ContactType::Id::Workplace:
                                return this->workplace;
                        case ContactType::Id::PrimaryCommunity:
                                return this->prim_com;
                        case ContactType::Id::SecondaryCommunity:
                                return this->sec_com;
                        case ContactType::Id::Daycare:
                                return this->daycare;
                        case ContactType::Id::PreSchool:
                                return this->preschool;
                }

                throw std::runtime_error{"Error: GetPool called on unknown pool type '" + ContactType::ToString(poolId) + "' which is not handled "};
        }

        PoolTypeData& GetPool(const ContactType::Id& poolId)
        {
                // do const cast to prevent code duplication.
                return const_cast<PoolTypeData&>(const_cast<const LocationPopData&>(*this).GetPool(poolId));
        }
    };

/**
 * Given a Location object, process the population inside that location and produce a PopData object.
 * @param loc The location whose population will be processed.
 * @return The LocationPopData that contains a representation of the population of the location.
 */
const LocationPopData ProcessPopulation(const geopop::Location& loc)
{
        LocationPopData retval;

        // Collect pooltype-specific information
        for(const auto& pool_type : ContactType::IdList)
        {
                // retrieve reference to current pool stats
                PoolTypeData& pool_stats = retval.GetPool(pool_type);

                // retrieve pools of this type.
                const auto& pools = loc.CRefPools(pool_type);

                // init counters to zero
                int  immune      = 0;
                int  infected    = 0;
                int  infectious  = 0;
                int  recovered   = 0;
                int  susceptible = 0;
                int  symptomatic = 0;
                int  total_pop   = 0;

                for(const auto& pool: pools) {
                        total_pop += pool->size();

                        // Iterate over population to collect data
                        for(const auto& person: *pool)
                        {
                                const auto health = person->GetHealth();

                                if (health.IsImmune())      immune++;

                                if (health.IsInfected())    infected++;

                                if (health.IsInfectious())  infectious++;

                                if (health.IsRecovered())   recovered++;

                                if (health.IsSusceptible()) susceptible++;

                                if (health.IsSymptomatic()) symptomatic++;
                        }
                }

                pool_stats.population  = total_pop;

                // prevent division by zero.
                if(total_pop == 0)
                        total_pop = 1;

                pool_stats.immune      = (double)immune / total_pop;
                pool_stats.infected    = (double)infected / total_pop;
                pool_stats.infectious  = (double)infectious / total_pop;
                pool_stats.recovered   = (double)recovered / total_pop;
                pool_stats.susceptible = (double)susceptible / total_pop;
                pool_stats.symptomatic = (double)symptomatic / total_pop;
        }

        return retval;
}


// function to calculate the fractions per-contactpool

EpiOutputFile::EpiOutputFile() : m_fstream() {}

EpiOutputFile::~EpiOutputFile() { m_fstream.close(); }

EpiOutputJSON::EpiOutputJSON(const std::string& output_prefix) : EpiOutputFile(), m_data() { Initialize(output_prefix); }

void EpiOutputJSON::Initialize(const std::string& output_prefix)
{
        const auto p            = FileSys::BuildPath(output_prefix, "EpiOutput.json");
        m_fstream.open(p.c_str(), std::ios::trunc | std::ios::out);
        m_data["Timesteps"] = json::array();
}

void EpiOutputJSON::Update(std::shared_ptr<const Population> population)
{
        // Create timestep info
        json timestep = json::array();

        const geopop::GeoGrid& geogrid = population->CRefGeoGrid();
        for(const auto& location: geogrid) {
                const LocationPopData popdata = ProcessPopulation(*location);

                json loc_json           = json::object();
                loc_json["name"]        = location->GetName();

                loc_json["coordinates"] = json::array();
                const auto coordinate   = location->GetCoordinate();
                loc_json["coordinates"].push_back(coordinate.get<0>());
                loc_json["coordinates"].push_back(coordinate.get<1>());

                for(const auto& pool_type: ContactType::IdList)
                {
                        const PoolTypeData& pool_stats = popdata.GetPool(pool_type);

                        json pool_json = json::object();

                        pool_json["population"]  = pool_stats.population;
                        pool_json["immune"]      = pool_stats.immune;
                        pool_json["infected"]    = pool_stats.infected;
                        pool_json["infectious"]  = pool_stats.infectious;
                        pool_json["recovered"]   = pool_stats.recovered;
                        pool_json["susceptible"] = pool_stats.susceptible;
                        pool_json["symptomatic"] = pool_stats.symptomatic;

                        loc_json[ContactType::ToString(pool_type)] = pool_json;
                }

                timestep.push_back(loc_json);
        }
        m_data["Timesteps"].push_back(timestep);
}

void EpiOutputJSON::Finish()
{
        m_fstream.seekp(0); // Reset write position
        m_fstream << std::setw(4) << m_data << std::endl;
}

EpiOutputHDF5::EpiOutputHDF5(const std::string& output_dir) : EpiOutputFile(), m_data(), m_timestep(0)
{
        Initialize(output_dir);
}

void EpiOutputHDF5::Initialize(const std::string& output_prefix)
{
        const auto p     = FileSys::BuildPath(output_prefix, "EpiOutput.h5");
        Exception::dontPrint();
        m_data = H5File(p.c_str(), H5F_ACC_TRUNC);
}

// TODO use common code.

void EpiOutputHDF5::Update(std::shared_ptr<const Population> population)
{
        // data layout
        CompType comp_type(sizeof(PoolTypeData));

        comp_type.insertMember("population", HOFFSET(PoolTypeData, population),   PredType::NATIVE_UINT);
        comp_type.insertMember("immune", HOFFSET(PoolTypeData, immune),           PredType::NATIVE_DOUBLE);
        comp_type.insertMember("infected", HOFFSET(PoolTypeData, infected),       PredType::NATIVE_DOUBLE);
        comp_type.insertMember("infectious", HOFFSET(PoolTypeData, infectious),   PredType::NATIVE_DOUBLE);
        comp_type.insertMember("recovered", HOFFSET(PoolTypeData, recovered),     PredType::NATIVE_DOUBLE);
        comp_type.insertMember("susceptible", HOFFSET(PoolTypeData, susceptible), PredType::NATIVE_DOUBLE);
        comp_type.insertMember("symptomatic", HOFFSET(PoolTypeData, symptomatic), PredType::NATIVE_DOUBLE);

        // Create timestep info
        const std::string timestep_name = std::to_string(m_timestep);

        Group timestep = m_data.createGroup(timestep_name);
        m_timestep++;

        int loc_ctr = 0;
        const geopop::GeoGrid& geogrid = population->CRefGeoGrid();
        for(const auto& location : geogrid) {
                try {
                        // init group that contains location data
                        const std::string loc_name = "loc" + std::to_string(loc_ctr++);
                        Group loc_group = timestep.createGroup(loc_name);

                        // add location name
                        DataSpace attr_ds = DataSpace(H5S_SCALAR);
                        StrType str_dt(PredType::C_S1, 256);
                        Attribute name_attr = loc_group.createAttribute("name", str_dt, attr_ds);
                        name_attr.write(str_dt, location->GetName());

                        // add coordinate
                        WriteCoordinate(loc_group, location->GetCoordinate());

                        // add population data
                        const LocationPopData popdata = ProcessPopulation(*location);
                        for(const auto& pool_type : ContactType::IdList) {
                                const PoolTypeData& pool_stats = popdata.GetPool(pool_type);

                                hsize_t   dim = 1;
                                DataSpace pool_ds(1, &dim);
                                DataSet pool = loc_group.createDataSet(ContactType::ToString(pool_type), comp_type, pool_ds);
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

void EpiOutputHDF5::WriteCoordinate(H5::Group& loc, const geopop::Coordinate& coordinate)
{
        hsize_t   dim[2] = {1, 2};
        DataSpace dataspace(2, dim);
        Attribute attribute         = loc.createAttribute("coordinate", PredType::NATIVE_DOUBLE, dataspace);
        double    attribute_data[2] = {boost::geometry::get<0>(coordinate), boost::geometry::get<1>(coordinate)};
        attribute.write(PredType::NATIVE_DOUBLE, attribute_data);
}

// TODO add protobuf writer

} // namespace output
} // namespace stride
