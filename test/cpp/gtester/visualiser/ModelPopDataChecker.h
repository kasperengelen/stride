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
#include "geopop/VisLocation.h"
#include "geopop/PopStats.h"

using stride::visualiser::Model;
using geopop::PoolStats;
using namespace stride;

/**
 * Check if the values within a population section are correct.
 * @param pool_id The index that the pool type has inside the ContractType::IdList has.
 * @param popsection The population that will be checked.
 */
inline void TestPoolStats(const unsigned int pool_id, const PoolStats popsection)
{
    EXPECT_EQ(pool_id, popsection.population);
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
    const auto& pop = loc.GetPopStats();

    TestPoolStats(1, pop.GetPool(ContactType::Id::Household));
    TestPoolStats(2, pop.GetPool(ContactType::Id::K12School));
    TestPoolStats(3, pop.GetPool(ContactType::Id::College));
    TestPoolStats(4, pop.GetPool(ContactType::Id::Workplace));
    TestPoolStats(5, pop.GetPool(ContactType::Id::PrimaryCommunity));
    TestPoolStats(6, pop.GetPool(ContactType::Id::SecondaryCommunity));
    TestPoolStats(7, pop.GetPool(ContactType::Id::Daycare));
    TestPoolStats(8, pop.GetPool(ContactType::Id::PreSchool));
}

