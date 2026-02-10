function(pycgal_add_package)

  file(RELATIVE_PATH PYCGAL_PACKAGE_PATH ${PYCGAL_PACKAGES_DIR}
       ${CMAKE_CURRENT_SOURCE_DIR}
  )
  string(REPLACE "/" "_" PYCGAL_PACKAGE_NAME ${PYCGAL_PACKAGE_PATH})
  string(REPLACE "/" "::" PYCGAL_PACKAGE_NAMESPACE ${PYCGAL_PACKAGE_PATH})

  foreach(module_name ${PYCGAL_MODULES})

    set(PYCGAL_CLASS_NAME ${module_name})
    # the following file is generated in ${CMAKE_CURRENT_SOURCE_DIR} and can be
    # modified on a per package basis
    set(wrap_source ${CMAKE_CURRENT_SOURCE_DIR}/${module_name}.cpp)
    if(NOT EXISTS ${wrap_source})
      configure_file(
        ${PYCGAL_SOURCE_TEMPLATE_DIR}/wrapping_code.cpp.template ${wrap_source}
      )
    endif()
    set(target_name ${PYCGAL_PACKAGE_NAME}_${module_name})
    pybind11_add_module(${target_name} ${wrap_source})
    target_include_directories(${target_name} PUBLIC ${PYCGAL_INCLUDE_DIR})
    target_link_libraries(${target_name} CGAL::CGAL)
    set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${module_name})
    if(PYCGAL_SHIPS_GMP_AND_MPFR)
      set_target_properties(${target_name} PROPERTIES INSTALL_RPATH "\$ORIGIN")
    endif(PYCGAL_SHIPS_GMP_AND_MPFR)

    install(TARGETS ${target_name} DESTINATION ${PYCGAL_PACKAGE_PATH}/_impl)

  endforeach()

endfunction()
