

project(nepomuk-webminer)

find_package(NepomukWebMiner REQUIRED)

# -------------------------------------------
# build the dolphin right click menu
# bulk extractor from the lib
# -------------------------------------------

set( nepomukwebminer_SRCS main.cpp)

kde4_add_executable( nepomuk-webminer ${nepomukwebminer_SRCS})

target_link_libraries( nepomuk-webminer
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KIO_LIBS}
  ${KDE4_KDEUI_LIBS}

  nepomuk-webminerlib
  ${NEPOMUK_CORE_LIBRARY}
)

install(TARGETS nepomuk-webminer ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS nepomuk-webminer.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
