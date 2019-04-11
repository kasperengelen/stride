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
 */

/**
 * @file
 * Header file for the Timestep class.
 */

#pragma once

#include "datavis/epi_output_model/Locality.h"

#include <vector>

namespace stride {
namespace datavisualiser {

/**
 * Class that represents what a geographical area look like during
 * a specific timestep. It contains a list of localities.
 */
class Timestep
{
public:
	/**
	 * Default constructor.
	 */
	Timestep();

	/**
	 * Add a locality to the timestep.
	 */
	void AddLocality(const Locality& locality);

	/**
	 * Retrieve the localities that are part of the timestep.
	 */
	const std::vector<Locality>& GetLocalities() const;

private:
	std::vector<Locality> m_localities;
};

}
}


