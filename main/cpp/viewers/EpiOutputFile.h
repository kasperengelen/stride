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
 * @file Header for the EpiOutputFile class.
 */

#pragma once

#include "contact/ContactType.h"
#include "geopop/Location.h"

#include <fstream>
#include <memory>
#include <string>

namespace stride {

class Population;

namespace output {

/**
 * Base class for viewers that produce a file that gives the epidemiological status per timestep.
 */
class EpiOutputFile
{
public:
        /// Constructor: initialize.
        EpiOutputFile();

        /// Destructor: close the file stream.
        virtual ~EpiOutputFile();

        /// Update the data.
        virtual void Update(std::shared_ptr<const Population> population) = 0;

        /// Finish writing the data.
        virtual void Finish() = 0;

private:
        /// Generate file name and open the file stream.
        virtual void Initialize(const std::string& output_dir) = 0;

protected:
        std::fstream m_fstream;
};

/**
 * Struct that contains information about the population that belongs to one type of contact pool.
 */
struct PoolTypeData
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
* Struct that contains information about the population inside a location.
*/
struct LocationPopData
{
private:
        PoolTypeData household;
        PoolTypeData k12_school;
        PoolTypeData college;
        PoolTypeData workplace;
        PoolTypeData prim_com;
        PoolTypeData sec_com;
        PoolTypeData daycare;
        PoolTypeData preschool;

public:
    /**
     * Constructor. Initialises all the stats to zero.
     */
    LocationPopData()
        : household{}, k12_school{}, college{}, workplace{}, prim_com{}, sec_com{}, daycare{}, preschool{}
    {}

    /**
     * Retrieve a const reference to the PoolTypeData object that keeps track of the
     * contact pools of the specified type.
     */
    const PoolTypeData& GetPool(const ContactType::Id& poolId) const;

    /**
     * Retrieve a non-const reference to the PoolTypeData object that keeps track of the
     * contact pools of the specified type.
     */
    PoolTypeData& GetPool(const ContactType::Id& poolId);
};

/**
 * Given a Location object, process the population inside that location and produce a PopData object.
 * @param loc The location whose population will be processed.
 * @return The LocationPopData that contains a representation of the population of the location.
 */
const LocationPopData ProcessPopulation(const geopop::Location& loc);

} // namespace output
} // namespace stride
