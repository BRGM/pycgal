cmake_minimum_required(VERSION 3.14)

project(pycgal CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PYCGAL_CGAL_VERSION 5.3.1)
set(PYCGAL_PYBIND11_VERSION 2.8.1)

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

set(PYCGAL_PACKAGES core Epick Mesh_2 Polygon_mesh_processing Surface_mesh)

option(PYCGAL_SHIPS_GMP OFF)
if(PYCGAL_SHIPS_GMP)
  # For some reasons the generator expression set(SHIPPED_GMP_LIBRARY
  # $<IF:$<CONFIG:Debug>,${GMP_LIBRARY_DEBUG},${GMP_LIBRARY_RELEASE}>) is not
  # handled in the later glob expressions
  if($<CONFIG:Debug>)
    set(SHIPPED_GMP_LIBRARY ${GMP_LIBRARY_DEBUG})
  else()
    set(SHIPPED_GMP_LIBRARY ${GMP_LIBRARY_RELEASE})
  endif()
  if(WIN32)
    string(REPLACE .lib .dll SHIPPED_GMP_LIBRARY ${SHIPPED_GMP_LIBRARY})
    if(NOT EXISTS ${SHIPPED_GMP_LIBRARY})
      # will try a GNU like directory structure (which is used by vcpkg)
      get_filename_component(GMP_LIB_DIRECTORY ${SHIPPED_GMP_LIBRARY} DIRECTORY)
      get_filename_component(
        SHIPPED_GMP_LIBRARY ${GMP_LIB_DIRECTORY}/../bin/gmp.dll ABSOLUTE
      )
      if(NOT EXISTS ${SHIPPED_GMP_LIBRARY})
        message(FATAL_ERROR "Could not find a dll for gmp library!")
      endif()
    endif()
  else(WIN32)
    message(FATAL_ERROR "gmp library can only be shipped on Windows platforms!")
  endif(WIN32)
  install(FILES ${SHIPPED_GMP_LIBRARY} DESTINATION pycgal)
endif(PYCGAL_SHIPS_GMP)

add_subdirectory(src)
