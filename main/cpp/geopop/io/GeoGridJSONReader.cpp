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

#include "GeoGridJSONReader.h"
#include "geopop/GeoGrid.h"
#include "pop/Population.h"
#include "util/Exception.h"

#include <boost/lexical_cast.hpp>

namespace geopop {

using namespace std;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;
using json = nlohmann::json;

GeoGridJSONReader::GeoGridJSONReader(unique_ptr<istream> inputStream, Population* pop)
    : GeoGridReader(move(inputStream), pop)
{
}

void GeoGridJSONReader::Read()
{
        json root;
        try {
                *m_inputStream >> root;
        } catch (runtime_error&) {
                throw Exception("Problem parsing JSON file, check whether empty or invalid JSON.");
        }

        auto& geoGrid = m_population->RefGeoGrid();

        for (const auto& person_json : root["persons"]) {
                auto person               = ParsePerson(person_json);
                m_people[person->GetId()] = person;
        }

        for (const auto& location : root["locations"]) {
                geoGrid.AddLocation(ParseLocation(location));
        }

        AddCommutes(geoGrid);
        m_commutes.clear();
        m_people.clear();
}

Person* GeoGridJSONReader::ParsePerson(const json& person)
{
        const auto id   = json_cast<unsigned int>(person["id"]);
        const auto age  = json_cast<float>(person["age"]);
        const auto hhId = json_cast<unsigned int>(person["Household"]);
        const auto ksId = json_cast<unsigned int>(person["K12School"]);
        const auto coId = json_cast<unsigned int>(person["College"]);
        const auto wpId = json_cast<unsigned int>(person["Workplace"]);
        const auto pcId = json_cast<unsigned int>(person["PrimaryCommunity"]);
        const auto scId = json_cast<unsigned int>(person["SecondaryCommunity"]);
        const auto dcId = json_cast<unsigned int>(person["Daycare"]);
        const auto psId = json_cast<unsigned int>(person["Preschool"]);

        return m_population->CreatePerson(id, age, hhId, ksId, coId, wpId, pcId, scId, dcId, psId);
}

shared_ptr<SimLocation> GeoGridJSONReader::ParseLocation(const json& location)
{
        const auto   id         = json_cast<unsigned int>(location["id"]);
        const string name       = location["name"];
        const auto   province   = json_cast<unsigned int>(location["province"]);
        const auto   population = json_cast<unsigned int>(location["population"]);
        const auto   coordinate = ParseCoordinate(location["coordinate"]);

        auto result = make_shared<SimLocation>(id, province, coordinate, name, population);

        for (const auto& pools : location["contactPools"]) {
                const string type_str = pools["type"];
                Id           type;
                if (type_str == ToString(Id::K12School)) {
                        type = Id::K12School;
                } else if (type_str == ToString(Id::College)) {
                        type = Id::College;
                } else if (type_str == ToString(Id::Household)) {
                        type = Id::Household;
                } else if (type_str == ToString(Id::PrimaryCommunity)) {
                        type = Id::PrimaryCommunity;
                } else if (type_str == ToString(Id::SecondaryCommunity)) {
                        type = Id::SecondaryCommunity;
                } else if (type_str == ToString(Id::Workplace)) {
                        type = Id::Workplace;
                } else if (type_str == ToString(Id::Daycare)) {
                        type = Id::Daycare;
                } else if (type_str == ToString(Id::PreSchool)) {
                        type = Id::PreSchool;
                } else {
                        throw util::Exception("No such ContactCenter type: " + type_str);
                }
                for (const auto& pool : pools["pools"]) {
                        result->RegisterPool(ParseContactPool(pool, type), type);
                }
        }

        for (auto commute : location["commute"]) {
                const auto to     = json_cast<unsigned int>(commute["to"]);
                const auto amount = json_cast<double>(commute["proportion"]);
                m_commutes.emplace_back(id, to, amount);
        }

        return result;
}

Coordinate GeoGridJSONReader::ParseCoordinate(const json& coordinate)
{
        const auto longitude = json_cast<double>(coordinate["longitude"]);
        const auto latitude  = json_cast<double>(coordinate["latitude"]);
        return {longitude, latitude};
}

ContactPool* GeoGridJSONReader::ParseContactPool(const json& contactPool, ContactType::Id typeId)

{
        // Don't use the id of the ContactPool but the let the Population create an id.
        auto result = m_population->RefPoolSys().CreateContactPool(typeId);

        for (const auto& person : contactPool["people"]) {
                auto person_id = json_cast<unsigned int>(person);
                if (m_people.count(person_id) == 0) {
                        throw Exception("No such person: " + to_string(person_id));
                }
                result->AddMember(m_people[person_id]);
        }

        return result;
}

template <typename T>
T GeoGridJSONReader::json_cast(const json& js) const
{
        if (js.is_string()) {
                return boost::lexical_cast<T>(string(js));
        } else if (js.is_number()) {
                return static_cast<T>(js);
        } else {
                return boost::lexical_cast<T>(js);
        }
}

} // namespace geopop
