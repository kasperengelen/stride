#pragma once

#include "GeoGridWriter.h"

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
    };

} // namespace geopop
