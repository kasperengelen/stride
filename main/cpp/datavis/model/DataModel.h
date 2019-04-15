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
 * Header file for the DataModel class.
 */

#pragma once

#include "datavis/model/Timestep.h"

#include <vector>

namespace stride {
namespace datavisualiser {


class DataModel
{
public:
	/**
	 * Default constructor.
	 */
	DataModel();

	/**
	 * Add a timestep.
	 */
	void AddTimestep(const Timestep& timestep);

	/**
	 * Retrieve a list of timesteps.
	 */
	const std::vector<Timestep>& GetTimesteps() const ;

private:
	// vector<vector<Locality>>
	std::vector<Timestep> m_timesteps;
};


}
}
