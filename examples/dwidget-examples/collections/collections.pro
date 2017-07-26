QT += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = collections
TEMPLATE = app
CONFIG += c++11

DEFINES += QT_MESSAGELOGCONTEXT

unix {
    QT += x11extras dbus
    CONFIG += link_pkgconfig
    PKGCONFIG += dtkcore
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

linux{
    INCLUDEPATH += $$PWD/../../../dwidget/platforms/linux
}
win32* {
    INCLUDEPATH += $$PWD/../../../dwidget/platforms/windows
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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../src/ -ldtkwidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../src/ -ldtkwidgetd
else:unix: LIBS += -L$$OUT_PWD/../../../src/ -ldtkwidget

INCLUDEPATH += $$PWD/../../../src
INCLUDEPATH += $$PWD/../../../src/widgets
DEPENDPATH += $$PWD/../../../src
