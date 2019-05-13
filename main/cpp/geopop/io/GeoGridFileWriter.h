#include <utility>

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

#pragma once

#include "GeoGridWriter.h"

namespace geopop {

class GeoGrid;

/**
 * An interface for writing the GeoGrid to a file, can be implemented with multiple file types.
 * HDF5 is currently implemented.
 */
class GeoGridFileWriter : public GeoGridWriter
{
public:
        /// Construct the Writer.
        explicit GeoGridFileWriter(std::string filename) : m_filename(std::move(filename)){};

        /// Write the GeoGrid to the File.
        void Write(GeoGrid& geoGrid) override = 0;

protected:
        /// Filename from File to write.
        std::string m_filename;
};

} // namespace geopop
