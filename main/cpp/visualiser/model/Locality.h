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
#include "disease/Health.h"
#include "geopop/Coordinate.h"

#include "PopData.h"

#include <map>
#include <string>
#include <algorithm>

namespace stride {
namespace visualiser {

/**
 * @class Locality
 * Class that represents a locality. This is a geographical location
 * that has a population, of which a certain fraction can be infected.
 */
class Locality
{
public:
        /**
         * Constructor based on a name, coordinate and population.
         */
        explicit Locality(const std::string& name, const geopop::Coordinate& coord, const PopData& popData)
        	: m_name{name}, m_coordinate{coord}, m_popdata{popData}
        {}

        /**
         * Retrieve the place name of the locality.
         */
        const std::string& GetName() const { return m_name; }

        /**
         * Retrieve the geographical coordinate of the locality. The coordinate is of the (longtidude, latitude) format.
         */
        const geopop::Coordinate& GetCoordinate() const { return m_coordinate; }

        /**
         * Retrieve a PopData object that represents the population that lives inside the
         * locality.
         */
        const PopData& GetPopData() const { return m_popdata; }

private:
        /// The name of the locality
        std::string m_name;

        /// The coordinate of the locality. The coordinate is of the (longtidude, latitude) format.
        geopop::Coordinate m_coordinate;

        /// The population that lives inside the locality
        PopData m_popdata;
};

} // namespace visualiser
} // namespace stride
