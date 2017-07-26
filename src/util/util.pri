INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/dfileiconprovider.h \
    $$PWD/dthumbnailprovider.h \
    $$PWD/dwindowmanagerhelper.h \
    $$PWD/dwidgetutil.h \
    $$PWD/ddesktopservices.h \
    $$PWD/dtrashmanager.h

SOURCES += \
    $$PWD/dfileiconprovider.cpp \
    $$PWD/dthumbnailprovider.cpp \
    $$PWD/dwindowmanagerhelper.cpp \
    $$PWD/dwidgetutil.cpp

linux* {
CONFIG += link_pkgconfig
PKGCONFIG += gsettings-qt

SOURCES += \
    $$PWD/ddesktopservices_linux.cpp \
    $$PWD/dtrashmanager_linux.cpp
}

win32* | macx* {
SOURCES += \
    $$PWD/ddesktopservices_win.cpp \
    $$PWD/dtrashmanager_win.cpp
}

packagesExist(gtk+-2.0) {
    DEFINES += USE_GTK_PLUS_2_0
    INCLUDE_PATH = $$system(pkg-config --cflags-only-I gtk+-2.0)
    INCLUDEPATH += $$replace(INCLUDE_PATH, -I, )
}

includes.files += $$PWD/*.h \
            $$PWD/DWidgetUtil \
            $$PWD/DDesktopServices \
            $$PWD/DTrashManager \
            $$PWD/DThumbnailProvider \
            $$PWD/DFileIconProvider \
            $$PWD/DWindowManagerHelper

DISTFILES += \
    $$PWD/desktop.pri
