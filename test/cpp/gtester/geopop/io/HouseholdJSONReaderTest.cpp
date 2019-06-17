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
 *  Copyright 2018, 2019, Jan Broeckhove and Bistromatics group.
 */

#include "geopop/io/HouseholdJSONReader.h"
#include "geopop/GeoGridConfig.h"
#include "util/FileSys.h"

#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::util;

namespace {

TEST(HouseholdJSONReader, test1)
{
        GeoGridConfig geoConfig{};
        auto          file = make_unique<ifstream>();
        file->open(FileSys::GetTestsDir().string() + "/testdata/io/household.json");
        HouseholdJSONReader reader(move(file));

        reader.SetReferenceHouseholds(geoConfig.refHH.person_count[0], geoConfig.refHH.ages[0]);

        EXPECT_EQ(geoConfig.refHH.person_count[0], 23U);

        const vector<vector<unsigned int>>& HHages = geoConfig.refHH.ages[0];

        EXPECT_EQ(HHages.size(), 8U);
        EXPECT_EQ(HHages[0].size(), 3U);
        EXPECT_EQ(HHages[1].size(), 2U);
        EXPECT_EQ(HHages[2].size(), 4U);
        EXPECT_EQ(HHages[3].size(), 2U);
        EXPECT_EQ(HHages[4].size(), 2U);
        EXPECT_EQ(HHages[5].size(), 4U);
        EXPECT_EQ(HHages[6].size(), 4U);
        EXPECT_EQ(HHages[7].size(), 2U);

        EXPECT_EQ(HHages[0][0], 42U);
        EXPECT_EQ(HHages[0][1], 38U);
        EXPECT_EQ(HHages[0][2], 15U);

        EXPECT_EQ(HHages[1][0], 70U);
        EXPECT_EQ(HHages[1][1], 68U);

        EXPECT_EQ(HHages[6][0], 35U);
        EXPECT_EQ(HHages[6][1], 32U);
        EXPECT_EQ(HHages[6][2], 6U);
        EXPECT_EQ(HHages[6][3], 3U);

        EXPECT_EQ(HHages[7][0], 78U);
        EXPECT_EQ(HHages[7][1], 75U);
}

} // namespace
