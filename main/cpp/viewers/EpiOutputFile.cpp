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
 * @file Implementation of the EpiOutputFile class.
 */

#include "EpiOutputFile.h"

#include "contact/ContactType.h"
#include "pop/Population.h"

#include <iostream>

namespace stride {
namespace output {

EpiOutputFile::EpiOutputFile() : m_fstream() {}

EpiOutputFile::~EpiOutputFile() { m_fstream.close(); }

const PoolTypeData& LocationPopData::GetPool(const ContactType::Id& poolId) const
{
    switch(poolId)
    {
        case ContactType::Id::Household:
            return this->household;
        case ContactType::Id::K12School:
            return this->k12_school;
        case ContactType::Id::College:
            return this->college;
        case ContactType::Id::Workplace:
            return this->workplace;
        case ContactType::Id::PrimaryCommunity:
            return this->prim_com;
        case ContactType::Id::SecondaryCommunity:
            return this->sec_com;
        case ContactType::Id::Daycare:
            return this->daycare;
        case ContactType::Id::PreSchool:
            return this->preschool;
    }

    throw std::runtime_error{"Error: GetPool called on unknown pool type '" + ContactType::ToString(poolId) + "' which is not handled "};
}

PoolTypeData& LocationPopData::GetPool(const ContactType::Id& poolId)
{
    // do const cast to prevent code duplication.
    return const_cast<PoolTypeData&>(const_cast<const LocationPopData&>(*this).GetPool(poolId));
}

const LocationPopData ProcessPopulation(const geopop::Location& loc)
{
    LocationPopData retval;

    // Collect pooltype-specific information
    for(const auto& pool_type : ContactType::IdList)
    {
        // retrieve reference to current pool stats
        PoolTypeData& pool_stats = retval.GetPool(pool_type);

        // retrieve pools of this type.
        const auto& pools = loc.CRefPools(pool_type);

        // init counters to zero
        int  immune      = 0;
        int  infected    = 0;
        int  infectious  = 0;
        int  recovered   = 0;
        int  susceptible = 0;
        int  symptomatic = 0;
        int  total_pop   = 0;

        for(const auto& pool: pools) {
            total_pop += pool->size();

            // Iterate over population to collect data
            for(const auto& person: *pool)
            {
                const auto health = person->GetHealth();

                if (health.IsImmune())      immune++;

                if (health.IsInfected())    infected++;

                if (health.IsInfectious())  infectious++;

                if (health.IsRecovered())   recovered++;

                if (health.IsSusceptible()) susceptible++;

                if (health.IsSymptomatic()) symptomatic++;
            }
        }

        pool_stats.population  = total_pop;

        // prevent division by zero.
        if(total_pop == 0)
            total_pop = 1;

        pool_stats.immune      = (double)immune / total_pop;
        pool_stats.infected    = (double)infected / total_pop;
        pool_stats.infectious  = (double)infectious / total_pop;
        pool_stats.recovered   = (double)recovered / total_pop;
        pool_stats.susceptible = (double)susceptible / total_pop;
        pool_stats.symptomatic = (double)symptomatic / total_pop;
    }

    return retval;
}

} // namespace output
} // namespace stride
