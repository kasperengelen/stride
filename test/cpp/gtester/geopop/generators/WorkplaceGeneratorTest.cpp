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
 *  Copyright 2018, Jan Broeckhove and Bistromatics group.
 */

#include "geopop/generators/Generator.h"

#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/Location.h"
#include "pop/Population.h"
#include "util/RnMan.h"

#include <gtest/gtest.h>
#include <tuple>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

namespace {

class WorkplaceGeneratorTest : public testing::Test
{
public:
        WorkplaceGeneratorTest()
            : m_rn_man(RnInfo()), m_workplace_generator(m_rn_man), m_gg_config(), m_pop(Population::Create()),
              m_geo_grid(m_pop.get())
        {
        }

protected:
        RnMan                  m_rn_man;
        WorkplaceGenerator     m_workplace_generator;
        GeoGridConfig          m_gg_config;
        shared_ptr<Population> m_pop;
        GeoGrid                m_geo_grid;
        unsigned int           m_ppwp = m_gg_config.pools[Id::Workplace];
};

// Check that generator can handle empty GeoGrid.
TEST_F(WorkplaceGeneratorTest, ZeroLocationTest)
{

        m_gg_config.param.pop_size           = 10000;
        m_gg_config.info.popcount_college    = 20000;

        m_gg_config.workplaceSD.ratios = {0.778532842256952, 0.171901116625764, 0.0410039025210945,0.00856213859618965};
        m_gg_config.workplaceSD.sizes  = {make_pair(1,9), make_pair(10,49), make_pair(50,199),make_pair(200,400)};

        m_workplace_generator.Apply(m_geo_grid, m_gg_config);

        EXPECT_EQ(m_geo_grid.size(), 0);
}

// Check that situation without commutes is OK.
TEST_F(WorkplaceGeneratorTest, NoCommuting)
{
        m_gg_config.param.pop_size                     = 5 * 1000 * 1000;
        m_gg_config.info.popcount_workplace            = static_cast<unsigned int>(0.20 * 5 * 1000 * 1000);
        m_gg_config.param.particpation_workplace       = 0.20;
        m_gg_config.param.fraction_workplace_commuters = 0;
        m_gg_config.workplaceSD.ratios = {0.778532842256952, 0.171901116625764, 0.0410039025210945,0.00856213859618965};
        m_gg_config.workplaceSD.sizes  = {make_pair(1,9), make_pair(10,49), make_pair(50,199),make_pair(200,400)};


        array<unsigned int, 50> sizes{128331, 50784,  191020, 174476, 186595, 105032, 136388, 577,   111380, 171014,
                                      63673,  49438,  45590,  164666, 185249, 141389, 82525,  40397, 123307, 168128,
                                      172937, 15581,  22891,  143505, 178516, 116959, 144659, 20775, 156009, 91951,
                                      49823,  181594, 119075, 27700,  116959, 146583, 102531, 58864, 76946,  91951,
                                      134464, 59248,  10003,  125423, 15004,  8656,   13658,  50784, 61749,  165243};
        for (const auto size : sizes) {
                m_geo_grid.AddLocation(make_shared<Location>(1, 4, Coordinate(0, 0), "Size: " + to_string(size), size));
        }
        m_workplace_generator.Apply(m_geo_grid, m_gg_config);

        array<unsigned int, sizes.size()> expected{
                1588, 612,  2352, 2160, 2299, 1286, 1574, 7,    1350, 2080, 784, 529,  523,  1969, 2267, 1742, 953,
                462,  1416, 1984, 2055, 188,  248,  1731, 2161, 1415, 1762, 242, 1868, 1124, 571,  2135, 1393, 321,
                1437, 1731, 1278, 708,  972,  1053, 1640, 707,  120,  1554, 163, 111,  167,  616,  728,  1970};

        for (auto i = 0U; i < sizes.size(); i++) {
                EXPECT_EQ(expected[i] * m_ppwp, m_geo_grid[i]->CRefPools(Id::Workplace).size());
        }
}

// As many commutes from A to B as from B to A.
TEST_F(WorkplaceGeneratorTest, NullCommuting)
{
        m_gg_config.param.pop_size                     = 5 * 1000 * 1000;
        m_gg_config.info.popcount_workplace            = static_cast<unsigned int>(0.20 * 5 * 1000 * 1000);
        m_gg_config.param.particpation_workplace       = 0.20;
        m_gg_config.param.fraction_workplace_commuters = 0.10;

        m_gg_config.workplaceSD.ratios = {0.778532842256952, 0.171901116625764, 0.0410039025210945,0.00856213859618965};
        m_gg_config.workplaceSD.sizes  = {make_pair(1,9), make_pair(10,49), make_pair(50,199),make_pair(200,400)};

        array<unsigned int, 50> sizes{128331, 50784,  191020, 174476, 186595, 105032, 136388, 577,   111380, 171014,
                                      63673,  49438,  45590,  164666, 185249, 141389, 82525,  40397, 123307, 168128,
                                      172937, 15581,  22891,  143505, 178516, 116959, 144659, 20775, 156009, 91951,
                                      49823,  181594, 119075, 27700,  116959, 146583, 102531, 58864, 76946,  91951,
                                      134464, 59248,  10003,  125423, 15004,  8656,   13658,  50784, 61749,  165243};
        for (const auto size : sizes) {
                m_geo_grid.AddLocation(make_shared<Location>(1, 4, Coordinate(0, 0), "Size: " + to_string(size), size));
        }

        // 10% of the pop of loc0 is commuting to loc1 = 12.833 people
        m_geo_grid[0]->AddOutgoingCommute(m_geo_grid[1], 0.10); // 0.10 is relative to loc0
        m_geo_grid[1]->AddIncomingCommute(m_geo_grid[0], 0.10); // 0.10 is relative to loc0

        // 25,26% of the pop of loc1 is commuting to loc0 = 12.833 people
        m_geo_grid[1]->AddOutgoingCommute(m_geo_grid[0], 0.252697700063012); // 0.25 is relative to loc1
        m_geo_grid[0]->AddIncomingCommute(m_geo_grid[1], 0.252697700063012); // 0.25 is relative to loc1

        EXPECT_EQ(1283, m_geo_grid[0]->GetIncomingCommuteCount(m_gg_config.param.fraction_workplace_commuters));
        EXPECT_EQ(1283, m_geo_grid[0]->GetOutgoingCommuteCount(m_gg_config.param.fraction_workplace_commuters));
        EXPECT_EQ(1283, m_geo_grid[1]->GetIncomingCommuteCount(m_gg_config.param.fraction_workplace_commuters));
        EXPECT_EQ(1283, m_geo_grid[1]->GetOutgoingCommuteCount(m_gg_config.param.fraction_workplace_commuters));

        m_workplace_generator.Apply(m_geo_grid, m_gg_config);

        array<unsigned int, sizes.size()> expected{
                1598, 623,  2365, 2154, 2291, 1285, 1571, 5,    1352, 2091, 780, 519,  519,  1979, 2264, 1738, 954,
                459,  1419, 1984, 2054, 181,  255,  1726, 2160, 1419, 1762, 245, 1862, 1128, 568,  2132, 1397, 319,
                1436, 1731, 1278, 708,  969,  1053, 1641, 704,  121,  1553, 163, 112,  166,  615,  728,  1970};

        for (auto i = 0U; i < sizes.size(); i++) {
                EXPECT_EQ(expected[i] * m_ppwp, m_geo_grid[i]->CRefPools(Id::Workplace).size());
        }
}

TEST_F(WorkplaceGeneratorTest, TenCommuting)
{
        m_gg_config.param.pop_size                     = 5 * 1000 * 1000;
        m_gg_config.info.popcount_workplace            = static_cast<unsigned int>(0.20 * 5 * 1000 * 1000);
        m_gg_config.param.particpation_workplace       = 0.20;
        m_gg_config.param.fraction_workplace_commuters = 0.10;

        m_gg_config.workplaceSD.ratios = {0.778532842256952, 0.171901116625764, 0.0410039025210945,0.00856213859618965};
        m_gg_config.workplaceSD.sizes  = {make_pair(1,9), make_pair(10,49), make_pair(50,199),make_pair(200,400)};

        array<unsigned int, 50> sizes{128331, 50784,  191020, 174476, 186595, 105032, 136388, 577,   111380, 171014,
                                      63673,  49438,  45590,  164666, 185249, 141389, 82525,  40397, 123307, 168128,
                                      172937, 15581,  22891,  143505, 178516, 116959, 144659, 20775, 156009, 91951,
                                      49823,  181594, 119075, 27700,  116959, 146583, 102531, 58864, 76946,  91951,
                                      134464, 59248,  10003,  125423, 15004,  8656,   13658,  50784, 61749,  165243};
        for (const auto size : sizes) {
                m_geo_grid.AddLocation(make_shared<Location>(1, 4, Coordinate(0, 0), "Size: " + to_string(size), size));
        }

        array<tuple<size_t, size_t, double>, 6> commuting{make_tuple(0, 10, 0.23),  make_tuple(25, 3, 0.43),
                                                          make_tuple(38, 17, 0.65), make_tuple(10, 17, 0.25),
                                                          make_tuple(15, 17, 0.22), make_tuple(27, 17, 0.47)};

        for (const auto& info : commuting) {
                m_geo_grid[get<0>(info)]->AddOutgoingCommute(m_geo_grid[get<1>(info)], get<2>(info));
                m_geo_grid[get<1>(info)]->AddIncomingCommute(m_geo_grid[get<0>(info)], get<2>(info));
        }

        // = 0,23 * 128331 * 0,10 = 2951,613
        EXPECT_EQ(2951, m_geo_grid[0]->GetOutgoingCommuteCount(m_gg_config.param.fraction_workplace_commuters));
        EXPECT_EQ(0, m_geo_grid[0]->GetIncomingCommuteCount(m_gg_config.param.fraction_workplace_commuters));

        // = 0,25 * 63673 * 0,10 = 1591,825
        EXPECT_EQ(1591, m_geo_grid[10]->GetOutgoingCommuteCount(m_gg_config.param.fraction_workplace_commuters));

        // = 0,23 * 128331 * 0,10 = 2951,613
        EXPECT_EQ(2951, m_geo_grid[10]->GetIncomingCommuteCount(m_gg_config.param.fraction_workplace_commuters));

        // = 0,43 * 116959 * 0,10 = 5029,023
        EXPECT_EQ(5029, m_geo_grid[25]->GetOutgoingCommuteCount(m_gg_config.param.fraction_workplace_commuters));
        EXPECT_EQ(0, m_geo_grid[25]->GetIncomingCommuteCount(m_gg_config.param.fraction_workplace_commuters));
        EXPECT_EQ(0, m_geo_grid[3]->GetOutgoingCommuteCount(m_gg_config.param.fraction_workplace_commuters));

        // = 0,43 * 116959 * 0,10 = 5029,023
        EXPECT_EQ(5029, m_geo_grid[3]->GetIncomingCommuteCount(m_gg_config.param.fraction_workplace_commuters));
        EXPECT_EQ(0, m_geo_grid[17]->GetOutgoingCommuteCount(m_gg_config.param.fraction_workplace_commuters));

        // = 0,10 * (0,65 * 76946 + 0,22 * 141389 + 0,47 * 20775 + 0,25*63673) = 10680,298
        EXPECT_EQ(10680, m_geo_grid[17]->GetIncomingCommuteCount(m_gg_config.param.fraction_workplace_commuters));

        // = 0,65 * 76946 * 0,10 = 5001,048
        EXPECT_EQ(5001, m_geo_grid[38]->GetOutgoingCommuteCount(m_gg_config.param.fraction_workplace_commuters));
        EXPECT_EQ(0, m_geo_grid[38]->GetIncomingCommuteCount(m_gg_config.param.fraction_workplace_commuters));

        // 0,22 * 141389 * 0,10 = 3310,558
        EXPECT_EQ(3110, m_geo_grid[15]->GetOutgoingCommuteCount(m_gg_config.param.fraction_workplace_commuters));
        EXPECT_EQ(0, m_geo_grid[15]->GetIncomingCommuteCount(m_gg_config.param.fraction_workplace_commuters));
        EXPECT_EQ(0, m_geo_grid[17]->GetOutgoingCommuteCount(m_gg_config.param.fraction_workplace_commuters));

        // = 0,10 * (0,65 * 76946 + 0,22  * 141389 + 0,47 * 20775 + 0,25* 63673) = 10680,298
        EXPECT_EQ(10680, m_geo_grid[17]->GetIncomingCommuteCount(m_gg_config.param.fraction_workplace_commuters));

        m_workplace_generator.Apply(m_geo_grid, m_gg_config);

        array<unsigned int, sizes.size()> expected{
                1571, 615,  2345, 2214, 2291, 1283, 1567, 6,    1349, 2077, 810, 522,  516,  1975, 2264, 1723, 954,
                563,  1444, 1985, 2032, 167,  263,  1713, 2170, 1410, 1742, 263, 1854, 1115, 564,  2131, 1387, 331,
                1428, 1738, 1261, 713,  937,  1057, 1642, 701,  116,  1549, 169, 108,  159,  625,  723,  1964};

        for (auto i = 0U; i < sizes.size(); i++) {
                EXPECT_EQ(expected[i] * m_ppwp, m_geo_grid[i]->CRefPools(Id::Workplace).size());
        }
}

} // namespace
