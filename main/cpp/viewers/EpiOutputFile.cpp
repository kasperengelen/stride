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

#include "EpiOutputFile.h"

#include "pop/Population.h"
#include "util/FileSys.h"

namespace stride {
namespace output {

using namespace std;
using namespace stride::util;

EpiOutputFile::EpiOutputFile(const string& output_prefix, const std::string& output_type) 
        : m_fstream() { Initialize(output_prefix, output_type); }

EpiOutputFile::~EpiOutputFile() { m_fstream.close(); }

void EpiOutputFile::Initialize(const string& output_prefix, const string& output_type)
{
        const auto p = FileSys::BuildPath(output_prefix, "EpiOutput." + output_type);
        m_fstream.open(p.c_str());
        // add header
        m_fstream << "placeholder" << endl;
}

void EpiOutputFile::Print(std::shared_ptr<const Population> population)
{
        for (const auto& p : *population) {
                const auto& h = p.GetHealth();
                m_fstream << p.GetId() << "," << p.GetAge() << std::endl;
        }
}

} // namespace output
} // namespace stride
