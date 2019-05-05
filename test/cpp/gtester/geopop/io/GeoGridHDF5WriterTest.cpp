#include "GeoGridIOUtils.h"

#include "GeoGridCompare.h"
#include "contact/ContactPool.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/io/GeoGridHDF5Reader.h"
#include "geopop/io/GeoGridHDF5Writer.h"
#include "pop/Population.h"
#include "util/FileSys.h"

#include <H5Cpp.h>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <stdio.h>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

namespace {

bool compareGeoGrid(GeoGrid& geoGrid, const std::string& filename)
{
        GeoGridHDF5Writer writer(FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/write");
        writer.Write(geoGrid);
        auto              pop = Population::Create();
        GeoGridHDF5Reader reader(pop.get(), FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/" + filename);
        reader.Read();
        GeoGridCompare c;
        c.CompareGeoGrid(geoGrid, pop->RefGeoGrid());
        remove((FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/write").c_str());
        return true;
}

TEST(GeoGridHDF5WriterTest, locationTest)
{
        const auto pop     = Population::Create();
        auto&      geoGrid = pop->RefGeoGrid();
        geoGrid.AddLocation(make_shared<Location>(1, 4, Coordinate(0, 0), "Bavikhove", 2500));
        geoGrid.AddLocation(make_shared<Location>(2, 3, Coordinate(0, 0), "Gent", 5000));
        geoGrid.AddLocation(make_shared<Location>(3, 2, Coordinate(0, 0), "Mons", 2500));

        EXPECT_TRUE(compareGeoGrid(geoGrid, "test0.h5"));
}

TEST(GeoGridHDF5WriterTest, contactPoolsTest)
{
        auto  pop     = Population::Create();
        auto& geoGrid = pop->RefGeoGrid();
        auto  loc     = make_shared<Location>(1, 4, Coordinate(0, 0), "Bavikhove", 2500);
        loc->RefPools(Id::K12School).emplace_back(pop->RefPoolSys().CreateContactPool(Id::K12School));
        loc->RefPools(Id::PrimaryCommunity).emplace_back(pop->RefPoolSys().CreateContactPool(Id::PrimaryCommunity));
        loc->RefPools(Id::College).emplace_back(pop->RefPoolSys().CreateContactPool(Id::College));
        loc->RefPools(Id::Household).emplace_back(pop->RefPoolSys().CreateContactPool(Id::Household));
        loc->RefPools(Id::Workplace).emplace_back(pop->RefPoolSys().CreateContactPool(Id::Workplace));
        loc->RefPools(Id::Daycare).emplace_back(pop->RefPoolSys().CreateContactPool(Id::Daycare));
        loc->RefPools(Id::PreSchool).emplace_back(pop->RefPoolSys().CreateContactPool(Id::PreSchool));
        geoGrid.AddLocation(loc);

        EXPECT_TRUE(compareGeoGrid(geoGrid, "test1.h5"));
}

TEST(GeoGridHDF5WriterTest, peopleTest)
{
        auto pop = Population::Create();
        EXPECT_TRUE(compareGeoGrid(*GetPopulatedGeoGrid(pop.get()), "test2.h5"));
}

TEST(GeoGridHDF5WriterTest, commutesTest)
{
        auto pop = Population::Create();
        EXPECT_TRUE(compareGeoGrid(*GetCommutesGeoGrid(pop.get()), "test3.h5"));
}

} // namespace
