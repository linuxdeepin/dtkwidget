
linux {
    include($$PWD/x11/x11.pri)
}

win32* {
    include($$PWD/windows/windows.pri)
}
