



add_subdirectory(libhdf5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
add_library(libhdf5 INTERFACE)


#set_property(TARGET libhdf5 PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(libhdf5
    INTERFACE
    hdf5-static
    hdf5_cpp-static
    hdf5_hl-static
    hdf5_hl_cpp-static
    )

set(HDF5_INCLUDE
    ${CMAKE_HOME_DIRECTORY}/main/resources/lib/hdf5/libhdf5/src/
    ${CMAKE_HOME_DIRECTORY}/main/resources/lib/hdf5/libhdf5/c++/src/
    ${CMAKE_HOME_DIRECTORY}/main/resources/lib/hdf5/libhdf5/hl/src/
    ${CMAKE_HOME_DIRECTORY}/main/resources/lib/hdf5/libhdf5/hl/c++/src/)
