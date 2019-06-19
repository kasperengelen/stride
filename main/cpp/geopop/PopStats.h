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
 * @file Header for the LocationPopData class.
 */

#pragma once

#include "contact/ContactType.h"
#include "geopop/SimLocation.h"

namespace geopop {

/**
 * Struct that contains information about the population that belongs to one type of contact pool.
 */
struct PoolStats
{
        unsigned int population  = 0;
        double       immune      = 0;
        double       infected    = 0;
        double       infectious  = 0;
        double       recovered   = 0;
        double       susceptible = 0;
        double       symptomatic = 0;
};

/**
 * Class that keeps track of stats about the health status of a population.
 */
class PopStats
{
public:
        /**
         * Default constructor. This is useful when the PopStats object needs to be constructed
         * manually.
         */
        PopStats();

        /**
         * Constructor. Process the specified location and determine information about
         * the population.
         */
        PopStats(const geopop::SimLocation& loc);

        /**
         * Retrieve a const reference to the PoolTypeData object that keeps track of the
         * contact pools of the specified type.
         */
        const PoolStats& GetPool(const stride::ContactType::Id& poolId) const;

        /**
         * Retrieve a non-const reference to the PoolTypeData object that keeps track of the
         * contact pools of the specified type.
         */
        PoolStats& GetPool(const stride::ContactType::Id& poolId);

        /**
         * Set the information stored about the specified type of pool to
         * the specified geopop::PoolStats object.
         */
        void SetPool(const stride::ContactType::Id& poolId, PoolStats poolStats);

private:
        PoolStats m_household;
        PoolStats m_k12_school;
        PoolStats m_college;
        PoolStats m_workplace;
        PoolStats m_prim_com;
        PoolStats m_sec_com;
        PoolStats m_daycare;
        PoolStats m_preschool;
};

} // namespace geopop
