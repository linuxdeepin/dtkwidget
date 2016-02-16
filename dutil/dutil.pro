DCOMMON_DIR = $$PWD/../common
include($$DCOMMON_DIR/lib.pri)
include(dlog/dlog.pri)

QT -= gui

TARGET = dtkutil

DEFINES += LIBDTKUTIL_LIBRARY

HEADERS += \
    DLog

includes.path = $${DTK_INCLUDEPATH}/DUtil
includes.files += \
    $$system($$DCOMMON_DIR/trheader.sh $$PWD/dlog/DLog) \
    $$PWD/dlog/DLog

QMAKE_PKGCONFIG_NAME = DTK_UTIL
QMAKE_PKGCONFIG_DESCRIPTION = Deepin Tool Kit Util Module
QMAKE_PKGCONFIG_INCDIR = $$includes.path
