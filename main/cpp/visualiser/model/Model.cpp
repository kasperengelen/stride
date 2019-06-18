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

namespace stride {
namespace visualiser {

const geopop::PopStats Model::GetPopulationInRadius(const geopop::Coordinate& center, const double radius,
                                           const unsigned int day) const
{

        const VisGeoGrid& day_locs = *m_timesteps.at(day);

        PopStats retval{};

        // convert to km
        const double radius_km = radius / 1000.0;


        const auto loc_list = day_locs.LocationsInRadius(center.get<0>(), center.get<1>(), radius_km);

        std::cout << "SELECTED " << loc_list.size() << " LOCATIONS." << std::endl;



        // TODO fill with selection
        return retval;
}

const geopop::PopStats Model::GetPopulationInBox(const geopop::Coordinate& pointA, const geopop::Coordinate& pointB,
                                        const unsigned int day) const
{
        const VisGeoGrid& day_locs = *m_timesteps.at(day);


        const auto loc_list = day_locs.LocationsInBox(pointA.get<0>(), pointA.get<1>(), pointB.get<0>(), pointB.get<1>());

        std::cout << "SELECTED " << loc_list.size() << " LOCATIONS." << std::endl;

        PopStats retval{};

        // TODO fill with selection
        return retval;
}

} // namespace visualiser
} // namespace stride
