

project(metadataextractor)

find_package(NepomukMetadataExtractor REQUIRED)

# -------------------------------------------
# build the dolphin right click menu
# bulk extractor from the lib
# -------------------------------------------

set( metadataextractor_SRCS main.cpp)

kde4_add_executable( metadataextractor ${metadataextractor_SRCS})

target_link_libraries( metadataextractor
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KIO_LIBS}
  ${KDE4_KDEUI_LIBS}

  nepomukmetadataextractor
  ${NEPOMUK_CORE_LIBRARY}
  ${NEPOMUK_CORE_SYNC_LIBRARY}
)

install(TARGETS metadataextractor ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS metadataextractor.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
