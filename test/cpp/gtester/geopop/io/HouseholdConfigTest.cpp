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

TEST(HouseholdConfig, test1)
{
        GeoGridConfig geoConfig{};

        geoConfig.SetData(FileSys::GetTestsDir().string() + "/testdata/io/householdConfig.xml", false);


        EXPECT_EQ(geoConfig.refHH.person_count[1], 11U);
        EXPECT_EQ(geoConfig.refHH.person_count[2], 3U);

        const vector<vector<unsigned int>>& HHages1 = geoConfig.refHH.ages[1];
        const vector<vector<unsigned int>>& HHages2 = geoConfig.refHH.ages[2];

        EXPECT_EQ(HHages1.size(), 3U);
        EXPECT_EQ(HHages1[0].size(), 4U);
        EXPECT_EQ(HHages1[1].size(), 2U);
        EXPECT_EQ(HHages1[2].size(), 5U);

        EXPECT_EQ(HHages1[0][0], 20U);
        EXPECT_EQ(HHages1[0][1], 50U);
        EXPECT_EQ(HHages1[0][2], 2U);
        EXPECT_EQ(HHages1[0][3], 25U);

        EXPECT_EQ(HHages1[1][0], 2U);
        EXPECT_EQ(HHages1[1][1], 17U);

        EXPECT_EQ(HHages1[2][0], 34U);
        EXPECT_EQ(HHages1[2][1], 22U);
        EXPECT_EQ(HHages1[2][2], 12U);
        EXPECT_EQ(HHages1[2][3], 25U);
        EXPECT_EQ(HHages1[2][4], 44U);

        EXPECT_EQ(HHages2.size(), 1U);
        EXPECT_EQ(HHages2[0].size(), 3U);
        
        EXPECT_EQ(HHages2[0][0], 32U);
        EXPECT_EQ(HHages2[0][1], 3U);
        EXPECT_EQ(HHages2[0][2], 26U);
}

} // namespace
