include(../src/d_version.pri)

TEMPLATE = app
CONFIG -= app_bundle

# 如果不需要编译打印预览的单元测试 可以打开这个宏
#DEFINES += DTK_NO_PRINTPREVIEWTEST

QT += widgets dtkcore$$D_VERION dtkgui$$D_VERION testlib

unix:QMAKE_RPATHDIR += $$OUT_PWD/../src
unix:LIBS += -lgtest -lglib-2.0

QMAKE_CXXFLAGS += -fno-access-control
QMAKE_LFLAGS += -fno-access-control

CONFIG(debug, debug|release) {
LIBS += -lgtest
QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -fsanitize=address -fsanitize-recover=address -O2
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -fsanitize=address -fsanitize-recover=address -O2
QMAKE_CXX += -g -fprofile-arcs -ftest-coverage -fsanitize=address -fsanitize-recover=address -O2
}

# 指定moc文件生成目录和src一样
MOC_DIR=$$OUT_PWD/../src

include($$PWD/src.pri)
include($$PWD/testcases/testcases.pri)

SOURCES += \
    $$PWD/main.cpp

load(dtk_testcase)
