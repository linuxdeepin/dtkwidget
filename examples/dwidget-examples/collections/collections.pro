QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = collections
TEMPLATE = app
CONFIG += c++11

DEFINES += QT_MESSAGELOGCONTEXT

unix{
    QT += x11extras dbus
    CONFIG += link_pkgconfig
    PKGCONFIG += dtksettings dtksettingsview
}

win32* {
    DEFINES += STATIC_LIB

    #DEPENDS dtksettings
    INCLUDEPATH += $$INCLUDE_INSTALL_DIR\libdtk-1.0\DSettings
    DEPENDPATH += $$INCLUDE_INSTALL_DIR\libdtk-1.0\DSettings
    LIBS += -L$$LIB_INSTALL_DIR -ldtksettings

    #DEPENDS dtksettingsview
    INCLUDEPATH += $$INCLUDE_INSTALL_DIR\libdtk-1.0\DSettingsView
    DEPENDPATH += $$INCLUDE_INSTALL_DIR\libdtk-1.0\DSettingsView
    LIBS += -L$$LIB_INSTALL_DIR -ldtksettingsview
}

SOURCES += main.cpp\
    mainwindow.cpp \
    buttontab.cpp \
    linetab.cpp \
    bartab.cpp \
    inputtab.cpp \
    slidertab.cpp \
    buttonlisttab.cpp \
    segmentedcontrol.cpp\
    indicatortab.cpp \
    widgetstab.cpp \
    comboboxtab.cpp \
    buttongridtab.cpp \
    imagebuttongridtab.cpp \
    cameraform.cpp \
    graphicseffecttab.cpp

HEADERS  += mainwindow.h \
    buttontab.h \
    linetab.h \
    bartab.h \
    inputtab.h \
    slidertab.h \
    buttonlisttab.h \
    segmentedcontrol.h \
    indicatortab.h \
    widgetstab.h \
    comboboxtab.h \
    buttongridtab.h \
    imagebuttongridtab.h \
    cameraform.h \
    graphicseffecttab.h

RESOURCES += \
    images.qrc \
    resources.qrc

FORMS += \
    cameraform.ui

unix{
    INCLUDEPATH += $$PWD/../../../dwidget/platforms/linux
}
win32* {
    INCLUDEPATH += $$PWD/../../../dwidget/platforms/windows
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../dbase/release/ -ldtkbase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../dbase/debug/ -ldtkbase
else:unix: LIBS += -L$$OUT_PWD/../../../dbase/ -ldtkbase

INCLUDEPATH += $$PWD/../../../dbase
DEPENDPATH += $$PWD/../../../dbase

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../dwidget/release/ -ldtkwidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../dwidget/debug/ -ldtkwidget
else:unix: LIBS += -L$$OUT_PWD/../../../dwidget/ -ldtkwidget

INCLUDEPATH += $$PWD/../../../dwidget
INCLUDEPATH += $$PWD/../../../dwidget/widgets
INCLUDEPATH += $$PWD/../../../dwidget/util
DEPENDPATH += $$PWD/../../../dwidget

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../dutil/release/ -ldtkutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../dutil/debug/ -ldtkutil
else:unix: LIBS += -L$$OUT_PWD/../../../dutil/ -ldtkutil

INCLUDEPATH += $$PWD/../../../dutil
INCLUDEPATH += $$PWD/../../../dutil/dlog
DEPENDPATH += $$PWD/../../../dutil

unix:QMAKE_RPATHDIR += $$OUT_PWD/../../../dbase/
unix:QMAKE_RPATHDIR += $$OUT_PWD/../../../dutil/
unix:QMAKE_RPATHDIR += $$OUT_PWD/../../../dwidget/

win32* {
    #DEPENDS dtksettings
    INCLUDEPATH += $$INCLUDE_INSTALL_DIR\libdtk-0.1.0\DSettings
    DEPENDPATH += $$INCLUDE_INSTALL_DIR\libdtk-0.1.0\DSettings
    LIBS += -L$$LIB_INSTALL_DIR -ldtksettings

    #DEPENDS dtksettingsview
    INCLUDEPATH += $$INCLUDE_INSTALL_DIR\libdtk-0.1.0\DSettingsView
    DEPENDPATH += $$INCLUDE_INSTALL_DIR\libdtk-0.1.0\DSettingsView
    LIBS += -L$$LIB_INSTALL_DIR -ldtksettingsview
}
