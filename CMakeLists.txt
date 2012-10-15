project(metadataextractor)

#--------------------------------------------------------------------------
#  nepomuk metadata extractor version
#--------------------------------------------------------------------------
set(CMAKE_METDADATAEXTRACTOR_VERSION_MAJOR 0)
set(CMAKE_METDADATAEXTRACTOR_VERSION_MINOR 2)
set(CMAKE_METDADATAEXTRACTOR_VERSION_RELEASE 0)
set(CMAKE_METDADATAEXTRACTOR_VERSION "${CMAKE_METDADATAEXTRACTOR_VERSION_MAJOR}.${CMAKE_METDADATAEXTRACTOR_VERSION_MINOR}.${CMAKE_METDADATAEXTRACTOR_VERSION_RELEASE}")

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

include(MacroLibrary)
include(MacroOptionalAddSubdirectory)

find_package(QJSON REQUIRED)
find_package(Poppler REQUIRED)
find_package(Taglib REQUIRED)

macro_log_feature(HAVE_POPPLER_0_12_1 "Poppler-Qt4" "A PDF rendering library" "http://poppler.freedesktop.org" TRUE "0.12.1" "Poppler necessary to extract data from pdf files")
macro_log_feature(SHAREDDESKTOPONTOLOGIES_FOUND "Shared desktop ontologies" "Desktop ontologies" "http://oscaf.sourceforge.net" TRUE ${SDO_MIN_VERSION} "Ontologies necessary for the Nepomuk semantic desktop.")
macro_log_feature(TAGLIB_FOUND "TagLib" "library for reading and editing audio meta data" "http://taglib.github.com/" TRUE "1.4" "TagLib necessary to find music meta data")

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
  ${CMAKE_SOURCE_DIR}/lib/sro
  ${CMAKE_SOURCE_DIR}/lib/ui
  ${CMAKE_CURRENT_BUILD_DIR}/lib
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/lib
)

add_subdirectory( lib )
add_subdirectory( extractor )
add_subdirectory( service )
add_subdirectory( browser/konqueror )
add_subdirectory( ontology )
add_subdirectory( kcm )

macro_display_feature_log()

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
