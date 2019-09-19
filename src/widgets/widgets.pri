include(private/private.pri)
include(dialogs.pri)
include($$PWD/../platforms/platforms.pri)

win32* {
    INCLUDEPATH += $$PWD/../platforms/windows
}

linux{
    HEADERS += \
        $$PWD/dmpriscontrol.h

    SOURCES += \
        $$PWD/dmpriscontrol.cpp

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
    $$PWD/dbaseline.h \
    $$PWD/dheaderline.h \
    $$PWD/dbaseexpand.h \
    $$PWD/darrowbutton.h \
    $$PWD/darrowlineexpand.h \
    $$PWD/dswitchlineexpand.h \
    $$PWD/dimagebutton.h \
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
    $$PWD/darrowrectangle.h \
    $$PWD/dgraphicsgloweffect.h \
    $$PWD/dboxwidget.h \
    $$PWD/dcircleprogress.h \
    $$PWD/dstackwidget.h \
    $$PWD/dfilechooseredit.h \
    $$PWD/dpasswordedit.h \
    $$PWD/dipv4lineedit.h \
    $$PWD/dspinbox.h \
    $$PWD/dpicturesequenceview.h \
    $$PWD/dflowlayout.h \
    $$PWD/dlistview.h \
    $$PWD/denhancedwidget.h \
    $$PWD/dtitlebar.h \
    $$PWD/dplatformwindowhandle.h \
    $$PWD/dmainwindow.h \
    $$PWD/dblureffectwidget.h \
    $$PWD/dpageindicator.h \
    $$PWD/dclipeffectwidget.h \
    $$PWD/dgraphicsclipeffect.h \
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
    $$PWD/dstyle.h \
    $$PWD/dfloatingbutton.h \
    $$PWD/dwidgetstype.h \
    $$PWD/dstyleditemdelegate.h \
    $$PWD/diconbutton.h \
    $$PWD/dfloatingwidget.h \
    $$PWD/dapplicationhelper.h \
    $$PWD/dfloatingmessage.h \
    $$PWD/dmessagemanager.h \
    $$PWD/dbuttonbox.h \
    $$PWD/dwarningbutton.h

SOURCES += $$PWD/dslider.cpp \
    $$PWD/dbackgroundgroup.cpp \
    $$PWD/dthememanager.cpp \
    $$PWD/dapplication.cpp \
    $$PWD/dbaseline.cpp \
    $$PWD/dheaderline.cpp \
    $$PWD/dbaseexpand.cpp \
    $$PWD/darrowbutton.cpp \
    $$PWD/darrowlineexpand.cpp \
    $$PWD/dswitchlineexpand.cpp \
    $$PWD/dimagebutton.cpp \
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
    $$PWD/darrowrectangle.cpp \
    $$PWD/dgraphicsgloweffect.cpp \
    $$PWD/dboxwidget.cpp \
    $$PWD/dcircleprogress.cpp \
    $$PWD/dstackwidget.cpp \
    $$PWD/dfilechooseredit.cpp \
    $$PWD/dpasswordedit.cpp \
    $$PWD/dipv4lineedit.cpp \
    $$PWD/dspinbox.cpp \
    $$PWD/dpicturesequenceview.cpp \
    $$PWD/dflowlayout.cpp \
    $$PWD/dlistview.cpp \
    $$PWD/denhancedwidget.cpp \
    $$PWD/dtitlebar.cpp \
    $$PWD/dplatformwindowhandle.cpp \
    $$PWD/dmainwindow.cpp \
    $$PWD/dblureffectwidget.cpp \
    $$PWD/dpageindicator.cpp \
    $$PWD/dclipeffectwidget.cpp \
    $$PWD/dgraphicsclipeffect.cpp \
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
    $$PWD/dstyle.cpp \
    $$PWD/dfloatingbutton.cpp \
    $$PWD/dstyleditemdelegate.cpp \
    $$PWD/diconbutton.cpp \
    $$PWD/dfloatingwidget.cpp \
    $$PWD/dapplicationhelper.cpp \
    $$PWD/dfloatingmessage.cpp \
    $$PWD/dmessagemanager.cpp \
    $$PWD/dbuttonbox.cpp \
    $$PWD/dwarningbutton.cpp

RESOURCES += \
    $$PWD/icons.qrc

INCLUDEPATH += $$PWD

includes.files += $$PWD/*.h
includes.files += \
    $$PWD/DTitlebar \
    $$PWD/DMainWindow \
    $$PWD/DAboutDialog \
    $$PWD/DApplication \
    $$PWD/DBlurEffectWidget \
    $$PWD/DClipEffectWidget \
    $$PWD/DGraphicsDropShadowEffect \
    $$PWD/DPlatformWindowHandle \
    $$PWD/DGraphicsClipEffect \
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
    $$PWD/DStyleOptionViewItem \
    $$PWD/DScrollBar \
    $$PWD/DPushButton \
    $$PWD/DToolButton \
    $$PWD/DRadioButton \
    $$PWD/DCheckButton \
    $$PWD/DCommandLinkButton \
    $$PWD/DDialogButtonBox \
    $$PWD/DListWidget \
    $$PWD/DTreeWidget \
    $$PWD/DTableWidget \
    $$PWD/DGroupBox \
    $$PWD/DScrollArea \
    $$PWD/DToolBox \
    $$PWD/DTableWidget \
    $$PWD/DStackedWidget \
    $$PWD/DFrame \
    $$PWD/DWidget \
    $$PWD/DMDIArea \
    $$PWD/DDockWidget \
    $$PWD/DComboBox \
    $$PWD/DFontComboBox \
    $$PWD/DLineEdit \
    $$PWD/DTextEdit \
    $$PWD/DPlainTextEdit \
    $$PWD/DSpinBox \
    $$PWD/DDoubleSpinBox \
    $$PWD/DTimeEdit \
    $$PWD/DDateEdit \
    $$PWD/DDateTimeEdit \
    $$PWD/DDial \
    $$PWD/DHorizontalScrollBar \
    $$PWD/DVerticalScrollBar \
    $$PWD/DHorizontalSlider \
    $$PWD/DVerticalSlider \
    $$PWD/DKeySequenceEdit \
    $$PWD/DLabel \
    $$PWD/DTextBrowser \
    $$PWD/DGraphicsView \
    $$PWD/DCalendarWidget \
    $$PWD/DLCDNumber \
    $$PWD/DProgressBar \
    $$PWD/DHorizontalLine \
    $$PWD/DVerticalLine \
    $$PWD/DOpenGLWidget \
    $$PWD/DQuickWidget \
    $$PWD/DWebView \
    $$PWD/DAccessibleWidget \
    $$PWD/DCheckBox \
    $$PWD/DColorDialog \
    $$PWD/DColumnView \
    $$PWD/DDataWidgetMapper \
    $$PWD/DFocusFrame \
    $$PWD/DHeaderView \
    $$PWD/DInputDialog \
    $$PWD/DMdiArea \
    $$PWD/DMdiSubWindow \
    $$PWD/DErrorMessage \
    $$PWD/DFontDialog \
    $$PWD/DMenu \
    $$PWD/DMenuBar \
    $$PWD/DMessageBox \
    $$PWD/DRubberBand \
    $$PWD/DSlider \
    $$PWD/DSplitter \
    $$PWD/DStatusBar \
    $$PWD/DTabWidget \
    $$PWD/DTableView \
    $$PWD/DTileRules \
    $$PWD/DToolBar \
    $$PWD/DToolTip \
    $$PWD/DTreeView \
    $$PWD/DUndoView \
    $$PWD/DWhatsThis \
    $$PWD/DWizard \
    $$PWD/DWizardPage \
    $$PWD/DDialog \
    $$PWD/DStyledItemDelegate \
    $$PWD/DStandardItem \
    $$PWD/DIconButton \
    $$PWD/DFloatingWidget \
    $$PWD/DStyleHelper \
    $$PWD/DStylePainter \
    $$PWD/DStyledIconEngine \
    $$PWD/DArrowRectangle \
    $$PWD/DImageButton \
    $$PWD/DSwitchButton \
    $$PWD/DWindowCloseButton \
    $$PWD/DWindowMaxButton \
    $$PWD/DWindowMinButton \
    $$PWD/DWindowOptionButton \
    $$PWD/DApplicationHelper \
    $$PWD/DFloatingWidget \
    $$PWD/DFloatingMessage \
    $$PWD/DMessageManager \
    $$PWD/DButtonBox \
    $$PWD/DApplicationSettings \
    $$PWD/DWarningButton
