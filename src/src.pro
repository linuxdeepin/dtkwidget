TARGET = dtkwidget
TEMPLATE = lib
QT += dtkcore

CONFIG += internal_module

QT += network concurrent multimedia multimediawidgets dtkgui
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets widgets-private
  # Qt >= 5.8
  greaterThan(QT_MAJOR_VERSION, 5)|greaterThan(QT_MINOR_VERSION, 7): QT += gui-private
  else: QT += platformsupport-private
}

linux* {
    QT += x11extras dbus

    ###(zccrs): use load(dtk_qmake), dtkcore > 2.0.9
    ARCH = $$QMAKE_HOST.arch
    isEqual(ARCH, sw_64) | isEqual(ARCH, mips64) | isEqual(ARCH, mips32) {
        DEFINES += FORCE_RASTER_WIDGETS
    }

    isEmpty(DTK_NO_AI_SERVICE) {
        DEFINES += ENABLE_AI
        DBUS_INTERFACES += \
            $$PWD/widgets/dbus/com.iflytek.aiservice.iat.xml\
            $$PWD/widgets/dbus/com.iflytek.aiservice.session.xml
    }
}

mac* {
    QT += svg dbus
    DEFINES += DTK_TITLE_DRAG_WINDOW
}

win* {
    QT += svg
    DEFINES += DTK_TITLE_DRAG_WINDOW
}

!isEmpty(DTK_NO_MULTIMEDIA){
    DEFINES += DTK_NO_MULTIMEDIA
    QT -= multimedia multimediawidgets
}

!isEmpty(DTK_STATIC_LIB){
    DEFINES += DTK_STATIC_LIB
    CONFIG += staticlib
}

HEADERS += dtkwidget_global.h

includes.files += \
    $$PWD/dtkwidget_global.h\
    $$PWD/DtkWidgets\
    $$PWD/dtkwidget_config.h

include($$PWD/util/util.pri)
include($$PWD/widgets/widgets.pri)

linux* {
    includes.files += $$PWD/platforms/linux/*.h
}
win32* {
    includes.files += $$PWD/platforms/windows/*.h
}

DTK_MODULE_NAME=$$TARGET
load(dtk_build)

INSTALLS += includes target

load(dtk_cmake)

load(dtk_module)
