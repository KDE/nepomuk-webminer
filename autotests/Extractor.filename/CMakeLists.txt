

kde4_add_unit_test(Extractor.filename
  analysefilename.cpp
)

target_link_libraries(Extractor.filename
#include nepomuk
        ${NEPOMUK_CORE_LIBRARY}

# include static libs
        nepomuk-webminerlib

# include general kde libs
        ${KDE4_KDECORE_LIBS}
        ${KDE4_KDEUI_LIBS}
        ${KDE4_KIO_LIBS}

        ${QT_QTTEST_LIBRARY}
)
