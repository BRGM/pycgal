set(PYCGAL_PACKAGES_DIR ${CMAKE_CURRENT_SOURCE_DIR})

foreach(package ${PYCGAL_PACKAGES})
  add_subdirectory(${package})
endforeach()
