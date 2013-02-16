project(nepomuk-webminer)

#--------------------------------------------------------------------------
#  Nepomuk-WebMiner version
#--------------------------------------------------------------------------
set(CMAKE_NEPOMUKWEBMINER_VERSION_MAJOR 0)
set(CMAKE_NEPOMUKWEBMINER_VERSION_MINOR 5)
set(CMAKE_NEPOMUKWEBMINER_VERSION_RELEASE 0)
set(CMAKE_NEPOMUKWEBMINER_VERSION "${CMAKE_NEPOMUKWEBMINER_VERSION_MAJOR}.${CMAKE_NEPOMUKWEBMINER_VERSION_MINOR}.${CMAKE_NEPOMUKWEBMINER_VERSION_RELEASE}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)


#--------------------------------------------------------------------------
# find required dependencies
#--------------------------------------------------------------------------
set(KDE_MIN_VERSION "4.7.00")
find_package(KDE4 4.7.0 REQUIRED)
include(KDE4Defaults)

find_package(NepomukCore REQUIRED)
set(SDO_MIN_VERSION "0.10.0")
find_package(SharedDesktopOntologies)
find_package(Soprano)

include(MacroLibrary)
include(MacroOptionalAddSubdirectory)
include(NepomukAddSimpleResourceClasses)
include(NepomukAddOntologyClasses)

find_package(QJSON REQUIRED)

macro_log_feature(SHAREDDESKTOPONTOLOGIES_FOUND "Shared desktop ontologies" "Desktop ontologies" "http://oscaf.sourceforge.net" TRUE ${SDO_MIN_VERSION} "Ontologies necessary for the Nepomuk semantic desktop.")


#--------------------------------------------------------------------------
# global include diretories for all subdirectories
#--------------------------------------------------------------------------
include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${SOPRANO_INCLUDE_DIR}
  ${NEPOMUK_CORE_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/kcm
  ${CMAKE_SOURCE_DIR}/lib
  ${CMAKE_SOURCE_DIR}/lib/ui
  ${CMAKE_CURRENT_BUILD_DIR}/lib
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/lib
  ${CMAKE_CURRENT_BINARY_DIR}/ontology
)

add_subdirectory( ontology )
add_subdirectory( lib )
add_subdirectory( extractor )
add_subdirectory( service )
add_subdirectory( browser/konqueror )
add_subdirectory( kcm )
add_subdirectory( doc )

macro_display_feature_log()
MESSAGE("###############################################################################################")
MESSAGE("See INSTALL file for the python runtime dependencies that are necessary for the plugins to work")
MESSAGE("###############################################################################################")

#--------------------------------------------------------------------------
# add a target to generate API documentation with Doxygen
#--------------------------------------------------------------------------
find_package(Doxygen)
if(DOXYGEN_FOUND)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
                         ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                         COMMENT "Generating API documentation with Doxygen" VERBATIM
                      )
endif(DOXYGEN_FOUND)
