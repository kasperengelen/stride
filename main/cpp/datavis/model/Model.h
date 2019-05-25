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
 * Header file for the Model class.
 */

#pragma once

#include "datavis/model/Locality.h"
#include "geopop/Coordinate.h"

#include <vector>

namespace stride {
namespace datavis {

/**
 * Class that represents the model of the visualiser. This contains all the stored simulation data.
 */
class Model
{
public:
        /**
         * Constructor.
         */
        explicit Model() : m_timesteps() {}

        /**
         * Deleted copy CTOR.
         */
        Model(const Model&) = delete;

        /**
         * Deleted assignment operator.
         */
        Model& operator=(const Model&) = delete;

        /**
         * Set the timesteps contained in the model to the specified timesteps.
         */
        void SetTimesteps(const std::vector<std::vector<Locality>>& timesteps) { m_timesteps = timesteps; }

        /**
         * Retrieve simulation data from the model.
         */
        const std::vector<std::vector<Locality>>& GetEpiData() const { return m_timesteps; }

        /**
         * Retrieve information about the part of the population that is within
         * the specified radius of the specified coordinates. The selection is based
         * on what the locations looked like on the specified day.
         */
        const PopData GetPopulationInRadius(const geopop::Coordinate& center, const double radius, const unsigned int day) const;

        /**
         * Retrieve information about the part of the population that is within
         * a rectangle that spans between the two specified points. The selection is based
         * on what the locations looked like on the specified day.
         */
        const PopData GetPopulationInBox(const geopop::Coordinate& pointA, const geopop::Coordinate& pointB, const unsigned int day) const;

private:
        /// Contains the currently stored simulation timesteps.
        std::vector<std::vector<Locality>> m_timesteps;
};

} // namespace datavis
} // namespace stride
