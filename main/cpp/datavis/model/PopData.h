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
 * Header file for the PopData and PopSection structs.
 */

#pragma once

namespace stride {
namespace datavis {

/**
 * @struct PopSection
 * Struct that keeps track of a section of the population inside a
 * locality. Note that the fractions of people who are immune, infected, etc.
 * don't add up to 1.0, since people can belong to many disease categories
 * simultaneously.
 */
struct PopSection
{
        /// Total number of individuals that belong to this type of contact pool
        unsigned int pop;

        /// The fraction of individuals that return true for HealthStatus::IsImmune().
        double immune;

        /// The fraction of individuals that return true for HealthStatus::IsInfected().
        double infected;

        /// The fraction of individuals that return true for HealthStatus::IsInfectious().
        double infectious;

        /// The fraction of individuals that return true for HealthStatus::IsRecovered().
        double recovered;

        /// The fraction of individuals that return true for HealthStatus::IsSusceptible().
        double susceptible;

        /// The fraction of individuals that return true for HealthStatus::isSymptomatic().
        double symptomatic;
};

/**
 * @struct PopData
 * Struct that contains information about a population, such as total population statistics,
 * the statistics of each section of the population, etc.
 */
struct PopData
{
	/// The total population.
	PopSection total;

	/// Part of the population that is part of a household ContactPool.
	PopSection household;

	/// Part of the population that is part of a k12-school ContactPool.
	PopSection k12school;

	/// Part of the population that is part of a college ContactPool.
	PopSection college;

	/// Part of the population that is part of a workplace ContactPool.
	PopSection workplace;

	/// Part of the population that is part of a primary community ContactPool.
	PopSection primCom;

	/// Part of the population that is part of a secondary community ContactPool.
	PopSection secCom;

	/// Part of the population that is part of a daycare ContactPool.
	PopSection daycare;

	/// Part of the population that is part of a pre-school ContactPool.
	PopSection preschool;
};

} // namespace datavis
} // namespace stride
