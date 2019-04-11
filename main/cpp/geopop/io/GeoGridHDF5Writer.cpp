#include "GeoGridHDF5Writer.h"
#include <iostream>

#include <string>
#include <H5Cpp.h>

#include "geopop/GeoGrid.h"
#include "pop/Person.h"
using namespace H5;



namespace geopop {

using namespace std;
using namespace stride;

GeoGridHDF5Writer::GeoGridHDF5Writer() : m_persons_found() {}


void GeoGridHDF5Writer::Write(GeoGrid& geoGrid, ostream& stream) {

        try
        {
                /// Create file
                Exception::dontPrint();
                H5File file("GeoGrid.h5", H5F_ACC_TRUNC);

                Group locations(file.createGroup("Locations"));
                for (const auto& location : geoGrid) {
                        WriteLocation(*location, file);
                }

                for (const auto& person : m_persons_found) {
                        WritePerson(person, file);
                }




        }
        catch(FileIException& error)
        {
                error.printErrorStack();
//                return -1;
        }

                // catch failure caused by the DataSet operations
        catch(DataSetIException& error)
        {
                error.printErrorStack();
//                return -1;
        }

                // catch failure caused by the DataSpace operations
        catch(DataSpaceIException& error)
        {
                error.printErrorStack();
//                return -1;
        }
                // catch failure caused by the Group operations
        catch(GroupIException& error)
        {
                error.printErrorStack();
//                return -1;
        }
}

void GeoGridHDF5Writer::WriteLocation(const Location& location, H5File& file) {
        unsigned int id = location.GetID();
        std::string filePosition = "Locations/Loc" + std::to_string(id);
        Group loc(file.createGroup(filePosition));

        hsize_t dims[2] = {2, 2};
        DataSpace commute_ds(2, dims);
        DataSet commute = file.createDataSet(filePosition + "/Commute", PredType::STD_I32BE, commute_ds);


        dims[1] = 3;
        DataSpace cp_ds(2, dims);
        DataSet contactPools = file.createDataSet(filePosition + "/ContactPools", PredType::STD_I32BE, cp_ds);
}

void GeoGridHDF5Writer::WritePerson(stride::Person* person, H5::H5File& file) {

        hsize_t dims[2] = {2, 10};
        DataSpace Persons_ds(2, dims);

        DataSet persons = file.createDataSet("Persons", PredType::STD_I32BE, Persons_ds);
}


}