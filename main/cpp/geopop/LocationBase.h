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
 * Header file for the LocationBase class.
 */

#pragma once

#include "geopop/Coordinate.h"

#include <string>

namespace geopop {

/**
 * Location for use within the GeoGrid, contains Coordinate and index to ContactPools.
 * This class serves as a base class for the SimLocation and VisLocation classes and will
 * be used by GeoGridBase.
 */
class LocationBase {
public:
        /// Parametrized constructor.
        explicit LocationBase(Coordinate coordinate = Coordinate{0.0, 0.0}, std::string name = "");

        /// Perform a full comparison with the other location.
        bool operator==(const LocationBase& other) const;

        /// Gets the Coordinate of this Location.
        const Coordinate GetCoordinate() const { return m_coordinate; }

        /// Gets the name.
        std::string GetName() const { return m_name; }

        /// Sets the Coordinate of this Location.
        void SetCoordinate(const Coordinate& coordinate) { m_coordinate = coordinate; }

private:
        Coordinate   m_coordinate;   ///< Coordinate of the Location.
        std::string  m_name;         ///< Name.
};

} // namespace geopop
