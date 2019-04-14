#include "GeoGridHDF5Reader.h"

#include <H5Cpp.h>
#include "geopop/GeoGrid.h"
#include "pop/Population.h"


namespace geopop {

using namespace H5;
using namespace std;
using namespace stride;
using namespace stride::ContactType;
using namespace stride::util;

GeoGridHDF5Reader::GeoGridHDF5Reader(Population* pop, const std::string& filename)
    : GeoGridReader(unique_ptr<istream>(), pop), m_filename(filename) {
}


void GeoGridHDF5Reader::Read()
{

        try {

                /// Create file
                H5::Exception::dontPrint();
                H5File file(m_filename, H5F_ACC_RDONLY);

                auto& geogrid = m_population->RefGeoGrid();
                ParsePersons(geogrid, file);



//                Group locations(file.createGroup("Locations"));
//                int   count = 1;
//                for (const auto& location : geoGrid) {
//                        WriteLocation(*location, locations, count++);
//                }
//
//                WritePersons(file);

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
                // catch failure caused by the DataSpace operations
        catch( DataTypeIException& error )
        {
                error.printErrorStack();
        }


//        boost::property_tree::ptree root;
//        try {
//            boost::property_tree::read_json(*m_inputStream, root);
//        } catch (runtime_error&) {
//            throw Exception("Problem parsing JSON file, check whether empty or invalid JSON.");
//        }
//
//        auto& geoGrid = m_population->RefGeoGrid();
//        auto  people  = root.get_child("persons");
//
//        for (auto it = people.begin(); it != people.end(); it++) {
//            auto person               = ParsePerson(it->second.get_child(""));
//            m_people[person->GetId()] = person;
//        }
//        auto locations = root.get_child("locations");
//
//        for (auto it = locations.begin(); it != locations.end(); it++) {
//            shared_ptr<Location> loc;
//            loc = ParseLocation(it->second.get_child(""));
//            geoGrid.AddLocation(move(loc));
//        }
//
//        AddCommutes(geoGrid);
//        m_commutes.clear();
//        m_people.clear();
}

void GeoGridHDF5Reader::ParsePersons(GeoGrid& geogrid, const H5::H5Location& h5_location)
{
        DataSet persons(h5_location.openDataSet("Persons"));

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

        Attribute size_att = persons.openAttribute("size");
        unsigned int size[1];
        size_att.read(PredType::NATIVE_UINT, size);
        vector<PersonsData> persons_data(size[0]);
        persons.read(&persons_data.front(), comp_type);
        for (auto person_data : persons_data) {
                Person person(person_data.id, person_data.age, person_data.household, person_data.k12school,
                              person_data.collage, person_data.workplace, person_data.primary_community,
                              person_data.secondary_community);

        }



}


}