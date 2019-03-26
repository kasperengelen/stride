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

//#include "geopop/generators/PreschoolGenerator.h"

#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/Location.h"
#include "pop/Population.h"
#include "util/RnMan.h"

#include <gtest/gtest.h>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

namespace {

class PreschoolGeneratorTest : public testing::Test
{
public:
        PreschoolGeneratorTest()
            : m_rn_man(RnInfo()), m_geogrid_config(), m_pop(Population::Create()),
              m_geo_grid(m_pop.get())
//            : m_rn_man(RnInfo()), m_preschool_generator(m_rn_man), m_geogrid_config(), m_pop(Population::Create()),
//              m_geo_grid(m_pop.get())

        {
        }

protected:
        RnMan                  m_rn_man;
//        PreschoolGenerator     m_preschool_generator;
        GeoGridConfig          m_geogrid_config;
        shared_ptr<Population> m_pop;
        GeoGrid                m_geo_grid;
};

TEST(PreschoolGeneratorTest, OneLocationTest) {}
TEST(PreschoolGeneratorTest, ZeroLocationTest) {}
TEST(PreschoolGeneratorTest, MultipleLocationsTest) {}
TEST(PreschoolGeneratorTest, ParticipationTest) {}

} // namespace
