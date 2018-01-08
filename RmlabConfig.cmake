# only add PUBLIC dependencies as well
#   https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html#creating-a-package-configuration-file
#include(CMakeFindDependencyMacro)
#find_dependency(SomeLib 1.2.3)

include("${CMAKE_CURRENT_LIST_DIR}/RmlabTargets.cmake")

# if necessary, add COMPONENTS here
