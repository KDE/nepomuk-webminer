
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
  #nepomukpipe/publicationpipe.cpp
  nepomukpipe/moviepipe.cpp
  nepomukpipe/tvshowpipe.cpp
  
  ui/fetcherdialog.cpp
  ui/searchresultsmodel.cpp
  ui/searchresultdelegate.cpp
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
  ${NEPOMUK_CORE_LIBRARY}
  ${NEPOMUK_CORE_SYNC_LIBRARY}

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
  ${CMAKE_CURRENT_BINARY_DIR}/ui_fetcherdialog.h
)

install( FILES ${nepomukmetadataextractor_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/nepomukmetadataextractor
  COMPONENT Devel
)

install(DIRECTORY webextractor/plugins DESTINATION ${DATA_INSTALL_DIR}/nepomukmetadataextractor)


# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
                         ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	                 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	                 COMMENT "Generating API documentation with Doxygen" VERBATIM
                      )
endif(DOXYGEN_FOUND)
