set(PYCGAL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PYCGAL_INCLUDE_DIR ${PYCGAL_SOURCE_DIR})
set(PYCGAL_SOURCE_TEMPLATE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/template)

include(cmake/pycgal_add_package.cmake)

add_subdirectory(pyCGAL)
