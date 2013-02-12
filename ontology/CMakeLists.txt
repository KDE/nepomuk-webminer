
# -------------------------------------------
# Generate the SimpleResource classes for all
# ontologies used in the library
# -------------------------------------------

message(STATUS "#####################################################################################################")
message(STATUS "Generating ontology classes")
message(STATUS "#####################################################################################################")

NEPOMUK_ADD_SIMPLERESOURCE_CLASSES(ONTOLOGIES
                                   ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nfo.trig
                                   ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nco.trig
                                   ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nmm.trig
                                   ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/pimo/pimo.trig
                                   ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nie.trig
                                   ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nepomuk/nao.trig
                                   ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/ncal.trig
                                   ${CMAKE_SOURCE_DIR}/ontology/nbib/nbib.trig
                                   )

set(nbibontology_LIB_SRCS)

# -DFPIC -fPIC are needed to compile and use the nbib ontology
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFPIC -fPIC")

soprano_add_ontology(nbibontology_LIB_SRCS ${NEPOMUK_CORE_ONTOLOGY_DIR}/kde/kext.trig "KExt" "Nepomuk2::Vocabulary" "trig")
soprano_add_ontology(nbibontology_LIB_SRCS ${CMAKE_SOURCE_DIR}/ontology/nbib/nbib.trig "NBIB" "Nepomuk2::Vocabulary" "trig")
kde4_add_library( nbibontology STATIC ${nbibontology_LIB_SRCS} )

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
