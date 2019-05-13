#include <utility>

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
 *  Copyright 2018, 2019, Jan Broeckhove and Bistromatics group.
 */

#include "GeoGridJSONWriter.h"

#include "contact/ContactPool.h"
#include "geopop/GeoGrid.h"
#include "pop/Person.h"

#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <omp.h>

#include "util/FileSys.h"

namespace geopop {

using namespace std;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;
using json = nlohmann::json;

GeoGridJSONWriter::GeoGridJSONWriter(shared_ptr<ostream> outputStream)
    : GeoGridStreamWriter(move(outputStream)), m_persons_found()
{
}

void GeoGridJSONWriter::Write(GeoGrid& geoGrid)
{
        json root;

        vector<json> locations;
        for (const auto& location : geoGrid) {
                locations.push_back(WriteLocation(*location));
        }
        root["locations"] = locations;

        vector<json> persons;
        for (const auto& person : m_persons_found) {
                persons.push_back(WritePerson(person));
        }
        root["persons"] = persons;

        m_persons_found.clear();
        *m_outputStream << root;
}

json GeoGridJSONWriter::WriteLocation(const Location& location)
{
        json location_root;
        location_root["id"]                      = location.GetID();
        location_root["name"]                    = location.GetName();
        location_root["province"]                = location.GetProvince();
        location_root["population"]              = location.GetPopCount();
        location_root["coordinate"]["latitude"]  = boost::geometry::get<0>(location.GetCoordinate());
        location_root["coordinate"]["longitude"] = boost::geometry::get<1>(location.GetCoordinate());
        auto         commutes                    = location.CRefOutgoingCommutes();
        vector<json> commute_vec;
        for (auto commute_pair : commutes) {
                json commutes_root;
                commutes_root["to"]         = commute_pair.first->GetID();
                commutes_root["proportion"] = commute_pair.second;
                commute_vec.push_back(commutes_root);
        }
        location_root["commute"] = commute_vec;
        vector<json> contact_pools;
        for (const auto& type : IdList) {
                const auto&  pools = location.CRefPools(type);
                vector<json> type_contact_pools;
                for (auto pool : pools) {
                        type_contact_pools.push_back(WriteContactPool(pool));
                }
                json contactPool_root;
                contactPool_root["type"]  = ToString(type);
                contactPool_root["pools"] = type_contact_pools;
                contact_pools.push_back(contactPool_root);
        }
        location_root["contactPools"] = contact_pools;

        return location_root;
}

json GeoGridJSONWriter::WriteContactPool(ContactPool* contactPool)
{
        json pool_root;
        pool_root["id"] = contactPool->GetId();
        vector<int> people;
        for (auto person : *contactPool) {
                json person_root;
                m_persons_found.insert(person);
                people.push_back(person->GetId());
        }
        pool_root["people"] = people;
        return pool_root;
}

json GeoGridJSONWriter::WritePerson(Person* person)
{
        json person_root;
        person_root["id"]                 = person->GetId();
        person_root["age"]                = person->GetAge();
        person_root["K12School"]          = person->GetPoolId(Id::K12School);
        person_root["College"]            = person->GetPoolId(Id::College);
        person_root["Household"]          = person->GetPoolId(Id::Household);
        person_root["Workplace"]          = person->GetPoolId(Id::Workplace);
        person_root["PrimaryCommunity"]   = person->GetPoolId(Id::PrimaryCommunity);
        person_root["SecondaryCommunity"] = person->GetPoolId(Id::SecondaryCommunity);
        person_root["Daycare"]            = person->GetPoolId(Id::Daycare);
        person_root["Preschool"]          = person->GetPoolId(Id::PreSchool);
        return person_root;
}

} // namespace geopop
