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

#pragma once

#include "GeoGridStreamWriter.h"
#include "contact/ContactPool.h"
#include "geopop/Location.h"
#include "json.hpp"
#include "pop/Person.h"
#include <set>

namespace geopop {

/**
 * Writes a GeoGrid to a JSON file.
 */
class GeoGridJSONWriter : public GeoGridStreamWriter
{
public:
        /// Construct the GeoGridJSONWriter.
        explicit GeoGridJSONWriter(std::shared_ptr<std::ostream> stream);

        /// Write the provided GeoGrid to the ostream in JSON format.
        void Write(GeoGrid& geoGrid) override;

private:
        /// Create a nlohmann::json containing all info needed to reconstruct a Location.
        nlohmann::json WriteLocation(const Location& location);

        /// Create a nlohmann::json containing all info needed to reconstruct a ContactPool.
        nlohmann::json WriteContactPool(stride::ContactPool* contactPool);

        /// Create a nlohmann::json containing all info needed to reconstruct a Person.
        nlohmann::json WritePerson(stride::Person* person);

private:
        /// The persons found when looping over the ContactPools.
        std::set<stride::Person*> m_persons_found;
};

} // namespace geopop
