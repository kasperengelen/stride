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

#include "datavis/model/Timestep.h"

#include <vector>

namespace stride {
namespace datavis {


class Model
{
public:
	/// Constructor.
	Model();

	void AddTimestep(const Timestep& timestep);
	void ClearTimesteps();
	void SetTimesteps(const std::vector<Timestep>& timesteps);

	const Timestep& GetCurrentTimestepData();

	bool HasNextTimestep() const;
	bool HasPrevTimestep();

	void NextTimestep();
	void PrevTimestep();

	void FirstTimestep();
	void LastTimestep();

private:
	// vector<vector<Locality>>
	std::vector<Timestep> m_timesteps;

	int m_currentTimestep;
};


} // namespace datavis
} // namespace stride
