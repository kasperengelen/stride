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
 * Implementation file for the Locality class.
 */

#include "Locality.h"

namespace stride {
namespace datavis {

const QVariantMap contactPoolView(const ContactPoolStat& pool);

/**
 * Construct a QVariantMap that represents a view for this locality.
 */
const QVariantMap Locality::GetView() const
{
	// create map
	QVariantMap map;

	map.insert("name",   QString::fromStdString(m_name));
	map.insert("lat",    m_coordinate.get<0>());
	map.insert("lon",    m_coordinate.get<1>());
	map.insert("totPop", m_total_pop);

	map.insert("household",     contactPoolView(m_household_pop));
	map.insert("k12school",     contactPoolView(m_k12school_pop));
	map.insert("college",       contactPoolView(m_college_pop));
	map.insert("workplace",     contactPoolView(m_workplace_pop));
	map.insert("primCommunity", contactPoolView(m_prim_community_pop));
	map.insert("secCommunity",  contactPoolView(m_sec_community_pop));
	map.insert("daycare",       contactPoolView(m_daycare_pop));
	map.insert("preschool",     contactPoolView(m_preschool_pop));


	return map;
}

const QVariantMap contactPoolView(const ContactPoolStat& pool)
{
	QVariantMap map;

	map.insert("total",          pool.total);
	map.insert("susceptible",    pool.susceptible);
	map.insert("exposed",        pool.exposed);
	map.insert("infectious",     pool.infectious);
	map.insert("symptomatic",    pool.symptomatic);
	map.insert("infectAndSympt", pool.infect_and_sympt);
	map.insert("recovered",      pool.recovered);
	map.insert("immune",         pool.immune);

	return map;
}

} // namespace datavis
} // namespace stride
