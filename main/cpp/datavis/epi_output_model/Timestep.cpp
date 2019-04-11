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

#include "datavis/epi_output_model/Timestep.h"

namespace stride {
namespace datavisualiser {

Timestep::Timestep()
 : m_localities()
{}

void Timestep::AddLocality(const Locality& locality)
{
	m_localities.push_back(locality);
}

const std::vector<Locality>& Timestep::GetLocalities() const
{
	return m_localities;
}

}
}

