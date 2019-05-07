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

#include "GeoGridFileWriter.h"
#include "geopop/Location.h"
#include <H5Cpp.h>

namespace stride {
class ContactPool;
class Person;
} // namespace stride

namespace geopop {

/**
 * Writes a GeoGrid to a HDF5 file.
 */
class GeoGridHDF5Writer : public GeoGridFileWriter
{
public:
        /// Construct the GeoGridHDF5Writer.
        explicit GeoGridHDF5Writer(const std::string& filename);

        /// Write the provided GeoGrid to a file with the proved filename in HDF5 format.
        void Write(GeoGrid& geoGrid) override;

private:
        void WriteAttribute(H5::H5Object& object, const std::string& name, unsigned int data);
        void WriteCoordinate(H5::H5Object& object, const Coordinate& coordinate);
        void WriteContactPool(H5::H5Location& h5_location, const stride::ContactPool* pool, unsigned int count);
        void WriteLocation(const Location& location, H5::H5Location& h5_location, unsigned int count);

        void WritePersons(H5::H5Location& h5_location);

private:
        std::set<stride::Person*> m_persons_found; ///< The persons found when looping over the ContactPools.
};

} // namespace geopop
