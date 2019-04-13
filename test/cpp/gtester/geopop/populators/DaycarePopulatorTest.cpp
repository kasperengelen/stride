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

//#include "geopop/populators/DaycarePopulator.h"
//#include "geopop/generators/DaycareGenerator.h"

#include "MakeGeoGrid.h"
#include "contact/AgeBrackets.h"
#include "geopop/GeoGrid.h"
#include "geopop/GeoGridConfig.h"
#include "geopop/Location.h"
#include "pop/Population.h"
#include "util/LogUtils.h"
#include "util/RnMan.h"

#include <gtest/gtest.h>
#include <map>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

namespace {

class DaycarePopulatorTest : public testing::Test
{
public:
        DaycarePopulatorTest()
            //            : m_rn_man(RnInfo()), m_daycare_populator(m_rn_man), m_geogrid_config(),
            //            m_pop(Population::Create()),
            //              m_geo_grid(m_pop->RefGeoGrid()), m_daycare_generator(m_rn_man)
            : m_rn_man(RnInfo()), m_geogrid_config(), m_pop(Population::Create()), m_geo_grid(m_pop->RefGeoGrid())

        {
        }

protected:
        RnMan m_rn_man;
        //        DaycarePopulator       m_daycare_populator;
        GeoGridConfig          m_geogrid_config;
        shared_ptr<Population> m_pop;
        GeoGrid&               m_geo_grid;
        //        DaycareGenerator       m_daycare_generator;
        // const unsigned int     m_ppd = GeoGridConfig{}.pools.pools_per_daycare;
};

TEST_F(DaycarePopulatorTest, NoPopulation) {}
TEST_F(DaycarePopulatorTest, OneLocationTest) {}
TEST_F(DaycarePopulatorTest, TwoLocationTest) {}

} // namespace
