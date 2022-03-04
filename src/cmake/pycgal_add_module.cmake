function(
  collect_module_sources
  parent_path
  module_name
  target_name
  classes
  functions
  wrapper_path
  all_sources
)

  string(REPLACE "/" "::" PYCGAL_MODULE_NAMESPACE
                 "${parent_path}${module_name}"
  )
  string(REPLACE "${PYCGAL_PACKAGE_NAME}/" "" PYCGAL_GENERIC_MODULE_PATH
                 "${parent_path}${module_name}"
  )
  string(REPLACE "${PYCGAL_PACKAGE_NAME}::" "" PYCGAL_GENERIC_MODULE_NAMESPACE
                 "${parent_path}"
  )
  if(wrapper_path)
    set(PYCGAL_WRAPPER_PATH ${wrapper_path})
  else()
    set(PYCGAL_WRAPPER_PATH ${PYCGAL_GENERIC_MODULE_PATH})
  endif()

  set(main_source ${module_name}-module.cpp)
  set(collected_sources ${main_source})
  unset(_decl)
  unset(_call)

  if(classes)
    foreach(name ${classes})
      set(PYCGAL_MODULE_ELEMENT ${name})
      set(element_source ${name}.cpp)
      if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${element_source})
        configure_file(
          ${PYCGAL_SOURCE_TEMPLATE_DIR}/bind_module_class.cpp.template
          ${CMAKE_CURRENT_SOURCE_DIR}/${element_source}
        )
      endif()
      list(APPEND collected_sources ${element_source})
      set(wrap wrap_${name}_class)
      string(APPEND _decl "void ${wrap}(py::module&);\n")
      string(APPEND _call
             "pyCGAL::${PYCGAL_MODULE_NAMESPACE}::${wrap}(module);\n"
      )
    endforeach()
  endif(classes)

  if(functions)
    foreach(name ${functions})
      set(PYCGAL_MODULE_ELEMENT ${name})
      set(element_source ${name}.cpp)
      if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${element_source})
        configure_file(
          ${PYCGAL_SOURCE_TEMPLATE_DIR}/bind_module_element.cpp.template
          ${CMAKE_CURRENT_SOURCE_DIR}/${element_source}
        )
      endif()
      list(APPEND collected_sources ${element_source})
      string(APPEND _decl "void wrap_${name}(py::module&);\n")
      string(APPEND _call
             "pyCGAL::${PYCGAL_MODULE_NAMESPACE}::wrap_${name}(module);\n"
      )
    endforeach()
  endif(functions)

  set(PYCGAL_TARGET_NAME ${target_name})
  string(STRIP "${_decl}" PYCGAL_WRAP_ALGORITHMS_DECLARATION)
  string(STRIP "${_call}" PYCGAL_CALL_WRAP_ALGORITHMS)
  configure_file(
    ${PYCGAL_SOURCE_TEMPLATE_DIR}/bind_module.cpp.template
    ${CMAKE_CURRENT_SOURCE_DIR}/${main_source}
  )

  set(${all_sources}
      ${collected_sources}
      PARENT_SCOPE
  )

endfunction()

# provide classes and functions as list arguments
function(pycgal_add_core_module module_name)

  set(target_name ${module_name})
  set(module_sources)
  collect_module_sources(
    ""
    ${module_name}
    ${target_name}
    "${${ARGV1}}"
    "${${ARGV2}}"
    "${ARGV3}"
    module_sources
  )

  pybind11_add_module(${target_name} ${module_sources})
  target_include_directories(${target_name} PUBLIC ${PYCGAL_INCLUDE_DIR})
  set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${module_name})
  if(PYCGAL_SHIPS_GMP_AND_MPFR)
    set_target_properties(${target_name} PROPERTIES INSTALL_RPATH "\$ORIGIN")
  endif(PYCGAL_SHIPS_GMP_AND_MPFR)

  install(TARGETS ${target_name} LIBRARY DESTINATION pycgal)

endfunction()

function(pycgal_add_submodule module_name)

  get_directory_property(PYCGAL_PARENT_MODULE_DIR PARENT_DIRECTORY)
  file(RELATIVE_PATH PYCGAL_MODULE_PATH ${PYCGAL_PACKAGES_DIR}
       ${PYCGAL_PARENT_MODULE_DIR}
  )
  set(PYCGAL_MODULE_PARENT_NAMESPACE ${PYCGAL_MODULE_PATH})
  if(NOT PYCGAL_MODULE_PATH STREQUAL "")
    set(PYCGAL_MODULE_PARENT_PATH "${PYCGAL_MODULE_PATH}/")
  else()
    set(PYCGAL_MODULE_PARENT_PATH "")
  endif()

  set(target_name ${PYCGAL_PACKAGE_NAME}_${module_name})
  set(module_sources)
  collect_module_sources(
    ""
    ${module_name}
    ${target_name}
    "${${ARGV1}}"
    "${${ARGV2}}"
    "${ARGV3}"
    module_sources
  )

  pybind11_add_module(${target_name} ${module_sources})
  target_include_directories(${target_name} PUBLIC ${PYCGAL_INCLUDE_DIR})
  set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${module_name})
  if(PYCGAL_SHIPS_GMP_AND_MPFR)
    set_target_properties(${target_name} PROPERTIES INSTALL_RPATH "\$ORIGIN")
  endif(PYCGAL_SHIPS_GMP_AND_MPFR)

  install(TARGETS ${target_name}
          LIBRARY DESTINATION pycgal/${PYCGAL_MODULE_PATH}
  )

endfunction()
