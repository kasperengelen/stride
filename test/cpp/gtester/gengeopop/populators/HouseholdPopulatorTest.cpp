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

#include "gengeopop/Household.h"
#include "gengeopop/populators/HouseholdPopulator.h"
#include "util/LogUtils.h"
#include "util/RnMan.h"

#include <gtest/gtest.h>

using namespace std;
using namespace gengeopop;
using namespace stride;
using namespace stride::util;

class HouseholdPopulatorTest : public testing::Test
{
public:
        HouseholdPopulatorTest() : householdPopulator(), rnManager(), config() {}

protected:
        void SetUp() override
        {
                rnManager = make_shared<RnMan>();
                auto logger = LogUtils::CreateCliLogger("stride_logger", "stride_log.txt");
                logger->set_level(spdlog::level::off);
                householdPopulator = make_shared<HouseholdPopulator>(*rnManager.get(), logger);
        }

        shared_ptr<HouseholdPopulator>  householdPopulator;
        shared_ptr<RnMan>               rnManager;
        GeoGridConfig                   config;
};

TEST_F(HouseholdPopulatorTest, OneHouseholdTest)
{
        auto householdType = make_shared<Household>();
        auto poolType      = new ContactPool(0, ContactPoolType::Id::Household);
        auto personType    = make_shared<Person>();
        personType->SetAge(18);
        poolType->AddMember(personType.get());
        householdType->AddPool(poolType);
        config.generated.household_types.push_back(householdType);

        auto pop = Population::Create();
        auto geoGrid   = make_shared<GeoGrid>(pop.get());
        auto loc1      = make_shared<Location>(1, 4, 2500, Coordinate(0, 0), "Antwerpen");
        auto household = make_shared<Household>();
        household->AddPool(new ContactPool(0, ContactPoolType::Id::Household));
        loc1->AddContactCenter(household);
        geoGrid->AddLocation(loc1);

        householdPopulator->Apply(geoGrid, config);

        const auto& pools = household->GetPools();
        ASSERT_EQ(pools.size(), 1);
        EXPECT_EQ(pools[0]->GetSize(), 1);
}

TEST_F(HouseholdPopulatorTest, ZeroHouseholdsTest)
{
        auto pop = Population::Create();
        auto geoGrid = make_shared<GeoGrid>(pop.get());

        EXPECT_NO_THROW(householdPopulator->Apply(geoGrid, config));
}

TEST_F(HouseholdPopulatorTest, FiveHouseholdsTest)
{
        auto householdType = make_shared<Household>();
        auto poolType      = new ContactPool(0, ContactPoolType::Id::Household);
        auto personType    = make_shared<Person>();
        personType->SetAge(18);
        poolType->AddMember(personType.get());
        householdType->AddPool(poolType);
        config.generated.household_types.push_back(householdType);

        auto pop = Population::Create();
        auto geoGrid = make_shared<GeoGrid>(pop.get());
        auto loc1    = make_shared<Location>(1, 4, 2500, Coordinate(0, 0), "Antwerpen");

        auto household1 = make_shared<Household>();
        household1->AddPool(new ContactPool(0, ContactPoolType::Id::Household));
        loc1->AddContactCenter(household1);
        auto household2 = make_shared<Household>();
        household2->AddPool(new ContactPool(0, ContactPoolType::Id::Household));
        loc1->AddContactCenter(household2);
        auto household3 = make_shared<Household>();
        household3->AddPool(new ContactPool(0, ContactPoolType::Id::Household));
        loc1->AddContactCenter(household3);
        auto household4 = make_shared<Household>();
        household4->AddPool(new ContactPool(0, ContactPoolType::Id::Household));
        loc1->AddContactCenter(household4);
        auto household5 = make_shared<Household>();
        household5->AddPool(new ContactPool(0, ContactPoolType::Id::Household));
        loc1->AddContactCenter(household5);

        geoGrid->AddLocation(loc1);

        householdPopulator->Apply(geoGrid, config);

        for (const auto& household : *loc1) {
                ASSERT_EQ(household->GetPools().size(), 1);
                ASSERT_EQ(household->GetPools()[0]->GetSize(), 1);
                EXPECT_EQ((*household->GetPools()[0]->begin())->GetAge(), 18);
        }
}
TEST_F(HouseholdPopulatorTest, MultipleHouseholdTypesTest)
{
        shared_ptr<Person> personType1;
        shared_ptr<Person> personType2;
        shared_ptr<Person> personType3;

        {
                auto householdType = make_shared<Household>();
                auto poolType      = new ContactPool(0, ContactPoolType::Id::Household);
                personType1        = make_shared<Person>();
                personType1->SetAge(18);
                poolType->AddMember(personType1.get());
                householdType->AddPool(poolType);
                config.generated.household_types.push_back(householdType);
        }
        {
                auto householdType = make_shared<Household>();
                auto poolType      = new ContactPool(0, ContactPoolType::Id::Household);
                personType2        = make_shared<Person>();
                personType2->SetAge(12);
                poolType->AddMember(personType2.get());
                householdType->AddPool(poolType);
                personType3 = make_shared<Person>();
                personType3->SetAge(56);
                poolType->AddMember(personType3.get());
                config.generated.household_types.push_back(householdType);
        }

        auto pop = Population::Create();
        const auto geoGrid   = make_shared<GeoGrid>(pop.get());
        const auto loc1      = make_shared<Location>(1, 4, 2500, Coordinate(0, 0), "Antwerpen");
        const auto household = make_shared<Household>();
        household->AddPool(new ContactPool(0, ContactPoolType::Id::Household));
        loc1->AddContactCenter(household);
        geoGrid->AddLocation(loc1);
        auto household2 = make_shared<Household>();
        household2->AddPool(new ContactPool(0, ContactPoolType::Id::Household));
        loc1->AddContactCenter(household2);
        householdPopulator->Apply(geoGrid, config);

        map<int, vector<ContactPool*>> pools_map;
        pools_map[household->GetPools()[0]->GetSize()]  = household->GetPools();
        pools_map[household2->GetPools()[0]->GetSize()] = household2->GetPools();
        {
                const auto& pools = pools_map[1];
                ASSERT_EQ(pools.size(), 1);
                EXPECT_EQ(pools[0]->GetSize(), 1);
                EXPECT_EQ((*pools[0]->begin())->GetAge(), 18);
        }
        {
                const auto& pools = pools_map[2];
                ASSERT_EQ(pools.size(), 1);
                EXPECT_EQ(pools[0]->GetSize(), 2);
                EXPECT_EQ((*pools[0]->begin())->GetAge(), 12);
                EXPECT_EQ((*(pools[0]->begin() + 1))->GetAge(), 56);
        }
}
