LIB_PATH = $$PWD/libs/$$QMAKE_HOST.arch
LIB_FILE = $$LIB_PATH/libmsc.so.0

exists($$LIB_FILE) {
    LIB_FILE_OUT = $$OUT_PWD/libmsc.so
    system(cp $$LIB_FILE $$LIB_FILE_OUT)
    LIBS += -L$$OUT_PWD -lmsc

    INCLUDEPATH += $$PWD/include
    DEFINES += ENABLE_XFYUN

    HEADERS += \
        $$PWD/include/msp_cmn.h \
        $$PWD/include/msp_errors.h \
        $$PWD/include/msp_types.h \
        $$PWD/include/qise.h \
        $$PWD/include/qisr.h \
        $$PWD/include/qtts.h

    #CONFIG(debug, debug|release) {
        unix:QMAKE_RPATHDIR += $$OUT_PWD
    #}

    libs.files += $$LIB_FILE_OUT
    libs.path = $${target.path}

    INSTALLS += libs
} else {
    warning(the "$$LIB_FILE" is not exists)
}
