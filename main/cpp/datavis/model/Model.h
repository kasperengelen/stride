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
	explicit Model() : m_timesteps()
	{}

	/**
	 * Deleted copy CTOR.
	 */
	Model(const Model&) = delete;

	/**
	 * Deleted assignment operator.
	 */
	Model& operator=(const Model&) = delete;

	/**
	 * Add a timestep to the model.
	 */
	void AddTimestep(const std::vector<Locality>& timestep) { m_timesteps.push_back(timestep); }

	/**
	 * Remove all the timesteps from the model.
	 */
	void ClearTimesteps() { m_timesteps.clear(); }

	/**
	 * Retrieve simulation data from the model.
	 */
	const std::vector<std::vector<Locality>>& GetEpiData() const { return m_timesteps; }


private:
	/// Contains the currently stored simulation timesteps.
	std::vector<std::vector<Locality>> m_timesteps;
};


} // namespace datavis
} // namespace stride
