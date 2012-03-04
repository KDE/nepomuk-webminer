
add_subdirectory( dms-copy )

# -------------------------------------------
# create the extractor library part
# -------------------------------------------
set( nepomukextractor_LIB_SRCS

  main.cpp

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

kde4_add_executable(nepomukextractor ${nepomukextractor_LIB_SRCS})
#kde4_add_library( nepomukextractor SHARED ${nepomukextractor_LIB_SRCS} )

target_link_libraries( nepomukextractor
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

#set_target_properties( nepomukextractor
#  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
#)

#install( TARGETS nepomukextractor ${INSTALL_TARGETS_DEFAULT_ARGS} )

#set( webextractor_LIB_HDRS
#  webextractor_export.h
#  extractor.h
#)

#install( FILES ${webextractor_LIB_HDRS}
#  DESTINATION ${INCLUDE_INSTALL_DIR}/nepomukextractor
#  COMPONENT Devel
#)

#install(TARGETS metadataextractor ${INSTALL_TARGETS_DEFAULT_ARGS})
#install(PROGRAMS metadataextractor.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

