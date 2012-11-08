#
# Use the Nepomuk simple resource class generator to generate convinient SimpleResource
# subclasses
# from ontologies.
#
# Usage:
#   NEPOMUK_ADD_SIMPLERESOURCE_CLASSES(
#         [ONTOLOGIES] <onto-file1> [<onto-file2> ...]
#       )
#
# The optional CLASSES parameter allows to specify the classes to be generated (RDF URIs) in
# case one does not want all classes in the ontologies to be generated.
#
# The optional VISIBILITY parameter can only be used in non-fast mode and allows to set the gcc visibility
# to make the generated classes usable in a publically exported API. The <visibility-name> is used to create
# the name of the export macro and the export include file. Thus, when using "VISIBILITY foobar" include
# file "foobar_export.h" needs to define FOOBAR_EXPORT.
#
# Copyright (c) 2009 Sebastian Trueg <trueg@kde.org>
# Copyright (c) 2011 Serebriyskiy Artem <v.for.vandal@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

macro(NEPOMUK_ADD_SIMPLERESOURCE_CLASSES )
  # extract arguments
  set(_current_arg_type "onto")
  foreach (_arg ${ARGN})
    if(${_arg} STREQUAL "ONTOLOGIES")
      set(_current_arg_type "onto")
    else(${_arg} STREQUAL "ONTOLOGIES")
      if(${_current_arg_type} STREQUAL "onto")
        list(APPEND _ontologies ${_arg})
        get_filename_component(_filename ${_arg} NAME)
        list(APPEND _ontofilenames ${_filename})
      endif(${_current_arg_type} STREQUAL "onto")
    endif(${_arg} STREQUAL "ONTOLOGIES")
  endforeach (_arg)

  # find our helper program (first in the install dir, then everywhere)
  unset(SRCGEN)
  if(NOT WINCE)
    find_program(SRCGEN nepomuk-simpleresource-rcgen.py PATHS ${CMAKE_SOURCE_DIR} NO_DEFAULT_PATH)
    find_program(SRCGEN nepomuk-simpleresource-rcgen PATHS ${KDE4_BIN_INSTALL_DIR} ${BIN_INSTALL_DIR}  NO_DEFAULT_PATH)
    find_program(SRCGEN nepomuk-simpleresource-rcgen)
  else(NOT WINCE)
    find_program(SRCGEN nepomuk-simpleresource-rcgen.py PATHS ${HOST_BINDIR} NO_DEFAULT_PATH)
  endif(NOT WINCE)

  if(NOT SRCGEN)
    message(WARNING "Failed to find the Nepomuk SimpleResource generator" )
  else(NOT SRCGEN)
    file(TO_NATIVE_PATH ${SRCGEN} SRCGEN)

    # we generate the files in the current binary dir
    set(_targetdir ${CMAKE_CURRENT_BINARY_DIR})

    # generate the list of headers files
    execute_process(
      COMMAND ${SRCGEN} --output ${_targetdir}/  ${_ontologies}
      RESULT_VARIABLE rcgen_result
      )
    if(NOT ${rcgen_result} EQUAL 0)
      message(SEND_ERROR "Running ${SRCGEN} to generate headers failed with error code ${rcgen_result}")
    endif(NOT ${rcgen_result} EQUAL 0)

    # make sure the includes are found
    include_directories(${_targetdir})

  endif(NOT SRCGEN)

  # reset variable names used
  unset(_current_arg_type)
  unset(_arg)
  unset(_ontologies)
  unset(_ontofilenames)
  unset(_targetdir)
  unset(rcgen_result)
endmacro(NEPOMUK_ADD_SIMPLERESOURCE_CLASSES)
