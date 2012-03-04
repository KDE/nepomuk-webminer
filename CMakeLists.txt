project(metadataextractor)

##################  metadataextractor version  ################################
set(CMAKE_METDADATAEXTRACTOR_VERSION_MAJOR 0)
set(CMAKE_METDADATAEXTRACTOR_VERSION_MINOR 1)
set(CMAKE_METDADATAEXTRACTOR_VERSION_RELEASE 0)
set(CMAKE_METDADATAEXTRACTOR_VERSION "${CMAKE_METDADATAEXTRACTOR_VERSION_MAJOR}.${CMAKE_METDADATAEXTRACTOR_VERSION_MINOR}.${CMAKE_METDADATAEXTRACTOR_VERSION_RELEASE}")


find_package(KDE4 4.6.0 REQUIRED)
find_package(Nepomuk REQUIRED)
include(KDE4Defaults)
include(SopranoAddOntology)
include(MacroLibrary)
include(MacroOptionalAddSubdirectory)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(Poppler)
macro_log_feature(HAVE_POPPLER_0_12_1 "Poppler-Qt4" "A PDF rendering library" "http://poppler.freedesktop.org" FALSE "0.12.1" "Support for PDF files in okular.")

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${SOPRANO_INCLUDE_DIR}
  ${NEPOMUK_INCLUDE_DIR}

  "/usr/include/python2.6"
  "/usr/include/python2.7"

  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/lib
  ${metadataextractor_BINARY_DIR}
)


add_subdirectory( lib )
#add_subdirectory( src )
#add_subdirectory( python )
