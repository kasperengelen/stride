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
 *  Copyright 2019, Jan Broeckhove.
 */

#include "HouseholdJSONReader.h"
#include "util/Exception.h"
#include <boost/lexical_cast.hpp>

#include "util/json.hpp"

namespace geopop {

using namespace std;
using namespace stride::util;
using json = nlohmann::json;

HouseholdJSONReader::HouseholdJSONReader(std::unique_ptr<std::istream> inputStream)
    : m_input_stream(std::move(inputStream))
{
}

void HouseholdJSONReader::SetReferenceHouseholds(unsigned int&                           ref_person_count,
                                                 std::vector<std::vector<unsigned int>>& ref_ages)
{
        json reader;
        try {
                *m_input_stream >> reader;
        } catch (runtime_error&) {
                throw Exception("Problem parsing JSON file, check whether empty or invalid JSON.");
        }

        unsigned int p_count = 0U;
        for (const auto& row : reader["householdsList"]) {
                vector<unsigned int> temp;
                for (const auto& i : row) {
                        auto age = json_cast<unsigned int>(i);
                        temp.emplace_back(age);
                }
                p_count += temp.size();
                ref_ages.emplace_back(temp);
        }
        ref_person_count = p_count;
}

template<typename T>
T HouseholdJSONReader::json_cast(const json& js) const
{
        if (js.is_string()) {
                return boost::lexical_cast<T>(string(js));
        } else if (js.is_number()) {
                return static_cast<T>(js);
        }
        else {
                return boost::lexical_cast<T>(js);
        }
}

} // namespace geopop
