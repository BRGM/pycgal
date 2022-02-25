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

function(ship_library SHIPPED_LIBRARY)
  get_filename_component(LIBNAME ${SHIPPED_LIBRARY} NAME_WE)
  message(STATUS "Trying to ship ${LIBNAME} library (${SHIPPED_LIBRARY})")
  if(WIN32)
    string(REPLACE .lib .dll SHIPPED_LIBRARY ${SHIPPED_LIBRARY})
    if(NOT EXISTS ${SHIPPED_LIBRARY})
      # will try a GNU like directory structure (which is used by vcpkg)
      get_filename_component(LIB_DIRECTORY ${SHIPPED_LIBRARY} DIRECTORY)
      get_filename_component(
        SHIPPED_LIBRARY ${LIB_DIRECTORY}/../bin/${LIBNAME}.dll ABSOLUTE
      )
      if(NOT EXISTS ${SHIPPED_LIBRARY})
        # Try to find version number
        file(
          GLOB SHIPPED_LIBRARY
          LIST_DIRECTORIES false
          ${LIB_DIRECTORY}/../bin/${LIBNAME}-[0-9.]*.dll
        )
        get_filename_component(SHIPPED_LIBRARY ${SHIPPED_LIBRARY} REALPATH)
        if(NOT EXISTS ${SHIPPED_LIBRARY})
          message(FATAL_ERROR "Could not find a dll for ${LIBNAME} library!")
        endif()
      endif()
    endif()
  else(WIN32)
    message(
      FATAL_ERROR "${LIBNAME} library can only be shipped on Windows platforms!"
    )
  endif(WIN32)
  message(STATUS "          shipping: ${SHIPPED_LIBRARY}")
  install(FILES ${SHIPPED_LIBRARY} DESTINATION pycgal)
endfunction()

option(PYCGAL_SHIPS_GMP_AND_MPFR OFF)
if(PYCGAL_SHIPS_GMP_AND_MPFR)
  # For some reasons the generator expression set(SHIPPED_GMP_LIBRARY
  # $<IF:$<CONFIG:Debug>,${GMP_LIBRARY_DEBUG},${GMP_LIBRARY_RELEASE}>) is not
  # handled in the later glob expressions
  if($<CONFIG:Debug>)
    ship_library(${GMP_LIBRARY_DEBUG})
  else()
    ship_library(${GMP_LIBRARY_RELEASE})
  endif()
  ship_library(${MPFR_LIBRARIES})
endif(PYCGAL_SHIPS_GMP_AND_MPFR)

add_subdirectory(src)
