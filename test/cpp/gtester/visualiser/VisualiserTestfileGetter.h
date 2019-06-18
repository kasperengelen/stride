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
 * Header file for functions that retrieve testfile filenames for the epi-output reader tests.
 */

#pragma once

#include "util/FileSys.h"
#include "visualiser/readers/HDF5EpiReader.h"
#include "visualiser/readers/JSONEpiReader.h"
#include "visualiser/readers/ProtobufEpiReader.h"

using stride::util::FileSys;
using stride::visualiser::HDF5EpiReader;
using stride::visualiser::JSONEpiReader;
using stride::visualiser::ProtobufEpiReader;

template <typename ReaderType>
const std::string GetPopdataTestfile();

template <>
inline const std::string GetPopdataTestfile<HDF5EpiReader>()
{
        return FileSys::GetTestsDir().string() + "/testdata/EpiReader/test_popdata.h5";
}

template <>
inline const std::string GetPopdataTestfile<JSONEpiReader>()
{
        return FileSys::GetTestsDir().string() + "/testdata/EpiReader/test_popdata.json";
}

template <>
inline const std::string GetPopdataTestfile<ProtobufEpiReader>()
{
        return FileSys::GetTestsDir().string() + "/testdata/EpiReader/test_popdata.proto";
}

template <typename ReaderType>
const std::string GetTimestepTestfile();

template <>
inline const std::string GetTimestepTestfile<HDF5EpiReader>()
{
        return FileSys::GetTestsDir().string() + "/testdata/EpiReader/test_timestep_order.h5";
}

template <>
inline const std::string GetTimestepTestfile<JSONEpiReader>()
{
        return FileSys::GetTestsDir().string() + "/testdata/EpiReader/test_timestep_order.json";
}

template <>
inline const std::string GetTimestepTestfile<ProtobufEpiReader>()
{
        return FileSys::GetTestsDir().string() + "/testdata/EpiReader/test_timestep_order.proto";
}
