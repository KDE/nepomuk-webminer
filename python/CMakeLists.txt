

#install pyton stuff
install(FILES async.py DESTINATION ${DATA_INSTALL_DIR}/metadataextractor)
install(FILES extractor.py DESTINATION ${DATA_INSTALL_DIR}/metadataextractor)
install(DIRECTORY webengines DESTINATION ${DATA_INSTALL_DIR}/metadataextractor)
