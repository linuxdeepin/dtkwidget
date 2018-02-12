isEmpty(VERSION) {
    PKG_CONFIG_EXE = $$first($$list($$pkgConfigExecutable()))
    isEmpty(LIB_INSTALL_DIR) {
        dtk_version_module = $$system($$PKG_CONFIG_EXE --variable=libdir dtkcore)
        dtk_version_module = $$dtk_version_module/libdtk/modules/version.pri
        exists($$dtk_version_module): include($$dtk_version_module)
        else: VERSION = $$system($$PKG_CONFIG_EXE --modversion dtkcore)
    } else {
        dtk_version_module = $$LIB_INSTALL_DIR/libdtk/modules/version.pri
        exists($$dtk_version_module): include($$dtk_version_module)
        else: VERSION = $$system($$PKG_CONFIG_EXE --modversion dtkcore)
    }

    isEmpty(VERSION): error(VERSION is empty)
}
