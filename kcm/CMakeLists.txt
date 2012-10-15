project(kcm_metadataextractor)

# Metadata extractor KCM
# -----------------------------

set(kcm_metadataextractor_SRCS
  metadataextractorkcm.cpp
  configfetcher.cpp
  configservice.cpp
  pluginlist.cpp
)

kde4_add_ui_files(kcm_metadataextractor_SRCS
  configfetcher.ui
  configservice.ui
  pluginlist.ui
)

kde4_add_kcfg_files(kcm_metadataextractor_SRCS ../lib/mdesettings.kcfgc)
kde4_add_plugin(kcm_metadataextractor ${kcm_metadataextractor_SRCS})

target_link_libraries(kcm_metadataextractor
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
  ${KDE4_KDECORE_LIBS}
  
  nepomukmetadataextractor
  )
# -----------------------------


# Installation
# -----------------------------
install(
  FILES kcm_metadataextractor.desktop
  DESTINATION ${SERVICES_INSTALL_DIR})

install(
  TARGETS kcm_metadataextractor
  DESTINATION ${PLUGIN_INSTALL_DIR})
# -----------------------------
