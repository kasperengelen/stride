#include "GeoGridHDF5Writer.h"
#include <iostream>

#include <H5Cpp.h>
#include <H5DataSpace.h>
#include <string>

#include "geopop/GeoGrid.h"
#include "pop/Person.h"
using namespace H5;

namespace geopop {

using namespace std;
using namespace stride;

GeoGridHDF5Writer::GeoGridHDF5Writer() : m_persons_found() {}

void GeoGridHDF5Writer::Write(GeoGrid& geoGrid, ostream& stream)
{

        try {

                /// Create file
                Exception::dontPrint();
                H5File file("Geo/GeoGrid.h5", H5F_ACC_TRUNC);

                Group locations(file.createGroup("Locations"));
                for (const auto& location : geoGrid) {

                        WriteLocation(*location, locations);
                }

                for (const auto& person : m_persons_found) {
                        WritePerson(person, file);
                }

        } catch (FileIException& error) {
                error.printErrorStack();
                //                return -1;
        }

        // catch failure caused by the DataSet operations
        catch (DataSetIException& error) {
                error.printErrorStack();
                //                return -1;
        }

        // catch failure caused by the DataSpace operations
        catch (DataSpaceIException& error) {
                error.printErrorStack();
                //                return -1;
        }
        // catch failure caused by the Group operations
        catch (GroupIException& error) {
                error.printErrorStack();
                //                return -1;
        }
}

template <typename T>
void WriteAttribute(Group& group, const std::string& name, const DataType& type, const T& data)
{
        hsize_t   dim[1] = {1};
        DataSpace dataspace(1, dim);
        Attribute attribute         = group.createAttribute(name, type, dataspace);
        T         attribute_data[1] = {data};
        attribute.write(type, attribute_data);
}

void WriteCoordinate(Group& group, const Coordinate& coordinate)
{
        hsize_t   dim[2] = {1, 2};
        DataSpace dataspace(2, dim);
        Attribute attribute         = group.createAttribute("coordinate", PredType::NATIVE_DOUBLE, dataspace);
        double    attribute_data[2] = {boost::geometry::get<0>(coordinate), boost::geometry::get<1>(coordinate)};
        attribute.write(PredType::NATIVE_DOUBLE, attribute_data);
}

void GeoGridHDF5Writer::WriteLocation(const Location& location, H5Location& h5_location)
{
        unsigned int id = location.GetID();
        Group        loc(h5_location.createGroup("Loc" + std::to_string(id)));
        WriteAttribute(loc, "id", PredType::NATIVE_INT, id);
        WriteAttribute(loc, "name", StrType(PredType::C_S1, location.GetName().size() + 1), location.GetName());
        WriteAttribute(loc, "province", PredType::NATIVE_INT, location.GetProvince());
        WriteAttribute(loc, "population", PredType::NATIVE_INT, location.GetPopCount());
        WriteCoordinate(loc, location.GetCoordinate());

        struct CommuteData
        {
                int    to;
                double proportion;
        };

        const auto& commutes = location.CRefOutgoingCommutes();

        CommuteData commute_data[commutes.size()];
        for (auto i = 0; i < commutes.size(); i++) {

                commute_data[i].to         = commutes[i].first->GetID();
                commute_data[i].proportion = commutes[i].second;
        }

        CompType comp_type(sizeof(CommuteData));
        comp_type.insertMember("to", HOFFSET(CommuteData, to), PredType::NATIVE_INT);
        comp_type.insertMember("proportion", HOFFSET(CommuteData, proportion), PredType::NATIVE_DOUBLE);

        hsize_t   com_dim[] = {commutes.size()};
        DataSpace com_ds(1, com_dim);
        DataSet   commute = loc.createDataSet("Commute", comp_type, com_ds);
        commute.write(commute_data, comp_type);

        hsize_t dim[2] = {2, 3};

        DataSpace cp_ds(2, dim);
        DataSet   contactPools = loc.createDataSet("ContactPools", PredType::STD_I32BE, cp_ds);
}

void GeoGridHDF5Writer::WritePerson(stride::Person* person, H5::H5File& file)
{

        // The data of the attribute.
        vector<string> att_vector;
        att_vector.emplace_back("id");
        att_vector.emplace_back("age");
        att_vector.emplace_back("daycare");
        const int RANK = 1;
        hsize_t   dims[RANK];
        StrType   str_type(PredType::C_S1, H5T_VARIABLE);
        dims[0] = att_vector.size(); // The attribute will have 3 strings
        DataSpace att_datspc(RANK, dims);

        DataSet   persons = file.createDataSet("Persons", PredType::STD_I32BE, att_datspc);
        Attribute att(persons.createAttribute("Column_Names", str_type, att_datspc));

        vector<const char*> cStrArray;
        for (auto& index : att_vector) {
                cStrArray.push_back(index.c_str());
        }
        // att_vector must not change
        att.write(str_type, (void*)&cStrArray[0]);
}

} // namespace geopop
