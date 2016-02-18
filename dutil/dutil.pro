DCOMMON_DIR = $$PWD/../common
include($$DCOMMON_DIR/lib.pri)
include(dlog/dlog.pri)

QT -= gui

TARGET = dtkutil

DEFINES += LIBDTKUTIL_LIBRARY

HEADERS += \
    dutil_global.h

includes.path = $${DTK_INCLUDEPATH}/DUtil
includes.files += \
    $$PWD/dutil_global.h \
    $$PWD/dlog/DLog \
    $$system($$DCOMMON_DIR/trheader.sh $$PWD/dlog/DLog) \

QMAKE_PKGCONFIG_NAME = DTK_UTIL
QMAKE_PKGCONFIG_DESCRIPTION = Deepin Tool Kit Util Module
QMAKE_PKGCONFIG_INCDIR = $$includes.path


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dbase/release/ -ldtkbase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dbase/debug/ -ldtkbase
else:unix: LIBS += -L$$OUT_PWD/../dbase/ -ldtkbase

INCLUDEPATH += $$PWD/../dbase
DEPENDPATH += $$PWD/../dbase
