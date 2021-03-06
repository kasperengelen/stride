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

#include "GeoGridHDF5Reader.h"

#include "geopop/GeoGrid.h"
#include "pop/Population.h"
#include "util/Exception.h"
#include <boost/lexical_cast.hpp>

namespace geopop {

using namespace H5;
using namespace std;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

GeoGridHDF5Reader::GeoGridHDF5Reader(Population* pop, std::string filename)
    : GeoGridReader(unique_ptr<istream>(), pop), m_filename(std::move(filename))
{
}

void GeoGridHDF5Reader::Read()
{
        /// Create file
        H5::Exception::dontPrint();
        H5File file(m_filename, H5F_ACC_RDONLY);

        auto& geoGrid = m_population->RefGeoGrid();

        DataSet persons(file.openDataSet("Persons"));
        ParsePersons(persons);

        Group        locations(file.openGroup("Locations"));
        unsigned int size;
        locations.openAttribute("size").read(PredType::NATIVE_UINT, &size);
        for (unsigned long i = 1; i <= size; i++) {
                Group loc(locations.openGroup("Loc" + to_string(i)));
                auto  location = ParseLocation(loc);
                geoGrid.AddLocation(move(location));
        }

        AddCommutes(geoGrid);
        m_commutes.clear();
        m_people.clear();
}

void GeoGridHDF5Reader::ParsePersons(const DataSet& persons)
{
        struct PersonsData
        {
                unsigned int id;
                float        age;
                unsigned int daycare;
                unsigned int preschool;
                unsigned int k12school;
                unsigned int household;
                unsigned int workplace;
                unsigned int primary_community;
                unsigned int secondary_community;
                unsigned int collage;
        };

        CompType comp_type(sizeof(PersonsData));
        comp_type.insertMember("id", HOFFSET(PersonsData, id), PredType::NATIVE_UINT);
        comp_type.insertMember("age", HOFFSET(PersonsData, age), PredType::NATIVE_FLOAT);
        comp_type.insertMember("daycare", HOFFSET(PersonsData, daycare), PredType::NATIVE_UINT);
        comp_type.insertMember("preschool", HOFFSET(PersonsData, preschool), PredType::NATIVE_UINT);
        comp_type.insertMember("k12school", HOFFSET(PersonsData, k12school), PredType::NATIVE_UINT);
        comp_type.insertMember("household", HOFFSET(PersonsData, household), PredType::NATIVE_UINT);
        comp_type.insertMember("workplace", HOFFSET(PersonsData, workplace), PredType::NATIVE_UINT);
        comp_type.insertMember("primary community", HOFFSET(PersonsData, primary_community), PredType::NATIVE_UINT);
        comp_type.insertMember("secondary community", HOFFSET(PersonsData, secondary_community), PredType::NATIVE_UINT);
        comp_type.insertMember("collage", HOFFSET(PersonsData, collage), PredType::NATIVE_UINT);

        unsigned int size;
        persons.openAttribute("size").read(PredType::NATIVE_UINT, &size);

        vector<PersonsData> persons_data(size);
        persons.read(&persons_data.front(), comp_type);

        for (auto p : persons_data) {
                m_people[p.id] =
                    m_population->CreatePerson(p.id, p.age, p.household, p.k12school, p.collage, p.workplace,
                                               p.primary_community, p.secondary_community, p.daycare, p.preschool);
        }
}

Coordinate GeoGridHDF5Reader::ParseCoordinate(const Group& loc)
{
        double coordinate[2];
        loc.openAttribute("coordinate").read(PredType::NATIVE_DOUBLE, coordinate);
        const auto longitude = boost::lexical_cast<double>(coordinate[0]);
        const auto latitude  = boost::lexical_cast<double>(coordinate[1]);
        return {longitude, latitude};
}

shared_ptr<SimLocation> GeoGridHDF5Reader::ParseLocation(const Group& loc)
{
        unsigned int id;
        loc.openAttribute("id").read(PredType::NATIVE_UINT, &id);
        string    name;
        Attribute attr_name = loc.openAttribute("name");
        StrType   stype     = attr_name.getStrType();
        attr_name.read(stype, name);
        unsigned int population;
        loc.openAttribute("population").read(PredType::NATIVE_UINT, &population);
        unsigned int province;
        loc.openAttribute("province").read(PredType::NATIVE_UINT, &province);
        Coordinate coordinate = ParseCoordinate(loc);

        auto location = make_shared<SimLocation>(id, province, coordinate, name, population);

        DataSet commute = loc.openDataSet("Commute");
        struct CommuteData
        {
                unsigned int to;
                double       proportion;
        };
        CompType comp_type(sizeof(CommuteData));
        comp_type.insertMember("to", HOFFSET(CommuteData, to), PredType::NATIVE_UINT);
        comp_type.insertMember("proportion", HOFFSET(CommuteData, proportion), PredType::NATIVE_DOUBLE);

        unsigned int size;
        commute.openAttribute("size").read(PredType::NATIVE_UINT, &size);

        vector<CommuteData> commute_data(size);
        commute.read(&commute_data.front(), comp_type);

        for (auto c : commute_data) {
                m_commutes.emplace_back(id, c.to, c.proportion);
        }

        Group        contact_pools = loc.openGroup("ContactPools");
        unsigned int pool_size;
        contact_pools.openAttribute("size").read(PredType::NATIVE_UINT, &pool_size);
        for (unsigned int i = 1; i <= pool_size; i++) {
                DataSet pool         = contact_pools.openDataSet("Pool" + to_string(i));
                auto    contact_pool = ParseContactPool(pool);
                location->RegisterPool(contact_pool, contact_pool->GetType());
        }
        return location;
}

ContactPool* GeoGridHDF5Reader::ParseContactPool(const DataSet& pool)
{
        // Don't use the id of the ContactPool but let the Population create an id.
        string    type_str;
        Attribute attr_name = pool.openAttribute("type");
        StrType   stype     = attr_name.getStrType();
        attr_name.read(stype, type_str);
        Id type;
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
        auto contact_pool = m_population->RefPoolSys().CreateContactPool(type);

        CompType comp_type(sizeof(unsigned int));
        comp_type.insertMember("people", 0, PredType::NATIVE_UINT);

        unsigned int size;
        pool.openAttribute("size").read(PredType::NATIVE_UINT, &size);

        vector<unsigned int> persons_data(size);
        pool.read(&persons_data.front(), comp_type);
        for (auto person_id : persons_data) {
                if (m_people.count(person_id) == 0) {
                        throw util::Exception("No such person: " + to_string(person_id));
                }
                contact_pool->AddMember(m_people[person_id]);
        }
        return contact_pool;
}

} // namespace geopop
