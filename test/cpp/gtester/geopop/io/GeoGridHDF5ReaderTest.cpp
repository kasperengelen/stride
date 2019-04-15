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

#include "geopop/io/GeoGridHDF5Reader.h"
#include "contact/ContactType.h"
#include "geopop/GeoGrid.h"
#include "geopop/Location.h"
#include "pop/Population.h"
#include "util/FileSys.h"

#include <fstream>
#include <gtest/gtest.h>
#include <memory>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

using boost::geometry::get;

namespace {

void getGeoGridFromFile(const string& filename, Population* pop)
{
        GeoGridHDF5Reader reader(pop, FileSys::GetTestsDir().string() + "/testdata/GeoGridHDF5/" + filename);
        reader.Read();
}

TEST(GeoGridHDF5ReaderTest, locationTest)
{
        auto pop = Population::Create();
        getGeoGridFromFile("test0.h5", pop.get());
        auto& geoGrid = pop->RefGeoGrid();

        map<unsigned int, shared_ptr<Location>> locations;
        locations[geoGrid[0]->GetID()] = geoGrid[0];
        locations[geoGrid[1]->GetID()] = geoGrid[1];
        locations[geoGrid[2]->GetID()] = geoGrid[2];

        const auto location1 = locations[1];
        const auto location2 = locations[2];
        const auto location3 = locations[3];

        EXPECT_EQ(location1->GetID(), 1);
        EXPECT_EQ(location1->GetName(), "Bavikhove");
        EXPECT_EQ(location1->GetProvince(), 4);
        EXPECT_EQ(location1->GetPopCount(), 2500);
        EXPECT_EQ(get<0>(location1->GetCoordinate()), 0);
        EXPECT_EQ(get<1>(location1->GetCoordinate()), 0);

        EXPECT_EQ(location2->GetID(), 2);
        EXPECT_EQ(location2->GetName(), "Gent");
        EXPECT_EQ(location2->GetProvince(), 3);
        EXPECT_EQ(location2->GetPopCount(), 5000);
        EXPECT_EQ(get<0>(location2->GetCoordinate()), 0);
        EXPECT_EQ(get<1>(location2->GetCoordinate()), 0);

        EXPECT_EQ(location3->GetID(), 3);
        EXPECT_EQ(location3->GetName(), "Mons");
        EXPECT_EQ(location3->GetProvince(), 2);
        EXPECT_EQ(location3->GetPopCount(), 2500);
        EXPECT_EQ(get<0>(location3->GetCoordinate()), 0);
        EXPECT_EQ(get<1>(location3->GetCoordinate()), 0);

}
TEST(GeoGridHDF5ReaderTest, contactPoolsTest) {}

TEST(GeoGridHDF5ReaderTest, peopleTest)
{
        auto pop = Population::Create();
        getGeoGridFromFile("test0.json", pop.get());
        auto& geoGrid = pop->RefGeoGrid();

        EXPECT_TRUE(true);
}

TEST(GeoGridHDF5ReaderTest, commutesTest) {}
TEST(GeoGridHDF5ReaderTest, emptyStreamTest) {}
TEST(GeoGridHDF5ReaderTest, invalidTypeTest) {}
TEST(GeoGridHDF5ReaderTest, invalidPersonTest) {}
TEST(GeoGridHDF5ReaderTest, invalidHDF5Test) {}

} // namespace
