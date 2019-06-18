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

/**
 * @file
 * Test file for the PopDataView class.
 */

#include <gtest/gtest.h>

#include "geopop/PopStats.h"
#include "visualiser/view/PopStatsView.h"

using stride::ContactType::Id;

/**
 * Test that verifies whether or not the PopDataView produces a correct QVariantMap
 * that contains all the data contained in the corresponding PopData object.
 */
TEST(TestEpiOutputPopDataView, test1)
{
        // create PopStats
        geopop::PopStats popstats;

        popstats.GetPool(Id::College).population            = 123456789;
        popstats.GetPool(Id::Daycare).immune                = 0.5;
        popstats.GetPool(Id::Household).symptomatic         = 0.56;
        popstats.GetPool(Id::K12School).recovered           = 0.999;
        popstats.GetPool(Id::PreSchool).susceptible         = 1.0;
        popstats.GetPool(Id::PrimaryCommunity).infected     = 0.65982;
        popstats.GetPool(Id::SecondaryCommunity).infectious = 0.36;
        popstats.GetPool(Id::Workplace).recovered           = 0.33;

        // create view
        QVariantMap qml_map = stride::visualiser::PopStatsView{popstats}.GetQVariantMap();

        EXPECT_EQ(popstats.GetPool(Id::College).population, qml_map["college"].toMap()["pop"]);
        EXPECT_EQ(popstats.GetPool(Id::Daycare).immune, qml_map["daycare"].toMap()["immune"]);
        EXPECT_EQ(popstats.GetPool(Id::Household).symptomatic, qml_map["household"].toMap()["symptomatic"]);

        EXPECT_EQ(popstats.GetPool(Id::K12School).recovered, qml_map["k12school"].toMap()["recovered"]);
        EXPECT_EQ(popstats.GetPool(Id::PreSchool).susceptible, qml_map["preschool"].toMap()["susceptible"]);
        EXPECT_EQ(popstats.GetPool(Id::PrimaryCommunity).infected, qml_map["primCom"].toMap()["infected"]);

        EXPECT_EQ(popstats.GetPool(Id::SecondaryCommunity).infectious, qml_map["secCom"].toMap()["infectious"]);
        EXPECT_EQ(popstats.GetPool(Id::Workplace).recovered, qml_map["workplace"].toMap()["recovered"]);
}
