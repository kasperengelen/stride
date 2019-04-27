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

#include "geopop/io/WorkplaceCSVReader.h"
#include "geopop/GeoGridConfig.h"

#include <gtest/gtest.h>
#include <memory>
#include <vector>

using namespace std;
using namespace geopop;
using namespace stride;

namespace {

TEST(WorkplaceCSVReader, test1)
{
        string csvString =
            R"(ratio,min_size,max_size
0.778532842256952,1,9
0.171901116625764,10,49
0.041003902521095,50,199
0.00856213859619,200,400

)";

        GeoGridConfig      geoConfig{};
        auto               instream = make_unique<istringstream>(csvString);
        WorkplaceCSVReader reader(move(instream));

        reader.SetWorkplaceData(geoConfig.workplaceSD.ratios, geoConfig.workplaceSD.sizes);

        const vector<double>& ratios = geoConfig.workplaceSD.ratios;

        EXPECT_EQ(ratios.size(), 4U);
        EXPECT_EQ(ratios[0], 0.778532842256952);
        EXPECT_EQ(ratios[1], 0.171901116625764);
        EXPECT_EQ(ratios[2], 0.041003902521095);
        EXPECT_EQ(ratios[3], 0.00856213859619);

        const vector<tuple<unsigned int, unsigned int>>& sizes = geoConfig.workplaceSD.sizes;

        EXPECT_EQ(sizes.size(), 4U);
        EXPECT_EQ(get<0>(sizes[0]), 1);
        EXPECT_EQ(get<1>(sizes[0]), 9);

        EXPECT_EQ(get<0>(sizes[1]), 10);
        EXPECT_EQ(get<1>(sizes[1]), 49);

        EXPECT_EQ(get<0>(sizes[2]), 50);
        EXPECT_EQ(get<1>(sizes[2]), 199);

        EXPECT_EQ(get<0>(sizes[3]), 200);
        EXPECT_EQ(get<1>(sizes[3]), 400);
}
} // namespace
