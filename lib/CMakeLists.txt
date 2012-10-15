
# -------------------------------------------
# create the extractor library part
# -------------------------------------------
set( nepomukextractor_LIB_SRCS

  webextractor/extractorfactory.cpp
  webextractor/webextractor.cpp
  webextractor/krossextractor.cpp

  resourceextractor/resourceextractor.cpp
  resourceextractor/popplerextractor.cpp
  resourceextractor/odfextractor.cpp
  resourceextractor/videoextractor.cpp
  resourceextractor/audioextractor.cpp

  nepomukpipe/nepomukpipe.cpp
  nepomukpipe/publicationpipe.cpp
  nepomukpipe/moviepipe.cpp
  nepomukpipe/tvshowpipe.cpp
  nepomukpipe/musicpipe.cpp

  ui/fetcher.cpp
  ui/fetcherdialog.cpp
  ui/batchextractor.cpp
  ui/searchresultsmodel.cpp
  ui/searchresultdelegate.cpp
  ui/automaticfetcher.cpp
  ui/metadatawidget.cpp

  sro/nbib.cpp
)

kde4_add_ui_files( nepomukextractor_LIB_SRCS
                   ui/fetcherdialog.ui
)

kde4_add_kcfg_files(nepomukextractor_LIB_SRCS mdesettings.kcfgc)
install(FILES nepomukmetadataextractor.kcfg DESTINATION ${KCFG_INSTALL_DIR})

kde4_add_library( nepomukmetadataextractor SHARED ${nepomukextractor_LIB_SRCS} )

target_link_libraries( nepomukmetadataextractor
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KIO_LIBS}
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KROSSUI_LIBS}
  ${KDE4_KROSSCORE_LIBS}

# include nepomuk stuff
  ${SOPRANO_LIBRARIES}
  ${NEPOMUK_CORE_LIBRARY}

# other parts
  ${POPPLER_LIBRARY}
  ${TAGLIB_LIBRARIES}

  ${QJSON_LIBRARIES} # added because the macro seems to exist with upper and lowercase depending on the distribution
  ${qjson_LIBRARIES}
)

set_target_properties( nepomukmetadataextractor
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( TARGETS nepomukmetadataextractor ${INSTALL_TARGETS_DEFAULT_ARGS} )

set( nepomukmetadataextractor_LIB_HDRS
  nepomukmetadataextractor_export.h
  metadataparameters.h

  resourceextractor/resourceextractor.h
  resourceextractor/videoextractor.h
  resourceextractor/audioextractor.h
  resourceextractor/popplerextractor.h
  resourceextractor/odfextractor.h

  webextractor/krossextractor.h
  webextractor/extractorfactory.h
  webextractor/webextractor.h

  nepomukpipe/nepomukpipe.h
  nepomukpipe/moviepipe.h
  nepomukpipe/publicationpipe.h
  nepomukpipe/tvshowpipe.h
  nepomukpipe/musicpipe.h

  ui/fetcher.h
  ui/batchextractor.h
  ui/automaticfetcher.h
  ui/fetcherdialog.h
  ui/metadatawidget.h
  ${CMAKE_CURRENT_BINARY_DIR}/ui_fetcherdialog.h
)

install( FILES ${nepomukmetadataextractor_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/nepomukmetadataextractor
  COMPONENT Devel
)

install(DIRECTORY webextractor/plugins DESTINATION ${DATA_INSTALL_DIR}/nepomukmetadataextractor)

