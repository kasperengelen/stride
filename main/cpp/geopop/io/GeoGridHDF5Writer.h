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
#include "contact/ContactPool.h"
#include "geopop/Location.h"
#include "pop/Person.h"
#include <H5Cpp.h>
#include <set>

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
        /// Create a H5::Attribute in the H5::H5Object containing all info needed to reconstruct the data.
        void WriteAttribute(H5::H5Object& object, const std::string& name, unsigned int data);

        /// Create a H5::Attribute in the H5::Group containing all info needed to reconstruct a Coordinate.
        void WriteCoordinate(H5::Group& loc, const Coordinate& coordinate);

        /// Create a H5::DataSet in the H5::Group containing all info needed to reconstruct a ContactPool.
        void WriteContactPool(H5::Group& contact_pools, const stride::ContactPool* pool, unsigned int count);

        /// Create a H5::Group in the H5::Group containing all info needed to reconstruct a Location.
        void WriteLocation(H5::Group& locations, const Location& location, unsigned int count);

        /// Create a H5::DataSet in the H5::Group containing all info needed to reconstruct the Persons.
        void WritePersons(H5::Group& group);

private:
        /// The persons found when looping over the ContactPools.
        std::set<stride::Person*> m_persons_found;
};

} // namespace geopop
