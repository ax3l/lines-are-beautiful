cmake_minimum_required(VERSION 3.7)

# version
include(CMakePackageConfigHelpers)
write_basic_package_version_file("RmlabConfigVersion.cmake"
    VERSION ${Rmlab_VERSION}
    COMPATIBILITY SameMajorVersion
)

# public dependencies in CMake package
include(CMakeFindDependencyMacro)
# find_dependency(Bar 2.0)
if(NOT WIN32)
    find_dependency(m)
endif*(
include("${CMAKE_CURRENT_LIST_DIR/RmlabTargets.cmake")
