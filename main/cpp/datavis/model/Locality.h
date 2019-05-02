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
 * Each individual in a locality can belong to one or more contact pools such
 * as a college, a workplace, etc. This struct keeps track of how many individuals
 * are part of a certain type of contact pool and number of individuals per
 * health status.
 */
struct ContactPoolStat
{
	/// Total number of individuals that belong to this type of contact pool
	unsigned int total;

	/// The number of individuals with the HealthStatus::Susceptible health status.
    unsigned int susceptible;

	/// The number of individuals with the HealthStatus::Exposed health status.
    unsigned int exposed;

	/// The number of individuals with the HealthStatus::Infectious health status.
	unsigned int infectious;

	/// The number of individuals with the HealthStatus::Symptomatic health status.
	unsigned int symptomatic;

	/// The number of individuals with the HealthStatus::InfectiousAndSymptomatic health status.
    unsigned int infect_and_sympt;

	/// The number of individuals with the HealthStatus::Recovered health status.
    unsigned int recovered;

	/// The number of individuals with the HealthStatus::Immune health status.
    unsigned int immune;
};

/**
 * Class that represents a locality. This is a geographical location
 * that has a population, of which a certain fraction can be infected.
 */
class Locality
{
public:

	/**
	 * Parametrised constructor.
	 *
     * @param name The place name of the locality.
	 * @param coord The coordinate that represents the geographical location of the locality.
	 * @param popCatMap An std::map that maps ContactType::Id to PopCategory objects.
	 */
	explicit Locality(const std::string& name, const geopop::Coordinate& coord, unsigned int totPop,
					  const ContactPoolStat& householdData, const ContactPoolStat& k12schoolData, const ContactPoolStat& collegeData, const ContactPoolStat& workplaceData,
					  const ContactPoolStat& primCommunityData, const ContactPoolStat& secCommunityData, const ContactPoolStat& daycareData, const ContactPoolStat& preschoolData)
		: m_name(name), m_coordinate(coord), m_total_pop(totPop),
		  m_household_pop(householdData), m_k12school_pop(k12schoolData), m_college_pop(collegeData), m_workplace_pop(workplaceData),
		  m_prim_community_pop(primCommunityData), m_sec_community_pop(secCommunityData), m_daycare_pop(daycareData), m_preschool_pop(preschoolData)
	{}

	/**
	 * Retrieve the place name of the locality.
	 */
	const std::string& GetName() const { return m_name; }

	/**
	 * Retrieve the geographical coordinate of the locality.
	 */
	const geopop::Coordinate& GetCoordinate() const { return m_coordinate; }

	/**
	 * Retrieve the total population within the locality.
	 */
	unsigned int GetTotalPop() const { return m_total_pop; }

	/**
	 * Retrieve information about the population that is part of a household.
	 */
	const ContactPoolStat& GetHouseholdPopData() const { return m_household_pop; }

	/**
	 * Retrieve information about the population that is part of a k12 school.
	 */
	const ContactPoolStat& GetK12SchoolPopData() const { return m_k12school_pop; }

	/**
	 * Retrieve information about the population that is part of a college.
	 */
	const ContactPoolStat& GetCollegePopData() const { return m_college_pop; }

	/**
	 * Retrieve information about the population that is part of a workplace.
	 */
	const ContactPoolStat& GetWorkplacePopData() const { return m_workplace_pop; }

	/**
	 * Retrieve information about the population that is part of a primary community.
	 */
	const ContactPoolStat& GetPrimCommunityPopData() const { return m_prim_community_pop; }

	/**
	 * Retrieve information about the population that is part of a secondary community.
	 */
	const ContactPoolStat& GetSecCommunityPopData() const { return m_sec_community_pop; }

	/**
	 * Retrieve information about the population that is part of a daycare.
	 */
	const ContactPoolStat& GetDaycarePopData() const { return m_daycare_pop; }

	/**
	 * Retrieve information about the population that is part of a preschool.
	 */
	const ContactPoolStat& GetPreSchoolPopData() const { return m_preschool_pop; }

	/**
	 * Construct a QVariantMap that represents a view for this locality.
	 */
	const QVariantMap GetView() const;

private:
	std::string        m_name;
	geopop::Coordinate m_coordinate;
	unsigned int       m_total_pop;

	ContactPoolStat    m_household_pop;
	ContactPoolStat    m_k12school_pop;
	ContactPoolStat    m_college_pop;
	ContactPoolStat    m_workplace_pop;
	ContactPoolStat    m_prim_community_pop;
	ContactPoolStat    m_sec_community_pop;
	ContactPoolStat    m_daycare_pop;
	ContactPoolStat    m_preschool_pop;
};

} // namespace datavis
} // namespace stride
