#pragma once

#include "GeoGridReader.h"

#include "H5Cpp.h"

namespace geopop {

class GeoGrid;

/**
 * An implementation of the GeoGridReader using HDF5.
 * This class is used to read a GeoGrid from a HDF5 file.
 */
class GeoGridHDF5Reader : public GeoGridReader
{
public:
        /// Construct the GeoGridHDF5Reader with the istream which contains the HDF5.
        GeoGridHDF5Reader(std::unique_ptr<std::istream> inputStream, stride::Population* pop);

        /// No copy constructor.
        GeoGridHDF5Reader(const GeoGridHDF5Reader&) = delete;

        /// No copy assignement.
        GeoGridHDF5Reader operator=(const GeoGridHDF5Reader&) = delete;

        /// Actually perform the read and return the GeoGrid.
        void Read() override;
};

} // namespace geopop
