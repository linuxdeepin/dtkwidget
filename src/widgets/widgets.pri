include(private/private.pri)
include(dialogs.pri)
include($$PWD/../platforms/platforms.pri)

win32* {
    INCLUDEPATH += $$PWD/../platforms/windows
}

linux{
    HEADERS += \
        $$PWD/dmpriscontrol.h \
        $$PWD/dpasswdeditanimated.h

    SOURCES += \
        $$PWD/dmpriscontrol.cpp \
        $$PWD/dpasswdeditanimated.cpp

    includes.files += \
        $$PWD/DPasswdEditAnimated
}

mac{
    HEADERS  +=\
        $$PWD/../platforms/mac/osxwindow.h

    OBJECTIVE_SOURCES += \
        $$PWD/../platforms/mac/osxwindow.mm

    INCLUDEPATH += $$PWD/../platforms/mac

    LIBS += -framework Foundation -framework Cocoa
}

isEmpty(DTK_NO_MULTIMEDIA){
    HEADERS += \
        $$PWD/dvideowidget.h

    SOURCES += \
        $$PWD/dvideowidget.cpp

    includes.files += \
        $$PWD/DVideoWidget
}

HEADERS += $$PWD/dslider.h\
    $$PWD/dbackgroundgroup.h \
    $$PWD/dthememanager.h \
    $$PWD/dapplication.h \
    $$PWD/dconstants.h \
    $$PWD/dseparatorhorizontal.h \
    $$PWD/dseparatorvertical.h \
    $$PWD/dbaseline.h \
    $$PWD/dheaderline.h \
    $$PWD/dbaseexpand.h \
    $$PWD/darrowbutton.h \
    $$PWD/darrowlineexpand.h \
    $$PWD/dswitchlineexpand.h \
    $$PWD/dbuttonlist.h \
    $$PWD/dimagebutton.h \
    $$PWD/dtextbutton.h \
    $$PWD/dloadingindicator.h \
    $$PWD/dsearchedit.h \
    $$PWD/dswitchbutton.h \
    $$PWD/dsegmentedcontrol.h \
    $$PWD/dlineedit.h \
    $$PWD/dwindowmaxbutton.h \
    $$PWD/dwindowminbutton.h \
    $$PWD/dwindowclosebutton.h \
    $$PWD/dwindowoptionbutton.h \
    $$PWD/dshortcutedit.h \
    $$PWD/dsimplelistview.h \
    $$PWD/dsimplelistitem.h \
    $$PWD/dexpandgroup.h \
    $$PWD/dlabel.h \
    $$PWD/dlinkbutton.h \
    $$PWD/dscrollbar.h \
    $$PWD/dscrollarea.h \
    $$PWD/darrowrectangle.h \
    $$PWD/dgraphicsgloweffect.h \
    $$PWD/dbasebutton.h \
    $$PWD/dlistwidget.h \
    $$PWD/dboxwidget.h \
    $$PWD/dcircleprogress.h \
    $$PWD/dstackwidget.h \
    $$PWD/dfilechooseredit.h \
    $$PWD/dpasswordedit.h \
    $$PWD/dipv4lineedit.h \
    $$PWD/dspinbox.h \
    $$PWD/doptionlist.h \
    $$PWD/doption.h \
    $$PWD/dpicturesequenceview.h \
    $$PWD/dflowlayout.h \
    $$PWD/dlistview.h \
    $$PWD/denhancedwidget.h \
    $$PWD/dtextedit.h \
    $$PWD/dtitlebar.h \
    $$PWD/dplatformwindowhandle.h \
    $$PWD/dmainwindow.h \
    $$PWD/dblureffectwidget.h \
    $$PWD/dpageindicator.h \
    $$PWD/dclipeffectwidget.h \
    $$PWD/dgraphicsclipeffect.h \
    $$PWD/dforeignwindow.h \
    $$PWD/dpushbutton.h \
    $$PWD/dtickeffect.h \
    $$PWD/dwaterprogress.h \
    $$PWD/dsettingswidgetfactory.h \
    $$PWD/dspinner.h \
    $$PWD/dcrumbedit.h \
    $$PWD/dtabbar.h \
    $$PWD/dsuggestbutton.h \
    $$PWD/dstyleoption.h \
    $$PWD/dtoast.h \
    $$PWD/danchors.h \
    $$PWD/dwindowgroupleader.h \
    $$PWD/dborderlesswidget.h \
    $$PWD/dborderlesswindow.h \
    $$PWD/dstyle.h \
    $$PWD/dfloatingbutton.h

SOURCES += $$PWD/dslider.cpp \
    $$PWD/dbackgroundgroup.cpp \
    $$PWD/dthememanager.cpp \
    $$PWD/dapplication.cpp \
    $$PWD/dseparatorhorizontal.cpp \
    $$PWD/dseparatorvertical.cpp \
    $$PWD/dbaseline.cpp \
    $$PWD/dheaderline.cpp \
    $$PWD/dbaseexpand.cpp \
    $$PWD/darrowbutton.cpp \
    $$PWD/darrowlineexpand.cpp \
    $$PWD/dswitchlineexpand.cpp \
    $$PWD/dbuttonlist.cpp \
    $$PWD/dimagebutton.cpp \
    $$PWD/dtextbutton.cpp \
    $$PWD/dloadingindicator.cpp \
    $$PWD/dsearchedit.cpp \
    $$PWD/dswitchbutton.cpp\
    $$PWD/dsegmentedcontrol.cpp \
    $$PWD/dlineedit.cpp \
    $$PWD/dwindowmaxbutton.cpp \
    $$PWD/dwindowminbutton.cpp \
    $$PWD/dwindowclosebutton.cpp \
    $$PWD/dwindowoptionbutton.cpp \
    $$PWD/dshortcutedit.cpp \
    $$PWD/dsimplelistview.cpp \
    $$PWD/dsimplelistitem.cpp \
    $$PWD/dexpandgroup.cpp \
    $$PWD/dlabel.cpp \
    $$PWD/dlinkbutton.cpp \
    $$PWD/dscrollbar.cpp \
    $$PWD/dscrollarea.cpp \
    $$PWD/darrowrectangle.cpp \
    $$PWD/dgraphicsgloweffect.cpp \
    $$PWD/dbasebutton.cpp \
    $$PWD/dlistwidget.cpp \
    $$PWD/dboxwidget.cpp \
    $$PWD/dcircleprogress.cpp \
    $$PWD/dstackwidget.cpp \
    $$PWD/dfilechooseredit.cpp \
    $$PWD/dpasswordedit.cpp \
    $$PWD/dipv4lineedit.cpp \
    $$PWD/dspinbox.cpp \
    $$PWD/doptionlist.cpp \
    $$PWD/doption.cpp \
    $$PWD/dpicturesequenceview.cpp \
    $$PWD/dflowlayout.cpp \
    $$PWD/dlistview.cpp \
    $$PWD/denhancedwidget.cpp \
    $$PWD/dtextedit.cpp \
    $$PWD/dtitlebar.cpp \
    $$PWD/dplatformwindowhandle.cpp \
    $$PWD/dmainwindow.cpp \
    $$PWD/dblureffectwidget.cpp \
    $$PWD/dpageindicator.cpp \
    $$PWD/dclipeffectwidget.cpp \
    $$PWD/dgraphicsclipeffect.cpp \
    $$PWD/dforeignwindow.cpp \
    $$PWD/dpushbutton.cpp \
    $$PWD/dtickeffect.cpp \
    $$PWD/dwaterprogress.cpp \
    $$PWD/dsettingswidgetfactory.cpp \
    $$PWD/dspinner.cpp \
    $$PWD/dcrumbedit.cpp \
    $$PWD/dtabbar.cpp \
    $$PWD/dsuggestbutton.cpp \
    $$PWD/dstyleoption.cpp \
    $$PWD/dtoast.cpp \
    $$PWD/danchors.cpp \
    $$PWD/dwindowgroupleader.cpp \
    $$PWD/dborderlesswidget.cpp \
    $$PWD/dborderlesswindow.cpp \
    $$PWD/dstyle.cpp \
    $$PWD/dfloatingbutton.cpp

RESOURCES += \
    $$PWD/themes/dui_theme_dark.qrc \
    $$PWD/themes/dui_theme_light.qrc \
    $$PWD/icons.qrc

INCLUDEPATH += $$PWD

includes.files += $$PWD/*.h
includes.files += \
    $$PWD/DTitlebar \
    $$PWD/DWindow \
    $$PWD/DMainWindow \
    $$PWD/DAboutDialog \
    $$PWD/DApplication \
    $$PWD/DBlurEffectWidget \
    $$PWD/DClipEffectWidget \
    $$PWD/DGraphicsDropShadowEffect \
    $$PWD/DPlatformWindowHandle \
    $$PWD/DGraphicsClipEffect \
    $$PWD/DForeignWindow \
    $$PWD/DExpandGroup \
    $$PWD/DArrowButton \
    $$PWD/DArrowLineExpand \
    $$PWD/DThemeManager \
    $$PWD/DWaterProgress \
    $$PWD/DSimpleListView \
    $$PWD/DSimpleListItem \
    $$PWD/DSearchEdit \
    $$PWD/DPageIndicator \
    $$PWD/DSettingsWidgetFactory \
    $$PWD/DSettingsDialog \
    $$PWD/DSpinner \
    $$PWD/DCrumbEdit \
    $$PWD/DTabBar \
    $$PWD/DSuggestButton \
    $$PWD/DStyleOption \
    $$PWD/DToast \
    $$PWD/DFileDialog \
    $$PWD/DLineEdit \
    $$PWD/DStyleOptionLineEdit \
    $$PWD/DWindowGroupLeader \
    $$PWD/DAnchors \
    $$PWD/DSegmentedControl \
    $$PWD/DSegmentedHighlight \
    $$PWD/DBackgroundGroup \
    $$PWD/DStyleOptionBackgroundGroup \
    $$PWD/DStyleOptionButton \
    $$PWD/DPalette \
    $$PWD/DFontSizeManager \
    $$PWD/DStyle \
    $$PWD/DFloatingButton \
    $$PWD/DListView \
    $$PWD/DStyleOptionViewItem

