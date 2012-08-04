
project(metadataextractorplugin)

find_package(NepomukMetadataExtractor REQUIRED)

set(metadataextractorplugin_SRCS metadataextractorplugin.cpp)

kde4_add_plugin(metadataextractorplugin ${metadataextractorplugin_SRCS})

target_link_libraries( metadataextractorplugin
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KHTML_LIBS}
    nepomukmetadataextractor
)

install(TARGETS metadataextractorplugin  DESTINATION ${PLUGIN_INSTALL_DIR} )
install(FILES metadataextractorplugin.rc metadataextractorplugin.desktop  DESTINATION  ${DATA_INSTALL_DIR}/khtml/kpartplugins )
install(FILES metadataextractorplugin.rc metadataextractorplugin.desktop  DESTINATION  ${DATA_INSTALL_DIR}/kwebkitpart/kpartplugins )

