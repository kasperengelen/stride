# Dissertation testplan


## End-user functionality

### Daycare & Preschool

* TestDaycareParticipation:
    Tests whether the actual daycare participation corresponds with a given configuration file.
    
* TestPreschoolParticipation:
    Tests whether the actual daycare participation corresponds with a given configuration file.
    

### Data formats

* TestGeoGridJSONWriteRead:
    Writes a GeoGrid to JSON, then reads it back and tests whether the two GeoGrids are the same.
    
* TestJSONHouseHoldReader:
    Reads a household file from JSON and tests whether the data corresponds with the file.
    
* TestGeoGridHDF5WriteRead:
    Writes a GeoGrid to HDF5, then reads it back and tests whether the two GeoGrids are the same.
    

### Data visualisation

* TestEpiOutputWriteRead:
    Using simulation output, writes a "epi-output" file, then reads it back and tests whether the data corresponds to the original data.
    

### Demographic profile

* TestReferenceSetSampling:
    Using several testing sets, create a population and test whether it has been properly sampled (wrt locations)
    
    
### Workplace size

* TestWorkplaceDistributionReader:
    Tests if the reader reads the correct data.
    
* TestWorkplaceDistributionGenerator:
    Tests if the generated distribution corresponds to the given distribution.
    
* TestWorkplacePopulator:
    ???

