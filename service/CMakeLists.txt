project(nepomuk-webminerservice)

set( nepomuk-webminerservice_SRCS
  nepomukwebminerservice.cpp
)

kde4_add_plugin(nepomuk-webminerservice ${nepomuk-webminerservice_SRCS})

target_link_libraries(nepomuk-webminerservice
  nbibontology

  ${KDE4_KDECORE_LIBS}
  ${KDE4_KIO_LIBS}

# include nepomuk stuff
  ${SOPRANO_LIBRARIES}
  ${NEPOMUK_CORE_LIBRARY}
  )

install(
  FILES nepomuk-webminerservice.desktop
  DESTINATION ${SERVICES_INSTALL_DIR})

install(
  TARGETS nepomuk-webminerservice
  DESTINATION ${PLUGIN_INSTALL_DIR})
# -----------------------------
