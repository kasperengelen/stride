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

#include "WorkplaceCSVReader.h"

#include "util/CSV.h"
#include "util/CSVRow.h"

namespace geopop {

using namespace std;
using namespace stride::util;

WorkplaceCSVReader::WorkplaceCSVReader(unique_ptr<istream> inputStream) : WorkplaceReader(move(inputStream)) {}

void WorkplaceCSVReader::SetWorkplaceData(std::vector<double>&                                ratios,
                                          std::vector<std::pair<unsigned int, unsigned int>>& sizes) const
{
        CSV reader(*(m_inputStream.get()));

        for (const CSVRow& row : reader) {
                // In file: ratio,size_min,size_max
                ratios.emplace_back(row.GetValue<double>(0));

                auto range = std::make_pair(row.GetValue<unsigned int>(1), row.GetValue<unsigned int>(2));
                sizes.emplace_back(range);
        }
}

} // namespace geopop
