#/**********************************************************\ 
# Auto-generated X11 project definition file for the
# MetaDataFetcherPlugin project
#\**********************************************************/

# X11 template platform definition CMake file
# Included from ../CMakeLists.txt

# Copyright (c) 2012, Jörg Ehrichs, <Joerg.Ehrichs@gmx.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

#include_directories(${FOO_INCLUDE_DIRS})

# remember that the current source dir is the project root; this file is in X11/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    X11/[^.]*.cpp
    X11/[^.]*.h
    X11/[^.]*.cmake
    )

SOURCE_GROUP(X11 FILES ${PLATFORM})

# use this to add preprocessor definitions
add_definitions(
)

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
    )

add_x11_plugin(${PROJECT_NAME} SOURCES)

# add library dependencies here; leave ${PLUGIN_INTERNAL_DEPS} there unless you know what you're doing!
target_link_libraries(${PROJECT_NAME}
    ${PLUGIN_INTERNAL_DEPS}
    ${QT_LIBRARIES}
    nepomuk-webminerlib
    )
