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
 * Header file for the Locality class.
 */

#pragma once

#include "contact/ContactType.h"
#include "geopop/Coordinate.h"
#include "disease/Health.h"

#include <QVariant>

#include <string>
#include <map>

namespace stride {
namespace datavis {

/**
 * Struct that keeps track of a section of the population inside a
 * locality. Note that the fractions of people who are immune, infected, etc.
 * don't add up to 1.0, since people can belong to many disease categories
 * simultaneously.
 */
struct PopSection {
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
 * Class that represents a locality. This is a geographical location
 * that has a population, of which a certain fraction can be infected.
 */
class Locality {
public:

	/**
	 * Parametrised constructor.
	 *
	 * @param name The place name of the locality.
	 * @param coord The coordinate that represents the geographical location of the locality. The coordinate is of the (longtidude, latitude) format.
	 * @param totalPopData A PopSection object that represents the total population of the locality.
	 * @param householdData A PopSection object that represents the population that is part of a household.
	 * @param k12schoolData A PopSection object that represents the population that is part of a k12 school.
	 * @param collegeData A PopSection object that represents the population that is part of a college.
	 * @param workplaceData A PopSection object that represents the population that is part of a workplace.
	 * @param primCommunityData A PopSection object that represents the population that is part of a primary community.
	 * @param secCommunityData A PopSection object that represents the population that is part of a secondary community.
	 * @param daycareData A PopSection object that represents the population that is part of a daycare.
	 * @param preschoolData A PopSection object that represents the population that is part of a preschool.
	 */
	explicit Locality(const std::string& name, const geopop::Coordinate& coord,
			const PopSection& totalPopData, const PopSection& householdData,
			const PopSection& k12schoolData, const PopSection& collegeData,
			const PopSection& workplaceData,
			const PopSection& primCommunityData,
			const PopSection& secCommunityData, const PopSection& daycareData,
			const PopSection& preschoolData) :
			m_name(name), m_coordinate(coord), m_total_pop(totalPopData), m_household_pop(
					householdData), m_k12school_pop(k12schoolData), m_college_pop(
					collegeData), m_workplace_pop(workplaceData), m_prim_community_pop(
					primCommunityData), m_sec_community_pop(secCommunityData), m_daycare_pop(
					daycareData), m_preschool_pop(preschoolData) {
	}

	/**
	 * Retrieve the place name of the locality.
	 */
	const std::string& GetName() const {
		return m_name;
	}

	/**
	 * Retrieve the geographical coordinate of the locality. The coordinate is of the (longtidude, latitude) format.
	 */
	const geopop::Coordinate& GetCoordinate() const {
		return m_coordinate;
	}

	/**
	 * Retrieve information about the total population that is part of the locality.
	 */
	const PopSection& GetTotalPopData() const {
		return m_total_pop;
	}

	/**
	 * Retrieve information about the population that is part of a household. This
	 * also includes individuals that are not part of the locality, but sometimes visit the locality.
	 */
	const PopSection& GetHouseholdPopData() const {
		return m_household_pop;
	}

	/**
	 * Retrieve information about the population that is part of a k12 school. This
	 * also includes individuals that are not part of the locality, but sometimes visit the locality.
	 */
	const PopSection& GetK12SchoolPopData() const {
		return m_k12school_pop;
	}

	/**
	 * Retrieve information about the population that is part of a college. This
	 * also includes individuals that are not part of the locality, but sometimes visit the locality.
	 */
	const PopSection& GetCollegePopData() const {
		return m_college_pop;
	}

	/**
	 * Retrieve information about the population that is part of a workplace. This
	 * also includes individuals that are not part of the locality, but sometimes visit the locality.
	 */
	const PopSection& GetWorkplacePopData() const {
		return m_workplace_pop;
	}

	/**
	 * Retrieve information about the population that is part of a primary community. This
	 * also includes individuals that are not part of the locality, but sometimes visit the locality.
	 */
	const PopSection& GetPrimCommunityPopData() const {
		return m_prim_community_pop;
	}

	/**
	 * Retrieve information about the population that is part of a secondary community. This
	 * also includes individuals that are not part of the locality, but sometimes visit the locality.
	 */
	const PopSection& GetSecCommunityPopData() const {
		return m_sec_community_pop;
	}

	/**
	 * Retrieve information about the population that is part of a daycare. This
	 * also includes individuals that are not part of the locality, but sometimes visit the locality.
	 */
	const PopSection& GetDaycarePopData() const {
		return m_daycare_pop;
	}

	/**
	 * Retrieve information about the population that is part of a preschool. This
	 * also includes individuals that are not part of the locality, but sometimes visit the locality.
	 */
	const PopSection& GetPreSchoolPopData() const {
		return m_preschool_pop;
	}

private:
	/// The name of the locality
	std::string m_name;

	/// The coordinate of the locality. The coordinate is of the (longtidude, latitude) format.
	geopop::Coordinate m_coordinate;

	/// Total population
	PopSection m_total_pop;

	/// Household population
	PopSection m_household_pop;

	/// K12 school population
	PopSection m_k12school_pop;

	/// College population
	PopSection m_college_pop;

	/// Workplace population
	PopSection m_workplace_pop;

	/// Primary community population
	PopSection m_prim_community_pop;

	/// Secondary community population
	PopSection m_sec_community_pop;

	/// Daycare population
	PopSection m_daycare_pop;

	/// Preschool population
	PopSection m_preschool_pop;
};

} // namespace datavis
} // namespace stride

