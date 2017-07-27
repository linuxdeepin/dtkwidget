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
    #DEPENDS dtkcore
    INCLUDEPATH += $$INCLUDE_INSTALL_DIR\libdtk-1.2\DCore
    LIBS += -L$$LIB_INSTALL_DIR -ldtkcore
}

HEADERS += dtkwidget_global.h

includes.path = $${DTK_INCLUDEPATH}/DWidget
includes.files += $$PWD/dtkwidget_global.h

include($$PWD/util/util.pri)
include($$PWD/widgets/widgets.pri)

linux{
    includes.files += $$PWD/platforms/linux/*.h
}
win32* {
    includes.files += $$PWD/platforms/windows/*.h
}

QMAKE_PKGCONFIG_NAME = DTK_WIDGET
QMAKE_PKGCONFIG_DESCRIPTION = Deepin Tool Kit Widget Module
QMAKE_PKGCONFIG_INCDIR = $$includes.path
QMAKE_PKGCONFIG_REQUIRES += dtkcore


