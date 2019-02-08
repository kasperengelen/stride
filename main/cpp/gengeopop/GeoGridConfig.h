#pragma once
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

//#include "gengeopop/Household.h"
#include "pool/ContactPool.h"
#include "pop/Person.h"
#include "util/SegmentedVector.h"

#include <boost/property_tree/ptree.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>

namespace gengeopop {

class GeoGrid;
class Household;
class HouseholdReader;

/**
 * Configuration data for generating a GeoGrid.
 */

class GeoGridConfig
{
public:
        /// Default constructor needed.
        GeoGridConfig();

        /// Constructor that configures input data.
        explicit GeoGridConfig(const boost::property_tree::ptree& configPt);

        /// Fill the GeoGridConfig based on information in the provided GeoGrid and HouseholdReader.
        void Calculate(std::shared_ptr<GeoGrid> geoGrid, std::shared_ptr<HouseholdReader> householdReader);

        /// Prints the GeoGridconfig
        friend std::ostream& operator<<(std::ostream& stream, const GeoGridConfig& config);

        // -----------------------------------------------------------------------------------------
        // INPUT
        // -----------------------------------------------------------------------------------------
        struct
        {
                /// Fraction of calc_fraction_1826 which are students.
                double fraction_1826_student;

                /// Fraction of calc_1865_and_active which commute.
                double fraction_active_commutingPeople;

                /// Fraction of fraction_1826_years_WhichAreStudents which commute.
                double fraction_student_commuting;

                /// Fraction of (calculated._1865_years - calculated._1826_years_and_student) which are active
                double fraction_1865_active;

                /// Absolute size of the population.
                unsigned int pop_size;
        } input;

        // -----------------------------------------------------------------------------------------
        // CALCULATED
        // -----------------------------------------------------------------------------------------
        struct
        {
                /// Numbers of persons for which school is compulsory (i.e. 6-18 years old).
                unsigned int compulsory_pupils;

                /// Absolute amount of population which are [18, 65) years old.
                unsigned int popcount_1865;

                /// Absolute amount of population which are [18, 26) years old.
                unsigned int popcount_1826;

                /// Absolute amount of population which are [18, 26) years old and are a student.
                unsigned int popcount_1826_student;

                /// Absolute amount of population which are [18, 65] years old and active.
                unsigned int popcount_1865_active;

                /// The number of households needed with this population size and these types of households.
                unsigned int households;
        } calculated;

        // -----------------------------------------------------------------------------------------
        // CURRENT STATE
        // -----------------------------------------------------------------------------------------
        struct
        {
                /// The current number of ContactPools, used to obtain an Id for a new contactPool.
                /// ! 0 has special meaning (not assigned)!
                unsigned int contact_pool_count = 1;

                /// The current number of ContactCenters, used to obtain an Id for a new contactCenter.
                /// ! 0 has special meaning (not assigned)!
                unsigned int contact_center_count = 1;

                /// Reference households: the set of households that we sample from to generate the population.
                std::vector<std::shared_ptr<Household>> reference_households{};

                /// Persons in the reference households (segmented vector to have working pointers into it).
                stride::util::SegmentedVector<stride::Person> persons{};

                /// Contactpools used for reference households (segmented vector to have working pointers into it).
                stride::util::SegmentedVector<stride::ContactPool> contact_pools{};
        } generated;

        // -----------------------------------------------------------------------------------------
        // CONSTANTS (for now at least)
        // -----------------------------------------------------------------------------------------
        struct
        {
                /// Size used to calculate the amount of Colleges (double to prevent extra cast)
                double mean_college_size = 3000.0;

                /// Size used to calculate the amount of Communities (double to prevent extra cast)
                double mean_community_size = 2000.0;

                /// Size used to calculate the amount of K12Schools (double to prevent extra cast)
                double mean_K12_size = 500.0;

                /// Size used to calculate the amount of Workplaces (double to prevent extra cast)
                double mean_workplace_school_size = 20.0;
        } constants;
};

} // namespace gengeopop