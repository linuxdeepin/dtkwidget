include($$PWD/lib.pri)

TEMPLATE = lib
CONFIG += qt
TARGET = dtkwidget

DEFINES += LIBDTKWIDGET_LIBRARY

QT += multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
  # Qt >= 5.8
  greaterThan(QT_MAJOR_VERSION, 5)|greaterThan(QT_MINOR_VERSION, 7): QT += gui-private
  else: QT += platformsupport-private
}

macx{
    CONFIG += link_pkgconfig
    PKGCONFIG += dtkcore
}

linux* {
    QT += x11extras dbus
    CONFIG += link_pkgconfig
    PKGCONFIG += x11 xext dtkcore
}

win32* {
    #DEPENDS dtksettings
    INCLUDEPATH += $$INCLUDE_INSTALL_DIR\libdtk-1.0\DSettings
    DEPENDPATH += $$INCLUDE_INSTALL_DIR\libdtk-1.0\DSettings
    LIBS += -L$$LIB_INSTALL_DIR -ldtksettings

    #DEPENDS dtksettingsview
    INCLUDEPATH += $$INCLUDE_INSTALL_DIR\libdtk-1.0\DSettingsView
    DEPENDPATH += $$INCLUDE_INSTALL_DIR\libdtk-1.0\DSettingsView
    LIBS += -L$$LIB_INSTALL_DIR -ldtksettingsview
}

HEADERS += \
    dutility.h \
    dtkwidget_global.h

SOURCES += \
    dutility.cpp

includes.path = $${DTK_INCLUDEPATH}/DWidget
includes.files += $$PWD/dwidget_global.h \
            $$PWD/dutility.h\
            $$PWD/util/*.h \
            $$PWD/util/DThumbnailProvider \
            $$PWD/util/DFileIconProvider \
            $$PWD/util/DWindowManagerHelper

include($$PWD/util/util.pri)
include($$PWD/widgets/widgets.pri)

linux{
    includes.files += $$PWD/platforms/linux/*.h
}
win32* {
    includes.files += $$PWD/platforms/windows/*.h
}

QMAKE_PKGCONFIG_NAME = DTK_WIDGET
QMAKE_PKGCONFIG_DESCRIPTION = Deepin Tool Kit UI Module
QMAKE_PKGCONFIG_INCDIR = $$includes.path
QMAKE_PKGCONFIG_REQUIRES += dtkbase dtkutil


