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
 * Implementation file for the DataModel class.
 */

#include "Model.h"

namespace stride {
namespace visualiser {

using geopop::VisLocation;
using geopop::PoolStats;
using geopop::LocationBase;

using std::vector;

/**
 * Struct that keeps track of the total number of individuals
 * that belong to a certain type of contact pool and conform
 * to a certain health status.
 */
struct PoolTotals
{
    // totals
    unsigned int population = 0;
    unsigned int immune = 0;
    unsigned int infected = 0;
    unsigned int infectious = 0;
    unsigned int recovered = 0;
    unsigned int susceptible = 0;
    unsigned int symptomatic = 0;

    /**
     * Add a PoolStats object to the current totals.
     */
    void AugmentWithPoolStats(const PoolStats& poolStats)
    {
        population  += poolStats.population;
        immune      += (unsigned int) (poolStats.population * poolStats.immune);
        infected    += (unsigned int) (poolStats.population * poolStats.infected);
        infectious  += (unsigned int) (poolStats.population * poolStats.infectious);
        recovered   += (unsigned int) (poolStats.population * poolStats.recovered);
        susceptible += (unsigned int) (poolStats.population * poolStats.susceptible);
        symptomatic += (unsigned int) (poolStats.population * poolStats.symptomatic);
    }

    /**
     * Convert the PoolTotals object that contains absolute population numbers
     * to a PoolStats object that contains fractions.
     */
    const PoolStats ToPoolStats() const
    {
        PoolStats retval{};

        unsigned int population = this->population;

        retval.population  = population;

        if(population == 0)
            population = 1;

        retval.immune      = (double) immune      / (double) population;
        retval.infected    = (double) infected    / (double) population;
        retval.infectious  = (double) infectious  / (double) population;
        retval.recovered   = (double) recovered   / (double) population;
        retval.susceptible = (double) susceptible / (double) population;
        retval.symptomatic = (double) symptomatic / (double) population;

        return retval;
    }
};

/**
 * Given a list of VisLocations, this will summarize the population stats about
 * those locations in one PopStats object.
 */
const PopStats SummarizeLocations(const std::vector<const VisLocation*>& locations)
{
    // totals per pool
    std::map<ContactType::Id, PoolTotals> pool_totals = {
        {ContactType::Id::Household,          PoolTotals{}},
        {ContactType::Id::K12School,          PoolTotals{}},
        {ContactType::Id::College,            PoolTotals{}},
        {ContactType::Id::Workplace,          PoolTotals{}},
        {ContactType::Id::PrimaryCommunity,   PoolTotals{}},
        {ContactType::Id::SecondaryCommunity, PoolTotals{}},
        {ContactType::Id::Daycare,            PoolTotals{}},
        {ContactType::Id::PreSchool,          PoolTotals{}},
    };

    // go over locations
    for(const auto* loc : locations)
    {
        // retrieve population information
        const auto& popstats = loc->GetPopStats();

        // update totals for each type of contact pool
        for(const auto& pool_type : ContactType::IdList)
        {
            pool_totals.at(pool_type).AugmentWithPoolStats(popstats.GetPool(pool_type));
        }
    }

    // convert pool totals back to PopStats object
    PopStats retval{};

    for(const auto& pool_type : ContactType::IdList)
    {
        retval.SetPool(pool_type, pool_totals.at(pool_type).ToPoolStats());
    }

    return retval;
}

/**
 * Given a vector of LocationBase pointers, upcast these to a vector of VisLocation pointers.
 */
const vector<const VisLocation*> UpcastLocPtrVector(const vector<const LocationBase*>& locPtrVec)
{
    std::vector<const VisLocation*> retval;

    for(const auto* locBasePtr : locPtrVec)
    {
        retval.push_back(static_cast<const VisLocation*>(locBasePtr));
    }

    return retval;
}

const PopStats Model::GetPopulationInRadius(const Coordinate& center, const double radius, const unsigned int day) const
{
        // get timestep
        const VisGeoGrid& day_locs = *m_timesteps.at(day);

        // convert to km
        const double radius_km = radius / 1000.0;

        // filter locations
        const auto loc_list = day_locs.LocationsInRadius(center.get<0>(), center.get<1>(), radius_km);

        // summarize selection and return
        return SummarizeLocations(UpcastLocPtrVector(loc_list));
}

const PopStats Model::GetPopulationInBox(const Coordinate& pointA, const Coordinate& pointB, const unsigned int day) const
{
        // get timestep
        const VisGeoGrid& day_locs = *m_timesteps.at(day);

        // filter locations
        const auto loc_list = day_locs.LocationsInBox(pointA.get<0>(), pointA.get<1>(), pointB.get<0>(), pointB.get<1>());

        // summarize selection and return
        return SummarizeLocations(UpcastLocPtrVector(loc_list));
}

} // namespace visualiser
} // namespace stride
