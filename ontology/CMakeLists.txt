
# -------------------------------------------
# Generate the SimpleResource classes for all
# ontologies used in the library
# -------------------------------------------

if(DEFINED NOONTOLOGY)
    message(STATUS "#####################################################################################################")
    message(STATUS "Skip ontology class generation. Reuse old generated files.")
    message(STATUS "#####################################################################################################")
else(DEFINED NOONTOLOGY)
    message(STATUS "#####################################################################################################")
    message(STATUS "Generating ontology classes")
    message(STATUS "This will take a long time! (~20-30 minutes)")
    message(STATUS "All warnings can be ignored")
    message(STATUS "This can be skipped with the \"-DNOONTOLOGY=1\" cmake option")
    message(STATUS "#####################################################################################################")

    NEPOMUK_ADD_SIMPLERESOURCE_CLASSES(metadata_extractor_HEADERS ONTOLOGIES
                                       ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nfo.trig
                                       ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nco.trig
                                       ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nmm.trig
                                       ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/pimo/pimo.trig
                                       ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nie.trig
                                       ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nepomuk/nao.trig
                                       ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/ncal.trig
                                       ${CMAKE_SOURCE_DIR}/ontology/nbib/nbib.trig
                                       )
endif(DEFINED NOONTOLOGY)

# -------------------------------------------
# install the nbib ontology
# this is a temporary action until nbib is not in SDO
# -------------------------------------------

macro(INSTALL_ONTOLOGY _name _group)
  set(ONTO_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/ontology/${_group})
  configure_file("${_name}.ontology.in" "${CMAKE_CURRENT_BINARY_DIR}/${_name}.ontology")
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${_name}.ontology" DESTINATION ${ONTO_INSTALL_DIR})
  install(FILES "${_name}.trig" DESTINATION ${ONTO_INSTALL_DIR})
endmacro(INSTALL_ONTOLOGY)

install_ontology(nbib/nbib nepomuk)
