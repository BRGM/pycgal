cmake_minimum_required(VERSION 3.14)

project(pycgal CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PYCGAL_CGAL_VERSION 5.3.1)

find_package(CGAL ${PYCGAL_CGAL_VERSION} QUIET) # try locally
if(NOT CGAL_FOUND) # else fetch it from github
  message(STATUS "Could NOT find CGAL>=${PYCGAL_CGAL_VERSION}.")
  message(STATUS "Fetching CGAL ${PYCGAL_CGAL_VERSION} from GitHub.")
  message(
    WARNING
      "====================================================================\n"
      "Make sure CGAL dependencies are installed (especially gmp and mpfr).\n"
      "You may want to check:\n"
      "https://doc.cgal.org/latest/Manual/installation.html#seclibraries\n"
      "====================================================================\n"
  )
  include(FetchContent)
  FetchContent_Declare(
    CGAL
    GIT_REPOSITORY https://github.com/CGAL/cgal.git
    GIT_TAG v${PYCGAL_CGAL_VERSION}
    CGAL_HEADER_ONLY ON
  )
  FetchContent_MakeAvailable(CGAL)
else()
  include(${CGAL_USE_FILE})
endif()

find_package(pybind11 QUIET) # try locally
if(NOT pybind11_FOUND) # else fetch it from github
  message(STATUS "Could NOT find pybind11 (fetching it from GitHub)")
  include(FetchContent)
  FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v2.6.1
  )
  FetchContent_MakeAvailable(pybind11)
endif()

set(PYCGAL_PACKAGES core Epick Mesh_2 Polygon_mesh_processing Surface_mesh)

add_subdirectory(src)
