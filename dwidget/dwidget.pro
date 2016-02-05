include($$PWD/../common/lib.pri)
include(widgets/widgets.pri)

TARGET = dtkwidget

DEFINES += LIBDTKWIDGET_LIBRARY

QT += multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix{
    QT += x11extras dbus
}

HEADERS += libdui_global.h
includes.path = $${DTK_INCLUDEPATH}/DWidget
includes.files += libdui_global.h widgets/*.h

QMAKE_PKGCONFIG_NAME = DTK_WIDGET
QMAKE_PKGCONFIG_DESCRIPTION = Deepin Tool Kit UI Module
QMAKE_PKGCONFIG_INCDIR = $$includes.path
