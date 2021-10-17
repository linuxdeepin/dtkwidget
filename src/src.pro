include(d_version.pri)

TARGET = dtkwidget$$D_VERSION
TEMPLATE = lib
QT += dtkcore$$D_VERSION

CONFIG += internal_module
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

# 龙芯架构上没有默认添加PT_GNU_STACK-section,所以此处手动指定一下
contains(QMAKE_HOST.arch, mips.*): QMAKE_LFLAGS_SHLIB += "-Wl,-z,noexecstack"

QT += network concurrent dtkgui$$D_VERSION printsupport printsupport-private
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets widgets-private
  # Qt >= 5.8
  greaterThan(QT_MAJOR_VERSION, 5)|greaterThan(QT_MINOR_VERSION, 7): QT += gui-private
  else: QT += platformsupport-private
}

linux* {
    QT += x11extras dbus
    #LIBS += -lcups
    ###(zccrs): use load(dtk_qmake), dtkcore5.5 > 2.0.9
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

isEmpty(DTK_NO_MULTIMEDIA){
    DEFINES += DTK_NO_MULTIMEDIA
#    QT -= multimedia
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

load(dtk_multiversion)
dtkBuildMultiVersion(5.5)
load(dtk_install_multiversion)
