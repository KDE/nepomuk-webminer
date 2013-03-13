

kde4_add_unit_test(Pipe.tvshow
  pipetvshow.cpp
)

target_link_libraries(Pipe.tvshow
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
