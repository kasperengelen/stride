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
 *  Copyright 2019, Meyer J
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

using namespace std;
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
        PoolTypeData household;
        PoolTypeData k12_school;
        PoolTypeData college;
        PoolTypeData workplace;
        PoolTypeData prim_com;
        PoolTypeData sec_com;
        PoolTypeData daycare;
        PoolTypeData preschool;

        PoolTypeData& GetPool(const ContactType::Id& poolId)
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
        for(const ContactType::Id& pool_type : ContactType::IdList)
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

EpiOutputJSON::EpiOutputJSON(const string& output_prefix) : EpiOutputFile(), m_data() { Initialize(output_prefix); }

void EpiOutputJSON::Initialize(const string& output_prefix)
{
        string     fname = "EpiOutput.json";
        const auto p     = FileSys::BuildPath(output_prefix, fname);
        m_fstream.open(p.c_str(), std::ios::trunc | std::ios::out);
        m_data["Timesteps"] = json::array();
}

// TODO use common code.

void EpiOutputJSON::Update(std::shared_ptr<const Population> population)
{
        // Create timestep info
        json timestep = json::array();

        const geopop::GeoGrid& geogrid = population->CRefGeoGrid();
        for (auto loc_it = geogrid.cbegin(); loc_it != geogrid.cend(); ++loc_it) {
                json loc           = json::object();
                loc["name"]        = (*loc_it)->GetName();
                loc["coordinates"] = json::array();
                auto coordinate    = (*loc_it)->GetCoordinate();
                loc["coordinates"].push_back(coordinate.get<0>());
                loc["coordinates"].push_back(coordinate.get<1>());

                // Collect pooltype-specific information
                for (auto type_it = ContactType::IdList.begin(); type_it != ContactType::IdList.end(); ++type_it) {
                        stride::util::SegmentedVector<stride::ContactPool*>& pools      = (*loc_it)->RefPools(*type_it);
                        json                                                 pool       = json::object();
                        int                                                  immune     = 0;
                        int                                                  infected   = 0;
                        int                                                  infectious = 0;
                        int                                                  recovered  = 0;
                        int                                                  susceptible = 0;
                        int                                                  symptomatic = 0;
                        int                                                  total_pop   = 0;
                        for (auto pool_it = pools.begin(); pool_it != pools.end(); ++pool_it) {
                                total_pop += (*pool_it)->size();
                                // Iterate over population to collect data
                                for (auto mem_it = (*pool_it)->begin(); mem_it != (*pool_it)->end(); ++mem_it) {
                                        auto health = (*mem_it)->GetHealth();
                                        if (health.IsImmune()) {
                                                immune++;
                                        }
                                        if (health.IsInfected()) {
                                                infected++;
                                        }
                                        if (health.IsInfectious()) {
                                                infectious++;
                                        }
                                        if (health.IsRecovered()) {
                                                recovered++;
                                        }
                                        if (health.IsSusceptible()) {
                                                susceptible++;
                                        }
                                        if (health.IsSymptomatic()) {
                                                symptomatic++;
                                        }
                                }
                        }
                        pool["population"]                   = total_pop;
                        pool["immune"]                       = (double)immune / total_pop;
                        pool["infected"]                     = (double)infected / total_pop;
                        pool["infectious"]                   = (double)infectious / total_pop;
                        pool["recovered"]                    = (double)recovered / total_pop;
                        pool["susceptible"]                  = (double)susceptible / total_pop;
                        pool["symptomatic"]                  = (double)symptomatic / total_pop;
                        loc[ContactType::ToString(*type_it)] = pool;
                }

                timestep.push_back(loc);
        }
        m_data["Timesteps"].push_back(timestep);
}

void EpiOutputJSON::Finish()
{
        m_fstream.seekp(0); // Reset write position
        m_fstream << setw(4) << m_data << std::endl;
}

EpiOutputHDF5::EpiOutputHDF5(const std::string& output_dir) : EpiOutputFile(), m_data(), m_timestep(0)
{
        Initialize(output_dir);
}

void EpiOutputHDF5::Initialize(const string& output_prefix)
{
        string     fname = "EpiOutput.h5";
        const auto p     = FileSys::BuildPath(output_prefix, fname);
        Exception::dontPrint();
        m_data = H5File(p.c_str(), H5F_ACC_TRUNC);
}

// TODO use common code.

void EpiOutputHDF5::Update(std::shared_ptr<const Population> population)
{

        // Create timestep info
        string timestep_name = to_string(m_timestep);

        Group* timestep = new Group(m_data.createGroup(timestep_name));
        m_timestep++;
        int loc_ctr = 0;

        const geopop::GeoGrid& geogrid = population->CRefGeoGrid();
        for (auto loc_it = geogrid.cbegin(); loc_it != geogrid.cend(); ++loc_it) {
                try {
                        // Use name that doesn't contain "/" (see HDF5 spec)
                        string loc_name = "loc" + to_string(loc_ctr++);// (*loc_it)->GetName();
                        //std::replace(loc_name.begin(), loc_name.end(), '/', '|');
                        Group* loc = new Group(timestep->createGroup(loc_name));
                        DataSpace attr_ds = DataSpace(H5S_SCALAR);
                        StrType str_dt(PredType::C_S1, 256);
                        const H5std_string attr_name("name");
                        const H5std_string loc_name_buff((*loc_it)->GetName());
                        Attribute name_attr = loc->createAttribute(attr_name, str_dt, attr_ds);
                        name_attr.write(str_dt, loc_name_buff);
                        WriteCoordinate(*loc, (*loc_it)->GetCoordinate());

                        // Collect pooltype-specific information
                        struct PoolTypeData
                        {
                                unsigned int population;
                                double       immune;
                                double       infected;
                                double       infectious;
                                double       recovered;
                                double       susceptible;
                                double       symptomatic;
                        };

                        for (auto type_it = ContactType::IdList.begin(); type_it != ContactType::IdList.end();
                             ++type_it) {
                                stride::util::SegmentedVector<stride::ContactPool*>& pools =
                                    (*loc_it)->RefPools(*type_it);
                                CompType      comp_type(sizeof(PoolTypeData));
                                PoolTypeData* data = new PoolTypeData();
                                comp_type.insertMember("population", HOFFSET(PoolTypeData, population),
                                                       PredType::NATIVE_UINT);
                                comp_type.insertMember("immune", HOFFSET(PoolTypeData, immune),
                                                       PredType::NATIVE_DOUBLE);
                                comp_type.insertMember("infected", HOFFSET(PoolTypeData, infected),
                                                       PredType::NATIVE_DOUBLE);
                                comp_type.insertMember("infectious", HOFFSET(PoolTypeData, infectious),
                                                       PredType::NATIVE_DOUBLE);
                                comp_type.insertMember("recovered", HOFFSET(PoolTypeData, recovered),
                                                       PredType::NATIVE_DOUBLE);
                                comp_type.insertMember("susceptible", HOFFSET(PoolTypeData, susceptible),
                                                       PredType::NATIVE_DOUBLE);
                                comp_type.insertMember("symptomatic", HOFFSET(PoolTypeData, symptomatic),
                                                       PredType::NATIVE_DOUBLE);
                                for (auto pool_it = pools.begin(); pool_it != pools.end(); ++pool_it) {
                                        data->population += (*pool_it)->size();
                                        // Iterate over population to collect data
                                        for (auto mem_it = (*pool_it)->begin(); mem_it != (*pool_it)->end(); ++mem_it) {
                                                auto health = (*mem_it)->GetHealth();
                                                if (health.IsImmune()) {
                                                        data->immune++;
                                                }
                                                if (health.IsInfected()) {
                                                        data->infected++;
                                                }
                                                if (health.IsInfectious()) {
                                                        data->infectious++;
                                                }
                                                if (health.IsRecovered()) {
                                                        data->recovered++;
                                                }
                                                if (health.IsSusceptible()) {
                                                        data->susceptible++;
                                                }
                                                if (health.IsSymptomatic()) {
                                                        data->symptomatic++;
                                                }
                                        }
                                }

                                if (data->population > 0) {
                                        data->immune      /= data->population;
                                        data->infected    /= data->population;
                                        data->infectious  /= data->population;
                                        data->recovered   /= data->population;
                                        data->susceptible /= data->population;
                                        data->symptomatic /= data->population;
                                } else {
                                        // All fractions to 0 by convention
                                        data->immune      = 0.0;
                                        data->infected    = 0.0;
                                        data->infectious  = 0.0;
                                        data->recovered   = 0.0;
                                        data->susceptible = 0.0;
                                        data->symptomatic = 0.0;
                                }

                                hsize_t   dim = 1;
                                DataSpace pool_ds(1, &dim);
                                DataSet pool = loc->createDataSet(ContactType::ToString(*type_it), comp_type, pool_ds);
                                pool.write(data, comp_type);
                                delete data;
                        }
                        delete loc;
                } catch (...) {
                        // Ignore locations that can't be written
                        continue;
                }
        }
        delete timestep;
}

void EpiOutputHDF5::Finish()
{ 
        m_data.close();
}

void EpiOutputHDF5::WriteAttribute(H5Object& object, const std::string& name, unsigned int data)
{
        hsize_t   dim       = 1;
        Attribute attribute = object.createAttribute(name, PredType::NATIVE_UINT, DataSpace(1, &dim));
        attribute.write(PredType::NATIVE_UINT, &data);
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
