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
 * Header file for the PopCategory class.
 */

#pragma once

#include "disease/Health.h"

#include <map>

namespace stride {
namespace datavis {

class PopCategory
{
public:
	explicit PopCategory(const unsigned int popCount, std::map<HealthStatus, unsigned int> healthMap);

private:
	unsigned int m_pop_count;
	std::map<HealthStatus, unsigned int> m_healthstatus_counts;

};

} // namespace datavis
} // namespace stride

