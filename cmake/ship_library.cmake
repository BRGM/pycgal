function(ship_library SHIPPED_LIBRARY LIBRARY_INSTALL_DESTINATION)
  get_filename_component(LIBNAME ${SHIPPED_LIBRARY} NAME_WE)
  message(STATUS "Trying to ship ${LIBNAME} library (${SHIPPED_LIBRARY})")
  set(SHIPPING_OK FALSE)
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
    set(SHIPPING_OK TRUE)
  endif(WIN32)
  if(UNIX)
    get_filename_component(SHIPPED_LIBRARY ${SHIPPED_LIBRARY} REALPATH)
    if(NOT EXISTS ${SHIPPED_LIBRARY})
      message(FATAL_ERROR "Could not find a so file for ${LIBNAME} library!")
    endif()
    get_filename_component(LIB_FULLNAME ${SHIPPED_LIBRARY} NAME)
    get_filename_component(LIB_BASENAME ${SHIPPED_LIBRARY} NAME_WE)
    string(REGEX MATCH ${LIB_BASENAME}.so.[0-9]+ SHIPPED_LIBRARY_NAME
                 ${LIB_FULLNAME}
    )
    if(NOT SHIPPED_LIBRARY_NAME)
      message(
        FATAL_ERROR "Could not find a suitable name for ${LIBNAME} library!"
      )
    endif()
    set(SHIPPING_OK TRUE)
  endif(UNIX)
  if(NOT SHIPPING_OK)
    message(FATAL_ERROR "Library cannot be shipped on current platform!")
  endif()
  message(STATUS "          shipping: ${SHIPPED_LIBRARY}")
  if(NOT SKBUILD)
    message(
      FATAL_ERROR
        "The build must be driven by scikit-build-core to ship libraries."
    )
  endif()
  install(
    FILES ${SHIPPED_LIBRARY}
    DESTINATION ${SKBUILD_PLATLIB_DIR}
    RENAME ${SHIPPED_LIBRARY_NAME}
  )
endfunction()
