project(metadataextractorservice)

set( metadataextractorservice_SRCS
  metadataextractorservice.cpp
)

kde4_add_plugin(metadataextractorservice ${metadataextractorservice_SRCS})

target_link_libraries(metadataextractorservice
  nbibontology

  ${KDE4_KDECORE_LIBS}
  ${KDE4_KIO_LIBS}

# include nepomuk stuff
  ${SOPRANO_LIBRARIES}
  ${NEPOMUK_CORE_LIBRARY}
  )

install(
  FILES metadataextractorservice.desktop
  DESTINATION ${SERVICES_INSTALL_DIR})

install(
  TARGETS metadataextractorservice
  DESTINATION ${PLUGIN_INSTALL_DIR})
# -----------------------------
