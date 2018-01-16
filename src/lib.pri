include($$PWD/config.pri)

CONFIG += c++11 create_pc create_prl no_install_prl

isEmpty(LIB_INSTALL_DIR) {
    target.path = $$PREFIX/lib
} else {
    target.path = $$LIB_INSTALL_DIR
}

QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_VERSION = $$VERSION
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

isEmpty(INCLUDE_INSTALL_DIR) {
    DTK_INCLUDEPATH = $$PREFIX/include/libdtk-$$VERSION
} else {
    DTK_INCLUDEPATH = $$INCLUDE_INSTALL_DIR/libdtk-$$VERSION
}

load(configure)
qtCompileTest(libdframeworkdbus) {
    DEFINES += DBUS_VERSION_0_4_2
}

INSTALLS += includes target

win32* {
    CONFIG += staticlib
}
