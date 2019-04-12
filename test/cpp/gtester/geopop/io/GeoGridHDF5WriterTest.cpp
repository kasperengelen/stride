#include "GeoGridIOUtils.h"

#include "geopop/ContactCenter.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/io/GeoGridHDF5Writer.h"
#include "pop/Population.h"
#include "util/FileSys.h"

#include "H5Cpp.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;
using boost::property_tree::ptree;

namespace {

bool compareGeoGrid(GeoGrid& geoGrid, const string& testname)
{
        GeoGridHDF5Writer writer;
        stringstream      ss;
        writer.Write(geoGrid, ss);

        return true;
}

TEST(GeoGridHDF5WriterTest, locationTest)
{
        const auto pop     = Population::Create();
        auto&      geoGrid = pop->RefGeoGrid();
        geoGrid.AddLocation(make_shared<Location>(1, 4, Coordinate(0, 0), "Bavikhove", 2500));
        geoGrid.AddLocation(make_shared<Location>(2, 3, Coordinate(0, 0), "Gent", 5000));
        geoGrid.AddLocation(make_shared<Location>(3, 2, Coordinate(0, 0), "Mons", 2500));

        EXPECT_TRUE(compareGeoGrid(geoGrid, "test0.json"));
}
} // namespace
