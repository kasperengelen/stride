#pragma once

#include "GeoGridReader.h"

#include <H5Cpp.h>

namespace stride {
class ContactPool;
} // namespace stride

namespace geopop {

class GeoGrid;
class Location;

/**
 * An implementation of the GeoGridReader using HDF5.
 * This class is used to read a GeoGrid from a HDF5 file.
 */
class GeoGridHDF5Reader : public GeoGridReader
{
public:
        /// Construct the GeoGridHDF5Reader with the istream which contains the HDF5.
        GeoGridHDF5Reader(stride::Population* pop, std::string filename);

        /// No copy constructor.
        GeoGridHDF5Reader(const GeoGridHDF5Reader&) = delete;

        /// No copy assignement.
        GeoGridHDF5Reader operator=(const GeoGridHDF5Reader&) = delete;

        /// Actually perform the read and return the GeoGrid.
        void Read() override;

private:
        //        /// Create a ContactCenter based on the information stored in the provided boost property tree.
        //        std::shared_ptr<ContactCenter> ParseContactCenter(boost::property_tree::ptree& contactCenter);
        //
        /// Create a ContactCenter based on the information stored in the provided boost property tree.
        stride::ContactPool* ParseContactPool(const H5::DataSet& pool);
        //
        //        /// Create a Coordinate based on the information stored in the provided boost property tree.
        //        Coordinate ParseCoordinate(boost::property_tree::ptree& coordinate);
        //

        /// Create a Location based on the information stored in the provided boost property tree.
        std::shared_ptr<geopop::Location> ParseLocation(const H5::H5Object& loc);

        /// Create a Person based on the information stored in the provided boost property tree.
        void ParsePersons(const H5::H5Location& loc);

private:
        std::string m_filename;
};

} // namespace geopop
