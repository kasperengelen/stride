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
 * Header for the EpiOutputFile class.
 */

#pragma once

#include <fstream>
#include <memory>
#include <string>

namespace stride {

class Population;

namespace output {

/**
 * Produces a file that gives the epidemiological status per timestep .
 */
class EpiOutputFile
{
public:
        /// Constructor: initialize.
        /// Accepted filetypes are "json", "hdf5", and "protobuf"
        explicit EpiOutputFile(const std::string& output_dir = "output", const std::string& output_type = "json");

        /// Destructor: close the file stream.
        ~EpiOutputFile();

        /// Print the given cases with corresponding tag.
        void Print(std::shared_ptr<const Population> population);

private:
        /// Generate file name and open the file stream.
        void Initialize(const std::string& output_dir, const std::string& output_type);

private:
        std::ofstream m_fstream;
};

} // namespace output
} // namespace stride
