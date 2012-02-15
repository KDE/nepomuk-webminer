project(metadataextractor)

##################  Nepomuktvnamer version  ################################
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
  ${metadataextractor_BINARY_DIR}
)

set( metadataextractor_BIN_SRCS
  main.cpp
  metadatafetcher.cpp

  fileextractor/popplerextractor.cpp
  fileextractor/odfextractor.cpp
  search/searchmicrosoftacademic.cpp
)

kde4_add_executable(metadataextractor ${metadataextractor_BIN_SRCS})

target_link_libraries(metadataextractor
  ${NEPOMUK_LIBRARIES}
  ${QT_QTGUI_LIBRARY}
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
  ${SOPRANO_LIBRARIES}
  ${NEPOMUK_QUERY_LIBRARIES}


  ${POPPLER_LIBRARY}
)

install(TARGETS metadataextractor ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS metadataextractor.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
