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
#include "util/FileSys.h"
#include "util/json.hpp"

namespace stride {
namespace output {

using namespace std;
using namespace stride::util;
using json = nlohmann::json;

EpiOutputFile::EpiOutputFile(const string& output_prefix, const std::string& output_type) 
        : m_fstream() { Initialize(output_prefix, output_type); }

EpiOutputFile::~EpiOutputFile() { m_fstream.close(); }

void EpiOutputFile::Initialize(const string& output_prefix, const string& output_type)
{
        m_file_type = output_type;
        string fname = "EpiOutput." + m_file_type;
        const auto p = FileSys::BuildPath(output_prefix, fname);
        m_fstream.open(p.c_str(), std::ios::trunc | std::ios::out | std::ios::in);

        if (m_file_type == "json") {
                this->InitializeJSON();
        }
}

void EpiOutputFile::InitializeJSON()
{
        json j;
        j["Timesteps"] = json::array();
        m_fstream << setw(4) << j << endl;
}

void EpiOutputFile::Print(std::shared_ptr<const Population> population)
{
        if (m_file_type == "json") {
                this->PrintJSON(population);
        }
}

void EpiOutputFile::PrintJSON(std::shared_ptr<const Population> population)
{
        m_fstream.seekg(0); // Reset read position
        json j;
        m_fstream >> j;
        
        // Create timestep info
        json timestep = json::array();
        timestep["population"] = population->GetInfectedCount();
        timestep[""]
        j["Timesteps"].push_back(timestep);

        m_fstream.seekp(0); // Reset write position
        m_fstream << setw(4) << j << std::endl;
}

} // namespace output
} // namespace stride
