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

#include "LocationBase.h"

namespace geopop {

LocationBase::LocationBase(Coordinate coordinate, std::string name)
    : m_coordinate{coordinate}, m_name{move(name)}
{}

bool LocationBase::operator==(const LocationBase& other) const
{
    return GetName() == other.GetName()
            && GetCoordinate().get<0>() == other.GetCoordinate().get<0>()
            && GetCoordinate().get<1>() == other.GetCoordinate().get<1>();
}

} // namespace geopop
