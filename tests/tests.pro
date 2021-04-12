TEMPLATE = app
CONFIG -= app_bundle

# 如果不需要编译打印预览的单元测试 可以打开这个宏
#DEFINES += DTK_NO_PRINTPREVIEWTEST

QT += widgets dtkcore dtkgui testlib

unix:QMAKE_RPATHDIR += $$OUT_PWD/../src
unix:LIBS += -L$$OUT_PWD/../src -ldtkwidget -lgtest -lglib-2.0

QMAKE_CXXFLAGS += -fno-access-control
QMAKE_LFLAGS += -fno-access-control

# 指定moc文件生成目录和src一样
MOC_DIR=$$OUT_PWD/../src

include($$PWD/src.pri)
include($$PWD/testcases/testcases.pri)

SOURCES += \
    $$PWD/main.cpp

load(dtk_testcase)
