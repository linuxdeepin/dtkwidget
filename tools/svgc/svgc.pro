QT += gui svg
QT += dtkcore

TARGET = dtk-svgc
CONFIG += c++11
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += QT_MESSAGELOGCONTEXT

!isEmpty(DTK_STATIC_LIB){
    DEFINES += DTK_STATIC_LIB
}

SOURCES += main.cpp

DTK_MODULE_NAME=dtkwidget
load(dtk_build_config)
target.path = $$TOOL_INSTALL_DIR

INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/release/ -ldtkwidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/debug/ -ldtkwidget
else:unix: LIBS += -L$$OUT_PWD/../../src/ -ldtkwidget

INCLUDEPATH += $$PWD/../../src
INCLUDEPATH += $$PWD/../../src/util
DEPENDPATH += $$PWD/../../src
