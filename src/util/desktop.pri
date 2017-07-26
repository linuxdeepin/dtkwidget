HEADERS += \
    $$PWD/ddesktopservices.h \
    $$PWD/dtrashmanager.h

linux {
    qtHaveModule(dbus) {
        QT += dbus
        SOURCES += $$PWD/ddesktopservices_linux.cpp
    }

    CONFIG += link_pkgconfig
    PKGCONFIG += gsettings-qt

    SOURCES += \
        $$PWD/dtrashmanager_linux.cpp
} else:win* {
    SOURCES += \
        $$PWD/ddesktopservices_win.cpp \
        $$PWD/dtrashmanager_win.cpp
} else:mac* {
    SOURCES += \
        $$PWD/ddesktopservices_win.cpp \
        $$PWD/dtrashmanager_win.cpp
}

includes.files += $$PWD/*.h
includes.files += $$PWD/*.cpp
includes.files += \
    $$PWD/DDesktopServices \
    $$PWD/DTrashManager
