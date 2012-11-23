
project(nepomukwebminerplugin)

find_package(NepomukWebMiner REQUIRED)

set(nepomukwebminerplugin_SRCS nepomukwebminerplugin.cpp)

kde4_add_plugin(nepomukwebminerplugin ${nepomukwebminerplugin_SRCS})

target_link_libraries( nepomukwebminerplugin
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KHTML_LIBS}
    nepomuk-webminerlib
)

install(TARGETS nepomukwebminerplugin  DESTINATION ${PLUGIN_INSTALL_DIR} )
install(FILES nepomukwebminerplugin.rc nepomukwebminerplugin.desktop  DESTINATION  ${DATA_INSTALL_DIR}/khtml/kpartplugins )
install(FILES nepomukwebminerplugin.rc nepomukwebminerplugin.desktop  DESTINATION  ${DATA_INSTALL_DIR}/kwebkitpart/kpartplugins )

