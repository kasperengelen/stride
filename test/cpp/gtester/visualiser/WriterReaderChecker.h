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
 * Test file for writer epi data and then reading it and checking it
 * is correct.
 */

#pragma once

#include "VisualiserTestfileGetter.h"

#include "pop/Population.h"
#include "gtester/geopop/io/GeoGridIOUtils.h"

using stride::util::FileSys;
using stride::Population;
using stride::visualiser::Model;

/**
 * Let the specified write write a list of geogrids to a file. Then let the
 * specified reader read that file.
 */
template <typename WriterType, typename ReaderType>
void CheckWriterReaderPair()
{
    constexpr unsigned int num_timesteps = 5;

    std::vector<unsigned int> timestep_sizes{};

    // create writer
    WriterType writer{GetWriterTestDir<WriterType>()};

    // for i = 0..4 (size=5)
    //      Create population
    //      Writer pop
    for(unsigned int i = 0; i < num_timesteps; i++)
    {
        auto pop = Population::Create();
        const auto& geogrid = GetPopulatedGeoGrid(pop.get());

        timestep_sizes.push_back(geogrid.size());

        writer.Update(pop);
    }

    // write file
    writer.Finish();

    // create reader
    //ReaderType reader{GetWriterTestFile<WriterType>()};
    ReaderType reader{GetWriterTestFile<WriterType>()};

    // create model
    Model model{};

    // read into model
    reader.ReadIntoModel(model);

    // check that the number of timesteps is correct
    EXPECT_EQ(num_timesteps, model.GetEpiData().size());

    for(unsigned int i = 0; i < num_timesteps; i++)
    {
        EXPECT_EQ(timestep_sizes.at(i), model.GetEpiData().at(i)->size());
    }
}
