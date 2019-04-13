#pragma once

#include "GeoGridWriter.h"
#include "H5Cpp.h"
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
class GeoGridHDF5Writer : public GeoGridWriter
{
public:
        /// Construct the GeoGridHDF5Writer.
        GeoGridHDF5Writer();

        /// Write the provided GeoGrid to the proved ostream in HDF5 format.
        void Write(GeoGrid& geoGrid, std::ostream& stream) override;

private:
        void WriteContactPool(H5::H5Location& h5_location, const stride::ContactPool* pool, int count);
        void WriteLocation(const Location& location, H5::H5Location& h5_location, int count);

        void WritePerson(stride::Person* person, H5::H5File& file);

private:
        std::set<stride::Person*> m_persons_found; ///< The persons found when looping over the ContactPools.
};

} // namespace geopop