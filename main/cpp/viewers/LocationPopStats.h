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
#include "geopop/Location.h"

namespace stride {
namespace output {

/**
* Struct that contains information about the population that belongs to one type of contact pool.
*/
struct PoolTypeData {
        unsigned int population = 0;
        double immune = 0;
        double infected = 0;
        double infectious = 0;
        double recovered = 0;
        double susceptible = 0;
        double symptomatic = 0;
};

/**
* Class that calculates stats about the disease status of the population within a location.
*/
class LocationPopData {
public:
        /**
         * Constructor. Process the specified location and determine information about
         * the population.
         */
        LocationPopData(const geopop::Location &loc);

        /**
         * Retrieve a const reference to the PoolTypeData object that keeps track of the
         * contact pools of the specified type.
         */
        const PoolTypeData &GetPool(const ContactType::Id &poolId) const;

        /**
         * Retrieve a non-const reference to the PoolTypeData object that keeps track of the
         * contact pools of the specified type.
         */
        PoolTypeData &GetPool(const ContactType::Id &poolId);

private:
        PoolTypeData m_household;
        PoolTypeData m_k12_school;
        PoolTypeData m_college;
        PoolTypeData m_workplace;
        PoolTypeData m_prim_com;
        PoolTypeData m_sec_com;
        PoolTypeData m_daycare;
        PoolTypeData m_preschool;
};

}
}