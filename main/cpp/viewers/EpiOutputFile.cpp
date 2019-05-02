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
#include "pop/Population.h"
#include "geopop/Location.h"
#include "util/FileSys.h"
#include "util/json.hpp"

namespace stride {
namespace output {

using namespace std;
using namespace stride::util;
using json = nlohmann::json;

EpiOutputFile::EpiOutputFile() : m_fstream() {}

EpiOutputFile::~EpiOutputFile() { m_fstream.close(); }

EpiOutputJSON::EpiOutputJSON(const string& output_prefix) : EpiOutputFile(), m_data()
{ 
        Initialize(output_prefix); 
}

void EpiOutputJSON::Initialize(const string& output_prefix)
{
        string fname = "EpiOutput.json";
        const auto p = FileSys::BuildPath(output_prefix, fname);
        m_fstream.open(p.c_str(), std::ios::trunc | std::ios::out);
        m_data["Timesteps"] = json::array();
}

void EpiOutputJSON::Update(std::shared_ptr<const Population> population)
{
        // Create timestep info
        json timestep = json::array();

        const geopop::GeoGrid& geogrid = population->CRefGeoGrid();
        for (auto it = geogrid.cbegin(); it != geogrid.cend(); ++it) {
                json loc = json::object();
                loc["name"] = (*it)->GetName();
                loc["population"] = (*it)->GetPopCount();
                loc["infected"] = (*it)->GetInfectedCount();
                timestep.push_back(loc);
        }
        m_data["Timesteps"].push_back(timestep);
}

void EpiOutputJSON::Finish(std::shared_ptr<const Population> population)
{
        m_fstream.seekp(0); // Reset write position
        m_fstream << setw(4) << m_data << std::endl;
}

} // namespace output
} // namespace stride
