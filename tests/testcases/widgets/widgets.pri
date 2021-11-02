INCLUDEPATH += $$PWD/../../../src/
INCLUDEPATH += $$PWD/../../../src/widgets/
INCLUDEPATH += $$OUT_PWD/../src/

!contains(QMAKE_HOST.arch, mips.*) {
SOURCES += \
    $$PWD/ut_daboutdialog.cpp \
    $$PWD/ut_dabstractdialog.cpp \
    $$PWD/ut_dalertcontrol.cpp \
    $$PWD/ut_dcrumbedit.cpp \
    $$PWD/ut_danchors.cpp \
#    $$PWD/ut_dapplication.cpp \
    $$PWD/ut_darrowbutton.cpp \
    $$PWD/ut_darrowrectangle.cpp \
    $$PWD/ut_dbackgroundgroup.cpp \
    $$PWD/ut_dbaseline.cpp \
    $$PWD/ut_dboxwidget.cpp \
#    $$PWD/ut_dblureffectwidget.cpp \
    $$PWD/ut_dbuttonbox.cpp \
    $$PWD/ut_dcircleprogress.cpp \
    $$PWD/ut_dclipeffectwidget.cpp \
    $$PWD/ut_dcoloredprogressbar.cpp \
    $$PWD/ut_dcommandlinkbutton.cpp \
    $$PWD/ut_ddialog.cpp \
    $$PWD/ut_ddialogclosebutton.cpp \
    $$PWD/ut_ddrawer.cpp \
    $$PWD/ut_ddrawergroup.cpp \
    $$PWD/ut_denhancedwidget.cpp \
    $$PWD/ut_dfilechooseredit.cpp \
    $$PWD/ut_dfiledialog.cpp \
    $$PWD/ut_dfloatingbutton.cpp \
    $$PWD/ut_dfloatingwidget.cpp \
    $$PWD/ut_dflowlayout.cpp \
    $$PWD/ut_dframe.cpp \
    $$PWD/ut_dgraphicsclipeffect.cpp \
    $$PWD/ut_dgraphicsgloweffect.cpp \
    $$PWD/ut_dheaderline.cpp \
    $$PWD/ut_diconbutton.cpp \
    $$PWD/ut_dtoolbutton.cpp \
    $$PWD/ut_dprogressbar.cpp \
    $$PWD/ut_dwaterprogress.cpp \
    $$PWD/ut_danchor.cpp \
    $$PWD/ut_dinputdialog.cpp \
    $$PWD/ut_dlabel.cpp \
    $$PWD/ut_dlineedit.cpp \
    $$PWD/ut_dlistview.cpp \
    $$PWD/ut_dloadingindicator.cpp \
    $$PWD/ut_dmainwindow.cpp \
    $$PWD/ut_dfloatingmessage.cpp \
    $$PWD/ut_dswitchbutton.cpp \
    $$PWD/ut_dwarningbutton.cpp \
    $$PWD/ut_dmessagemanager.cpp \
    $$PWD/ut_dmpriscontrol.cpp \
    $$PWD/ut_dpasswordedit.cpp \
    $$PWD/ut_dpicturesequenceview.cpp \
    $$PWD/ut_dsearchcombobox.cpp \
    $$PWD/ut_dsearchedit.cpp \
    $$PWD/ut_dsettingsdialog.cpp \
    $$PWD/ut_dsettingswidgetfactory.cpp \
    $$PWD/ut_dsimplelistview.cpp \
    $$PWD/ut_dkeysequenceedit.cpp \
    $$PWD/ut_dslider.cpp \
    $$PWD/ut_dwindowmaxbutton.cpp \
    $$PWD/ut_dipv4lineedit.cpp \
    $$PWD/ut_darrowlinedrawer.cpp \
    $$PWD/ut_darrowlineexpand.cpp \
    $$PWD/ut_dspinbox.cpp \
    $$PWD/ut_dspinner.cpp \
    $$PWD/ut_dshaowline.cpp \
    $$PWD/ut_dstackwidget.cpp \
#    $$PWD/ut_dstyle.cpp \
    $$PWD/ut_dstyleditemdelegate.cpp \
    $$PWD/ut_dstyleoption.cpp \
    $$PWD/ut_dsuggestbutton.cpp \
    $$PWD/ut_dtabbar.cpp \
#    $$PWD/ut_dthememanager.cpp \
    $$PWD/ut_dtickeffect.cpp \
    $$PWD/ut_dtooltip.cpp \
#    $$PWD/ut_dvideowidget.cpp \
    $$PWD/ut_dwindowclosebutton.cpp \
    $$PWD/ut_dwindowminbutton.cpp \
    $$PWD/ut_dwindowoptionbutton.cpp \
    $$PWD/ut_dwindowquitfullbutton.cpp \
    $$PWD/ut_dtextedit.cpp \
    $$PWD/ut_dpushbutton.cpp \
    $$PWD/ut_dtitlebar.cpp \
    $$PWD/ut_dpageindicator.cpp \
    $$PWD/ut_dtiplabel.cpp
}
