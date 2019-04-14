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
        GeoGridHDF5Reader(stride::Population* pop, const std::string& filename);

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
//        /// Create a ContactCenter based on the information stored in the provided boost property tree.
//        stride::ContactPool* ParseContactPool(boost::property_tree::ptree& contactPool, stride::ContactType::Id typeId);
//
//        /// Create a Coordinate based on the information stored in the provided boost property tree.
//        Coordinate ParseCoordinate(boost::property_tree::ptree& coordinate);
//
//        /// Create a Location based on the information stored in the provided boost property tree.
//        std::shared_ptr<Location> ParseLocation(boost::property_tree::ptree& location);

        /// Create a Person based on the information stored in the provided boost property tree.
        void ParsePersons(GeoGrid& geogrid, const H5::H5Location& h5_location);
private:
        std::string m_filename;
};

} // namespace geopop
