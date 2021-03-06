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
 *  Copyright 2019, Jan Broeckhove.
 */

#include "geopop/generators/Generator.h"
#include "geopop/populators/Populator.h"

#include "geopop/Coordinate.h"
#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/SimLocation.h"
#include "pop/Population.h"
#include "util/Exception.h"
#include "util/RnMan.h"

#include <gtest/gtest.h>
#include <map>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

class HouseholdPopulatorTest : public testing::Test
{
public:
        HouseholdPopulatorTest()
            : m_rn_man(RnInfo()), m_household_populator(m_rn_man), m_gg_config(), m_pop(Population::Create()),
              m_geo_grid(m_pop->RefGeoGrid()), m_household_generator(m_rn_man)
        {
        }

protected:
        RnMan                  m_rn_man;
        HouseholdPopulator     m_household_populator;
        GeoGridConfig          m_gg_config;
        shared_ptr<Population> m_pop;
        GeoGrid&               m_geo_grid;
        HouseholdGenerator     m_household_generator;
};

TEST_F(HouseholdPopulatorTest, OneHouseholdTest)
{
        m_gg_config.refHH.ages[0] = vector<vector<unsigned int>>{{8U}};

        auto loc1 = make_shared<SimLocation>(1, 4, Coordinate(0, 0), "Antwerpen", 2500);
        m_household_generator.AddPools(*loc1, m_pop.get(), m_gg_config);

        m_geo_grid.AddLocation(loc1);
        m_household_populator.Apply(m_geo_grid, m_gg_config);

        const auto& hPools = loc1->RefPools(Id::Household);
        ASSERT_EQ(hPools.size(), 1);
        ASSERT_EQ(hPools[0]->size(), 1);
}

TEST_F(HouseholdPopulatorTest, ZeroHouseholdsTest)
{
        EXPECT_NO_THROW(m_household_populator.Apply(m_geo_grid, m_gg_config));
}

TEST_F(HouseholdPopulatorTest, FiveHouseholdsTest)
{
        m_gg_config.refHH.ages[0] = vector<vector<unsigned int>>{{18U}};

        auto loc1 = make_shared<SimLocation>(1, 4, Coordinate(0, 0), "Antwerpen", 2500);

        for (unsigned int i = 0U; i < 5U; ++i) {
                m_household_generator.AddPools(*loc1, m_pop.get(), m_gg_config);
        }

        m_geo_grid.AddLocation(loc1);
        m_household_populator.Apply(m_geo_grid, m_gg_config);

        for (const auto& hPool : loc1->RefPools(Id::Household)) {
                ASSERT_EQ(hPool->size(), 1);
                EXPECT_EQ((*hPool)[0]->GetAge(), 18);
        }
}

TEST_F(HouseholdPopulatorTest, MultipleHouseholdTypesTest)
{
        m_gg_config.refHH.ages[0] = vector<vector<unsigned int>>{{18U}, {12U, 56U}};

        const auto loc1 = make_shared<SimLocation>(1, 4, Coordinate(0, 0), "Antwerpen", 2500);
        m_household_generator.AddPools(*loc1, m_pop.get(), m_gg_config);
        m_household_generator.AddPools(*loc1, m_pop.get(), m_gg_config);

        m_geo_grid.AddLocation(loc1);
        m_household_populator.Apply(m_geo_grid, m_gg_config);

        const auto& hPools = loc1->RefPools(Id::Household);
        const auto& pool1  = *hPools[0];
        const auto& pool2  = *hPools[1];

        ASSERT_EQ(hPools.size(), 2);
        ASSERT_EQ(pool1.size(), 1);
        EXPECT_EQ(pool1[0]->GetAge(), 18);
        ASSERT_EQ(pool2.size(), 2);
        EXPECT_EQ(pool2[0]->GetAge(), 12);
        EXPECT_EQ(pool2[1]->GetAge(), 56);
}

TEST_F(HouseholdPopulatorTest, OneHouseholdMultiDrawTest)
{
        m_gg_config.refHH.ages[1] = vector<vector<unsigned int>>{{8U}};
        m_gg_config.refHH.multiHH = true;
        auto loc1                 = make_shared<SimLocation>(1, 4, Coordinate(0, 0), "Antwerpen", 2500);
        m_household_generator.AddPools(*loc1, m_pop.get(), m_gg_config);

        m_geo_grid.AddLocation(loc1);
        m_household_populator.Apply(m_geo_grid, m_gg_config);

        const auto& hPools = loc1->RefPools(Id::Household);
        ASSERT_EQ(hPools.size(), 1);
        ASSERT_EQ(hPools[0]->size(), 1);
}

TEST_F(HouseholdPopulatorTest, FiveHouseholdsMultiDrawTest)
{
        m_gg_config.refHH.ages[1] = vector<vector<unsigned int>>{{18U}};
        m_gg_config.refHH.multiHH = true;

        auto loc1 = make_shared<SimLocation>(1, 4, Coordinate(0, 0), "Antwerpen", 2500);

        for (unsigned int i = 0U; i < 5U; ++i) {
                m_household_generator.AddPools(*loc1, m_pop.get(), m_gg_config);
        }

        m_geo_grid.AddLocation(loc1);
        m_household_populator.Apply(m_geo_grid, m_gg_config);

        for (const auto& hPool : loc1->RefPools(Id::Household)) {
                ASSERT_EQ(hPool->size(), 1);
                EXPECT_EQ((*hPool)[0]->GetAge(), 18);
        }
}

TEST_F(HouseholdPopulatorTest, MultipleHouseholdTypesMultiDrawTest)
{
        m_gg_config.refHH.ages[1] = vector<vector<unsigned int>>{{18U}, {12U, 56U}};
        m_gg_config.refHH.multiHH = true;

        const auto loc1 = make_shared<SimLocation>(1, 4, Coordinate(0, 0), "Antwerpen", 2500);
        m_household_generator.AddPools(*loc1, m_pop.get(), m_gg_config);
        m_household_generator.AddPools(*loc1, m_pop.get(), m_gg_config);

        m_geo_grid.AddLocation(loc1);
        m_household_populator.Apply(m_geo_grid, m_gg_config);

        const auto& hPools = loc1->RefPools(Id::Household);
        const auto& pool1  = *hPools[0];
        const auto& pool2  = *hPools[1];

        ASSERT_EQ(hPools.size(), 2);
        ASSERT_EQ(pool1.size(), 1);
        EXPECT_EQ(pool1[0]->GetAge(), 18);
        ASSERT_EQ(pool2.size(), 2);
        EXPECT_EQ(pool2[0]->GetAge(), 12);
        EXPECT_EQ(pool2[1]->GetAge(), 56);
}

TEST_F(HouseholdPopulatorTest, MultiDrawCityTest)
{
        m_gg_config.refHH.ages[1] = vector<vector<unsigned int>>{{12U, 56U}};
        m_gg_config.refHH.ages[2] = vector<vector<unsigned int>>{{19U, 15U, 40U}};
        m_gg_config.refHH.multiHH = true;

        const auto loc1 = make_shared<SimLocation>(1, 4, Coordinate(0, 0), "Antwerpen", 2500);
        const auto loc2 = make_shared<SimLocation>(2, 1, Coordinate(0, 0), "Leuven", 5000);
        m_household_generator.AddPools(*loc1, m_pop.get(), m_gg_config);
        m_household_generator.AddPools(*loc2, m_pop.get(), m_gg_config);

        m_geo_grid.AddLocation(loc1);
        m_geo_grid.AddLocation(loc2);
        m_household_populator.Apply(m_geo_grid, m_gg_config);

        const auto& hPools  = loc1->RefPools(Id::Household);
        const auto& hPools2 = loc2->RefPools(Id::Household);
        ASSERT_EQ(hPools.size(), 1);
        ASSERT_EQ(hPools2.size(), 1);
        const auto& pool1 = *hPools[0];
        const auto& pool2 = *hPools2[0];

        ASSERT_EQ(pool1.size(), 2);
        EXPECT_EQ(pool1[0]->GetAge(), 12);
        EXPECT_EQ(pool1[1]->GetAge(), 56);
        ASSERT_EQ(pool2.size(), 3);
        EXPECT_EQ(pool2[0]->GetAge(), 19);
        EXPECT_EQ(pool2[1]->GetAge(), 15);
        EXPECT_EQ(pool2[2]->GetAge(), 40);
}

TEST_F(HouseholdPopulatorTest, MultiDrawProvinceTest)
{
        m_gg_config.refHH.ages[1] = vector<vector<unsigned int>>{{12U, 56U}};
        m_gg_config.refHH.ages[2] = vector<vector<unsigned int>>{{19U, 15U, 40U}};
        m_gg_config.refHH.multiHH = true;

        const auto loc1 = make_shared<SimLocation>(4, 1, Coordinate(0, 0), "Antwerpen", 2500);
        const auto loc2 = make_shared<SimLocation>(5, 2, Coordinate(0, 0), "Leuven", 5000);
        m_household_generator.AddPools(*loc1, m_pop.get(), m_gg_config);
        m_household_generator.AddPools(*loc2, m_pop.get(), m_gg_config);

        m_geo_grid.AddLocation(loc1);
        m_geo_grid.AddLocation(loc2);
        m_household_populator.Apply(m_geo_grid, m_gg_config);

        const auto& hPools  = loc1->RefPools(Id::Household);
        const auto& hPools2 = loc2->RefPools(Id::Household);
        ASSERT_EQ(hPools.size(), 1);
        ASSERT_EQ(hPools2.size(), 1);
        const auto& pool1 = *hPools[0];
        const auto& pool2 = *hPools2[0];

        ASSERT_EQ(pool1.size(), 2);
        EXPECT_EQ(pool1[0]->GetAge(), 12);
        EXPECT_EQ(pool1[1]->GetAge(), 56);
        ASSERT_EQ(pool2.size(), 3);
        EXPECT_EQ(pool2[0]->GetAge(), 19);
        EXPECT_EQ(pool2[1]->GetAge(), 15);
        EXPECT_EQ(pool2[2]->GetAge(), 40);
}

TEST_F(HouseholdPopulatorTest, MultiDrawDefaultTest)
{
        m_gg_config.refHH.ages[1] = vector<vector<unsigned int>>{{12U, 56U}};
        m_gg_config.refHH.ages[0] = vector<vector<unsigned int>>{{19U, 15U, 40U}};
        m_gg_config.refHH.multiHH = true;

        const auto loc1 = make_shared<SimLocation>(4, 2, Coordinate(0, 0), "Antwerpen", 2500);
        const auto loc2 = make_shared<SimLocation>(5, 2, Coordinate(0, 0), "Leuven", 5000);
        m_household_generator.AddPools(*loc1, m_pop.get(), m_gg_config);
        m_household_generator.AddPools(*loc2, m_pop.get(), m_gg_config);

        m_geo_grid.AddLocation(loc1);
        m_geo_grid.AddLocation(loc2);
        m_household_populator.Apply(m_geo_grid, m_gg_config);

        const auto& hPools  = loc1->RefPools(Id::Household);
        const auto& hPools2 = loc2->RefPools(Id::Household);
        ASSERT_EQ(hPools.size(), 1);
        ASSERT_EQ(hPools2.size(), 1);
        const auto& pool1 = *hPools[0];
        const auto& pool2 = *hPools2[0];

        ASSERT_EQ(pool1.size(), 3);
        EXPECT_EQ(pool1[0]->GetAge(), 19);
        EXPECT_EQ(pool1[1]->GetAge(), 15);
        EXPECT_EQ(pool1[2]->GetAge(), 40);
        ASSERT_EQ(pool2.size(), 3);
        EXPECT_EQ(pool2[0]->GetAge(), 19);
        EXPECT_EQ(pool2[1]->GetAge(), 15);
        EXPECT_EQ(pool2[2]->GetAge(), 40);
}

TEST_F(HouseholdPopulatorTest, MultiDrawErrorTest)
{
        m_gg_config.refHH.multiHH = true;

        const auto loc = make_shared<SimLocation>(5, 3, Coordinate(0, 0), "Leuven", 5000);
        m_household_generator.AddPools(*loc, m_pop.get(), m_gg_config);

        m_geo_grid.AddLocation(loc);
        EXPECT_THROW(m_household_populator.Apply(m_geo_grid, m_gg_config), stride::util::Exception);
}
