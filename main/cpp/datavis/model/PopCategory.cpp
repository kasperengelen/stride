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
 * Implementation file for the PopCategory class.
 */

#include "PopCategory.h"

namespace stride {
namespace datavis {


PopCategory::PopCategory(const unsigned int popCount) : m_pop_count(popCount), m_healthstatus_counts()
{
	// TODO insert health statusses here
	m_healthstatus_counts.insert({HealthStatus::Susceptible, 50});
}

} // namespace datavis
} // namespace stride


