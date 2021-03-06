
# -------------------------------------------
# create the extractor library part
# -------------------------------------------
set( nepomuk-webminer_LIB_SRCS

  metadataparameters.cpp

  webextractor/extractorfactory.cpp
  webextractor/webextractor.cpp
  webextractor/krossextractor.cpp

  resourceextractor/resourceextractor.cpp
  resourceextractor/filenameanalyzer.cpp

  nepomukpipe/nepomukpipe.cpp
  nepomukpipe/publicationpipe.cpp
  nepomukpipe/moviepipe.cpp
  nepomukpipe/tvshowpipe.cpp
  nepomukpipe/musicpipe.cpp

  metadatawidget/metadatawidget.cpp
  metadatawidget/metadatabasewidget.cpp
  metadatawidget/moviewidget.cpp
  metadatawidget/musicwidget.cpp
  metadatawidget/publicationwidget.cpp
  metadatawidget/tvshowwidget.cpp

  ui/fetcher.cpp
  ui/fetcherdialog.cpp
  ui/batchextractor.cpp
  ui/searchresultsmodel.cpp
  ui/searchresultdelegate.cpp
  ui/automaticfetcher.cpp
  ui/startupdialog.cpp
)

kde4_add_ui_files( nepomuk-webminer_LIB_SRCS

                   metadatawidget/moviewidget.ui
                   metadatawidget/musicwidget.ui
                   metadatawidget/publicationwidget.ui
                   metadatawidget/tvshowwidget.ui

                   ui/fetcherdialog.ui
                   ui/startupdialog.ui
)

kde4_add_kcfg_files(nepomuk-webminer_LIB_SRCS mdesettings.kcfgc)
install(FILES nepomuk-webminer.kcfg DESTINATION ${KCFG_INSTALL_DIR})

kde4_add_library( nepomuk-webminerlib SHARED ${nepomuk-webminer_LIB_SRCS} )

target_link_libraries( nepomuk-webminerlib

  nbibontology

  ${KDE4_KDECORE_LIBS}
  ${KDE4_KIO_LIBS}
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KROSSUI_LIBS}
  ${KDE4_KROSSCORE_LIBS}
  ${KDE4_SOLID_LIBS}

# include nepomuk stuff
  ${SOPRANO_LIBRARIES}
  ${NEPOMUK_CORE_LIBRARY}

# other parts
  ${QJSON_LIBRARIES} # added because the macro seems to exist with upper and lowercase depending on the distribution
  ${qjson_LIBRARIES}
)

set_target_properties( nepomuk-webminerlib
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( TARGETS nepomuk-webminerlib ${INSTALL_TARGETS_DEFAULT_ARGS} )

set( nepomuk-webminer_LIB_HDRS
  nepomukwebminer_export.h
  metadataparameters.h

  resourceextractor/resourceextractor.h
  resourceextractor/filenameanalyzer.h

  webextractor/krossextractor.h
  webextractor/extractorfactory.h
  webextractor/webextractor.h

  metadatawidget/metadatawidget.h
  metadatawidget/metadatabasewidget.h
  metadatawidget/moviewidget.h
  metadatawidget/musicwidget.h
  metadatawidget/publicationwidget.h
  metadatawidget/tvshowwidget.h

  nepomukpipe/nepomukpipe.h
  nepomukpipe/moviepipe.h
  nepomukpipe/publicationpipe.h
  nepomukpipe/tvshowpipe.h
  nepomukpipe/musicpipe.h

  ui/fetcher.h
  ui/batchextractor.h
  ui/automaticfetcher.h
  ui/fetcherdialog.h
  ui/startupdialog.h
  ${CMAKE_CURRENT_BINARY_DIR}/ui_fetcherdialog.h
)

install( FILES ${nepomuk-webminer_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/nepomuk-webminer
  COMPONENT Devel
)

install(DIRECTORY webextractor/plugins DESTINATION ${DATA_INSTALL_DIR}/nepomuk-webminer)

