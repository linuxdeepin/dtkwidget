CONFIG += c++11 link_pkgconfig
PKGCONFIG += xext x11

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/dkeyboardmonitor.h

SOURCES += \
    $$PWD/dkeyboardmonitor.cpp
