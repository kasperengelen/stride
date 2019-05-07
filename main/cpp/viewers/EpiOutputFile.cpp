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
#include "geopop/Location.h"
#include "pop/Population.h"
#include "util/FileSys.h"


namespace stride {
namespace output {

using namespace std;
using namespace stride::util;
using namespace H5;
using json = nlohmann::json;

EpiOutputFile::EpiOutputFile() : m_fstream() {}

EpiOutputFile::~EpiOutputFile() { m_fstream.close(); }

EpiOutputJSON::EpiOutputJSON(const string& output_prefix) : EpiOutputFile(), m_data()
{ 
        Initialize(output_prefix); 
}

void EpiOutputJSON::Initialize(const string& output_prefix)
{
        string     fname = "EpiOutput.json";
        const auto p     = FileSys::BuildPath(output_prefix, fname);
        m_fstream.open(p.c_str(), std::ios::trunc | std::ios::out);
        m_data["Timesteps"] = json::array();
}

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


EpiOutputHDF5::EpiOutputHDF5(const std::string& output_dir) : EpiOutputFile(), m_data()
{
        Initialize(output_dir);
}

void EpiOutputHDF5::Initialize(const string& output_prefix)
{
        string fname = "EpiOutput.h5";
        const auto p = FileSys::BuildPath(output_prefix, fname);
        Exception::dontPrint();
        m_data = H5File(p.c_str(), H5F_ACC_TRUNC);
} 

void EpiOutputHDF5::Update(std::shared_ptr<const Population> population) 
{
        return;
}

void EpiOutputHDF5::Finish()
{
        m_data.close();
}

void writeAttribute(H5Object& object, const std::string& name, unsigned int data, H5::PredType type)
{
        hsize_t   dim       = 1;
        Attribute attribute = object.createAttribute(name, type, DataSpace(1, &dim));
        attribute.write(type, &data);
}

} // namespace output
} // namespace stride
