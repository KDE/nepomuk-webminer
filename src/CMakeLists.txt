


add_subdirectory( dms-copy )

set( metadataextractor_BIN_SRCS
  main.cpp
  fetcherdialog.cpp
  metadatafetcher.cpp
  selectenginedialog.cpp
  selectsearchresultdialog.cpp
  searchresultsmodel.cpp
  searchresultdelegate.cpp

  fileextractor/popplerextractor.cpp
  fileextractor/odfextractor.cpp
  fileextractor/videoextractor.cpp

  nepomukpipe/nepomukpipe.cpp
  nepomukpipe/publicationpipe.cpp
  nepomukpipe/moviepipe.cpp

  sro/nbib.cpp
)

kde4_add_ui_files( metadataextractor_BIN_SRCS

                   fetcherdialog.ui
                   selectenginedialog.ui
                   selectsearchresultdialog.ui
)

kde4_add_executable(metadataextractor ${metadataextractor_BIN_SRCS})

target_link_libraries(metadataextractor

  ${KDE4_KDECORE_LIBS}
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}

# include nepomuk stuff
  ${SOPRANO_LIBRARIES}
  ${NEPOMUK_LIBRARIES}
  ${NEPOMUK_QUERY_LIBRARIES}
  nepomukdatamanagement-copy

  ${POPPLER_LIBRARY}
  PythonQt
)

install(TARGETS metadataextractor ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS metadataextractor.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

