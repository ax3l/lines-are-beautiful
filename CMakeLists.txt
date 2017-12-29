# Copyright 2017 Axel Huebl
#
# This file is part of lines-are-beautiful.
#
# lines-are-beautiful is free software: you can edistribute it and/or modify
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

project(Rmlab VERSION 0.1.0)

#HACK!
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake-tmp/)

# Targets #####################################################################
#

# libraries
add_library(Rmlab
    include/rmlab/Notebook.cpp
)

target_include_directories(Rmlab PUBLIC
    $<BUILD_INTERFACE:${Rmlab_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${Rmlab_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
set_target_properties(Rmlab PROPERTIES CXX_STANDARD 11)


# Dependencies ################################################################
#

if(NOT WIN32)
    target_link_libraries(Rmlab PUBLIC m)
endif()

find_package(PNGwriter)

if(PNGwriter_FOUND)
    # tools / executables
    add_executable(lines2png
        include/rmlab/renderer/lines2png.cpp
    )

    set_property(TARGET lines2png PROPERTY CXX_STANDARD 11)
    target_link_libraries(lines2png PRIVATE
        Rmlab
        ${PNGwriter_LIBRARIES}
    )
    target_include_directories(lines2png SYSTEM PRIVATE ${PNGwriter_INCLUDE_DIRS})

    set(Rmlab_EXTRA_TARGETS lines2png)
else()
    message(STATUS "PNGwriter NOT found! lines2png will not be build!")
endif()


# Warnings ####################################################################
#
# GNU
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
# Clang
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
# ICC
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
# PGI
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Minform=inform")
endif()


# Installs ####################################################################
#
# headers, libraries and exectuables
install(TARGETS Rmlab ${Rmlab_EXTRA_TARGETS} EXPORT RmlabTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
# CMake package file for find_package(Rmlab::Rmlab) in depending projects
install(EXPORT RmlabTargets
    FILE RmlabTargets.cmake
    NAMESPACE Rmlab::
    DESTINATION lib/cmake/Rmlab
)
install(FILES "RmlabConfig.cmake" "RmlabConfig.cmake"
    DESTINATION lib/cmake/Rmlab
)


# Packages ####################################################################
#
# TODO (in separate file that is included)


# Tests #######################################################################
#
# TODO

