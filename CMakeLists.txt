cmake_minimum_required(VERSION 3.14)

project(pycgal CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PYCGAL_CGAL_VERSION 5.4.1)
set(PYCGAL_PYBIND11_VERSION 2.9.2)

set(CGAL_HEADER_ONLY ON)
set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)

find_package(CGAL ${PYCGAL_CGAL_VERSION} EXACT QUIET) # try locally
if(NOT CGAL_FOUND) # else fetch it from github
  message(STATUS "Could NOT find CGAL>=${PYCGAL_CGAL_VERSION}.")
  message(STATUS "Fetching CGAL ${PYCGAL_CGAL_VERSION} from GitHub.")
  message(
    STATUS
      "===================================================================="
  )
  message(
    STATUS
      "Make sure CGAL dependencies are installed (especially gmp and mpfr)!"
  )
  message(STATUS "You may want to check:")
  message(
    STATUS "https://doc.cgal.org/latest/Manual/installation.html#seclibraries"
  )
  message(
    STATUS
      "===================================================================="
  )
  include(FetchContent)
  FetchContent_Declare(
    CGAL
    GIT_REPOSITORY https://github.com/CGAL/cgal.git
    GIT_TAG v${PYCGAL_CGAL_VERSION}
  )
  # FIXME: this will add_subdirectory CGAL which is not necessary cf.
  # include(${CGAL_USE_FILE}) command below
  FetchContent_MakeAvailable(CGAL)
  if(NOT ${cgal_POPULATED})
    message(FATAL_ERROR "Something went wrong in fetching CGAL from github.")
  endif()
  find_package(
    CGAL
    ${PYCGAL_CGAL_VERSION}
    EXACT
    REQUIRED
    PATHS
    ${cgal_SOURCE_DIR}
    NO_DEFAULT_PATH
  )
endif()
include(${CGAL_USE_FILE})

find_package(pybind11 ${PYCGAL_PYBIND11_VERSION} QUIET) # try locally
if(NOT pybind11_FOUND) # else fetch it from github
  message(STATUS "Could NOT find pybind11>=${PYCGAL_PYBIND11_VERSION}.")
  message(STATUS "Fetching pybind11 ${PYCGAL_PYBIND11_VERSION} from GitHub.")
  include(FetchContent)
  FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v${PYCGAL_PYBIND11_VERSION}
  )
  FetchContent_MakeAvailable(pybind11)
endif()

find_package(Eigen3 3.3.4 REQUIRED NO_MODULE)
include(CGAL_Eigen3_support)

include(cmake/ship_library.cmake)

option(PYCGAL_SHIPS_GMP_AND_MPFR OFF)
if(PYCGAL_SHIPS_GMP_AND_MPFR)
  # For some reasons the generator expression set(SHIPPED_GMP_LIBRARY
  # $<IF:$<CONFIG:Debug>,${GMP_LIBRARY_DEBUG},${GMP_LIBRARY_RELEASE}>) is not
  # handled in the later glob expressions
  if($<CONFIG:Debug>)
    ship_library(${GMP_LIBRARY_DEBUG} pycgal)
  else()
    ship_library(${GMP_LIBRARY_RELEASE} pycgal)
  endif()
  if(CGAL_WITH_GMPXX)
    ship_library(${GMPXX_LIBRARIES} pycgal)
  endif()
  ship_library(${MPFR_LIBRARIES} pycgal)
endif(PYCGAL_SHIPS_GMP_AND_MPFR)

set(PYCGAL_PACKAGES core Epick Mesh_2 Polygon_mesh_processing Surface_mesh)

add_subdirectory(src)
