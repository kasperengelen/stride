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
 *  Copyright 2019, Jan Broeckhove.
 */

#include "geopop/populators/Populator.h"

#include "MakeGeoGrid.h"
#include "contact/AgeBrackets.h"
#include "geopop/SimLocation.h"
#include "geopop/generators/Generator.h"
#include "pop/Population.h"
#include "util/RnMan.h"

#include <gtest/gtest.h>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

namespace {

class WorkplacePopulatorTest : public testing::Test
{
public:
        WorkplacePopulatorTest()
            : m_rn_man(RnInfo()), m_workplace_populator(m_rn_man), m_gg_config(), m_pop(Population::Create()),
              m_geo_grid(m_pop->RefGeoGrid()), m_workplace_generator(m_rn_man)
        {
        }

protected:
        RnMan                  m_rn_man;
        WorkplacePopulator     m_workplace_populator;
        GeoGridConfig          m_gg_config;
        shared_ptr<Population> m_pop;
        GeoGrid&               m_geo_grid;
        WorkplaceGenerator     m_workplace_generator;
        const unsigned int     m_ppwp = m_gg_config.pools[Id::Workplace];
};

TEST_F(WorkplacePopulatorTest, NoPopulation)
{
        m_geo_grid.AddLocation(make_shared<SimLocation>(0, 0, Coordinate(0.0, 0.0), "", 0));
        m_geo_grid.Finalize();

        EXPECT_NO_THROW(m_workplace_populator.Apply(m_geo_grid, m_gg_config));
}

TEST_F(WorkplacePopulatorTest, NoActive)
{
        MakeGeoGrid(m_gg_config, 3, 100, 0, 0, 3, 33, 3, m_pop.get());

        m_gg_config.param.particpation_workplace = 0;
        m_gg_config.param.participation_college  = 1;
        m_gg_config.workplaceSD.ratios           = {0.778532842256952, 0.171901116625764, 0.0410039025210945,
                                          0.00856213859618965};
        m_gg_config.workplaceSD.sizes = {make_pair(1, 9), make_pair(10, 49), make_pair(50, 199), make_pair(200, 400)};

        // Nobody works, everybody in the student age bracket goes to college: so workplace is empty.
        // Brasschaat and Schoten are close to each other. There is no commuting, but they are so close
        // they will receive students from each other. Kortrijk will only receive students from Kortrijk.
        auto brasschaat = *m_geo_grid.begin();
        brasschaat->SetCoordinate(Coordinate(51.29227, 4.49419));

        auto schoten = *(m_geo_grid.begin() + 1);
        schoten->SetCoordinate(Coordinate(51.2497532, 4.4977063));

        auto kortrijk = *(m_geo_grid.begin() + 2);
        kortrijk->SetCoordinate(Coordinate(50.82900246, 3.264406009));

        m_geo_grid.Finalize();
        m_workplace_populator.Apply(m_geo_grid, m_gg_config);

        for (const Person& p : *m_geo_grid.GetPopulation()) {
                EXPECT_EQ(0, p.GetPoolId(Id::Workplace));
        }
}

TEST_F(WorkplacePopulatorTest, NoCommuting)
{
        MakeGeoGrid(m_gg_config, 3, 100, 0, 0, 3, 33, 3, m_pop.get());

        m_gg_config.param.fraction_workplace_commuters = 0;
        m_gg_config.param.particpation_workplace       = 1;
        m_gg_config.param.participation_college        = 0.5;
        m_gg_config.workplaceSD.ratios                 = {0.778532842256952, 0.171901116625764, 0.0410039025210945,
                                          0.00856213859618965};
        m_gg_config.workplaceSD.sizes = {make_pair(1, 9), make_pair(10, 49), make_pair(50, 199), make_pair(200, 400)};

        // Brasschaat and Schoten are close to each other
        // There is no commuting, but since they will still receive students from each other
        // Kortrijk will only receive students from Kortrijik

        auto brasschaat = *m_geo_grid.begin();
        brasschaat->SetCoordinate(Coordinate(51.29227, 4.49419));
        m_workplace_generator.AddPools(*brasschaat, m_pop.get(), m_gg_config);
        m_workplace_generator.AddPools(*brasschaat, m_pop.get(), m_gg_config);

        auto schoten = *(m_geo_grid.begin() + 1);
        schoten->SetCoordinate(Coordinate(51.2497532, 4.4977063));
        m_workplace_generator.AddPools(*schoten, m_pop.get(), m_gg_config);
        m_workplace_generator.AddPools(*schoten, m_pop.get(), m_gg_config);

        auto kortrijk = *(m_geo_grid.begin() + 2);
        kortrijk->SetCoordinate(Coordinate(50.82900246, 3.264406009));
        m_workplace_generator.AddPools(*kortrijk, m_pop.get(), m_gg_config);
        m_workplace_generator.AddPools(*kortrijk, m_pop.get(), m_gg_config);

        m_geo_grid.Finalize();
        m_workplace_populator.Apply(m_geo_grid, m_gg_config);

        // Assert that persons of Schoten only go to Schoten or Brasschaat
        for (const auto& hPool : schoten->RefPools(Id::Household)) {
                for (auto p : hPool[0]) {
                        const auto workId = p->GetPoolId(Id::Workplace);
                        if (AgeBrackets::Workplace::HasAge(p->GetAge()) && !AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE(workId >= 1 && workId <= 4 * m_ppwp);
                        } else if (AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE((workId >= 1 && workId <= 4 * m_ppwp) || workId == 0);
                        } else {
                                EXPECT_EQ(0, workId);
                        }
                }
        }

        // Assert that persons of Brasschaat only go to Schoten or Brasschaat
        for (const auto& hPool : brasschaat->RefPools(Id::Household)) {
                for (auto p : hPool[0]) {
                        const auto workId = p->GetPoolId(Id::Workplace);
                        if (AgeBrackets::Workplace::HasAge(p->GetAge()) && !AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE(workId >= 1 && workId <= 4 * m_ppwp);
                        } else if (AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE((workId >= 1 && workId <= 4 * m_ppwp) || workId == 0);
                        } else {
                                EXPECT_EQ(0, workId);
                        }
                }
        }

        // Assert that persons of Kortrijk only go to Kortijk
        for (const auto& hPool : kortrijk->RefPools(Id::Household)) {
                for (auto p : hPool[0]) {
                        const auto workId = p->GetPoolId(Id::Workplace);
                        if (AgeBrackets::Workplace::HasAge(p->GetAge()) && !AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE(workId > 4 * m_ppwp && workId <= 6 * m_ppwp);
                        } else if (AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE((workId > 4 * m_ppwp && workId <= 6 * m_ppwp) || workId == 0);
                        } else {
                                EXPECT_EQ(0, workId);
                        }
                }
        }
}

TEST_F(WorkplacePopulatorTest, OnlyCommuting)
{
        MakeGeoGrid(m_gg_config, 3, 100, 0, 0, 3, 33, 3, m_pop.get());

        m_gg_config.param.fraction_workplace_commuters = 0;
        m_gg_config.param.fraction_workplace_commuters = 1;
        m_gg_config.param.fraction_college_commuters   = 0;
        m_gg_config.info.popcount_workplace            = 1;
        m_gg_config.param.particpation_workplace       = 1;
        m_gg_config.param.participation_college        = 0.5;
        m_gg_config.workplaceSD.ratios                 = {0.778532842256952, 0.171901116625764, 0.0410039025210945,
                                          0.00856213859618965};
        m_gg_config.workplaceSD.sizes = {make_pair(1, 9), make_pair(10, 49), make_pair(50, 199), make_pair(200, 400)};

        // only commuting

        auto schoten = *(m_geo_grid.begin());
        schoten->SetCoordinate(Coordinate(51.2497532, 4.4977063));
        m_workplace_generator.AddPools(*schoten, m_pop.get(), m_gg_config);
        m_workplace_generator.AddPools(*schoten, m_pop.get(), m_gg_config);

        auto kortrijk = *(m_geo_grid.begin() + 1);
        kortrijk->SetCoordinate(Coordinate(50.82900246, 3.264406009));
        m_workplace_generator.AddPools(*kortrijk, m_pop.get(), m_gg_config);
        m_workplace_generator.AddPools(*kortrijk, m_pop.get(), m_gg_config);

        schoten->AddOutgoingCommute(kortrijk, 0.5);
        kortrijk->AddIncomingCommute(schoten, 0.5);
        kortrijk->AddOutgoingCommute(schoten, 0.5);
        schoten->AddIncomingCommute(kortrijk, 0.5);

        m_geo_grid.Finalize();
        m_workplace_populator.Apply(m_geo_grid, m_gg_config);

        // Assert that persons of Schoten only go to Kortrijk
        for (const auto& hPool : schoten->RefPools(Id::Household)) {
                for (auto p : hPool[0]) {
                        const auto workId = p->GetPoolId(Id::Workplace);
                        if (AgeBrackets::Workplace::HasAge(p->GetAge()) && !AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE(workId > 2 * m_ppwp && workId <= 4 * m_ppwp);
                        } else if (AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE((workId > 2 * m_ppwp && workId <= 4 * m_ppwp) || workId == 0);
                        } else {
                                EXPECT_EQ(0, workId);
                        }
                }
        }

        // Assert that persons of Kortrijk only go to Schoten
        for (const auto& hPool : kortrijk->RefPools(Id::Household)) {
                for (auto p : hPool[0]) {
                        const auto workId = p->GetPoolId(Id::Workplace);
                        if (AgeBrackets::Workplace::HasAge(p->GetAge()) && !AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE(workId >= 1 && workId <= 2 * m_ppwp);
                        } else if (AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE((workId >= 1 && workId <= 2 * m_ppwp) || workId == 0);
                        } else {
                                EXPECT_EQ(0, workId);
                        }
                }
        }
}

TEST_F(WorkplacePopulatorTest, NoCommutingAvailable)
{
        MakeGeoGrid(m_gg_config, 3, 100, 0, 0, 3, 33, 3, m_pop.get());

        m_gg_config.param.fraction_workplace_commuters = 0;
        m_gg_config.param.fraction_workplace_commuters = 1;
        m_gg_config.param.fraction_college_commuters   = 0;
        m_gg_config.info.popcount_workplace            = 1;
        m_gg_config.param.particpation_workplace       = 1;
        m_gg_config.param.participation_college        = 0.5;
        m_gg_config.workplaceSD.ratios                 = {0.778532842256952, 0.171901116625764, 0.0410039025210945,
                                          0.00856213859618965};
        m_gg_config.workplaceSD.sizes = {make_pair(1, 9), make_pair(10, 49), make_pair(50, 199), make_pair(200, 400)};

        auto brasschaat = *m_geo_grid.begin();
        brasschaat->SetCoordinate(Coordinate(51.29227, 4.49419));
        m_workplace_generator.AddPools(*brasschaat, m_pop.get(), m_gg_config);
        m_workplace_generator.AddPools(*brasschaat, m_pop.get(), m_gg_config);

        auto schoten = *(m_geo_grid.begin() + 1);
        schoten->SetCoordinate(Coordinate(51.2497532, 4.4977063));
        m_workplace_generator.AddPools(*schoten, m_pop.get(), m_gg_config);
        m_workplace_generator.AddPools(*schoten, m_pop.get(), m_gg_config);

        auto kortrijk = *(m_geo_grid.begin() + 2);
        kortrijk->SetCoordinate(Coordinate(50.82900246, 3.264406009));
        m_workplace_generator.AddPools(*kortrijk, m_pop.get(), m_gg_config);
        m_workplace_generator.AddPools(*kortrijk, m_pop.get(), m_gg_config);

        // test case is only commuting but between nobody is commuting from or to Brasschaat
        schoten->AddOutgoingCommute(kortrijk, 0.5);
        kortrijk->AddIncomingCommute(schoten, 0.5);
        kortrijk->AddOutgoingCommute(schoten, 0.5);
        schoten->AddIncomingCommute(kortrijk, 0.5);

        m_geo_grid.Finalize();
        m_workplace_populator.Apply(m_geo_grid, m_gg_config);

        // Assert that persons of Schoten only go to Kortrijk
        for (const auto& hPool : schoten->RefPools(Id::Household)) {
                for (auto p : hPool[0]) {
                        const auto workId = p->GetPoolId(Id::Workplace);
                        if (AgeBrackets::Workplace::HasAge(p->GetAge()) && !AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE(workId > 4 * m_ppwp && workId <= 6 * m_ppwp);
                        } else if (AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE((workId > 4 * m_ppwp && workId <= 6 * m_ppwp) || workId == 0);
                        } else {
                                EXPECT_EQ(0, workId);
                        }
                }
        }

        // Assert that persons of Brasschaat only go to Brasschaat or Schoten
        for (const auto& hPool : brasschaat->RefPools(Id::Household)) {
                for (auto p : hPool[0]) {
                        const auto workId = p->GetPoolId(Id::Workplace);
                        if (AgeBrackets::Workplace::HasAge(p->GetAge()) && !AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE(workId >= 1 && workId <= 4 * m_ppwp);
                        } else if (AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE((workId >= 1 && workId <= 4 * m_ppwp) || workId == 0);
                        } else {
                                EXPECT_EQ(0, workId);
                        }
                }
        }

        // Assert that persons of Kortrijk only go to Schoten
        for (const auto& hPool : kortrijk->RefPools(Id::Household)) {
                for (auto p : hPool[0]) {
                        const auto workId = p->GetPoolId(Id::Workplace);
                        if (AgeBrackets::Workplace::HasAge(p->GetAge()) && !AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE(workId > 2 * m_ppwp && workId <= 4 * m_ppwp);
                        } else if (AgeBrackets::College::HasAge(p->GetAge())) {
                                EXPECT_TRUE((workId > 2 * m_ppwp && workId <= 4 * m_ppwp) || workId == 0);
                        } else {
                                EXPECT_EQ(0, workId);
                        }
                }
        }
}

TEST_F(WorkplacePopulatorTest, Distribution)
{
        MakeGeoGrid(m_gg_config, 3, 10000, 0, 0, 3, 3333, 3, m_pop.get());

        m_gg_config.param.fraction_workplace_commuters = 0.3;
        m_gg_config.param.fraction_college_commuters   = 0;
        m_gg_config.info.popcount_workplace            = 1;
        m_gg_config.param.particpation_workplace       = 1;
        m_gg_config.param.participation_college        = 0;
        m_gg_config.workplaceSD.ratios                 = {0.778532842256952, 0.171901116625764, 0.0410039025210945,
                                          0.00856213859618965};
        m_gg_config.workplaceSD.sizes = {make_pair(1, 9), make_pair(10, 49), make_pair(50, 199), make_pair(200, 400)};

        double avgPplPerWorkplace = 0;
        auto   EmployeeCount      = 17200;
        for (auto i = 0; i < (int)m_gg_config.workplaceSD.ratios.size(); i++) {
                const auto min_size = m_gg_config.workplaceSD.sizes[i].first;
                const auto max_size = m_gg_config.workplaceSD.sizes[i].second;

                avgPplPerWorkplace += m_gg_config.workplaceSD.ratios[i] * (min_size + max_size) / 2;
        }

        int pools = static_cast<unsigned int>(ceil(EmployeeCount / (avgPplPerWorkplace * (3))));

        auto brasschaat = *m_geo_grid.begin();
        brasschaat->SetCoordinate(Coordinate(51.29227, 4.49419));
        for (int _ = 0; _ < pools; _++) {
                m_workplace_generator.AddPools(*brasschaat, m_pop.get(), m_gg_config);
        }

        auto schoten = *(m_geo_grid.begin() + 1);
        schoten->SetCoordinate(Coordinate(51.2497532, 4.4977063));
        for (int _ = 0; _ < pools; _++) {
                m_workplace_generator.AddPools(*schoten, m_pop.get(), m_gg_config);
        }

        auto kortrijk = *(m_geo_grid.begin() + 2);
        kortrijk->SetCoordinate(Coordinate(50.82900246, 3.264406009));
        for (int _ = 0; _ < pools; _++) {
                m_workplace_generator.AddPools(*kortrijk, m_pop.get(), m_gg_config);
        }

        m_geo_grid.Finalize();
        m_workplace_populator.Apply(m_geo_grid, m_gg_config);

        vector<unsigned long> ranges(m_gg_config.workplaceSD.ratios.size());
        vector<unsigned long> sizes(m_gg_config.workplaceSD.ratios.size());

        unsigned int zero  = 0;
        unsigned int other = 0;
        // Check pools for consistency
        for (const auto& loc : m_geo_grid) {
                for (auto& workPool : loc->RefPools(Id::Workplace)) {

                        auto sizePool = workPool->size();
                        bool setP     = false;

                        if (sizePool == 0) {
                                zero++;
                                setP = true;
                        }

                        for (int i = 0; i < (int)m_gg_config.workplaceSD.ratios.size(); i++) {
                                if (sizePool >= m_gg_config.workplaceSD.sizes[i].first and
                                    sizePool <= m_gg_config.workplaceSD.sizes[i].second) {
                                        ranges[i]++;
                                        sizes[i] += sizePool;
                                        setP = true;
                                        break;
                                }
                        }

                        if (!setP) {
                                other++;
                        }
                }
        }

        EXPECT_EQ(0, zero);
        EXPECT_EQ(803, ranges[0]);
        EXPECT_EQ(172, ranges[1]);
        EXPECT_EQ(48, ranges[2]);
        EXPECT_EQ(12, ranges[3]);
        EXPECT_EQ(0, other);
}

} // namespace
