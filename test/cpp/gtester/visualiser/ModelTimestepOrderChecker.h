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
 * Test file for reading timesteps in the correct order.
 */

#pragma once

#include <gtest/gtest.h>

#include "VisualiserTestfileGetter.h"

/**
 * Function that tests if the reader of the specified type correctly reads an epi-data file.
 * @tparam ReaderType The type of the EpiReader that will be tested.
 */
template <typename ReaderType>
void TestTimestepOrder()
{
        // read file into model
        Model      model{};
        ReaderType reader{GetTimestepTestfile<ReaderType>()};
        reader.ReadIntoModel(model);

        // check the amount of timesteps
        EXPECT_EQ(5, model.GetEpiData().size());

        // check if the first timestep has 4 locations
        EXPECT_EQ(4, model.GetEpiData().at(0)->size());

        // check if the second timestep has 8 locations
        EXPECT_EQ(8, model.GetEpiData().at(1)->size());

        // check if the third timestep has 6 locations
        EXPECT_EQ(6, model.GetEpiData().at(2)->size());
}
