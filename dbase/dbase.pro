DCOMMON_DIR = $$PWD/../common
include($$DCOMMON_DIR/lib.pri)
include(private/private.pri)

QT -= gui

TARGET = dtkbase

DEFINES += LIBDTKBASE_LIBRARY

HEADERS += \
    dtk_global.h \
    dobject.h

includes.path = $${DTK_INCLUDEPATH}/DBase
includes.files += *.h

QMAKE_PKGCONFIG_NAME = DTK_BASE
QMAKE_PKGCONFIG_DESCRIPTION = Deepin Tool Kit Base Header Files
QMAKE_PKGCONFIG_INCDIR = $$includes.path

SOURCES += \
    dobject.cpp
