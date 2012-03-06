project(metadataextractor)

##################  nepomukbulkextractor version  ################################
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
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/lib
  ${CMAKE_SOURCE_DIR}/lib/sro
  ${CMAKE_CURRENT_BUILD_DIR}/lib
  ${CMAKE_CURRENT_BINARY_DIR}/lib
)


add_subdirectory( lib )



# -------------------------------------------
# build the dolphin right click menu
# bulk extractor from the lib
# -------------------------------------------

set( metadataextractor_SRCS main.cpp )

kde4_add_executable( metadataextractor ${metadataextractor_SRCS})

target_link_libraries( metadataextractor
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KIO_LIBS}
  ${KDE4_KDEUI_LIBS}

  nepomukmetadataextractor
)

install(TARGETS metadataextractor ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS metadataextractor.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
