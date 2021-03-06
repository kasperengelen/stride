#############################################################################
#  This file is part of the Stride software.
#  It is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or any
#  later version.
#  The software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License,
#  along with the software. If not, see <http://www.gnu.org/licenses/>.
#  see http://www.gnu.org/licenses/.
#
#  Copyright 2017, 2018 Willem L, Kuylen E & Broeckhove J
#############################################################################

#============================================================================
# Protocol Buffer sources.
#============================================================================

if(Protobuf_FOUND)
    message("Protobuf: Generating Protobuf C++ APIs.")
    protobuf_generate_cpp(PROTO_SRC PROTO_HDR protobuf_formats/epiformat.proto protobuf_formats/geogrid.proto)
else()
    message("Protobuf: Using default Protobuf C++ APIs.")
    set(PROTO_SRC ${Protobuf_PBS_DIR}/geogrid.pb.cc ${Protobuf_PBS_DIR}/epiformat.pb.cc)
    set(PROTO_HDR ${Protobuf_PBS_DIR}/geogrid.pb.h  ${Protobuf_PBS_DIR}/epiformat.pb.h)
endif()

set_source_files_properties(
    geopop/io/GeoGridProtoReader.cpp
    geopop/io/GeoGridProtoWriter.cpp
    viewers/EpiOutputProto.cpp
    viewers/EpiOutputFileViewer.cpp
    visualiser/readers/ProtobufEpiReader.cpp
    PROPERTIES
    COMPILE_FLAGS "-I${Protobuf_PBS_DIR}"
)

set_property(SOURCE ${PROTO_HDR} ${PROTO_SRC}
    PROPERTY
    COMPILE_FLAGS "-Wno-all -Wno-effc++ -Wno-extra -Wno-pedantic"
)

#============================================================================
# Sources.
#============================================================================
set(STRIDE_SRC
    #---
    calendar/Calendar.cpp
    #---
    contact/AgeContactProfile.cpp
    contact/ContactLogMode.cpp
    contact/ContactPool.cpp
    contact/ContactPoolSys.cpp
    contact/ContactType.cpp
    contact/Infector.cpp
    contact/TransmissionProfile.cpp
    #---
    disease/DiseaseSeeder.cpp
    disease/Health.cpp
    disease/HealthSeeder.cpp
    disease/Immunizer.cpp
    disease/PublicHealthAgency.cpp
    #---
    execs/ControlHelper.cpp
    execs/GenPopController.cpp
    execs/SimController.cpp
    execs/StanController.cpp
    #---
    #geopop/ContactCenter.cpp
    geopop/Coordinate.h
    geopop/GeoGrid.cpp
    geopop/GeoGridBase.cpp
    geopop/GeoGridConfig.cpp
        geopop/SimLocation.cpp
    geopop/LocationBase.cpp
        #---
    geopop/geo/GeoGridKdTree.cpp
    geopop/geo/KdTree2DPoint.cpp
    #---
    geopop/generators/CollegeGenerator.cpp
    geopop/generators/DaycareGenerator.cpp
    geopop/generators/HouseholdGenerator.cpp
    geopop/generators/K12SchoolGenerator.cpp
    geopop/generators/PreSchoolGenerator.cpp
    geopop/generators/PrimaryCommunityGenerator.cpp
    geopop/generators/SecondaryCommunityGenerator.cpp
    geopop/generators/WorkplaceGenerator.cpp
    #---
    geopop/io/LocationsCSVReader.cpp
    geopop/io/CommutesCSVReader.cpp
    geopop/io/GeoGridFileWriter.h
    geopop/io/GeoGridJSONReader.cpp
    geopop/io/GeoGridJSONWriter.cpp
    geopop/io/GeoGridHDF5Reader.cpp
    geopop/io/GeoGridHDF5Writer.cpp
    geopop/io/GeoGridProtoReader.cpp
    geopop/io/GeoGridProtoReader.h
    geopop/io/GeoGridProtoWriter.cpp
    geopop/io/GeoGridProtoWriter.h
    geopop/io/GeoGridReader.cpp
    geopop/io/GeoGridReaderFactory.cpp
    geopop/io/GeoGridReaderFactory.h
    geopop/io/GeoGridStreamWriter.h
    geopop/io/GeoGridWriterFactory.cpp
    geopop/io/GeoGridWriterFactory.h
    geopop/io/HouseholdCSVReader.cpp
    geopop/io/HouseholdJSONReader.cpp
    geopop/io/ReaderFactory.cpp
    geopop/io/WorkplaceCSVReader.cpp
    geopop/io/WorkplaceCSVReader.h
    #---
    geopop/populators/CollegePopulator.cpp
    geopop/populators/DaycarePopulator.cpp
    geopop/populators/HouseholdPopulator.cpp
    geopop/populators/K12SchoolPopulator.cpp
    geopop/populators/PreSchoolPopulator.cpp
    geopop/populators/PrimaryCommunityPopulator.cpp
    geopop/populators/SecondaryCommunityPopulator.cpp
    geopop/populators/WorkplacePopulator.cpp
    # protobuf files
    ${PROTO_SRC}
    #---
    viewers/InfectedFile.cpp
    viewers/PersonsFile.cpp
    viewers/SummaryFile.cpp
    viewers/EpiOutputFile.cpp
    viewers/EpiOutputJSON.cpp
    viewers/EpiOutputHDF5.cpp
    viewers/EpiOutputProto.cpp
    geopop/PopStats.cpp
    #---
    pop/AbstractPopBuilder.cpp
    pop/Person.cpp
    pop/Population.cpp
    pop/DefaultPopBuilder.cpp
    pop/GeoPopBuilder.cpp
    pop/ImportPopBuilder.cpp
    pop/SurveySeeder.cpp
    #---
    sim/SimRunner.cpp
    sim/Sim.cpp
    sim/SimBuilder.cpp
    sim/event/Id.cpp
    sim/event/Subject.cpp
    #---
    util/ConfigInfo.cpp
    util/CSV.cpp
    util/CSVRow.cpp
    util/FileSys.cpp
    util/LogUtils.cpp
    util/Rn.cpp
    util/RnMan.cpp
    util/RunConfigManager.cpp
    util/TimeStamp.cpp
    util/TimeToString.cpp
    #---
    viewers/CliViewer.cpp
    viewers/InfectedFileViewer.cpp
    viewers/InfectedViewer.cpp
    viewers/PersonsFileViewer.cpp
    viewers/SummaryFileViewer.cpp
    viewers/EpiOutputFileViewer.cpp
)


#============================================================================
# Build the Stride library.
#============================================================================
add_library(libstride ${STRIDE_SRC})
target_compile_definitions(libstride PUBLIC "-DSTRIDE_GIT_HASH=${STRIDE_WC_REVISION_HASH}")
target_compile_definitions(libstride PUBLIC "-DPROCCOUNT=${PROCCOUNT}")
target_link_libraries(libstride PRIVATE ${LIBS})
if(Protobuf_FOUND)
    target_link_libraries(libstride PUBLIC ${Protobuf_LIBRARIES})
else()
    target_link_libraries(libstride PUBLIC libprotobuf)
endif()
if(HDF5_FOUND)
    target_link_libraries(libstride PRIVATE ${HDF5_LIBRARIES})
else()
    target_link_libraries(libstride PRIVATE hdf5-static)
    target_link_libraries(libstride PRIVATE hdf5_cpp-static)
endif()

set_target_properties(libstride
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        PREFIX ""
)
include(CheckIPOSupported)

# For future use: inter procedural optimization
# Disabled for now because of issues with gcc 8.1.0 on travis.
#check_ipo_supported(RESULT ipo_result)
#if(ipo_result)
#    set_target_properties(libstride PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
#endif()
#message( STATUS "CheckIPOSupported: ${ipo_result}" )

#============================================================================
# Build the stride binary.
#============================================================================
add_executable(stride execs/stride.cpp)
target_link_libraries(stride libstride)
install(TARGETS stride DESTINATION ${BIN_INSTALL_LOCATION})

#============================================================================
# Data visualiser sources.
#============================================================================
# WARNING!!: MAKE SURE THESE ARE -CORRECT-, otherwise a "no sources specified" error will be shown!!
set(VISUALISER_SOURCE
	# controller
	visualiser/controller/Controller.cpp
	# view
        visualiser/view/PopStatsView.cpp
	visualiser/view/View.cpp
    # model
    geopop/PopStats.cpp
    geopop/VisLocation.cpp
    geopop/VisGeoGrid.cpp
    visualiser/model/Model.cpp
    # epi output readers
    visualiser/readers/EpiReader.cpp
    visualiser/readers/HDF5EpiReader.cpp
    visualiser/readers/JSONEpiReader.cpp
    visualiser/readers/ProtobufEpiReader.cpp
    # protobuf files
    ${PROTO_HDR}
    ${PROTO_SRC}
    # main
    visualiser/main.cpp
    visualiser/qml.qrc
)

#============================================================================
# Build data visualiser binary.
#============================================================================
if(DO_BUILD_DATAVIS)
	message("Adding visualiser build target.")
    add_executable(stride_visualiser ${VISUALISER_SOURCE})

    target_link_libraries(stride_visualiser libstride) # give data visualiser access to the stride library

    target_link_libraries(stride_visualiser Qt5::Core)
    target_link_libraries(stride_visualiser Qt5::Widgets)
    target_link_libraries(stride_visualiser Qt5::Location)

    set_property(TARGET Qt5::Core PROPERTY INTERFACE_COMPILE_FEATURES "")

    install(TARGETS stride_visualiser DESTINATION ${BIN_INSTALL_LOCATION})


    # export as library to link with tests
    message("Adding visualiser library build target.")
    add_library(libvis ${VISUALISER_SOURCE})

    # give data visualiser access to the stride library
    # this also links to HDF5, Protobuf, etc.
    target_link_libraries(libvis libstride)

    target_link_libraries(libvis Qt5::Core)
    target_link_libraries(libvis Qt5::Widgets)
    target_link_libraries(libvis Qt5::Location)

	set_property(TARGET Qt5::Core PROPERTY INTERFACE_COMPILE_FEATURES "")
endif()

#############################################################################
