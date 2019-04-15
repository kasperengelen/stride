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
 * Implementation file for the DataModel class.
 */

#include "DataModel.h"

namespace stride {
namespace datavisualiser {

DataModel::DataModel()
	: m_timesteps{}
{}

/**
 * Add a timestep.
 */
void DataModel::AddTimestep(const Timestep& timestep)
{
	m_timesteps.push_back(timestep);
}

/**
 * Retrieve a list of timesteps.
 */
const std::vector<Timestep>& DataModel::GetTimesteps() const
{
	return m_timesteps;
}

}
}




