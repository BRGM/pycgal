function(pycgal_add_module module_name)

  list(SUBLIST ARGV 1 -1 _elements)

  get_directory_property(PYCGAL_PARENT_MODULE_DIR PARENT_DIRECTORY)
  file(RELATIVE_PATH PYCGAL_MODULE_PATH ${PYCGAL_PACKAGES_DIR}
       ${PYCGAL_PARENT_MODULE_DIR}
  )
  string(REPLACE "/" "::" PYCGAL_MODULE_NAMESPACE
                 "${PYCGAL_MODULE_PATH}::${module_name}"
  )
  string(REPLACE "${PYCGAL_PACKAGE_NAME}/" "" PYCGAL_GENERIC_MODULE_PATH
                 "${PYCGAL_MODULE_PATH}/${module_name}"
  )
  string(REPLACE "${PYCGAL_PACKAGE_NAME}::" "" PYCGAL_GENERIC_MODULE_NAMESPACE
                 "${PYCGAL_MODULE_NAMESPACE}"
  )

  set(main_source ${module_name}.cpp)
  set(module_sources ${main_source})
  unset(_decl)
  unset(_call)
  foreach(name ${_elements})
    set(PYCGAL_MODULE_ELEMENT ${name})
    set(element_source ${name}.cpp)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${element_source})
      configure_file(
        ${PYCGAL_SOURCE_TEMPLATE_DIR}/bind_module_element.cpp.template
        ${CMAKE_CURRENT_SOURCE_DIR}/${element_source}
      )
    endif()
    list(APPEND module_sources ${element_source})
    string(APPEND _decl "void wrap_${name}(py::module&);\n")
    string(APPEND _call
           "pyCGAL::${PYCGAL_MODULE_NAMESPACE}::wrap_${name}(module);\n"
    )
  endforeach()
  string(STRIP "${_decl}" PYCGAL_WRAP_ALGORITHMS_DECLARATION)
  string(STRIP "${_call}" PYCGAL_CALL_WRAP_ALGORITHMS)
  configure_file(
    ${PYCGAL_SOURCE_TEMPLATE_DIR}/bind_module.cpp.template
    ${CMAKE_CURRENT_SOURCE_DIR}/${main_source}
  )

  set(target_name ${PYCGAL_PACKAGE_NAME}_${module_name})
  pybind11_add_module(${target_name} ${module_sources})
  target_include_directories(${target_name} PUBLIC ${PYCGAL_INCLUDE_DIR})
  set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${module_name})
  install(TARGETS ${target_name} DESTINATION ${PYCGAL_MODULE_PATH})

endfunction()
