cmake_minimum_required(VERSION 3.14)

project(pycgal CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CGAL 5.2 REQUIRED)
# if (NOT CGAL_FOUND) # fetch it from github message(STATUS "Could NOT find CGAL
# (fetching it from GitHub)") include(FetchContent) FetchContent_Declare( CGAL
# GIT_REPOSITORY https://github.com/CGAL/cgal.git GIT_TAG releases/CGAL-5.0.2
# CGAL_HEADER_ONLY ON ) FetchContent_MakeAvailable(CGAL) endif()
include(${CGAL_USE_FILE})

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
