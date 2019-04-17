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
#  Copyright 2017, Willem L, Kuylen E & Broeckhove J
#############################################################################
#
#  This file contains the C++ compile & link configuration.
#  It is meant to be included in the src/CMakeLists.txt and
#  to provide identical C++ configuration for the main/cpp
#  and the test/ccp directories and subdirectories.
#
#############################################################################

#----------------------------------------------------------------------------
# Check CMAKE_BUILD_TYPE
#----------------------------------------------------------------------------
if( NOT (CMAKE_BUILD_TYPE MATCHES "Release" OR CMAKE_BUILD_TYPE MATCHES "Debug"))
	message(FATAL_ERROR  "========> CMAKE_BUILD_TYPE HAS TO MATCH EITHER Release OR Debug.")
endif()

#----------------------------------------------------------------------------
# Compile flags
#----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#
include(ProcessorCount)
ProcessorCount(PROCCOUNT)
#
# Required to avoid ld problems on Mac
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
#
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -pedantic -Weffc++")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast" )
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -O0"   )
#
include_directories(${CMAKE_HOME_DIRECTORY}/main/cpp)

# Prevents (static) libraries having a double "lib" prefix (when they are named libxxx).
set(CMAKE_STATIC_LIBRARY_PREFIX "")

#----------------------------------------------------------------------------
# Platform dependent compile flags
#----------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_HOST_APPLE)
	add_definitions( -D__APPLE__ )
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -stdlib=libc++")
#
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT CMAKE_HOST_APPLE )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-command-line-argument -Wno-self-assign")
	add_definitions(-D__extern_always_inline=inline)
#
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC")
#
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++1z")
endif()

#----------------------------------------------------------------------------
# Standard math lib
#----------------------------------------------------------------------------
set(LIBS   ${LIBS}   m)

#----------------------------------------------------------------------------
# Random number stuff: pcg, randutils, trng
#----------------------------------------------------------------------------
include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/pcg/include)
include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/randutils/include)
include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/trng-4.20/include)
set(LIBS ${LIBS} trng)

#----------------------------------------------------------------------------
# Spdlog Library (logging)
#----------------------------------------------------------------------------
include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/spdlog/include)

#----------------------------------------------------------------------------
# TCLAP
#----------------------------------------------------------------------------
include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/tclap/include)

#----------------------------------------------------------------------------
# System threads required by protobuf anf gtest
#----------------------------------------------------------------------------
find_package(Threads)

#----------------------------------------------------------------------------
# ProtoBuf
#----------------------------------------------------------------------------
if(NOT STRIDE_FORCE_NO_PROTOC)
    include(FindProtobuf)
    find_package(Protobuf)
    if(NOT Protobuf_FOUND)
            SET(Protobuf_VERSION 0.0.0)
    endif()
else()
    SET(Protobuf_VERSION 0.0.0) # deze setten, want als STRIDE_FORCE_NO_PROTOC dan wordt het vorige stuk code niet gerund en deze variabele niet geset.
endif()
#
if(Protobuf_FOUND AND ${Protobuf_VERSION} VERSION_GREATER_EQUAL 3.0.0)
    set(Protobuf_PBS_DIR ${CMAKE_BINARY_DIR}/main/cpp)
    include_directories(SYSTEM ${Protobuf_INCLUDE_DIRS})
else()
    set(Protobuf_PBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/main/cpp/geopop/io/proto_pb)
    include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/protobuf)
endif()

#----------------------------------------------------------------------------
# SHA1 hash code.
#----------------------------------------------------------------------------
include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/sha1/include)
set(LIBS ${LIBS} sha1)

#----------------------------------------------------------------------------
# Boost
#----------------------------------------------------------------------------
if (NOT STRIDE_FORCE_NO_BOOST)
    find_package(Boost COMPONENTS filesystem date_time)
endif()
if (Boost_FOUND)
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
    add_compile_definitions(BOOST_FOUND)
    set(LIBS   ${LIBS} ${Boost_LIBRARIES})
else()
    include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/boost/include)
    include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/date/include)
    if(CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?Clang" AND CMAKE_HOST_APPLE)
        set(LIBS ${LIBS} c++fs)
    else()
        set(LIBS ${LIBS} stdc++fs)
    endif()
endif()

#----------------------------------------------------------------------------
# OpenMP
#----------------------------------------------------------------------------
if(NOT STRIDE_FORCE_NO_OPENMP)
	if(NOT DEFINED HAVE_CHECKED_OpenMP)
	    set(HAVE_CHECKED_OpenMP  TRUE  CACHE  BOOL  "Have checked for OpenMP?" FORCE)
        find_package(OpenMP)
        if( OPENMP_FOUND )
            set(HAVE_FOUND_OpenMP  TRUE  CACHE  BOOL  "Have found OpenMP?" FORCE)
        else()
            set(HAVE_FOUND_OpenMP  FALSE  CACHE  BOOL  "Have found OpenMP?" FORCE)
	    endif()
    endif()
    if (HAVE_FOUND_OpenMP)
    	set(OPENMP_FOUND TRUE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    else()
    	# This is done to eliminate blank output of undefined CMake variables.
    	set(OPENMP_FOUND FALSE)
    endif()
else()
    # This is done to eliminate blank output of undefined CMake variables.
    set(OPENMP_FOUND FALSE)
    set(HAVE_CHECKED_OpenMP FALSE)
    set(HAVE_FOUND_OpenMP FALSE)
endif()
# If not found, use the dummy omp.
if(NOT OPENMP_FOUND)
    include_directories(${CMAKE_HOME_DIRECTORY}/main/resources/lib/domp/include)
endif()

#----------------------------------------------------------------------------
# Qt5
#----------------------------------------------------------------------------
if(NOT STRIDE_FORCE_NO_QT5)
	if(APPLE)
	    set(CMAKE_PREFIX_PATH /usr/local/opt/qt)
	else()
	    set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.12.2/gcc_64)
	endif()

	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTOUIC ON)
	set(CMAKE_AUTORCC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	find_package(Qt5 COMPONENTS Core Widgets Location Quick REQUIRED)

	if(Qt5Core_FOUND AND Qt5Widgets_FOUND AND Qt5Location_FOUND AND Qt5Quick_FOUND)
		message("Found QT5.")
		set(DO_BUILD_DATAVIS true)
	else()
	    message("Did not find QT5. Not building datavis.")
		set(DO_BUILD_DATAVIS false)
	endif()
else()
	set(DO_BUILD_DATAVIS false)
endif()


#----------------------------------------------------------------------------
# HDF5
#----------------------------------------------------------------------------
if (NOT STRIDE_FORCE_NO_HDF5)
    include(FindHDF5)
    find_package(HDF5)
endif()
if(HDF5_FOUND)
    include_directories(${HDF5_INCLUDE_DIR})
    set(LIBS ${LIBS} ${HDF5_LIBRARIES})
else()
    include(ExternalProject)
    set(ExternalProjectCMakeArgs
            -DHDF5_BUILD_CPP_LIB=ON
            )
    ExternalProject_Add(hdf5
            DOWNLOAD_COMMAND ""
            CMAKE_ARGS ${ExternalProjectCMakeArgs}
            SOURCE_DIR ${CMAKE_HOME_DIRECTORY}/main/resources/lib/hdf5
            BINARY_DIR ${CMAKE_BINARY_DIR}/main/resources/lib/hdf5/build
            STAMP_DIR  ${CMAKE_BINARY_DIR}/main/resources/lib/hdf5/stamp
            TMP_DIR    ${CMAKE_BINARY_DIR}/main/resources/lib/hdf5/tmp
            INSTALL_COMMAND ""
            )        
    include_directories(
                ${CMAKE_HOME_DIRECTORY}/main/resources/lib/hdf5/src
                ${CMAKE_SOURCE_DIR}/main/resources/lib/hdf5/c++/src
                ${CMAKE_BINARY_DIR}/main/resources/lib/hdf5/build
            )
    set(_hdf5_libs
            ${CMAKE_BINARY_DIR}/main/resources/lib/hdf5/build/bin/libhdf5_cpp.a
            ${CMAKE_BINARY_DIR}/main/resources/lib/hdf5/build/bin/libhdf5.a
            -ldl
            )
endif()

#############################################################################
