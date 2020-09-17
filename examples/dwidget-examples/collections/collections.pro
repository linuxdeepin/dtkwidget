QT += core gui svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += dtkcore dtkgui printsupport-private
linux* {
    CONFIG += link_pkgconfig
}

TARGET = collections
TEMPLATE = app
CONFIG += c++11

DEFINES += QT_MESSAGELOGCONTEXT

unix {
    QT += dbus
}

#!isEmpty(DTK_NO_MULTIMEDIA){
    DEFINES += DTK_NO_MULTIMEDIA
#    QT -= multimedia
#} else {
#    HEADERS  += \
#        cameraform.h
#    SOURCES += \
#        cameraform.cpp
#    FORMS += \
#        cameraform.ui
#}

!isEmpty(DTK_STATIC_LIB){
    DEFINES += DTK_STATIC_LIB
}

SOURCES += main.cpp\
    mainwindow.cpp \
    buttonexample.cpp \
    examplewindowinterface.cpp \
    pagewindowinterface.cpp \
    editexample.cpp \
    sliderexample.cpp \
    listviewexample.cpp \
    windowexample.cpp \
    tooltipexample.cpp \
    spinnerexample.cpp \
    dialogexample.cpp \
    progressbarexample.cpp \
    layoutexample.cpp \
    scrollbarexample.cpp \
    rubberbandexample.cpp \
    widgetexample.cpp \
    lcdnumberexample.cpp \
    menuexample.cpp

HEADERS  += mainwindow.h \
    buttonexample.h \
    examplewindowinterface.h \
    pagewindowinterface.h \
    editexample.h \
    sliderexample.h \
    listviewexample.h \
    windowexample.h \
    tooltipexample.h \
    spinnerexample.h \
    dialogexample.h \
    progressbarexample.h \
    layoutexample.h \
    scrollbarexample.h \
    rubberbandexample.h \
    widgetexample.h \
    lcdnumberexample.h \
    menuexample.h

RESOURCES += \
    images.qrc \
    resources.qrc \
    icons/theme-icons.qrc


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
