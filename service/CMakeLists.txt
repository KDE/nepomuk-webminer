project(metadataextractorservice)

set( metadataextractorservice_SRCS
  metadataextractorservice.cpp
)

kde4_add_plugin(metadataextractorservice ${metadataextractorservice_SRCS})

target_link_libraries(metadataextractorservice
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KIO_LIBS}
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
