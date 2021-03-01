QT += printsupport printsupport-private concurrent

INCLUDEPATH += $$PWD/../../../src/
INCLUDEPATH += $$PWD/../../../src/widgets/
INCLUDEPATH += $$PWD/../../../src/widgets/dialogs
INCLUDEPATH += $$PWD/../../../src/widgets/private
INCLUDEPATH += $$OUT_PWD/../../src/

HEADERS += \
    $$PWD/../../../src/widgets/dprintpreviewdialog.h \
    $$PWD/../../../src/widgets/dprintpreviewwidget.h \
    $$PWD/../../../src/widgets/dprintpickcolorwidget.h \
    $$PWD/../../../src/widgets/private/dprintpreviewdialog_p.h \
    $$PWD/../../../src/widgets/private/dprintpreviewwidget_p.h \

SOURCES += \
    $$PWD/ut_dprintpreviewwidget.cpp \
    $$PWD/ut_dprintpreviewdialog.cpp \
    $$PWD/ut_dprintpickcolorwidget.cpp \
    $$PWD/../../../src/widgets/dprintpreviewdialog.cpp \
    $$PWD/../../../src/widgets/dprintpreviewwidget.cpp \
    $$PWD/../../../src/widgets/dprintpickcolorwidget.cpp

RESOURCES += \
    $$PWD/res.qrc
