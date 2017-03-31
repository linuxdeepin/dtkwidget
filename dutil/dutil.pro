DCOMMON_DIR = $$PWD/../common
include($$DCOMMON_DIR/lib.pri)
include(dlog/dlog.pri)
include(private/private.pri)

QT -= gui

TARGET = dtkutil

DEFINES += LIBDTKUTIL_LIBRARY

HEADERS += \
    dutil_global.h \
    dpathbuf.h \
    dutil.h \
    ddesktopservices.h \
    dtrashmanager.h \
    dfilesystemwatcher.h \
    dfilewatcher.h \
    dfilewatchermanager.h \
    dbasefilewatcher.h

includes.path = $${DTK_INCLUDEPATH}/DUtil
includes.files += \
    $$PWD/DUtil \
    $$PWD/DPathBuf \
    $$PWD/dutil.h \
    $$PWD/dpathbuf.h \
    $$PWD/dutil_global.h \
    $$PWD/dlog/DLog \
    $$PWD/dlog/CuteLogger_global.h \
    $$PWD/dlog/RollingFileAppender.h \
    $$PWD/dlog/Logger.h \
    $$PWD/dlog/LogManager.h \
    $$PWD/dlog/FileAppender.h \
    $$PWD/dlog/ConsoleAppender.h \
    $$PWD/dlog/AbstractStringAppender.h \
    $$PWD/dlog/AbstractAppender.h \
    $$PWD/DDesktopServices \
    $$PWD/DTrashManager \
    $$PWD/DBaseFileWatcher \
    $$PWD/DFileSystemWatcher \
    $$PWD/DFileWatcher \
    $$PWD/DFileWatcherManager

QMAKE_PKGCONFIG_NAME = DTK_UTIL
QMAKE_PKGCONFIG_DESCRIPTION = Deepin Tool Kit Util Module
QMAKE_PKGCONFIG_INCDIR = $$includes.path

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dbase/release/ -ldtkbase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dbase/debug/ -ldtkbase
else:unix: LIBS += -L$$OUT_PWD/../dbase/ -ldtkbase

INCLUDEPATH += $$PWD/../dbase
DEPENDPATH += $$PWD/../dbase

linux {
    qtHaveModule(dbus) {
        QT += dbus
        SOURCES += ddesktopservices_linux.cpp
    }

    SOURCES += \
        dtrashmanager_linux.cpp \
        dfilesystemwatcher_linux.cpp
}

DISTFILES += \
    DUtil

SOURCES += \
    dfilewatcher.cpp \
    dfilewatchermanager.cpp \
    dbasefilewatcher.cpp
