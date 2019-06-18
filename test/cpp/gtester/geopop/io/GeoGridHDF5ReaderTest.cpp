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
#include "geopop/SimLocation.h"
#include "pop/Population.h"
#include "util/Exception.h"
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

        map<unsigned int, shared_ptr<SimLocation>> locations;
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
TEST(GeoGridHDF5ReaderTest, contactPoolsTest)
{
        auto pop = Population::Create();
        getGeoGridFromFile("test1.h5", pop.get());
        auto& geoGrid  = pop->RefGeoGrid();
        auto  location = geoGrid[0];

        vector<ContactPool*> pools;
        for (Id typ : IdList) {
                for (const auto& pool : location->RefPools(typ)) {
                        pools.emplace_back(pool);
                }
        }

        map<Id, bool> found = {{Id::K12School, false}, {Id::PrimaryCommunity, false}, {Id::College, false},
                               {Id::Household, false}, {Id::Workplace, false},        {Id::Daycare, false},
                               {Id::PreSchool, false}};

        for (unsigned int i = 0; i < 7; i++) {
                EXPECT_FALSE(found[pools[i]->GetType()]);
                found[pools[i]->GetType()] = true;
        }
        for (auto& type : found) {
                EXPECT_TRUE(type.second);
        }
}

TEST(GeoGridHDF5ReaderTest, peopleTest)
{
        auto pop = Population::Create();
        getGeoGridFromFile("test2.h5", pop.get());
        auto& geoGrid  = pop->RefGeoGrid();
        auto  location = geoGrid[0];

        vector<ContactPool*> pools;
        for (Id typ : IdList) {
                for (const auto& pool : location->RefPools(typ)) {
                        pools.emplace_back(pool);
                }
        }

        for (const auto& pool : pools) {
                EXPECT_EQ(pool->size(), 1);
                auto person = *(pool->begin());
                EXPECT_EQ(person->GetId(), 0);
                EXPECT_EQ(person->GetAge(), 18);
                EXPECT_EQ(person->GetPoolId(Id::K12School), 1);
                EXPECT_EQ(person->GetPoolId(Id::Household), 1);
                EXPECT_EQ(person->GetPoolId(Id::College), 1);
                EXPECT_EQ(person->GetPoolId(Id::Workplace), 1);
                EXPECT_EQ(person->GetPoolId(Id::PrimaryCommunity), 1);
                EXPECT_EQ(person->GetPoolId(Id::SecondaryCommunity), 1);
                EXPECT_EQ(person->GetPoolId(Id::Daycare), 1);
                EXPECT_EQ(person->GetPoolId(Id::PreSchool), 1);
        }
}

TEST(GeoGridHDF5ReaderTest, commutesTest)
{
        auto pop = Population::Create();
        getGeoGridFromFile("test3.h5", pop.get());
        auto& geoGrid = pop->RefGeoGrid();

        map<unsigned int, shared_ptr<SimLocation>> locations;

        locations[geoGrid[0]->GetID()] = geoGrid[0];
        locations[geoGrid[1]->GetID()] = geoGrid[1];
        locations[geoGrid[2]->GetID()] = geoGrid[2];

        auto location1 = locations[1];
        auto location2 = locations[2];
        auto location3 = locations[3];

        auto sortLoc = [](vector<pair<SimLocation*, double>> loc) {
                sort(begin(loc), end(loc),
                     [](const pair<SimLocation*, double>& a, const pair<SimLocation*, double>& b) {
                             return a.first->GetID() < b.first->GetID();
                     });
                return loc;
        };

        {
                auto commuting_in  = sortLoc(location1->CRefIncomingCommutes());
                auto commuting_out = sortLoc(location1->CRefOutgoingCommutes());
                EXPECT_EQ(commuting_in.size(), 1);
                EXPECT_EQ(commuting_out.size(), 2);

                EXPECT_EQ(commuting_in[0].first->GetID(), 2);
                EXPECT_DOUBLE_EQ(commuting_in[0].second, 0.75);

                EXPECT_EQ(commuting_out[0].first->GetID(), 2);
                EXPECT_DOUBLE_EQ(commuting_out[0].second, 0.50);
                EXPECT_EQ(commuting_out[1].first->GetID(), 3);
                EXPECT_DOUBLE_EQ(commuting_out[1].second, 0.25);
        }
        {
                auto commuting_in  = sortLoc(location2->CRefIncomingCommutes());
                auto commuting_out = sortLoc(location2->CRefOutgoingCommutes());
                EXPECT_EQ(commuting_out.size(), 2);
                EXPECT_EQ(commuting_in.size(), 1);

                EXPECT_EQ(commuting_in[0].first->GetID(), 1);
                EXPECT_DOUBLE_EQ(commuting_in[0].second, 0.50);

                EXPECT_EQ(commuting_out[0].first->GetID(), 1);
                EXPECT_DOUBLE_EQ(commuting_out[0].second, 0.75);
                EXPECT_EQ(commuting_out[1].first->GetID(), 3);
                EXPECT_DOUBLE_EQ(commuting_out[1].second, 0.5);
        }
        {
                auto commuting_in  = sortLoc(location3->CRefIncomingCommutes());
                auto commuting_out = sortLoc(location3->CRefOutgoingCommutes());
                EXPECT_EQ(commuting_out.size(), 0);
                EXPECT_EQ(commuting_in.size(), 2);

                EXPECT_EQ(commuting_in[0].first->GetID(), 1);
                EXPECT_DOUBLE_EQ(commuting_in[0].second, 0.25);
                EXPECT_EQ(commuting_in[1].first->GetID(), 2);
                EXPECT_DOUBLE_EQ(commuting_in[1].second, 0.5);
        }
}

TEST(GeoGridHDF5ReaderTest, emptyFileTest)
{
        auto pop = Population::Create();
        EXPECT_THROW(getGeoGridFromFile("test4.h5", pop.get()), H5::FileIException);
}

TEST(GeoGridHDF5ReaderTest, unexistingFileTest)
{
        auto pop = Population::Create();
        EXPECT_THROW(getGeoGridFromFile("unexistingFile.h5", pop.get()), H5::FileIException);
}

TEST(GeoGridHDF5ReaderTest, invalidTypeTest)
{
        auto pop = Population::Create();
        EXPECT_THROW(getGeoGridFromFile("test5.h5", pop.get()), Exception);
}

} // namespace
