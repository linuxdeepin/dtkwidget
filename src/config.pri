CONFIG += c++11
DEFINES += QT_MESSAGELOGCONTEXT

isEmpty(PREFIX){
    PREFIX = /usr
}

win32* {
    DEFINES += STATIC_LIB
}

isEmpty(VERSION) {
    PKG_CONFIG = $$pkgConfigExecutable()

    isEmpty(LIB_INSTALL_DIR) {
        dtk_version_module = $$system($$PKG_CONFIG --variable=libdir dtkcore)
        dtk_version_module = $$dtk_version_module/libdtk/modules/version.pri
        exists($$dtk_version_module): include($$dtk_version_module)
        else: VERSION = $$system($$PKG_CONFIG --modversion dtkcore)
    } else {
        dtk_version_module = $$LIB_INSTALL_DIR/libdtk/modules/version.pri
        exists($$dtk_version_module): include($$dtk_version_module)
        else: VERSION = $$system($$PKG_CONFIG --modversion dtkcore)
    }

    isEmpty(VERSION): error(VERSION is empty)
}
