#include "GeoGridHDF5Writer.h"

#include <H5Cpp.h>
#include <H5DataSpace.h>
#include <iostream>
#include <string>

#include "contact/ContactPool.h"
#include "geopop/GeoGrid.h"
#include "pop/Person.h"

namespace geopop {

using namespace H5;
using namespace std;
using namespace stride;
using namespace stride::ContactType;

GeoGridHDF5Writer::GeoGridHDF5Writer() : m_persons_found() {}

void GeoGridHDF5Writer::Write(GeoGrid& geoGrid, const string& filename)
{

        try {

                /// Create file
                Exception::dontPrint();
                H5File file(filename, H5F_ACC_TRUNC);

                Group locations(file.createGroup("Locations"));
                int   count = 1;
                for (const auto& location : geoGrid) {
                        WriteLocation(*location, locations, count++);
                }

                WritePersons(file);

        } catch (FileIException& error) {
                error.printErrorStack();
        }

        // catch failure caused by the DataSet operations
        catch (DataSetIException& error) {
                error.printErrorStack();
        }

        // catch failure caused by the DataSpace operations
        catch (DataSpaceIException& error) {
                error.printErrorStack();
        }
        // catch failure caused by the Group operations
        catch (GroupIException& error) {
                error.printErrorStack();
        }
}

template <typename T>
void WriteAttribute(H5Object& object, const std::string& name, const DataType& type, const T& data)
{
        hsize_t   dim[1] = {1};
        DataSpace dataspace(1, dim);
        Attribute attribute         = object.createAttribute(name, type, dataspace);
        T         attribute_data[1] = {data};
        attribute.write(type, attribute_data);
}

void WriteCoordinate(H5Object& object, const Coordinate& coordinate)
{
        hsize_t   dim[2] = {1, 2};
        DataSpace dataspace(2, dim);
        Attribute attribute         = object.createAttribute("coordinate", PredType::NATIVE_DOUBLE, dataspace);
        double    attribute_data[2] = {boost::geometry::get<0>(coordinate), boost::geometry::get<1>(coordinate)};
        attribute.write(PredType::NATIVE_DOUBLE, attribute_data);
}

void GeoGridHDF5Writer::WriteContactPool(H5Location& h5_location, const ContactPool* pool, int count)
{
        const auto&  persons = pool->GetPool();

        vector<unsigned int> persons_data;

        for (Person* person : persons) {
                persons_data.push_back(person->GetId());
                m_persons_found.insert(person);
        }
        CompType comp_type(sizeof(unsigned int));
        comp_type.insertMember("people", 0, PredType::NATIVE_UINT);

        hsize_t   pool_dim[] = {pool->size()};
        DataSpace pool_ds(1, pool_dim);
        DataSet   people = h5_location.createDataSet("Pool" + to_string(count), comp_type, pool_ds);
        people.write(&persons_data.front(), comp_type);

        WriteAttribute(people, "id", PredType::NATIVE_UINT, pool->GetId());
        WriteAttribute(people, "type", PredType::NATIVE_UINT, static_cast<unsigned int>(pool->GetType()));
}

void GeoGridHDF5Writer::WriteLocation(const Location& location, H5Location& h5_location, int count)
{
        Group loc(h5_location.createGroup("Loc" + to_string(count)));
        WriteAttribute(loc, "id", PredType::NATIVE_UINT, location.GetID());
        WriteAttribute(loc, "name", StrType(PredType::C_S1, location.GetName().size() + 1), location.GetName());
        WriteAttribute(loc, "province", PredType::NATIVE_UINT, location.GetProvince());
        WriteAttribute(loc, "population", PredType::NATIVE_UINT, location.GetPopCount());
        WriteCoordinate(loc, location.GetCoordinate());

        struct CommuteData
        {
                unsigned int to;
                double       proportion;
        };

        const auto& commutes = location.CRefOutgoingCommutes();

        vector<CommuteData> commute_data;
        for (const auto& commute : commutes) {
                CommuteData data{};
                data.to         = commute.first->GetID();
                data.proportion = commute.second;
                commute_data.push_back(data);
        }

        CompType comp_type(sizeof(CommuteData));
        comp_type.insertMember("to", HOFFSET(CommuteData, to), PredType::NATIVE_UINT);
        comp_type.insertMember("proportion", HOFFSET(CommuteData, proportion), PredType::NATIVE_DOUBLE);

        hsize_t   com_dim[] = {commutes.size()};
        DataSpace com_ds(1, com_dim);
        DataSet   commute = loc.createDataSet("Commute", comp_type, com_ds);
        commute.write(&commute_data.front(), comp_type);

        Group contact_pools(loc.createGroup("ContactPools"));

        int pool_count = 1;
        for (const auto& type : IdList) {
                const auto& pools = location.CRefPools(type);
                for (auto pool : pools) {
                        WriteContactPool(contact_pools, pool, pool_count++);
                }
        }
}

void GeoGridHDF5Writer::WritePersons(H5Location& h5_location)
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

        vector<PersonsData> persons_data;

        for (const auto& person : m_persons_found) {
                PersonsData data{};
                data.id                  = person->GetId();
                data.age                 = person->GetAge();
                data.daycare             = person->GetPoolId(Id::Daycare);
                data.preschool           = person->GetPoolId(Id::PreSchool);
                data.k12school           = person->GetPoolId(Id::K12School);
                data.household           = person->GetPoolId(Id::Household);
                data.workplace           = person->GetPoolId(Id::Workplace);
                data.primary_community   = person->GetPoolId(Id::PrimaryCommunity);
                data.secondary_community = person->GetPoolId(Id::SecondaryCommunity);
                data.collage             = person->GetPoolId(Id::College);
                persons_data.push_back(data);
        }

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

        hsize_t   pers_dim[] = {m_persons_found.size()};
        DataSpace pers_ds(1, pers_dim);
        DataSet   persons = h5_location.createDataSet("Persons", comp_type, pers_ds);
        persons.write(&persons_data.front(), comp_type);

        WriteAttribute(persons, "size", PredType::NATIVE_UINT, m_persons_found.size());
}

} // namespace geopop
