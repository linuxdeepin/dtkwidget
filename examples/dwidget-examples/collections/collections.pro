QT += core gui multimedia multimediawidgets svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += dtkcore
linux* {
    CONFIG += link_pkgconfig
    PKGCONFIG += dframeworkdbus
}

TARGET = collections
TEMPLATE = app
CONFIG += c++11

DEFINES += QT_MESSAGELOGCONTEXT

unix {
    QT += dbus
}

!isEmpty(DTK_NO_MULTIMEDIA){
    DEFINES += DTK_NO_MULTIMEDIA
    QT -= multimedia multimediawidgets
} else {
    HEADERS  += \
        cameraform.h
    SOURCES += \
        cameraform.cpp
    FORMS += \
        cameraform.ui
}

!isEmpty(DTK_STATIC_LIB){
    DEFINES += DTK_STATIC_LIB
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
    simplelistviewtab.cpp \
    singlelistview.cpp \
    singlelistitem.cpp \
    multilistview.cpp \
    multilistitem.cpp \
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
    simplelistviewtab.h \
    singlelistview.h \
    singlelistitem.h \
    multilistview.h \
    multilistitem.h \
    graphicseffecttab.h

RESOURCES += \
    images.qrc \
    resources.qrc


win32* {
    CONFIG += no_lflags_merge
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../src/release -ldtkwidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../src/debug -ldtkwidgetd
else:unix: LIBS += -L$$OUT_PWD/../../../src -ldtkwidget

INCLUDEPATH += $$PWD/../../../src
INCLUDEPATH += $$PWD/../../../src/widgets
INCLUDEPATH += $$PWD/../../../src/util
DEPENDPATH += $$PWD/../../../src

CONFIG(debug, debug|release) {
    unix:QMAKE_RPATHDIR += $$OUT_PWD/../../../src
}
