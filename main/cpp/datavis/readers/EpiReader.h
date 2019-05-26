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
 * Header file for the Reader class.
 */

#pragma once

#include "datavis/model/Model.h"

#include <fstream>
#include <istream>
#include <memory>

namespace stride {
namespace datavis {

/**
 * @class EpiReader
 * Base class for epi-output readers.
 */
class EpiReader
{
public:
		/**
		 * Constructor.
		 *
		 * @param path An std::string that contains the path to the file.
		 */
        explicit EpiReader(const std::string& path) : m_path(path){};

        /**
         * Default destructor.
         */
        virtual ~EpiReader() = default;

        /**
         * Read the epidemiological information contained in the file and add it to the
         * specified model.
         */
        virtual void ReadIntoModel(Model& datamodel) const = 0;

protected:
        /// Retrieve the istream that contains the file contents
        const std::unique_ptr<std::ifstream> GetInStream() const { return std::make_unique<std::ifstream>(m_path); }

private:
        /// stores that path to the file
        const std::string m_path;
};

} // namespace datavis
} // namespace stride
