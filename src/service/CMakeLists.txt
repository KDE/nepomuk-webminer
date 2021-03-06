project(nepomuk-webminerservice)

set( nepomuk-webminerservice_SRCS
  nepomukwebminerservice.cpp
  eventmonitor.cpp
  indexingqueue.cpp
  indexscheduler.cpp
  webminerindexingjob.cpp
  webminerindexingqueue.cpp
)

kde4_add_kcfg_files(nepomuk-webminerservice_SRCS ../lib/mdesettings.kcfgc)
qt4_add_dbus_adaptor(nepomuk-webminerservice_SRCS org.kde.nepomuk.WebMiner.xml nepomukwebminerservice.h NepomukWebMinerService )

kde4_add_plugin(nepomuk-webminerservice ${nepomuk-webminerservice_SRCS})


target_link_libraries(nepomuk-webminerservice
  nbibontology

  ${KDE4_KDECORE_LIBS}
  ${KDE4_KIO_LIBS}
  ${KDE4_SOLID_LIBS}
  ${KDE4_KIDLETIME_LIBS}

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
  
  
install(
  FILES org.kde.nepomuk.WebMiner.xml
  DESTINATION ${DBUS_INTERFACES_INSTALL_DIR}
  )

# -----------------------------
