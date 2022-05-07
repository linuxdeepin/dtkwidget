#-------------------------------------------------
#
# Project created by QtCreator 2022-05-10T10:54:04
#
#-------------------------------------------------

QT       += widgets core gui printsupport dtkcore dtkgui

TARGET = PrintPreviewSettingsPlugin
TEMPLATE = lib
CONFIG += plugin

DESTDIR = /usr/share/deepin/dtk/plugins/printsupport
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/../../../src
INCLUDEPATH += $$PWD/../../../src/widgets
INCLUDEPATH += $$PWD/../../../src/util
DEPENDPATH += $$PWD/../../../src
unix: LIBS += -L$$OUT_PWD/../../../src -ldtkwidget

SOURCES += \
    settingsplugin.cpp

HEADERS += \
    settingsplugin.h
DISTFILES += PrintPreviewSettingsPlugin.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
