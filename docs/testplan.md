# Dissertation testplan


## End-user functionality

### Daycare & Preschool

* DaycareGeneratorTest:
    Tests wether the DaycareGenerator can handle various amounts of locations.
    * OneLocationTest
    * ZeroLocationTest
    * FiveLocationTest

* PreSchoolGeneratorTest:
    Tests wether the PreSchoolGenerator can handle various amounts of locations.
    * OneLocationTest
    * ZeroLocationTest
    * FiveLocationTest


### Data formats

* TestGeoGridJSONWriteRead:
    Writes a GeoGrid to JSON, then reads it back and tests whether the two GeoGrids are the same.
    
* TestJSONHouseHoldReader:
    Reads a household file from JSON and tests whether the data corresponds with the file.
    
* TestGeoGridHDF5WriteRead:
    Writes a GeoGrid to HDF5, then reads it back and tests whether the two GeoGrids are the same.
    

### Data visualisation

* TestEpiOutputJSONReader:
    Test whether the JSON epi-output reader works correctly.
    This is done by first manually making a handful of test files, some of which are correct, some of which are invalid. These are then read by the JSON reader. The test will then check whether the correct JSON files were read correctly by manually checking the reader output. The test will also check whether invalid JSON files were detected by checking for exceptions.
    
* TestEpiOutputHDF5Reader:
    Test whether the HDF5 epi-output reader works correctly.
    This is done by first manually making a handful of test files, some of which are correct, some of which are invalid. These are then read by the HDF5 reader. The test will then check whether the correct HDF5 files were read correctly by manually checking the reader output. The test will also check whether invalid HDF5 files were detected by checking for exceptions.

* TestEpiOutputProtobufReader:
    Test whether the Protobuf epi-output reader works correctly.
    This is done by first manually making a handful of test files, some of which are correct, some of which are invalid. These are then read by the Protobuf reader. The test will then check whether the correct Protobuf files were read correctly by manually checking the reader output. The test will also check whether invalid Protobuf files were detected by checking for exceptions.
    
* TestPopDataView:
    Test whether the PopDataView class produces the correct Qt5 data.
    This is done by manually creating PopData objects and converting them to QVariantMap objects using the PopDataView class. The test then checks whether the QVariantMap contains the same data as the PopData object.

### Demographic profile

* TestReferenceSetSampling:
    Using several testing sets, create a population and test whether it has been properly sampled (wrt locations)
    
    
### Workplace size distribution

* WorkplaceCSVeaderTest:
    Tests if the reader reads the correct data.
    
* WorkplaceGeneratorTest:
    Existing tests are updated to reflect the changes in the algorithm for generated pools.
    
* WorkplacePopulatorTest:
    Existing tests are updated to reflect the changes in the algorithm for populating pools.
    * Distribution: By

## Responsibilities
The teammember that is responsible for implementing a feature, is also responsible for testing that feature. The tests are verified when the pull request for the feature is processed.
