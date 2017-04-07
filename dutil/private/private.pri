HEADERS += \
    $$PWD/dbasefilewatcher_p.h

linux {
    HEADERS += \
        $$PWD/dfilesystemwatcher_linux_p.h
} else:win* {
    HEADERS += \
        $$PWD/dfilesystemwatcher_win_p.h
}
