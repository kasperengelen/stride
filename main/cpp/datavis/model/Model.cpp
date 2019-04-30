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
 * Implementation file for the DataModel class.
 */

#include "Model.h"

#include <iostream>

namespace stride {
namespace datavis {

void Model::AddTimestep(const Timestep& timestep)
{
	m_timesteps.push_back(timestep);
}

void Model::ClearTimesteps()
{
	m_timesteps.clear();
	m_current_timestep = 0;
}

const Timestep& Model::GetCurrentTimestepData()
{
	return m_timesteps.at(m_current_timestep);
}


bool Model::HasNextTimestep() const
{
	return m_current_timestep < (m_timesteps.size() - 1);
}

bool Model::HasPrevTimestep() const
{
	return m_current_timestep > 0; // 0 is the first timestep
}


void Model::NextTimestep()
{
	m_current_timestep++;
}

void Model::PrevTimestep()
{
	m_current_timestep--;
}


void Model::FirstTimestep()
{
	m_current_timestep = 0;
}

void Model::LastTimestep()
{
	m_current_timestep = m_timesteps.size() - 1;
}



} // namespace datavis
} // namespace stride




