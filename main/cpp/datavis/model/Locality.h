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

#include "geopop/Coordinate.h"
#include "disease/Health.h"

#include <string>

namespace stride {
namespace datavis {

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
	 * @param popCount The total population count of the locality.
	 * @param infectedFrac The fraction of the population that is infected.
	 */
	explicit Locality(const std::string& name, geopop::Coordinate coord);

	/**
	 * Retrieve the place name of the locality.
	 */
	const std::string& GetName() const;

	/**
	 * Retrieve the geographical coordinate of the locality.
	 */
	const geopop::Coordinate& GetCoordinate() const;

private:
	geopop::Coordinate m_coordinate;
	std::string        m_name;

};

} // namespace datavis
} // namespace stride
