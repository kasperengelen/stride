#include "GeoGridIOUtils.h"

#include "contact/ContactPool.h"
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

namespace {

bool compareGeoGrid(GeoGrid& geoGrid)
{
        GeoGridHDF5Writer writer;
        writer.Write(geoGrid, "Geo/GeoGrid.h5");
        return true;
}

TEST(GeoGridHDF5WriterTest, locationTest)
{
        const auto pop     = Population::Create();
        auto&      geoGrid = pop->RefGeoGrid();
        geoGrid.AddLocation(make_shared<Location>(1, 4, Coordinate(0, 0), "Bavikhove", 2500));
        geoGrid.AddLocation(make_shared<Location>(2, 3, Coordinate(0, 0), "Gent", 5000));
        geoGrid.AddLocation(make_shared<Location>(3, 2, Coordinate(0, 0), "Mons", 2500));

        EXPECT_TRUE(compareGeoGrid(geoGrid));
}

TEST(GeoGridJSONWriterTest, contactPoolsTest)
{
        auto        pop      = Population::Create();
        auto&       geoGrid  = pop->RefGeoGrid();
        auto        location = make_shared<Location>(1, 4, Coordinate(0, 0), "Bavikhove", 2500);
        ContactPool pool1    = ContactPool(1, Id::K12School);
        ContactPool pool2    = ContactPool(2, Id::PrimaryCommunity);
        ContactPool pool3    = ContactPool(3, Id::College);
        ContactPool pool4    = ContactPool(4, Id::Household);
        ContactPool pool5    = ContactPool(5, Id::Workplace);
        Person      person1  = Person(1, 12, 0, 1, 0, 0, 0, 0);
        Person      person2  = Person(2, 40, 1, 0, 0, 2, 0, 0);
        Person      person3  = Person(3, 20, 2, 0, 1, 0, 0, 0);
        pool1.AddMember(&person1);
        pool1.AddMember(&person2);
        pool1.AddMember(&person3);

        location->RegisterPool(&pool1, pool1.GetType());
        location->RegisterPool(&pool2, pool2.GetType());
        location->RegisterPool(&pool3, pool3.GetType());
        location->RegisterPool(&pool4, pool4.GetType());
        location->RegisterPool(&pool5, pool5.GetType());

        geoGrid.AddLocation(location);

        EXPECT_TRUE(compareGeoGrid(geoGrid));
}

TEST(GeoGridHDF5WriterTest, commutesTest)
{
        auto pop = Population::Create();
        EXPECT_TRUE(compareGeoGrid(*GetCommutesGeoGrid(pop.get())));
}

TEST(GeoGridHDF5WriterTest, peopleTest)
{
        auto pop = Population::Create();
        EXPECT_TRUE(compareGeoGrid(*GetPopulatedGeoGrid(pop.get())));
}
} // namespace
