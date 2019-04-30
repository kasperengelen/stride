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

Locality::Locality(const std::string& name, geopop::Coordinate coord)
		: m_coordinate(coord), m_name(name), m_college_pop(0), m_daycare_pop(0)
{}

const std::string& Locality::GetName() const
{
	return m_name;
}

const geopop::Coordinate& Locality::GetCoordinate() const
{
	return m_coordinate;
}

} // namespace datavis
} // namespace stride
