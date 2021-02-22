TEMPLATE = app
CONFIG -= app_bundle
QT += widgets dtkcore dtkgui testlib

unix:QMAKE_RPATHDIR += $$OUT_PWD/../src
unix:LIBS += -L$$OUT_PWD/../src -ldtkwidget -lgtest

QMAKE_CXXFLAGS += -fno-access-control
QMAKE_LFLAGS += -fno-access-control

# 指定moc文件生成目录和src一样
message($$OUT_PWD/../src)
MOC_DIR=$$OUT_PWD/../src

include($$PWD/src/src.pri)

SOURCES += \
    $$PWD/main.cpp

load(dtk_testcase)
