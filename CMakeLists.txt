# Copyright 2017-2019 Axel Huebl
#
# This file is part of lines-are-beautiful.
#
# lines-are-beautiful is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# lines-are-beautiful is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with lines-are-beautiful.
# If not, see <http://www.gnu.org/licenses/>.

# Preamble ####################################################################
#

cmake_minimum_required(VERSION 3.7)

project(Rmlab VERSION 0.1.0 LANGUAGES CXX)


################################################################################
# CMake policies
#
# Search in <PackageName>_ROOT:
#   https://cmake.org/cmake/help/v3.12/policy/CMP0074.html

if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()


# Options and Variants ########################################################
#
function(rmlab_option name description default)
    set(Rmlab_USE_${name} ${default} CACHE STRING "${description}")
    set_property(CACHE Rmlab_USE_${name} PROPERTY
        STRINGS "ON;TRUE;AUTO;OFF;FALSE"
    )
    if(Rmlab_HAVE_${name})
        set(Rmlab_HAVE_${name} TRUE)
    else()
        set(Rmlab_HAVE_${name})
    endif()
    set(Rmlab_CONFIG_OPTIONS ${Rmlab_CONFIG_OPTIONS} ${name} PARENT_SCOPE)
endfunction()

rmlab_option(PNG "Enable support for PNG conversion" AUTO)

# TODO: add defines via configure to installed header files

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the build type, e.g. Debug." FORCE)
endif()


# Dependencies ################################################################
#
if(Rmlab_USE_PNG STREQUAL AUTO)
    find_package(PNGwriter 0.7.0 CONFIG)
elseif(Rmlab_USE_PNG)
    find_package(PNGwriter 0.7.0 REQUIRED CONFIG)
endif()

if(PNGwriter_FOUND)
    set(Rmlab_HAVE_PNG TRUE)
endif()


# Targets #####################################################################
#

# libraries
add_library(Rmlab
    include/rmlab/Notebook.cpp
    include/rmlab/auxiliary/Filesystem.cpp
)
add_library(Rmlab::Rmlab ALIAS Rmlab)

if(NOT WIN32)
    target_link_libraries(Rmlab PRIVATE m)
endif()
target_include_directories(Rmlab PUBLIC
    $<BUILD_INTERFACE:${Rmlab_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${Rmlab_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(Rmlab PUBLIC cxx_std_11)

# CLI tools / executables
add_executable(lineDemo
    include/rmlab/writer/lineDemo.cpp
)
target_compile_features(lineDemo PUBLIC cxx_std_11)
target_link_libraries(lineDemo PRIVATE
    Rmlab
)
set(Rmlab_EXTRA_TARGETS lineDemo)
if(Rmlab_HAVE_PNG)
    add_executable(lines2png
        include/rmlab/renderer/lines2png.cpp
    )

    target_compile_features(lines2png PUBLIC cxx_std_11)
    target_link_libraries(lines2png PRIVATE
        Rmlab
        PNGwriter::PNGwriter
    )

    set(Rmlab_EXTRA_TARGETS lines2png)
else()
    message(STATUS "PNGwriter NOT found! lines2png will not be build!")
endif()

add_executable(lines2svg
    include/rmlab/renderer/lines2svg.cpp
)

target_compile_features(lines2svg PUBLIC cxx_std_11)
target_link_libraries(lines2svg PRIVATE Rmlab)
list(APPEND Rmlab_EXTRA_TARGETS lines2svg)

add_executable(dump
    include/rmlab/tool/dump.cpp
)
target_compile_features(dump PUBLIC cxx_std_11)
target_link_libraries(dump PRIVATE Rmlab)
list(APPEND Rmlab_EXTRA_TARGETS dump)

# Generate Files with Configuration Options ###################################
#
configure_file(
    ${Rmlab_SOURCE_DIR}/RmlabConfig.cmake.in
    ${Rmlab_BINARY_DIR}/RmlabConfig.cmake
    @ONLY
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("RmlabConfigVersion.cmake"
    VERSION ${Rmlab_VERSION}
    COMPATIBILITY SameMajorVersion
)


# Installs ####################################################################
#
# headers, libraries and exectuables
install(TARGETS Rmlab ${Rmlab_EXTRA_TARGETS} EXPORT RmlabTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(
    DIRECTORY include/
    DESTINATION include
)
# CMake package file for find_package(Rmlab::Rmlab) in depending projects
install(EXPORT RmlabTargets
    FILE RmlabTargets.cmake
    NAMESPACE Rmlab::
    DESTINATION lib/cmake/Rmlab
)
install(
    FILES
        ${Rmlab_BINARY_DIR}/RmlabConfig.cmake
        ${Rmlab_BINARY_DIR}/RmlabConfigVersion.cmake
    DESTINATION lib/cmake/Rmlab
)


# Packages ####################################################################
#
# TODO (in separate file that is included)


# Tests #######################################################################
#
enable_testing()
if(Rmlab_HAVE_PNG)
    add_test(NAME Convert.PNG
        COMMAND lines2png
        ${Rmlab_SOURCE_DIR}/share/rmlab/examples/aa90b0e7-5c1a-42fe-930f-dad9cf3363cc
    )
endif()
add_test(NAME Convert.SVG
    COMMAND lines2svg
    ${Rmlab_SOURCE_DIR}/share/rmlab/examples/aa90b0e7-5c1a-42fe-930f-dad9cf3363cc
)
add_test(NAME Convert.Dump
    COMMAND dump
    ${Rmlab_SOURCE_DIR}/share/rmlab/examples/aa90b0e7-5c1a-42fe-930f-dad9cf3363cc
)

# Status Message for Build Options ############################################
#
message("")
message("Rmlab build configuration:")
message("  Rmlab Version: ${Rmlab_VERSION}")
message("  C++ Compiler : ${CMAKE_CXX_COMPILER_ID} "
                         "${CMAKE_CXX_COMPILER_VERSION} "
                         "${CMAKE_CXX_COMPILER_WRAPPER}")
message("    ${CMAKE_CXX_COMPILER}")
message("")
message("  Installation prefix: ${CMAKE_INSTALL_PREFIX}")
message("")
message("  Build Type: ${CMAKE_BUILD_TYPE}")
message("  Build Options:")

foreach(opt IN LISTS Rmlab_CONFIG_OPTIONS)
  if(${Rmlab_HAVE_${opt}})
    message("    ${opt}: ON")
  else()
    message("    ${opt}: OFF")
  endif()
endforeach()
message("")
