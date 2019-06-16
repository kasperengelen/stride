# Dissertation testplan


## End-user functionality

### Daycare & Preschool

* DaycareGeneratorTest:
    Tests whether the DaycareGenerator can handle various amounts of locations.
    This is done by first adding locations to the geogrid and then allowing the generator to add households to the locations based on the size of the population. The test can then compare the generated amount of households to an expected amount.
    * OneLocationTest
    * ZeroLocationTest
    * FiveLocationTest

* PreSchoolGeneratorTest:
    Tests whether the PreSchoolGenerator can handle various amounts of locations.
    This is done by first adding locations to the geogrid and then allowing the generator to add households to the locations based on the size of the population. The test can then compare the generated amount of households to an expected amount.
    * OneLocationTest
    * ZeroLocationTest
    * FiveLocationTest

* DaycarePopulatorTest:
    Tests whether the DaycarePopulator can handle various amounts of locations.
    This is done by first manually adding households to locations, then allowing the populator to populate the daycares. The test then checks that each person in the daycare is of the correct age.
    * NoPopulation
    * OneLocationTest
    * TwoLocationTest

* PreSchoolPopulatorTest:
    Tests whether the PreSchoolPopulator can handle various amounts of locations.
    This is done by first manually adding households to locations, then allowing the populator to populate the preschools. The test then checks that each person in the preschool is of the correct age.
    * NoPopulation
    * OneLocationTest
    * TwoLocationTest
    
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

* WorkplaceCSVReaderTest:
    Tests if the reader reads the correct data. Then the test will check whether the CSV files were read correctly by manually checking the reader output.
    
* WorkplaceGeneratorTest:
    Existing tests are updated to reflect the changes in the algorithm for generated pools.
    
* WorkplacePopulatorTest:
    Existing tests are updated to reflect the changes in the algorithm for populating pools.
    * Distribution: Tests, after populating the workplaces, if the amounts of pools per size class gets close to the specified ratios.

## Responsibilities
The teammember that is responsible for implementing a feature, is also responsible for testing that feature. The tests are verified when the pull request for the feature is processed.
