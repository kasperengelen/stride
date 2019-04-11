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

#include "datavis/epi_output_model/Locality.h"

namespace stride {
namespace datavisualiser {

Locality::Locality(const std::string& name, geopop::Coordinate coord, unsigned int popCount, double infectedFrac)
		: m_name(name), m_coordinate(coord), m_pop_count(popCount), m_infected_frac(infectedFrac)
{}

const std::string& Locality::GetName() const
{
	return m_name;
}

const geopop::Coordinate& Locality::GetCoordinate() const
{
	return m_coordinate;
}

unsigned int Locality::GetPopCount() const
{
	return m_pop_count;
}

double Locality::getInfectedFraction() const
{
	return m_infected_frac;
}

}
}
