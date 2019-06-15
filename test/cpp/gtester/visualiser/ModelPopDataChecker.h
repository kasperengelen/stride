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
 * Test file for reading PopData.
 */

#pragma once

#include <gtest/gtest.h>

#include "VisualiserTestfileGetter.h"
#include "visualiser/model/PopData.h"

using stride::visualiser::Model;
using stride::visualiser::PopSection;

/**
 * Check if the values within a population section are correct.
 * @param pool_id The index that the pool type has inside the ContractType::IdList has.
 * @param popsection The population that will be checked.
 */
inline void TestPopSection(const unsigned int pool_id, const PopSection popsection)
{
    EXPECT_EQ(pool_id, popsection.pop);
    EXPECT_DOUBLE_EQ(pool_id + 0.1, popsection.immune);
    EXPECT_DOUBLE_EQ(pool_id + 0.2, popsection.infected);
    EXPECT_DOUBLE_EQ(pool_id + 0.3, popsection.infectious);
    EXPECT_DOUBLE_EQ(pool_id + 0.4, popsection.recovered);
    EXPECT_DOUBLE_EQ(pool_id + 0.5, popsection.susceptible);
    EXPECT_DOUBLE_EQ(pool_id + 0.6, popsection.symptomatic);
}

/**
 * Function that tests if the reader of the specified type correctly reads an epi-data file.
 * @tparam ReaderType The type of the EpiReader that will be tested.
 */
template <typename ReaderType>
inline void TestPopData()
{
    // read file into model
    Model model{};
    ReaderType reader{GetPopdataTestfile<ReaderType>()};
    reader.ReadIntoModel(model);

    // take first loc: model[0][0]
    const auto& loc = model.GetEpiData().at(0).at(0);
    const auto& pop = loc.GetPopData();

    TestPopSection(1, pop.household);
    TestPopSection(2, pop.k12school);
    TestPopSection(3, pop.college);
    TestPopSection(4, pop.workplace);
    TestPopSection(5, pop.primCom);
    TestPopSection(6, pop.secCom);
    TestPopSection(7, pop.daycare);
    TestPopSection(8, pop.preschool);
}

