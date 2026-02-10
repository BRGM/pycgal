set(PYCGAL_PACKAGES_DIR ${CMAKE_CURRENT_SOURCE_DIR})

foreach(package ${PYCGAL_PACKAGES})
  set(PYCGAL_PACKAGE_NAME ${package})
  add_subdirectory(${package})
endforeach()
