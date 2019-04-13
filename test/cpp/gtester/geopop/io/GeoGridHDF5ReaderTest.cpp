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

//#include "geopop/io/GeoGridHDF5Reader.h"

#include "contact/ContactType.h"
#include "geopop/GeoGrid.h"
#include "pop/Population.h"
#include "util/Exception.h"
#include "util/FileSys.h"

#include <fstream>
#include <gtest/gtest.h>
#include <memory>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

using boost::geometry::get;

namespace {
TEST(GeoGridHDF5ReaderTest, locationTest) {}
TEST(GeoGridHDF5ReaderTest, contactPoolsTest) {}
TEST(GeoGridHDF5ReaderTest, peopleTest) {}
TEST(GeoGridHDF5ReaderTest, commutesTest) {}
TEST(GeoGridHDF5ReaderTest, emptyStreamTest) {}
TEST(GeoGridHDF5ReaderTest, invalidTypeTest) {}
TEST(GeoGridHDF5ReaderTest, invalidPersonTest) {}
TEST(GeoGridHDF5ReaderTest, invalidHDF5Test) {}

} // namespace
