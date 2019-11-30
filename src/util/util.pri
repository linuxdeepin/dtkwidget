INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/dfileiconprovider.h \
    $$PWD/dthumbnailprovider.h \
    $$PWD/dwidgetutil.h \
    $$PWD/ddesktopservices.h \
    $$PWD/dtrashmanager.h \
    $$PWD/dsvgrenderer.h \
    $$PWD/dhidpihelper.h \
    $$PWD/dapplicationsettings.h

SOURCES += \
    $$PWD/dfileiconprovider.cpp \
    $$PWD/dthumbnailprovider.cpp \
    $$PWD/dwidgetutil.cpp \
    $$PWD/dhidpihelper.cpp \
    $$PWD/dapplicationsettings.cpp

linux* {
CONFIG += link_pkgconfig
PKGCONFIG += gsettings-qt librsvg-2.0 dframeworkdbus

HEADERS += \
    $$PWD/dregionmonitor.h \
    $$PWD/private/dregionmonitor_p.h

SOURCES += \
    $$PWD/ddesktopservices_linux.cpp \
    $$PWD/dtrashmanager_linux.cpp \
    $$PWD/dsvgrenderer.cpp\
    $$PWD/dregionmonitor.cpp
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
            $$PWD/DFileIconProvider \
            $$PWD/DHiDPIHelper

DISTFILES += \
    $$PWD/desktop.pri \
    $$PWD/DHiDPIHelper
