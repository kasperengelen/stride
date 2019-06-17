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
 *  Copyright 2019, ACED
 */

/**
 * @file Implementation of the LocationPopData class.
 */

#include "PopStats.h"

#include "contact/ContactType.h"
#include "pop/Population.h"

namespace geopop {

PopStats::PopStats()
        : m_household{}, m_k12_school{}, m_college{}, m_workplace{},
          m_prim_com{}, m_sec_com{}, m_daycare{}, m_preschool{}
{}

PopStats::PopStats(const geopop::SimLocation &loc)
    : m_household{}, m_k12_school{}, m_college{}, m_workplace{},
      m_prim_com{}, m_sec_com{}, m_daycare{}, m_preschool{}
{
    for (const auto &pool_type : stride::ContactType::IdList) {
        // retrieve reference to current pool stats
        PoolStats &pool_stats = this->GetPool(pool_type);

        // retrieve pools of this type.
        const auto &pools = loc.CRefPools(pool_type);

        // init counters to zero
        int immune = 0;
        int infected = 0;
        int infectious = 0;
        int recovered = 0;
        int susceptible = 0;
        int symptomatic = 0;
        int total_pop = 0;

        for (const auto &pool: pools) {
            total_pop += pool->size();

            // Iterate over population to collect data
            for (const auto &person: *pool) {
                const auto health = person->GetHealth();

                if (health.IsImmune()) immune++;

                if (health.IsInfected()) infected++;

                if (health.IsInfectious()) infectious++;

                if (health.IsRecovered()) recovered++;

                if (health.IsSusceptible()) susceptible++;

                if (health.IsSymptomatic()) symptomatic++;
            }
        }

        pool_stats.population = total_pop;

        // prevent division by zero.
        if (total_pop == 0)
            total_pop = 1;

        pool_stats.immune      = (double) immune / total_pop;
        pool_stats.infected    = (double) infected / total_pop;
        pool_stats.infectious  = (double) infectious / total_pop;
        pool_stats.recovered   = (double) recovered / total_pop;
        pool_stats.susceptible = (double) susceptible / total_pop;
        pool_stats.symptomatic = (double) symptomatic / total_pop;
    }
}

const PoolStats& PopStats::GetPool(const stride::ContactType::Id &poolId) const {
    switch (poolId) {
        case stride::ContactType::Id::Household:
            return this->m_household;
        case stride::ContactType::Id::K12School:
            return this->m_k12_school;
        case stride::ContactType::Id::College:
            return this->m_college;
        case stride::ContactType::Id::Workplace:
            return this->m_workplace;
        case stride::ContactType::Id::PrimaryCommunity:
            return this->m_prim_com;
        case stride::ContactType::Id::SecondaryCommunity:
            return this->m_sec_com;
        case stride::ContactType::Id::Daycare:
            return this->m_daycare;
        case stride::ContactType::Id::PreSchool:
            return this->m_preschool;
    }

    throw std::runtime_error{"Error: GetPool called on unknown pool type '" + stride::ContactType::ToString(poolId) +
                             "' which is not handled "};
}

PoolStats& PopStats::GetPool(const stride::ContactType::Id &poolId) {
    // do const cast to prevent code duplication.
    return const_cast<PoolStats &>(const_cast<const PopStats &>(*this).GetPool(poolId));
}

void PopStats::SetPool(const stride::ContactType::Id& poolId, PoolStats poolStats)
{
    switch (poolId) {
        case stride::ContactType::Id::Household:
            m_household = std::move(poolStats);
            break;
        case stride::ContactType::Id::K12School:
            m_k12_school = std::move(poolStats);
            break;
        case stride::ContactType::Id::College:
            m_college = std::move(poolStats);
            break;
        case stride::ContactType::Id::Workplace:
            m_workplace = std::move(poolStats);
            break;
        case stride::ContactType::Id::PrimaryCommunity:
            m_prim_com = std::move(poolStats);
            break;
        case stride::ContactType::Id::SecondaryCommunity:
            m_sec_com = std::move(poolStats);
            break;
        case stride::ContactType::Id::Daycare:
            m_daycare = std::move(poolStats);
            break;
        case stride::ContactType::Id::PreSchool:
            m_preschool = std::move(poolStats);
            break;
    }
}


} // namespace geopop
