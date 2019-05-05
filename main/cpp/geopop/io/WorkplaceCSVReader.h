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
 *  Copyright 2018, Jan Broeckhove and Bistromatics group.
 */

#pragma once

#include <geopop/GeoGridConfig.h>
#include "WorkplaceReader.h"

namespace geopop {

/**
 * Create a Reader that fills a GeoGridConfig with the workplace size data found in a CSV input stream.
 * Data are expected in format:
 * ratio, size_min, size_max
 */
class WorkplaceCSVReader : public WorkplaceReader
{
public:
        /// Construct the WorkplaceCSVReader that reads istream CSV data.
        explicit WorkplaceCSVReader(std::unique_ptr<std::istream> inputStream);

        /// Add the workplace size info to the GeoGridConfig.
        void SetWorkplaceData(std::vector<double>& ratios, std::vector<std::pair<unsigned int, unsigned int>>& sizes) const override;
};

} // namespace geopop
