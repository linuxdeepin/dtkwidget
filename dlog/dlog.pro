include($$PWD/../common/lib.pri)
include(cutelogger.pri)

QT -= gui

TARGET = dtklog

DEFINES += LIBDTKLOG_LIBRARY

HEADERS += \
    DLog

includes.path = $${DTK_INCLUDEPATH}/DLog
includes.files += \
DLog \
$$system($$PWD/../common/trheader.sh $$PWD/DLog)

INSTALLS += includes

QMAKE_PKGCONFIG_NAME = DTK_LOG
QMAKE_PKGCONFIG_DESCRIPTION = Deepin Tool Kit Log Module
QMAKE_PKGCONFIG_INCDIR = $$includes.path
