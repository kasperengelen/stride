#pragma once

#include "GeoGridWriter.h"
#include <H5Cpp.h>
#include "geopop/Location.h"

namespace stride {
class ContactPool;
class Person;
} // namespace stride

namespace geopop {

class ContactCenter;

/**
 * Writes a GeoGrid to a HDF5 file.
 */
class GeoGridHDF5Writer
{
public:
        /// Construct the GeoGridHDF5Writer.
        GeoGridHDF5Writer();

        /// Write the provided GeoGrid to a file with the proved filename in HDF5 format.
        void Write(GeoGrid& geoGrid, const std::string& filename);

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
