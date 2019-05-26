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

#include "visualiser/model/PopData.h"
#include "visualiser/view/PopDataView.h"

/**
 * Test that verifies whether or not the PopDataView produces a correct QVariantMap
 * that contains all the data contained in the corresponding PopData object.
 */
TEST(TestPopDataView, test1)
{
        // create PopData
        stride::visualiser::PopData popdata;

        // set some members

        popdata.college.pop           = 123456789;
        popdata.daycare.immune        = 0.5;
        popdata.household.symptomatic = 0.56;
        popdata.k12school.recovered   = 0.999;
        popdata.preschool.susceptible = 1.0;
        popdata.primCom.infected      = 0.65982;
        popdata.secCom.infectious     = 0.36;
        popdata.total.recovered       = 0.75;
        popdata.workplace.symptomatic = 0.33;

        // create view
        QVariantMap qml_map = stride::visualiser::PopDataView{popdata}.GetQVariantMap();

        EXPECT_EQ(popdata.college.pop, qml_map["college"].toMap()["pop"]);
        EXPECT_EQ(popdata.daycare.immune, qml_map["daycare"].toMap()["immune"]);
        EXPECT_EQ(popdata.household.symptomatic, qml_map["household"].toMap()["symptomatic"]);

        EXPECT_EQ(popdata.k12school.recovered, qml_map["k12school"].toMap()["recovered"]);
        EXPECT_EQ(popdata.preschool.susceptible, qml_map["preschool"].toMap()["susceptible"]);
        EXPECT_EQ(popdata.primCom.infected, qml_map["primCom"].toMap()["infected"]);

        EXPECT_EQ(popdata.secCom.infectious, qml_map["secCom"].toMap()["infectious"]);
        EXPECT_EQ(popdata.total.recovered, qml_map["total"].toMap()["recovered"]);
        EXPECT_EQ(popdata.workplace.symptomatic, qml_map["workplace"].toMap()["symptomatic"]);
}
