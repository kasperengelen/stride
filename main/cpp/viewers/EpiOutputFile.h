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
 * @file Header for the EpiOutputFile class.
 */

#pragma once

#include "contact/ContactType.h"
#include "geopop/SimLocation.h"

#include <fstream>
#include <memory>
#include <string>

namespace stride {

class Population;

namespace output {

/**
 * Base class for viewers that produce a file that gives the epidemiological status per timestep.
 */
class EpiOutputFile
{
public:
        /// Constructor: initialize.
        EpiOutputFile();

        /// Destructor: close the file stream.
        virtual ~EpiOutputFile();

        /// Update the data.
        virtual void Update(std::shared_ptr<const Population> population) = 0;

        /// Finish writing the data.
        virtual void Finish() = 0;

private:
        /// Generate file name and open the file stream.
        virtual void Initialize(const std::string& output_dir) = 0;

protected:
        std::fstream m_fstream;
};

} // namespace output
} // namespace stride
