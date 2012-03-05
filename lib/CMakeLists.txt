
add_subdirectory( dms-copy )

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
  
  nepomukpipe/nepomukpipe.cpp
  nepomukpipe/publicationpipe.cpp
  nepomukpipe/moviepipe.cpp
  

  ui/fetcherdialog.cpp
  ui/searchresultsmodel.cpp
  ui/searchresultdelegate.cpp

  sro/nbib.cpp
)

kde4_add_ui_files( nepomukextractor_LIB_SRCS
                   ui/fetcherdialog.ui
)

kde4_add_library( nepomukmetadataextractor SHARED ${nepomukextractor_LIB_SRCS} )

target_link_libraries( nepomukmetadataextractor
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KIO_LIBS}
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KROSSUI_LIBS}
  ${KDE4_KROSSCORE_LIBS}

# include nepomuk stuff
  ${SOPRANO_LIBRARIES}
  ${NEPOMUK_LIBRARIES}
  ${NEPOMUK_QUERY_LIBRARIES}
  nepomukdatamanagement-copy

# other parts
  ${POPPLER_LIBRARY}
)

set_target_properties( nepomukmetadataextractor
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( TARGETS nepomukmetadataextractor ${INSTALL_TARGETS_DEFAULT_ARGS} )

set( nepomukmetadataextractor_LIB_HDRS
  nepomukmetadataextractor_export.h
  metadataparameters.h

  webextractor/extractorfactory.h
  webextractor/webextractor.h

  ui/fetcherdialog.h
)

install( FILES ${nepomukmetadataextractor_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/nepomukmetadataextractor
  COMPONENT Devel
)
