include($$PWD/../common/lib.pri)
include(widgets/widgets.pri)
include(util/util.pri)

TARGET = dtkwidget

DEFINES += LIBDTKWIDGET_LIBRARY

QT += multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
  # Qt >= 5.8
  greaterThan(QT_MAJOR_VERSION, 5)|greaterThan(QT_MINOR_VERSION, 7): QT += gui-private
  else: QT += platformsupport-private
}

unix{
    QT += x11extras dbus
    CONFIG += link_pkgconfig
    PKGCONFIG += x11 xext dtksettings dtksettingsview


    !system($$PWD/../tool/translate_generation.sh): error("Failed to generate translation")
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

HEADERS += dwidget_global.h \
    dutility.h

includes.path = $${DTK_INCLUDEPATH}/DWidget
includes.files += dwidget_global.h \
            widgets/*.h \
            util/*.h \
            dutility.h
unix{
    includes.files += $$PWD/platforms/linux/*.h
}
win32* {
    includes.files += $$PWD/platforms/windows/*.h
}
includes.files += \
    widgets/DTitlebar \
    widgets/DWindow \
    widgets/DMainWindow \
    widgets/DAboutDialog \
    widgets/DApplication \
    widgets/DBlurEffectWidget \
    widgets/DClipEffectWidget \
    widgets/DGraphicsDropShadowEffect \
    widgets/DPlatformWindowHandle \
    widgets/DGraphicsClipEffect \
    widgets/DVideoWidget \
    util/DThumbnailProvider \
    util/DFileIconProvider

QMAKE_PKGCONFIG_NAME = DTK_WIDGET
QMAKE_PKGCONFIG_DESCRIPTION = Deepin Tool Kit UI Module
QMAKE_PKGCONFIG_INCDIR = $$includes.path

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dbase/release/ -ldtkbase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dbase/debug/ -ldtkbase
else:unix: LIBS += -L$$OUT_PWD/../dbase/ -ldtkbase

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dutil/release/ -ldtkutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dutil/debug/ -ldtkutil
else:unix: LIBS += -L$$OUT_PWD/../dutil/ -ldtkutil

INCLUDEPATH += $$PWD/../dbase
DEPENDPATH += $$PWD/../dbase
INCLUDEPATH += $$PWD/../dutil
DEPENDPATH += $$PWD/../dutil

SOURCES += \
    dutility.cpp

TRANSLATIONS += $$PWD/translations/$${TARGET}.ts \
                $$PWD/translations/$${TARGET}_zh_CN.ts

translations.path = $$PREFIX/share/$${TARGET}/translations
translations.files = $$PWD/translations/*.qm

INSTALLS += translations
