project(kcm_nepomuk-webminer)

# Nepomuk-WebMiner KCM
# -----------------------------

set(kcm_nepomuk-webminer_SRCS
  nepomukwebminerkcm.cpp
  configfetcher.cpp
  pluginlist.cpp
  regexpbox.cpp
  configregexp.cpp
  regexpmodel.cpp
  regexpdialog.cpp
  indexfolderselectiondialog.cpp
  folderselectionmodel.cpp
)

kde4_add_ui_files(kcm_nepomuk-webminer_SRCS
  configfetcher.ui
  pluginlist.ui
  regexpbox.ui
  configregexp.ui
  regexpdialog.ui
  indexfolderselectionwidget.ui
)

kde4_add_kcfg_files(kcm_nepomuk-webminer_SRCS ../lib/mdesettings.kcfgc)
kde4_add_plugin(kcm_nepomuk-webminer ${kcm_nepomuk-webminer_SRCS})

target_link_libraries(kcm_nepomuk-webminer
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KROSSCORE_LIBS}

  nepomuk-webminerlib
  )
# -----------------------------


# Installation
# -----------------------------
install(
  FILES kcm_nepomuk-webminer.desktop
  DESTINATION ${SERVICES_INSTALL_DIR})

install(
  TARGETS kcm_nepomuk-webminer
  DESTINATION ${PLUGIN_INSTALL_DIR})
# -----------------------------
