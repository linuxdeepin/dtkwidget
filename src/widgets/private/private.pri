linux{
    include(keyboardmonitor/keyboardmonitor.pri)
    include(startupnotifications/startupnotifications.pri)
    HEADERS += \
        $$PWD/mpris/dmprismonitor.h \
        $$PWD/mpris/dbusinterface.h \
        $$PWD/dmpriscontrol_p.h \
        $$PWD/mpris/dbusmpris.h

    SOURCES += \
        $$PWD/mpris/dmprismonitor.cpp \
        $$PWD/mpris/dbusinterface.cpp \
        $$PWD/mpris/dbusmpris.cpp
}

HEADERS += \
    $$PWD/dthemehelper.h \
    $$PWD/dcircleprogress_p.h \
    $$PWD/dlineedit_p.h \
    $$PWD/dfilechooseredit_p.h \
    $$PWD/dstackwidget_p.h \
    $$PWD/dboxwidget_p.h \
    $$PWD/dpasswordedit_p.h \
    $$PWD/dabstractdialogprivate_p.h \
    $$PWD/ddialog_p.h \
    $$PWD/dloadingindicator_p.h \
    $$PWD/dinputdialog_p.h \
    $$PWD/dipv4lineedit_p.h \
    $$PWD/dspinbox_p.h \
    $$PWD/dpicturesequenceview_p.h \
    $$PWD/dflowlayout_p.h \
    $$PWD/dlistview_p.h \
    $$PWD/dapplication_p.h \
    $$PWD/dmainwindow_p.h \
    $$PWD/dblureffectwidget_p.h \
    $$PWD/dpageindicator_p.h \
    $$PWD/daboutdialog_p.h \
    $$PWD/darrowrectangle_p.h \
    $$PWD/dtickeffect_p.h \
    $$PWD/dswitchbutton_p.h \
    $$PWD/dimagebutton_p.h \
    $$PWD/diconbutton_p.h \
    $$PWD/dsearchedit_p.h \
    $$PWD/dfloatingwidget_p.h \
    $$PWD/dfloatingmessage_p.h \
    $$PWD/dbuttonbox_p.h \
    $$PWD/dslider_p.h \
    $$PWD/dtiplabel_p.h \
    $$PWD/dkeysequenceedit_p.h \
    $$PWD/dlabel_p.h \
    $$PWD/dframe_p.h \
    $$PWD/ddrawer_p.h \
    $$PWD/dalertcontrol_p.h \
    $$PWD/dsearchcombobox_p.h \
    $$PWD/dprintpreviewdialog_p.h \
    $$PWD/dprintpreviewwidget_p.h

SOURCES += \
    $$PWD/dthemehelper.cpp
