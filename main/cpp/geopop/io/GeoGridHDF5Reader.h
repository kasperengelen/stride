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

#include "GeoGridReader.h"
#include "contact/ContactPool.h"
#include "geopop/Location.h"

#include <H5Cpp.h>

namespace geopop {

/**
 * An implementation of the GeoGridReader using HDF5.
 * This class is used to read a GeoGrid from a HDF5 file.
 */
class GeoGridHDF5Reader : public GeoGridReader
{
public:
        /// Construct the GeoGridHDF5Reader with the file which contains the HDF5.
        GeoGridHDF5Reader(stride::Population* pop, std::string filename);

        /// No copy constructor.
        GeoGridHDF5Reader(const GeoGridHDF5Reader&) = delete;

        /// No copy assignement.
        GeoGridHDF5Reader operator=(const GeoGridHDF5Reader&) = delete;

        /// Actually perform the read and return the GeoGrid.
        void Read() override;

private:
        /// Create a ContactPool based on the information stored in the provided HDF5 DataSet.
        stride::ContactPool* ParseContactPool(const H5::DataSet& pool);

        /// Create a Coordinate based on the information stored in the provided HDF5 Group.
        Coordinate ParseCoordinate(const H5::Group& loc);

        /// Create a Location based on the information stored in the provided HDF5 Group.
        std::shared_ptr<geopop::Location> ParseLocation(const H5::Group& loc);

        /// Create the Persons based on the information stored in the provided HDF5 DataSet.
        void ParsePersons(const H5::DataSet& loc);

private:
        /// Filename from File to read from.
        std::string m_filename;
};

} // namespace geopop
