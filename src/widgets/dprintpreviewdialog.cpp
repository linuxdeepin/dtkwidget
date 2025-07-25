// SPDX-FileCopyrightText: 2022 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dprintpreviewdialog.h"

#include "private/dprintpreviewdialog_p.h"
#include "dframe.h"
#include "diconbutton.h"
#include "dlabel.h"
#include "dlineedit.h"
#include "dfloatingbutton.h"
#include "dswitchbutton.h"
#include "dsuggestbutton.h"
#include "dcommandlinkbutton.h"
#include "dspinbox.h"
#include "dbackgroundgroup.h"
#include "dfiledialog.h"
#include "dpalettehelper.h"
#include "dstyleoption.h"
#include "dapplication.h"
#include "dfilechooseredit.h"
#include "dslider.h"
#include "dtoolbutton.h"
#include "dtitlebar.h"

#include <DScrollArea>
#include <DScrollBar>
#include <DPlatformWindowHandle>
#include <DIconTheme>
#include <QPluginLoader>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QtPrintSupport/QPrinterInfo>
#include <QPrinter>
#include <QDebug>
#include <QTime>
#include <QFontDatabase>
#include <QDesktopServices>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStandardPaths>
#include <QTimer>
#include <QKeyEvent>
#include <private/qprint_p.h>
#include <private/qcups_p.h>
#include <private/qprintdevice_p.h>
#include <qpa/qplatformprintplugin.h>
#include <qpa/qplatformprintersupport.h>

#define NORMAL_LEFT_RIGHT 31.8
#define NORMAL_MODERATE_TOP_BOTTRM 25.4
#define MODERATE_LEFT_RIGHT 19.1

#define EightK_Weight 270
#define EightK_Height 390
#define SixteenK_Weight 195
#define SixteenK_Height 270

#define WIDTH_NORMAL 422
#define HEIGHT_NORMAL 48
#define SPACER_HEIGHT_HIDE 0
#define SPACER_HEIGHT_SHOW 370

#define PAGERANGE_ALL 0
#define PAGERANGE_CURRENT 1
#define PAGERANGE_SELECT 2
#define FIRST_PAGE 1

#define ACTUAL_SIZE 1
#define SCALE 2

#define PICKCOLOR_RADIUS 8
#define PICKCOLOR_WIDTH 314
#define PICKCOLOR_HEIGNT 375

#define WATERLAYOUT_CENTER 0
#define WATERLAYOUT_TILED 1
#define WATERFONT_SIZE 65

DWIDGET_BEGIN_NAMESPACE

static QLatin1String _d_printSettingNameMap[DPrintPreviewSettingInterface::SC_ControlCount] = {
    QLatin1String("PrinterFrame"),
    QLatin1String("CopyCountFrame"),
    QLatin1String("PageRangeFrame"),
    QLatin1String("PageRangeTypeWidget"),
    QLatin1String("CustomPageEdit"),
    QLatin1String("OrientationBackgroundGroup"),
    QLatin1String("PaperSizeFrame"),
    QLatin1String("DuplexFrame"),
    QLatin1String("DuplexTypeComboBox"),
    QLatin1String("N-UpFrame"),
    QLatin1String("N-UpNumberComboBox"),
    QLatin1String("N-UpButtonWidget"),
    QLatin1String("PrintOrderBackgroundGroup"),
    QLatin1String("PrintOrderSequentialPrintWidget"),
    QLatin1String("PrintOrderTypeComboBox"),
    QLatin1String("ColorModeFrame"),
    QLatin1String("MarginsFrame"),
    QLatin1String("MarginsTypeComboBox"),
    QLatin1String("MarginsAdjustWidget"),
    QLatin1String("ScalingContentBackgroundGroup"),
    QLatin1String("WaterMarkFrame"),
    QLatin1String("WaterMarkContentFrame"),
    QLatin1String("WaterMarkTypeBackgroundGroup"),
    QLatin1String("WaterMarkTextTypeComboBox"),
    QLatin1String("WaterMarkCustomTextEdit"),
    QLatin1String("WaterMarkTextFontComboBox"),
    QLatin1String("WaterMarkTextColorButton"),
    QLatin1String("WaterMarkImagePathEdit"),
    QLatin1String("WaterMarkLayoutFrame"),
    QLatin1String("WaterMarkAngleFrame"),
    QLatin1String("WaterMarkSizeFrame"),
    QLatin1String("WaterMarkTransparencyFrame"),
};

void setwidgetfont(QWidget *widget, DFontSizeManager::SizeType type = DFontSizeManager::T5)
{
    QFont font = widget->font();
    font.setBold(true);
    widget->setFont(font);
    DFontSizeManager::instance()->bind(widget, type);
}

static void _d_setSpinboxDefaultValue(QHash<QWidget *, QString> valueCaches, DSpinBox *spinBox)
{
    if (valueCaches.contains(spinBox->lineEdit()) && (valueCaches.value(spinBox->lineEdit()).isEmpty() || valueCaches.value(spinBox->lineEdit()) == "°")) {
        QVariant defaultVariant = spinBox->property("_d_printPreview_spinboxDefalutValue");
        if (defaultVariant.isValid())
            spinBox->setValue(defaultVariant.toInt());
    }
}

DPrintPreviewDialogPrivate::DPrintPreviewDialogPrivate(DPrintPreviewDialog *qq)
    : DDialogPrivate(qq)
{
}

void DPrintPreviewDialogPrivate::startup()
{
    Q_Q(DPrintPreviewDialog);

    this->printer = new DPrinter;
    this->settingHelper = new PreviewSettingsPluginHelper(this);
    PreviewSettingsPluginHelper::loadPlugin();

    if (qApp)
        qApp->installEventFilter(q);
    initui();
    initdata();
    initconnections();
}

void DPrintPreviewDialogPrivate::initui()
{
    Q_Q(DPrintPreviewDialog);

    DWidget *mainWidget = new DWidget(q);
    mainWidget->setObjectName("mainwidget");
    mainWidget->setAutoFillBackground(true);
    mainWidget->lower();

    DTitlebar *titlebar = q->findChild<DTitlebar *>();
    Q_ASSERT(titlebar);
    titlebar->setAutoFillBackground(true);
    titlebar->setSplitScreenEnabled(false);

    DPalette pa = DPaletteHelper::instance()->palette(titlebar);
    pa.setBrush(DPalette::Window, pa.base());
    DPaletteHelper::instance()->setPalette(titlebar, pa);

    QHBoxLayout *mainlayout = new QHBoxLayout();
    mainlayout->setContentsMargins(QMargins(0, 0, 0, 0));
    mainlayout->setSpacing(0);

    QVBoxLayout *pleftlayout = new QVBoxLayout;
    initleft(pleftlayout);
    QVBoxLayout *prightlayout = new QVBoxLayout;
    initright(prightlayout);

    DWidget *leftWidget = new DWidget;
    DWidget *rightWidget = new DWidget;
    leftWidget->setObjectName("leftWidget");
    rightWidget->setObjectName("rightWidget");
    mainlayout->addWidget(leftWidget);
    mainlayout->addWidget(rightWidget);
    leftWidget->setLayout(pleftlayout);
    rightWidget->setLayout(prightlayout);
    back = new DBackgroundGroup(mainlayout);
    back->setObjectName("backGround");
    back->setItemSpacing(2);
    q->addSpacing(5);
    q->addContent(back);

    colorWidget = new DFloatingWidget(q);
    colorWidget->setFixedSize(PICKCOLOR_WIDTH, PICKCOLOR_HEIGNT);
    pickColorWidget = new DPrintPickColorWidget(colorWidget);
    colorWidget->setWidget(pickColorWidget);
    colorWidget->hide();
}

void DPrintPreviewDialogPrivate::initleft(QVBoxLayout *layout)
{
    Q_Q(DPrintPreviewDialog);
    pview = new DPrintPreviewWidget(this->printer);
    pview->setLineWidth(0);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(pview);

    QHBoxLayout *pbottomlayout = new QHBoxLayout;
    pbottomlayout->setContentsMargins(0, 10, 0, 0);
    layout->addLayout(pbottomlayout);
    firstBtn = new DIconButton(DStyle::SP_ArrowPrev);
    prevPageBtn = new DIconButton(DStyle::SP_ArrowLeft);
    firstBtn->setIcon(DIconTheme::findQIcon("printer_original"));
    firstBtn->setEnabled(false);
    prevPageBtn->setEnabled(false);
    jumpPageEdit = new DSpinBox;
    jumpPageEdit->setMaximumWidth(105);
    jumpPageEdit->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
    jumpPageEdit->installEventFilter(q);
    DLabel *spaceLabel = new DLabel("/");
    totalPageLabel = new DLabel;
    originTotalPageLabel = new DLabel;
    originTotalPageLabel->setEnabled(false);
    nextPageBtn = new DIconButton(DStyle::SP_ArrowRight);
    lastBtn = new DIconButton(DStyle::SP_ArrowNext);
    lastBtn->setIcon(DIconTheme::findQIcon("printer_final"));
    pbottomlayout->addStretch();
    pbottomlayout->addWidget(firstBtn);
    pbottomlayout->addSpacing(10);
    pbottomlayout->addWidget(prevPageBtn);
    pbottomlayout->addSpacing(55);
    pbottomlayout->addWidget(jumpPageEdit);
    pbottomlayout->addWidget(spaceLabel);
    pbottomlayout->addWidget(totalPageLabel);
    pbottomlayout->addWidget(originTotalPageLabel);
    pbottomlayout->addSpacing(55);
    pbottomlayout->addWidget(nextPageBtn);
    pbottomlayout->addSpacing(10);
    pbottomlayout->addWidget(lastBtn);
    pbottomlayout->addStretch();

    QRegularExpression reg("^([1-9][0-9]*)");
    auto val = new QRegularExpressionValidator{reg, jumpPageEdit};
    jumpPageEdit->lineEdit()->setValidator(val);

    DPalette m_pa = DPaletteHelper::instance()->palette(pview);
    m_pa.setBrush(DPalette::Base, m_pa.itemBackground());
    DPaletteHelper::instance()->setPalette(pview, m_pa);
}

void DPrintPreviewDialogPrivate::initright(QVBoxLayout *layout)
{
    //top
    QVBoxLayout *ptoplayout = new QVBoxLayout;
    ptoplayout->setContentsMargins(0, 0, 0, 0);
    DWidget *ptopwidget = new DWidget;
    ptopwidget->setMinimumWidth(WIDTH_NORMAL);
    ptopwidget->setLayout(ptoplayout);

    basicsettingwdg = new DWidget;
    advancesettingwdg = new DWidget;
    if (Q_LIKELY(QLocale::system().language() != QLocale::Tibetan)) {
        basicsettingwdg->setFixedHeight(415);
    } else {
        basicsettingwdg->setFixedHeight(445);
    }
    scrollarea = new DScrollArea;
    scrollarea->setWidget(ptopwidget);
    scrollarea->setWidgetResizable(true);
    scrollarea->setFrameShape(QFrame::NoFrame);
    scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollarea->setBackgroundRole(QPalette::Base);
    advancesettingwdg->hide();

    advanceBtn = new DPushButton(qApp->translate("DPrintPreviewDialogPrivate", "Advanced"));
    advanceBtn->setLayoutDirection(Qt::RightToLeft);
    advanceBtn->setIcon(DIconTheme::findQIcon("printer_dropdown"));
    advanceBtn->setIconSize(QSize(12, 12));
    DPalette pa = advanceBtn->palette();
    pa.setColor(DPalette::ButtonText, pa.link().color());
    advanceBtn->setPalette(pa);
    advanceBtn->setFlat(true);

    QHBoxLayout *advancelayout = new QHBoxLayout;
    advancelayout->addStretch();
    advancelayout->addWidget(advanceBtn);
    advancelayout->addStretch();
    ptoplayout->addWidget(basicsettingwdg);
    ptoplayout->addLayout(advancelayout);
    ptoplayout->addStretch();
    ptoplayout->addWidget(advancesettingwdg);

    initbasicui();
    initadvanceui();
    //bottom
    QHBoxLayout *pbottomlayout = new QHBoxLayout;
    pbottomlayout->setContentsMargins(0, 10, 0, 10);
    cancelBtn = new DPushButton(qApp->translate("DPrintPreviewDialogPrivate", "Cancel", "button"));
    printBtn = new DSuggestButton(qApp->translate("DPrintPreviewDialogPrivate", "Print", "button"));

    cancelBtn->setFixedSize(170, 36);
    printBtn->setFixedSize(170, 36);
    pbottomlayout->addWidget(cancelBtn);
    pbottomlayout->addWidget(printBtn);

    layout->addWidget(scrollarea);
    layout->addLayout(pbottomlayout);
}

void DPrintPreviewDialogPrivate::initbasicui()
{
    Q_Q(DPrintPreviewDialog);
    QVBoxLayout *layout = new QVBoxLayout(basicsettingwdg);
    layout->setSpacing(10);
    DLabel *basicLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Basic"), basicsettingwdg);
    setwidgetfont(basicLabel);
    QHBoxLayout *basictitlelayout = new QHBoxLayout;
    layout->addLayout(basictitlelayout);
    basictitlelayout->setContentsMargins(10, 0, 0, 0);
    basictitlelayout->addWidget(basicLabel);
    basictitlelayout->setAlignment(basicLabel, Qt::AlignLeft | Qt::AlignBottom);

    //打印机选择
    DFrame *printerFrame = new DFrame(basicsettingwdg);
    printerFrame->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_PrinterWidget]);
    layout->addWidget(printerFrame);
    printerFrame->setMinimumSize(WIDTH_NORMAL, HEIGHT_NORMAL);
    setfrmaeback(printerFrame);
    QHBoxLayout *printerlayout = new QHBoxLayout(printerFrame);
    printerlayout->setContentsMargins(10, 0, 10, 0);
    DLabel *printerlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Printer"), printerFrame);
    printerlabel->setSizePolicy(QSizePolicy::Maximum, printerlabel->sizePolicy().verticalPolicy());
    printDeviceCombo = new DComboBox(basicsettingwdg);
    printerlayout->addWidget(printerlabel, 4);
    printerlayout->addStretch(1);
    printerlayout->addWidget(printDeviceCombo, 9);
    printerlayout->setAlignment(printDeviceCombo, Qt::AlignVCenter);

    //打印份数
    DFrame *copycountFrame = new DFrame(basicsettingwdg);
    copycountFrame->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_CopiesWidget]);
    layout->addWidget(copycountFrame);
    copycountFrame->setMinimumSize(WIDTH_NORMAL, HEIGHT_NORMAL);
    setfrmaeback(copycountFrame);
    QHBoxLayout *copycountlayout = new QHBoxLayout(copycountFrame);
    copycountlayout->setContentsMargins(10, 0, 10, 0);
    DLabel *copycountlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Copies"), copycountFrame);
    copycountlabel->setSizePolicy(QSizePolicy::Maximum, copycountlabel->sizePolicy().verticalPolicy());
    copycountspinbox = new DSpinBox(copycountFrame);
    copycountspinbox->setEnabledEmbedStyle(true);
    copycountspinbox->setRange(1, 999);
    copycountspinbox->installEventFilter(q);
    copycountlayout->addWidget(copycountlabel, 4);
    copycountlayout->addStretch(1);
    copycountlayout->addWidget(copycountspinbox, 9);

    QRegularExpression re("^[1-9][0-9][0-9]$");
    auto va = new QRegularExpressionValidator(re, copycountspinbox);
    copycountspinbox->lineEdit()->setValidator(va);

    //页码范围
    DFrame *pageFrame = new DFrame(basicsettingwdg);
    pageFrame->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_PageRangeWidget]);
    layout->addWidget(pageFrame);
    pageFrame->setMinimumSize(WIDTH_NORMAL, 94);
    setfrmaeback(pageFrame);
    QVBoxLayout *pagelayout = new QVBoxLayout(pageFrame);
    pagelayout->setContentsMargins(10, 5, 10, 5);
    DLabel *pagerangelabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Page range"), pageFrame);
    pagerangelabel->setSizePolicy(QSizePolicy::Maximum, printerlabel->sizePolicy().verticalPolicy());
    pageRangeCombo = new DComboBox(pageFrame);
    pageRangeCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "All"));
    pageRangeCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Current page"));
    pageRangeCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Select pages"));
    QWidget *hrangeWidget = new QWidget(q);
    hrangeWidget->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_PageRange_TypeControl]);
    QHBoxLayout *hrangebox = new QHBoxLayout(hrangeWidget);
    hrangebox->setContentsMargins(0, 0, 0, 0);
    hrangebox->addWidget(pagerangelabel, 4);
    hrangebox->addStretch(1);
    hrangebox->addWidget(pageRangeCombo, 9);

    pageRangeEdit = new DLineEdit;
    pageRangeEdit->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_PageRange_SelectEdit]);
    pagelayout->addWidget(hrangeWidget);
    pagelayout->addWidget(pageRangeEdit);
    pageRangeEdit->installEventFilter(q);

    QRegularExpression reg(R"(^([1-9][0-9]*?(-[1-9][0-9]*?)?,)*?([1-9][0-9]*?|[1-9][0-9]*?-[1-9][0-9]*?)$)");
    QRegularExpressionValidator *val = new QRegularExpressionValidator(reg, pageRangeEdit);
    pageRangeEdit->lineEdit()->setValidator(val);

    //打印方向
    DLabel *orientationLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Orientation"), basicsettingwdg);
    orientationLabel->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_OrientationWidget]);
    setwidgetfont(orientationLabel);
    QHBoxLayout *orientationtitlelayout = new QHBoxLayout;
    orientationtitlelayout->setContentsMargins(10, 0, 0, 0);
    orientationtitlelayout->addWidget(orientationLabel);
    orientationtitlelayout->setAlignment(orientationLabel, Qt::AlignLeft | Qt::AlignBottom);
    layout->addLayout(orientationtitlelayout);

    QVBoxLayout *orientationlayout = new QVBoxLayout;
    orientationlayout->setContentsMargins(0, 0, 0, 0);
    DRadioButton *verRadio = new DRadioButton;
    verRadio->setIcon(DIconTheme::findQIcon("printer_portrait"));
    verRadio->setIconSize(QSize(36, 36));
    DRadioButton *horRadio = new DRadioButton;
    horRadio->setIcon(DIconTheme::findQIcon("printer_landscape"));
    horRadio->setIconSize(QSize(36, 36));
    orientationgroup = new QButtonGroup(q);
    orientationgroup->addButton(verRadio, 0);
    orientationgroup->addButton(horRadio, 1);

    //纵向
    DWidget *portraitwdg = new DWidget;
    portraitwdg->setMinimumSize(WIDTH_NORMAL, HEIGHT_NORMAL);
    QHBoxLayout *portraitlayout = new QHBoxLayout;
    DLabel *orientationTextLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Portrait"), portraitwdg);
    portraitlayout->addWidget(verRadio);
    portraitlayout->addWidget(orientationTextLabel);
    portraitlayout->addStretch();
    portraitwdg->setLayout(portraitlayout);

    //横向
    DWidget *landscapewdg = new DWidget;
    landscapewdg->setMinimumSize(WIDTH_NORMAL, HEIGHT_NORMAL);
    QHBoxLayout *landscapelayout = new QHBoxLayout;
    DLabel *landscapeTextLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Landscape"), portraitwdg);
    landscapelayout->addWidget(horRadio);
    landscapelayout->addWidget(landscapeTextLabel);
    landscapelayout->addStretch();
    landscapewdg->setLayout(landscapelayout);

    orientationlayout->addWidget(portraitwdg);
    orientationlayout->addWidget(landscapewdg);
    DBackgroundGroup *back = new DBackgroundGroup(orientationlayout);
    back->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_OrientationWidget]);
    back->setItemSpacing(2);
    DPalette pa = DPaletteHelper::instance()->palette(back);
    pa.setBrush(DPalette::Base, pa.itemBackground());
    DPaletteHelper::instance()->setPalette(back, pa);
    layout->addWidget(back);
}

void DPrintPreviewDialogPrivate::initadvanceui()
{
    Q_Q(DPrintPreviewDialog);
    QVBoxLayout *layout = new QVBoxLayout(advancesettingwdg);
    layout->setContentsMargins(0, 0, 0, 0);
    advancesettingwdg->setMinimumWidth(WIDTH_NORMAL);

    //页面设置
    QVBoxLayout *pagelayout = new QVBoxLayout;
    pagelayout->setSpacing(10);
    pagelayout->setContentsMargins(10, 0, 10, 0);
    DLabel *pagesLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Pages"), advancesettingwdg);
    setwidgetfont(pagesLabel, DFontSizeManager::T5);
    QHBoxLayout *pagestitlelayout = new QHBoxLayout;
    pagestitlelayout->setContentsMargins(10, 20, 0, 0);
    pagestitlelayout->addWidget(pagesLabel);
    pagestitlelayout->setAlignment(pagesLabel, Qt::AlignLeft | Qt::AlignBottom);

    DFrame *colorframe = new DFrame;
    colorframe->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_ColorModeWidget]);
    setfrmaeback(colorframe);
    colorframe->setFixedHeight(HEIGHT_NORMAL);
    QHBoxLayout *colorlayout = new QHBoxLayout(colorframe);
    DLabel *colorlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Color mode"));
    colorlabel->setSizePolicy(QSizePolicy::Maximum, colorlabel->sizePolicy().verticalPolicy());
    colorModeCombo = new DComboBox;
    colorModeCombo->addItems(QStringList() << qApp->translate("DPrintPreviewDialogPrivate", "Color") << qApp->translate("DPrintPreviewDialogPrivate", "Grayscale"));
    colorlayout->addWidget(colorlabel, 4);
    colorlayout->addStretch(1);
    colorlayout->addWidget(colorModeCombo, 9);
    colorlayout->setContentsMargins(10, 4, 10, 4);

    DFrame *marginsframe = new DFrame;
    marginsframe->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_MarginWidget]);
    setfrmaeback(marginsframe);
    QVBoxLayout *marginslayout = new QVBoxLayout(marginsframe);
    marginslayout->setContentsMargins(10, 5, 10, 5);
    QHBoxLayout *marginscombolayout = new QHBoxLayout;
    DLabel *marginlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Margins"));
    marginlabel->setSizePolicy(QSizePolicy::Maximum, marginlabel->sizePolicy().verticalPolicy());
    marginsCombo = new DComboBox;
    marginsCombo->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Margin_TypeControl]);
    marginsCombo->addItems(QStringList() << qApp->translate("DPrintPreviewDialogPrivate", "Narrow (mm)") << qApp->translate("DPrintPreviewDialogPrivate", "Normal (mm)") << qApp->translate("DPrintPreviewDialogPrivate", "Moderate (mm)") << qApp->translate("DPrintPreviewDialogPrivate", "Customize (mm)"));
    marginscombolayout->addWidget(marginlabel, 4);
    marginscombolayout->addStretch(1);
    marginscombolayout->addWidget(marginsCombo, 9);

    QWidget *marginSpinWidget = new QWidget(q);
    marginSpinWidget->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Margin_AdjustContol]);
    QHBoxLayout *marginsspinlayout = new QHBoxLayout(marginSpinWidget);

    DLabel *toplabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Top"));
    marginTopSpin = new DDoubleSpinBox;
    marginTopSpin->installEventFilter(q);
    DLabel *leftlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Left"));
    marginLeftSpin = new DDoubleSpinBox;
    marginLeftSpin->installEventFilter(q);
    DLabel *bottomlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Bottom"));
    marginBottomSpin = new DDoubleSpinBox;
    marginBottomSpin->installEventFilter(q);
    DLabel *rightlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Right"));
    marginRightSpin = new DDoubleSpinBox;
    marginRightSpin->installEventFilter(q);
    QVBoxLayout *marginslabellayout1 = new QVBoxLayout;
    marginslabellayout1->addWidget(toplabel);
    marginslabellayout1->addWidget(leftlabel);
    QVBoxLayout *marginsspinboxlayout1 = new QVBoxLayout;
    marginsspinboxlayout1->addWidget(marginTopSpin);
    marginsspinboxlayout1->addWidget(marginLeftSpin);
    marginsspinboxlayout1->setContentsMargins(0, 0, 10, 0);
    QVBoxLayout *marginslabellayout2 = new QVBoxLayout;
    marginslabellayout2->addWidget(bottomlabel);
    marginslabellayout2->addWidget(rightlabel);
    QVBoxLayout *marginsspinboxlayout2 = new QVBoxLayout;
    marginsspinboxlayout2->addWidget(marginBottomSpin);
    marginsspinboxlayout2->addWidget(marginRightSpin);
    spacer = new QSpacerItem(130, 72);
    marginsspinlayout->addSpacerItem(spacer);
    marginsLayout(true);

    marginsspinlayout->setContentsMargins(20, 0, 0, 0);
    marginsspinlayout->addLayout(marginslabellayout1);
    marginsspinlayout->addLayout(marginsspinboxlayout1);
    marginsspinlayout->addLayout(marginslabellayout2);
    marginsspinlayout->addLayout(marginsspinboxlayout2);
    marginslayout->setSpacing(0);
    marginslayout->addLayout(marginscombolayout);
    marginslayout->addSpacing(10);
    marginslayout->addWidget(marginSpinWidget);

    QRegularExpression reg("^([5-5][0-4]|[1-4][0-9]|[0-9])?(\\.[0-9][0-9])|55(\\.[8-8][0-8])|55(\\.[0-7][0-9])");
    auto val = new QRegularExpressionValidator(reg, marginsframe);
    QList<DDoubleSpinBox *> list = marginsframe->findChildren<DDoubleSpinBox *>();
    for (int i = 0; i < list.size(); i++) {
        list.at(i)->setEnabledEmbedStyle(true);
        list.at(i)->lineEdit()->setClearButtonEnabled(false);
        list.at(i)->lineEdit()->setValidator(val);
        list.at(i)->setMaximum(55.88);
    }

    pagelayout->addLayout(pagestitlelayout);
    pagelayout->addWidget(colorframe);
    pagelayout->addWidget(marginsframe);

    //缩放
    QVBoxLayout *scalinglayout = new QVBoxLayout;
    scalinglayout->setContentsMargins(10, 0, 10, 0);
    DLabel *scalingLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Scaling"), advancesettingwdg);
    scalingLabel->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_ScalingWidget]);
    QHBoxLayout *scalingtitlelayout = new QHBoxLayout;
    scalingtitlelayout->setContentsMargins(10, 20, 0, 0);
    scalingtitlelayout->addWidget(scalingLabel);
    scalingtitlelayout->setAlignment(scalingLabel, Qt::AlignLeft | Qt::AlignBottom);

    setwidgetfont(scalingLabel, DFontSizeManager::T5);

    scaleGroup = new QButtonGroup(q);
    QVBoxLayout *scalingcontentlayout = new QVBoxLayout;
    scalingcontentlayout->setContentsMargins(0, 0, 0, 0);

    DWidget *actualwdg = new DWidget;
    actualwdg->setFixedHeight(HEIGHT_NORMAL);
    QHBoxLayout *actuallayout = new QHBoxLayout(actualwdg);
    actuallayout->setContentsMargins(10, 0, 10, 0);
    DRadioButton *actualSizeRadio = new DRadioButton(qApp->translate("DPrintPreviewDialogPrivate", "Actual size"));

    scaleGroup->addButton(actualSizeRadio, ACTUAL_SIZE);
    actuallayout->addWidget(actualSizeRadio);

    DWidget *customscalewdg = new DWidget;
    customscalewdg->setFixedHeight(HEIGHT_NORMAL);
    QHBoxLayout *customlayout = new QHBoxLayout(customscalewdg);
    customlayout->setContentsMargins(10, 0, 10, 0);
    DRadioButton *customSizeRadio = new DRadioButton(qApp->translate("DPrintPreviewDialogPrivate", "Scale"));
    scaleGroup->addButton(customSizeRadio, SCALE);
    scaleRateEdit = new DSpinBox;
    QRegularExpression scaleReg("^([1-9][0-9]?|[1][0-9]{2}|200)$");
    auto scaleVal = new QRegularExpressionValidator(scaleReg, scaleRateEdit);
    scaleRateEdit->lineEdit()->setValidator(scaleVal);
    scaleRateEdit->setEnabledEmbedStyle(true);
    scaleRateEdit->setRange(1, 200);
    scaleRateEdit->setFixedWidth(80);
    scaleRateEdit->installEventFilter(q);
    DLabel *scaleLabel = new DLabel("%");
    customlayout->addWidget(customSizeRadio);
    customlayout->addWidget(scaleRateEdit);
    customlayout->addWidget(scaleLabel);
    customlayout->addStretch(1);

    scalingcontentlayout->addWidget(actualwdg);
    scalingcontentlayout->addWidget(customscalewdg);

    DBackgroundGroup *back = new DBackgroundGroup(scalingcontentlayout);
    back->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_ScalingWidget]);
    back->setItemSpacing(1);
    DPalette pa = DPaletteHelper::instance()->palette(back);
    pa.setBrush(DPalette::Base, pa.itemBackground());
    DPaletteHelper::instance()->setPalette(back, pa);
    scalinglayout->addLayout(scalingtitlelayout);
    scalinglayout->addWidget(back);

    //纸张
    QVBoxLayout *paperlayout = new QVBoxLayout;
    paperlayout->setContentsMargins(10, 0, 10, 0);
    DLabel *paperLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Paper"), advancesettingwdg);
    paperLabel->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_PaperSizeWidget]);
    setwidgetfont(paperLabel, DFontSizeManager::T5);
    QHBoxLayout *papertitlelayout = new QHBoxLayout;
    papertitlelayout->setContentsMargins(10, 0, 0, 0);
    papertitlelayout->addWidget(paperLabel);
    papertitlelayout->setAlignment(paperLabel, Qt::AlignLeft | Qt::AlignBottom);

    DFrame *paperframe = new DFrame;
    paperframe->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_PaperSizeWidget]);
    setfrmaeback(paperframe);
    paperframe->setFixedHeight(HEIGHT_NORMAL);
    QHBoxLayout *paperframelayout = new QHBoxLayout(paperframe);
    DLabel *papersizelabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Paper size"));
    papersizelabel->setSizePolicy(QSizePolicy::Maximum, papersizelabel->sizePolicy().verticalPolicy());
    paperSizeCombo = new DComboBox;
    paperSizeCombo->setFixedHeight(36);
    paperframelayout->addWidget(papersizelabel, 4);
    paperframelayout->addStretch(1);
    paperframelayout->addWidget(paperSizeCombo, 9);
    paperframelayout->setContentsMargins(10, 4, 10, 4);
    paperlayout->addLayout(papertitlelayout);
    paperlayout->addWidget(paperframe);

    //打印方式
    QVBoxLayout *drawinglayout = new QVBoxLayout;
    drawinglayout->setSpacing(10);
    drawinglayout->setContentsMargins(10, 0, 10, 0);
    DLabel *drawingLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Print Layout"), advancesettingwdg);
    setwidgetfont(drawingLabel, DFontSizeManager::T5);
    QHBoxLayout *drawingtitlelayout = new QHBoxLayout;
    drawingtitlelayout->setContentsMargins(10, 20, 0, 0);
    drawingtitlelayout->addWidget(drawingLabel);
    drawingtitlelayout->setAlignment(drawingLabel, Qt::AlignLeft | Qt::AlignBottom);

    DFrame *duplexframe = new DFrame;
    duplexframe->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_DuplexWidget]);
    setfrmaeback(duplexframe);
    duplexframe->setFixedHeight(HEIGHT_NORMAL);
    QHBoxLayout *duplexlayout = new QHBoxLayout(duplexframe);
    duplexCombo = new DComboBox;
    duplexCombo->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Duplex_TypeControl]);
    duplexCheckBox = new DCheckBox(qApp->translate("DPrintPreviewDialogPrivate", "Duplex"));
    duplexCombo->setFixedHeight(36);
    duplexlayout->setContentsMargins(5, 5, 10, 5);
    duplexlayout->addWidget(duplexCheckBox, 4);
    duplexlayout->addWidget(duplexCombo, 9);

    //并列打印
    DFrame *sidebysideframe = new DFrame;
    sidebysideframe->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_NPrintWidget]);
    setfrmaeback(sidebysideframe);
    QVBoxLayout *sidebysideframelayout = new QVBoxLayout(sidebysideframe);
    sidebysideframelayout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *pagepersheetlayout = new QHBoxLayout;
    sidebysideCheckBox = new DCheckBox(qApp->translate("DPrintPreviewDialogPrivate", "N-up printing"));
    pagePerSheetCombo = new DComboBox;
    pagePerSheetCombo->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_NPrint_Numbers]);
    pagePerSheetCombo->addItems(QStringList() << qApp->translate("DPrintPreviewDialogPrivate", "2 pages/sheet, 1×2") << qApp->translate("DPrintPreviewDialogPrivate", "4 pages/sheet, 2×2") << qApp->translate("DPrintPreviewDialogPrivate", "6 pages/sheet, 2×3") << qApp->translate("DPrintPreviewDialogPrivate", "9 pages/sheet, 3×3") << qApp->translate("DPrintPreviewDialogPrivate", "16 pages/sheet, 4×4"));
    pagePerSheetCombo->setFixedHeight(36);
    pagepersheetlayout->setContentsMargins(5, 5, 10, 5);
    pagepersheetlayout->addWidget(sidebysideCheckBox, 4);
    pagepersheetlayout->addWidget(pagePerSheetCombo, 9);
    sidebysideframelayout->addLayout(pagepersheetlayout);

    QWidget *printDirectWidget = new QWidget(q);
    printDirectWidget->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_NPrint_Layout]);
    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_NPrint_Layout, false);
    QHBoxLayout *printdirectlayout = new QHBoxLayout(printDirectWidget);
    printdirectlayout->setContentsMargins(0, 0, 10, 5);
    DLabel *directlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Layout direction"));
    DToolButton *lrtbBtn = new DToolButton;
    DToolButton *rltbBtn = new DToolButton;
    DToolButton *tblrBtn = new DToolButton;
    DToolButton *tbrlBtn = new DToolButton;
    DToolButton *repeatBtn = new DToolButton;
    lrtbBtn->setIcon(DIconTheme::findQIcon("printer_lrtb_1"));
    rltbBtn->setIcon(DIconTheme::findQIcon("printer_lrtb_2"));
    tblrBtn->setIcon(DIconTheme::findQIcon("printer_lrtb_3"));
    tbrlBtn->setIcon(DIconTheme::findQIcon("printer_lrtb_4"));
    repeatBtn->setIcon(DIconTheme::findQIcon("printer_lrtb_5"));
    DWidget *btnWidget = new DWidget;
    directGroup = new QButtonGroup(q);
    QHBoxLayout *btnLayout = new QHBoxLayout(btnWidget);
    btnLayout->setContentsMargins(0, 0, 0, 0);
    printdirectlayout->addSpacing(30);
    btnLayout->addWidget(rltbBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(lrtbBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(tblrBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(tbrlBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(repeatBtn);
    printdirectlayout->addWidget(directlabel, 2);
    printdirectlayout->addWidget(btnWidget, 5);
    sidebysideframelayout->addWidget(printDirectWidget);
    QList<DToolButton *> listBtn = btnWidget->findChildren<DToolButton *>();
    int num = 0;
    for (DToolButton *btn : listBtn) {
        btn->setIconSize(QSize(18, 18));
        btn->setFixedSize(QSize(36, 36));
        btn->setCheckable(true);
        directGroup->addButton(btn, num);
        num++;
    }
    drawinglayout->addLayout(drawingtitlelayout);
    drawinglayout->addWidget(duplexframe);
    drawinglayout->addWidget(sidebysideframe);

    //打印顺序
    QVBoxLayout *orderlayout = new QVBoxLayout;
    orderlayout->setContentsMargins(10, 0, 10, 0);
    DLabel *orderLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Page Order"), advancesettingwdg);
    orderLabel->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_PageOrderWidget]);
    setwidgetfont(orderLabel, DFontSizeManager::T5);
    QHBoxLayout *ordertitlelayout = new QHBoxLayout;
    ordertitlelayout->setContentsMargins(0, 20, 0, 0);
    ordertitlelayout->addWidget(orderLabel);
    ordertitlelayout->setAlignment(orderLabel, Qt::AlignLeft | Qt::AlignBottom);

    QVBoxLayout *ordercontentlayout = new QVBoxLayout;
    ordercontentlayout->setContentsMargins(0, 0, 0, 0);
    DWidget *collatewdg = new DWidget;
    collatewdg->setObjectName("CollateWidget");
    QHBoxLayout *collatelayout = new QHBoxLayout(collatewdg);
    DRadioButton *printCollateRadio = new DRadioButton(qApp->translate("DPrintPreviewDialogPrivate", "Collate pages")); //逐份打印
    collatelayout->addWidget(printCollateRadio);
    printCollateRadio->setChecked(true);
    inorderwdg = new DWidget;
    inorderwdg->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_PageOrder_SequentialPrint]);
    QHBoxLayout *inorderlayout = new QHBoxLayout(inorderwdg);
    printInOrderRadio = new DRadioButton(qApp->translate("DPrintPreviewDialogPrivate", "Print pages in order")); //按顺序打印
    inorderCombo = new DComboBox;
    inorderCombo->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_PageOrder_TypeControl]);
    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_PageOrder_TypeControl, false);
    inorderCombo->addItems(QStringList() << qApp->translate("DPrintPreviewDialogPrivate", "Front to back") << qApp->translate("DPrintPreviewDialogPrivate", "Back to front"));
    inorderlayout->addWidget(printInOrderRadio, 4);
    inorderlayout->addWidget(inorderCombo, 9);

    ordercontentlayout->addWidget(collatewdg);
    ordercontentlayout->addWidget(inorderwdg);

    printOrderGroup = new QButtonGroup(q);
    printOrderGroup->addButton(printCollateRadio, 0);
    printOrderGroup->addButton(printInOrderRadio, 1);

    DBackgroundGroup *backorder = new DBackgroundGroup(ordercontentlayout);
    backorder->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_PageOrderWidget]);
    backorder->setItemSpacing(1);
    pa = DPaletteHelper::instance()->palette(backorder);
    pa.setBrush(DPalette::Base, pa.itemBackground());
    DPaletteHelper::instance()->setPalette(backorder, pa);

    orderlayout->addLayout(ordertitlelayout);
    orderlayout->addWidget(backorder);

    //水印
    QVBoxLayout *watermarklayout = new QVBoxLayout;
    watermarklayout->setContentsMargins(10, 0, 10, 0);
    DLabel *watermarkLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Watermark"), advancesettingwdg);
    watermarkLabel->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_WatermarkWidget]);
    QHBoxLayout *watermarktitlelayout = new QHBoxLayout;
    watermarktitlelayout->setContentsMargins(10, 20, 0, 0);
    watermarktitlelayout->addWidget(watermarkLabel);
    watermarktitlelayout->setAlignment(watermarkLabel, Qt::AlignLeft | Qt::AlignBottom);

    setwidgetfont(watermarkLabel, DFontSizeManager::T5);

    DFrame *watermarkframe = new DFrame;
    watermarkframe->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_WatermarkWidget]);
    setfrmaeback(watermarkframe);
    QHBoxLayout *texttypelayout = new QHBoxLayout;
    texttypelayout->setContentsMargins(10, 10, 10, 10);
    DLabel *addlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Add watermark"));
    addlabel->setSizePolicy(QSizePolicy::Maximum, addlabel->sizePolicy().verticalPolicy());
    waterMarkBtn = new DSwitchButton;
    waterMarkBtn->setChecked(false);
    texttypelayout->addWidget(addlabel, Qt::AlignLeft);
    texttypelayout->addStretch(1);
    texttypelayout->addWidget(waterMarkBtn, Qt::AlignRight);
    watermarkframe->setLayout(texttypelayout);

    watermarksettingwdg = new DWidget;
    watermarksettingwdg->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_WatermarkContentWidget]);
    watermarksettingwdg->setMinimumWidth(WIDTH_NORMAL);
    initWaterMarkui();
    watermarksettingwdg->hide();
    wmSpacer = new QSpacerItem(WIDTH_NORMAL, SPACER_HEIGHT_SHOW);

    watermarklayout->addLayout(watermarktitlelayout);
    watermarklayout->addWidget(watermarkframe);
    watermarklayout->addWidget(watermarksettingwdg);
    watermarklayout->addSpacerItem(wmSpacer);

    layout->addLayout(paperlayout);
    layout->addLayout(drawinglayout);
    layout->addLayout(orderlayout);
    layout->addLayout(pagelayout);
    layout->addLayout(scalinglayout);
    layout->addLayout(watermarklayout);
    layout->addStretch();
}

void DPrintPreviewDialogPrivate::initWaterMarkui()
{
    Q_Q(DPrintPreviewDialog);
    QVBoxLayout *vContentLayout = new QVBoxLayout;
    vContentLayout->setContentsMargins(0, 5, 0, 5);
    vContentLayout->setSpacing(10);
    QVBoxLayout *vWatertypeLayout = new QVBoxLayout;
    vWatertypeLayout->setContentsMargins(0, 0, 0, 0);
    textWatermarkWdg = new DWidget;
    picWatermarkWdg = new DWidget;
    vWatertypeLayout->addWidget(textWatermarkWdg);
    vWatertypeLayout->addWidget(picWatermarkWdg);

    QVBoxLayout *textVlayout = new QVBoxLayout;
    textVlayout->setContentsMargins(9, 9, 14, 9);
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    DRadioButton *textBtn = new DRadioButton(qApp->translate("DPrintPreviewDialogPrivate", "Text watermark"));
    waterTextCombo = new DComboBox;
    waterTextCombo->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Watermark_TextType]);

    waterTextCombo->addItems(QStringList() << qApp->translate("DPrintPreviewDialogPrivate", "Confidential") << qApp->translate("DPrintPreviewDialogPrivate", "Draft") << qApp->translate("DPrintPreviewDialogPrivate", "Sample") << qApp->translate("DPrintPreviewDialogPrivate", "Custom"));
    hlayout1->addWidget(textBtn, 4);
    hlayout1->addWidget(waterTextCombo, 9);

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    waterTextEdit = new DLineEdit;
    waterTextEdit->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Watermark_CustomText]);
    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_CustomText, false);
    waterTextEdit->lineEdit()->setMaxLength(16);
    waterTextEdit->lineEdit()->setPlaceholderText(qApp->translate("DPrintPreviewDialogPrivate", "Input your text"));
    hlayout2->addStretch(5);
    hlayout2->addWidget(waterTextEdit, 10);

    QHBoxLayout *hlayout3 = new QHBoxLayout;
    fontCombo = new DComboBox;
    fontCombo->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Watermark_TextFont]);

    waterColorBtn = new DIconButton(textWatermarkWdg);
    waterColorBtn->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Watermark_TextColor]);
    waterColorBtn->setFixedSize(36, 36);
    waterColor = QColor("#6f6f6f");
    _q_selectColorButton(waterColor);
    isInitBtnColor = true;
    hlayout3->addWidget(new DLabel, 18);
    hlayout3->addWidget(fontCombo, 35);
    hlayout3->addWidget(waterColorBtn, 1);

    textVlayout->addLayout(hlayout1);
    textVlayout->addLayout(hlayout2);
    textVlayout->addLayout(hlayout3);
    textWatermarkWdg->setLayout(textVlayout);

    QHBoxLayout *picHlayout = new QHBoxLayout;
    picHlayout->setContentsMargins(9, 9, 14, 9);
    DRadioButton *picBtn = new DRadioButton(qApp->translate("DPrintPreviewDialogPrivate", "Picture watermark"));
    picPathEdit = new DFileChooserEdit;
    picPathEdit->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Watermark_ImageEdit]);
    picPathEdit->setNameFilters(QStringList() << qApp->translate("DPrintPreviewDialogPrivate", "Images") + "(*.png *.jpg)");
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    picPathEdit->setDirectoryUrl(QUrl(desktopPath, QUrl::TolerantMode));

    picHlayout->addWidget(picBtn, 4);
    picHlayout->addWidget(picPathEdit, 9);
    picWatermarkWdg->setLayout(picHlayout);

    waterTypeGroup = new QButtonGroup(q);
    waterTypeGroup->addButton(textBtn, 0);
    waterTypeGroup->addButton(picBtn, 1);

    DBackgroundGroup *back = new DBackgroundGroup(vWatertypeLayout);
    back->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Watermark_TypeGroup]);
    back->setItemSpacing(2);

    DFrame *posframe = new DFrame;
    posframe->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Watermark_Layout]);
    setfrmaeback(posframe);
    posframe->setFixedHeight(HEIGHT_NORMAL);
    QHBoxLayout *posframelayout = new QHBoxLayout(posframe);
    DLabel *poslabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Layout"));
    poslabel->setSizePolicy(QSizePolicy::Maximum, poslabel->sizePolicy().verticalPolicy());
    waterPosCombox = new DComboBox;
    waterPosCombox->addItems(QStringList() << qApp->translate("DPrintPreviewDialogPrivate", "Tile") << qApp->translate("DPrintPreviewDialogPrivate", "Center"));
    waterPosCombox->setCurrentIndex(waterPosCombox->count() - 1);
    waterPosCombox->setFixedHeight(36);
    posframelayout->addWidget(poslabel, 4);
    posframelayout->addStretch(1);
    posframelayout->addWidget(waterPosCombox, 9);
    posframelayout->setContentsMargins(10, 4, 10, 4);

    DFrame *inclinatframe = new DFrame;
    inclinatframe->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Watermark_Angle]);
    setfrmaeback(inclinatframe);
    inclinatframe->setFixedHeight(HEIGHT_NORMAL);
    QHBoxLayout *inclinatframelayout = new QHBoxLayout(inclinatframe);
    DLabel *inclinatlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Angle"));
    inclinatlabel->setSizePolicy(QSizePolicy::Maximum, inclinatlabel->sizePolicy().verticalPolicy());
    inclinatBox = new DSpinBox;
    inclinatBox->setSuffix("°");
    inclinatBox->setValue(30);
    inclinatBox->setSingleStep(5);
    inclinatBox->setRange(0, 360);
    inclinatBox->setFixedHeight(36);
    inclinatBox->setEnabledEmbedStyle(true);
    inclinatframelayout->addWidget(inclinatlabel, 4);
    inclinatframelayout->addStretch(1);
    inclinatframelayout->addWidget(inclinatBox, 9);
    inclinatframelayout->setContentsMargins(10, 4, 10, 4);

    DFrame *sizeframe = new DFrame;
    sizeframe->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Watermark_Size]);
    setfrmaeback(sizeframe);
    sizeframe->setFixedHeight(HEIGHT_NORMAL);
    QHBoxLayout *sizeframelayout = new QHBoxLayout(sizeframe);
    DLabel *sizelabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Size"));
    sizelabel->setSizePolicy(QSizePolicy::Maximum, sizelabel->sizePolicy().verticalPolicy());
    waterSizeSlider = new DSlider;
    sizeBox = new DSpinBox;
    sizeBox->lineEdit()->setReadOnly(true);
    sizeBox->setFixedWidth(78);
    sizeBox->setRange(10, 200);
    sizeBox->setValue(100);
    sizeBox->setSuffix("%");
    sizeBox->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
    waterSizeSlider->setMaximum(200);
    waterSizeSlider->setValue(100);
    waterSizeSlider->setMinimum(10);
    sizeframelayout->addWidget(sizelabel, 4);
    sizeframelayout->addStretch(1);
    sizeframelayout->addWidget(waterSizeSlider, 7);
    sizeframelayout->addWidget(sizeBox, 2);
    sizeframelayout->setContentsMargins(10, 4, 10, 4);

    DFrame *opaframe = new DFrame;
    opaframe->setObjectName(_d_printSettingNameMap[DPrintPreviewSettingInterface::SC_Watermark_Transparency]);
    setfrmaeback(opaframe);
    opaframe->setFixedHeight(HEIGHT_NORMAL);
    QHBoxLayout *opaframelayout = new QHBoxLayout(opaframe);
    DLabel *opalabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Transparency"));
    opalabel->setSizePolicy(QSizePolicy::Maximum, opalabel->sizePolicy().verticalPolicy());
    wmOpaSlider = new DSlider;
    opaBox = new DSpinBox;
    opaBox->lineEdit()->setReadOnly(true);
    opaBox->setFixedWidth(78);
    opaBox->setRange(0, 100);
    opaBox->setValue(30);
    opaBox->setSuffix("%");
    opaBox->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
    wmOpaSlider->setValue(30);
    wmOpaSlider->setMaximum(100);
    opaframelayout->addWidget(opalabel, 4);
    opaframelayout->addStretch(1);
    opaframelayout->addWidget(wmOpaSlider, 7);
    opaframelayout->addWidget(opaBox, 2);
    opaframelayout->setContentsMargins(10, 4, 10, 4);

    vContentLayout->addWidget(back);
    vContentLayout->addWidget(posframe);
    vContentLayout->addWidget(inclinatframe);
    vContentLayout->addWidget(sizeframe);
    vContentLayout->addWidget(opaframe);

    DPalette pa = DPaletteHelper::instance()->palette(back);
    pa.setBrush(DPalette::Base, pa.itemBackground());
    DPaletteHelper::instance()->setPalette(back, pa);
    watermarksettingwdg->setLayout(vContentLayout);
}

/*!
  DPrintPreviewDialogPrivate::initWaterSettings 初始化水印属性设置
 */
void DPrintPreviewDialogPrivate::initWaterSettings()
{
    Q_EMIT waterPosCombox->currentIndexChanged(waterPosCombox->currentIndex());
    Q_EMIT waterSizeSlider->valueChanged(waterSizeSlider->value());
    Q_EMIT wmOpaSlider->valueChanged(wmOpaSlider->value());
    Q_EMIT inclinatBox->editingFinished();
    if (waterTypeGroup->button(0)->isChecked()) {
        Q_EMIT fontCombo->currentIndexChanged(fontCombo->currentIndex());
        pview->setWaterMarkColor(waterColor);
    }
}

void DPrintPreviewDialogPrivate::marginsLayout(bool adapted)
{
    if (adapted) {
        spacer->changeSize(130, 72);
    } else {
        spacer->changeSize(0, 72);
    }
}

void DPrintPreviewDialogPrivate::initdata()
{
    QStringList itemlist;
    itemlist << QPrinterInfo::availablePrinterNames()
             << qApp->translate("DPrintPreviewDialogPrivate", "Print to PDF")
             << qApp->translate("DPrintPreviewDialogPrivate", "Save as Image");
    printDeviceCombo->addItems(itemlist);
    QString defauledevice = QPrinterInfo::defaultPrinterName();
    for (int i = 0; i < itemlist.size(); ++i) {
        if (defauledevice.compare(itemlist.at(i)) == 0) {
            printDeviceCombo->setCurrentIndex(i);
            break;
        }
    }
    _q_pageRangeChanged(0);
    _q_pageMarginChanged(0);
    _q_printerChanged(printDeviceCombo->currentIndex());
    scaleGroup->button(1)->setChecked(true);
    orientationgroup->button(0)->setChecked(true);
    scaleRateEdit->setValue(100);
    scaleRateEdit->setEnabled(false);
    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Duplex_TypeControl, false);
    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_NPrint_Numbers, false);
    isInited = true;
    fontSizeMore = true;
}

void DPrintPreviewDialogPrivate::initconnections()
{
    Q_Q(DPrintPreviewDialog);

    QObject::connect(pview, QOverload<DPrinter *>::of(&DPrintPreviewWidget::paintRequested), q, QOverload<DPrinter *>::of(&DPrintPreviewDialog::paintRequested));
    QObject::connect(pview, QOverload<DPrinter *, const QVector<int> &>::of(&DPrintPreviewWidget::paintRequested), q, QOverload<DPrinter *, const QVector<int> &>::of(&DPrintPreviewDialog::paintRequested));

    QObject::connect(advanceBtn, &QPushButton::clicked, q, [this] { this->showadvancesetting(); });
    QObject::connect(printDeviceCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_printerChanged(int)));
    QObject::connect(cancelBtn, &DPushButton::clicked, q, &DPrintPreviewDialog::reject);
    QObject::connect(pageRangeCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_pageRangeChanged(int)));
    QObject::connect(marginsCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_pageMarginChanged(int)));
    QObject::connect(printBtn, SIGNAL(clicked(bool)), q, SLOT(_q_startPrint(bool)));
    QObject::connect(waterColorBtn, SIGNAL(clicked(bool)), q, SLOT(_q_colorButtonCliked(bool)));
    QObject::connect(colorModeCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_ColorModeChange(int)));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QObject::connect(orientationgroup, SIGNAL(buttonClicked(int)), q, SLOT(_q_orientationChanged(int)));
#else
    QObject::connect(orientationgroup, SIGNAL(idClicked(int)), q, SLOT(_q_orientationChanged(int)));
#endif
    QObject::connect(waterTextCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_textWaterMarkModeChanged(int)));
    QObject::connect(inorderCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_printOrderComboIndexChanged(int)));
    QObject::connect(waterTextEdit, SIGNAL(editingFinished()), q, SLOT(_q_customTextWatermarkFinished()));
    QObject::connect(pagePerSheetCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_pagePersheetComboIndexChanged(int)));
    QObject::connect(picPathEdit->lineEdit(), &QLineEdit::textEdited, q, [this] { this->customPictureWatermarkChoosed(picPathEdit->text()); });
    QObject::connect(picPathEdit, &DFileChooserEdit::fileChoosed, q, [this](const QString &filePath) { this->customPictureWatermarkChoosed(filePath); });

    QObject::connect(sizeBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), q, [this](int value) {
        waterSizeSlider->setValue(value);
    });
    QObject::connect(opaBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), q, [this](int value) {
        wmOpaSlider->setValue(value);
    });
    QObject::connect(fontCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), q, [this] {
        QFont font(fontCombo->currentText());
        font.setPointSize(WATERFONT_SIZE);
        pview->setWaterMarkFont(font);
    });
    QObject::connect(pickColorWidget, SIGNAL(selectColorButton(QColor)), q, SLOT(_q_selectColorButton(QColor)));
    QObject::connect(waterPosCombox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), q, [this](int index) {
        if (index == waterPosCombox->count() - 1) {
            pview->setWaterMarkLayout(WATERLAYOUT_CENTER);
        } else {
            pview->setWaterMarkLayout(WATERLAYOUT_TILED);
        }
    });
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QObject::connect(directGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked), q,
#else
    QObject::connect(directGroup, &QButtonGroup::buttonClicked, q,
#endif
    [this](QAbstractButton *btn) {
        auto index = directGroup->id(btn);
        directGroup->button(index)->setChecked(true);
        directChoice = index;
        pview->setOrder(DPrintPreviewWidget::Order(index));
    });
    QObject::connect(inclinatBox, &QSpinBox::editingFinished, q, [this]() {
        _d_setSpinboxDefaultValue(spinboxTextCaches, inclinatBox);
        pview->setWaterMarkRotate(inclinatBox->value());
    });
    QObject::connect(waterSizeSlider, &DSlider::valueChanged, q, [this](int value) {
        sizeBox->setValue(value);
        qreal m_value = static_cast<qreal>(value) / 100.00;
        pview->setWaterMarkScale(m_value);
    });
    QObject::connect(wmOpaSlider, &DSlider::valueChanged, q, [this](int value) {
        opaBox->setValue(value);
        qreal m_value = static_cast<qreal>(value) / 100.00;
        pview->setWaterMarkOpacity(m_value);
    });
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QObject::connect(printOrderGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked), q,
#else
    QObject::connect(printOrderGroup, &QButtonGroup::buttonClicked, q,
#endif
    [this](QAbstractButton *btn) {
        Q_Q(DPrintPreviewDialog);
        auto index = printOrderGroup->id(btn);
        if (index == 0) {
            settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_PageOrder_TypeControl, false);
            // 此时不是按照文件路径打印 将并打选项开启
            if (q->printFromPath().isEmpty())
                settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_NPrintWidget, true);
        } else {
            settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_PageOrder_TypeControl, true);
            settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_NPrintWidget, false);
        }
    });
    QObject::connect(waterMarkBtn, &DSwitchButton::checkedChanged, q, [this](bool checked) { this->waterMarkBtnClicked(checked); });
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QObject::connect(waterTypeGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked), q,
#else
    QObject::connect(waterTypeGroup, &QButtonGroup::buttonClicked, q,
#endif
    [this](QAbstractButton* btn) {
        auto index = waterTypeGroup->id(btn);
        this->watermarkTypeChoosed(index);
    });
    QObject::connect(pageRangeEdit, &DLineEdit::editingFinished, [this] {
        _q_customPagesFinished();
    });
    QObject::connect(pageRangeEdit, &DLineEdit::focusChanged, q, [this](bool onFocus) {
        isOnFocus = true;
        if (pageRangeEdit->text().right(1) == "-" && !onFocus) {
            this->_q_customPagesFinished();
        } else if (pageRangeEdit->text().isEmpty() && !onFocus) {
            pageRangeError(NullTip);
        }
    });
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QObject::connect(sidebysideCheckBox, &DCheckBox::checkStateChanged, q, [this](int status) {
#else
    QObject::connect(sidebysideCheckBox, &DCheckBox::stateChanged, q, [this](int status) {
#endif
        if (status == 0) {
            if (isActualPrinter(printDeviceCombo->currentText()))
                settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_PageOrder_SequentialPrint, true);
            setPageLayoutEnable(false);
            pview->setImposition(DPrintPreviewWidget::One);
            originTotalPageLabel->setVisible(false);
        } else {
            settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_PageOrder_SequentialPrint, false);
            printOrderGroup->button(0)->setChecked(true);
            setPageLayoutEnable(true);
            directGroup->button(directChoice)->setChecked(true);
            _q_pagePersheetComboIndexChanged(pagePerSheetCombo->currentIndex());
            originTotalPageLabel->setVisible(true);
        }
    });
    QObject::connect(jumpPageEdit->lineEdit(), &QLineEdit::textChanged, q, [ this ](QString str) {
        if (str.toInt() > totalPageLabel->text().toInt())
            jumpPageEdit->lineEdit()->setText(totalPageLabel->text());
    });

    QObject::connect(pview, &DPrintPreviewWidget::totalPages, q, [this](int pages) {
        int targetPage = pview->targetPageCount(pages);
        jumpPageEdit->setRange(FIRST_PAGE, targetPage);
        totalPageLabel->setText(QString::number(targetPage));
        totalPages = pages;
        originTotalPageLabel->setText(QString("(%1)").arg(pages));
        if (sidebysideCheckBox->isChecked())
            originTotalPageLabel->setVisible(true);
        else
            originTotalPageLabel->setVisible(false);
        jumpPageEdit->setMaximum(targetPage);
        setTurnPageBtnStatus();
        _q_customPagesFinished();
    });
    QObject::connect(pview, &DPrintPreviewWidget::pagesCountChanged, [this](int pages) {
        totalPageLabel->setNum(pview->targetPageCount(pages));
        if (pview->isAsynPreview()) {
            originTotalPageLabel->setText(QString("(%1)").arg(pview->originPageCount()));
            if (sidebysideCheckBox->isChecked())
                originTotalPageLabel->setVisible(true);
            else
                originTotalPageLabel->setVisible(false);
        }
        setTurnPageBtnStatus();
    });
    QObject::connect(firstBtn, &DIconButton::clicked, pview, &DPrintPreviewWidget::turnBegin);
    QObject::connect(prevPageBtn, &DIconButton::clicked, pview, &DPrintPreviewWidget::turnFront);
    QObject::connect(nextPageBtn, &DIconButton::clicked, pview, &DPrintPreviewWidget::turnBack);
    QObject::connect(lastBtn, &DIconButton::clicked, pview, &DPrintPreviewWidget::turnEnd);
    QObject::connect(pview, &DPrintPreviewWidget::currentPageChanged, q, [this](int page) {
        if (jumpPageEdit->value() == page)
            return;
        jumpPageEdit->setValue(page);
        setTurnPageBtnStatus();
    });
    QObject::connect(jumpPageEdit->lineEdit(), &QLineEdit::editingFinished, q, [this]() {
        _d_setSpinboxDefaultValue(spinboxTextCaches, jumpPageEdit);
        pview->setCurrentPage(jumpPageEdit->value());
        setTurnPageBtnStatus();
    });
    QObject::connect(paperSizeCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), q, [this](int) {
        if (paperSizeCombo->count() == 0) {
            printer->setPageSize(QPageSize::A4);
            return ;
        }

        matchFitablePageSize();
        if (isInited) {
            this->marginsUpdate(false);
        }
        if (pview->pageRangeMode() == DPrintPreviewWidget::SelectPage && pageRangeCombo->isEnabled())
            _q_customPagesFinished();

    });

    QObject::connect(scaleRateEdit->lineEdit(), &QLineEdit::editingFinished, q, [this] {
        if (scaleGroup->checkedId() == SCALE) {
            _d_setSpinboxDefaultValue(spinboxTextCaches, scaleRateEdit);
            if (scaleRateEdit->value() < 10)
                scaleRateEdit->setValue(10);
            qreal scale = scaleRateEdit->value() / 100.0;
            pview->setScale(scale);
            pview->updateView();
        }
    });
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QObject::connect(scaleGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked), q,
#else
    QObject::connect(scaleGroup, &QButtonGroup::buttonClicked, q, 
#endif
[this](QAbstractButton *btn) {
        auto id = scaleGroup->id(btn);
        if (id == ACTUAL_SIZE) {
            pview->setScale(1);
            scaleRateEdit->setEnabled(false);
        } else if (id == SCALE) {
            qreal scale = scaleRateEdit->value() / 100.0;
            pview->setScale(scale);
            scaleRateEdit->setEnabled(true);
        }
        pview->updateView();
    });

    QObject::connect(copycountspinbox, &QSpinBox::editingFinished, q, [this]() {
        _d_setSpinboxDefaultValue(spinboxTextCaches, copycountspinbox);
    });

    QObject::connect(marginTopSpin, SIGNAL(valueChanged(double)), q, SLOT(_q_marginspinChanged(double)));
    QObject::connect(marginRightSpin, SIGNAL(valueChanged(double)), q, SLOT(_q_marginspinChanged(double)));
    QObject::connect(marginLeftSpin, SIGNAL(valueChanged(double)), q, SLOT(_q_marginspinChanged(double)));
    QObject::connect(marginBottomSpin, SIGNAL(valueChanged(double)), q, SLOT(_q_marginspinChanged(double)));
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QObject::connect(duplexCheckBox, SIGNAL(checkStateChanged(int)), q, SLOT(_q_checkStateChanged(int)));
#else
    QObject::connect(duplexCheckBox, SIGNAL(stateChanged(int)), q, SLOT(_q_checkStateChanged(int)));
#endif
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, pview, &DPrintPreviewWidget::themeTypeChanged);
    QObject::connect(marginTopSpin, SIGNAL(editingFinished()), q, SLOT(_q_marginEditFinished()));
    QObject::connect(marginRightSpin, SIGNAL(editingFinished()), q, SLOT(_q_marginEditFinished()));
    QObject::connect(marginLeftSpin, SIGNAL(editingFinished()), q, SLOT(_q_marginEditFinished()));
    QObject::connect(marginBottomSpin, SIGNAL(editingFinished()), q, SLOT(_q_marginEditFinished()));
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
                     q, [this, q](DGuiApplicationHelper::ColorType themeType) {
        DTitlebar *titlebar = q->findChild<DTitlebar *>();
        DPalette pa = DPaletteHelper::instance()->palette(titlebar);
        pa.setBrush(DPalette::Window, pa.base());
        DPaletteHelper::instance()->setPalette(titlebar, pa);
        this->themeTypeChange(themeType);
    });
    QObject::connect(marginTopSpin->lineEdit(), SIGNAL(textEdited(const QString &)), q, SLOT(_q_spinboxValueEmptyChecked(const QString &)));
    QObject::connect(marginRightSpin->lineEdit(), SIGNAL(textEdited(const QString &)), q, SLOT(_q_spinboxValueEmptyChecked(const QString &)));
    QObject::connect(marginLeftSpin->lineEdit(), SIGNAL(textEdited(const QString &)), q, SLOT(_q_spinboxValueEmptyChecked(const QString &)));
    QObject::connect(marginBottomSpin->lineEdit(), SIGNAL(textEdited(const QString &)), q, SLOT(_q_spinboxValueEmptyChecked(const QString &)));
    QObject::connect(jumpPageEdit->lineEdit(), SIGNAL(textEdited(const QString &)), q, SLOT(_q_spinboxValueEmptyChecked(const QString &)));
    QObject::connect(copycountspinbox->lineEdit(), SIGNAL(textEdited(const QString &)), q, SLOT(_q_spinboxValueEmptyChecked(const QString &)));
    QObject::connect(scaleRateEdit->lineEdit(), SIGNAL(textEdited(const QString &)), q, SLOT(_q_spinboxValueEmptyChecked(const QString &)));
    QObject::connect(inclinatBox->lineEdit(), SIGNAL(textEdited(const QString &)), q, SLOT(_q_spinboxValueEmptyChecked(const QString &)));
}

void DPrintPreviewDialogPrivate::setfrmaeback(DFrame *frame)
{
    frame->setLineWidth(0);
    DPalette pa = DPaletteHelper::instance()->palette(frame);
    pa.setBrush(DPalette::Base, pa.itemBackground());
    DPaletteHelper::instance()->setPalette(frame, pa);
}

void DPrintPreviewDialogPrivate::showadvancesetting()
{
    if (advancesettingwdg->isHidden()) {
        advancesettingwdg->show();
        advanceBtn->setText(qApp->translate("DPrintPreviewDialogPrivate", "Collapse"));
        advanceBtn->setIcon(DIconTheme::findQIcon("printer_dropup"));
    } else {
        advancesettingwdg->hide();
        advanceBtn->setText(qApp->translate("DPrintPreviewDialogPrivate", "Advanced"));
        advanceBtn->setIcon(DIconTheme::findQIcon("printer_dropdown"));
    }
}

/*!
  DPrintPreviewDialogPrivate::setupPrinter 设置打印的相关基础属性和高级属性
 */
void DPrintPreviewDialogPrivate::setupPrinter()
{
    //基础设置
    //设置打印份数
    printer->setCopyCount(copycountspinbox->value());
    //设置打印方向
    if (orientationgroup->checkedId() == 0)
        printer->setPageOrientation(QPageLayout::Orientation::Portrait);
    else {
        printer->setPageOrientation(QPageLayout::Orientation::Landscape);
    }
    //高级设置
    //设置纸张大小
    matchFitablePageSize();
    //设置双面打印
    if (duplexCheckBox->isChecked()) {
        if (duplexCombo->count() == 1) {
            if (supportedDuplexFlag)
                printer->setDuplex(QPrinter::DuplexLongSide);
            else
                printer->setDuplex(QPrinter::DuplexShortSide);
        } else {
            if (duplexCombo->currentText() == qApp->translate("DPrintPreviewDialogPrivate", "Flip on long edge")) {
                printer->setDuplex(QPrinter::DuplexLongSide);
            } else {
                printer->setDuplex(QPrinter::DuplexShortSide);
            }
        }
    } else {
        printer->setDuplex(QPrinter::DuplexNone);
    }
    //设置色彩打印
    if (supportedColorMode) {
        QByteArray currentColorModel = pview->printerColorModel().isEmpty() ? QByteArrayLiteral("RGB") : pview->printerColorModel();
        QCUPSSupport::setCupsOption(printer, "ColorModel", currentColorModel);
    } else {
        QCUPSSupport::setCupsOption(printer, "ColorModel", "Gray");
    }
    //设置纸张打印边距
    printer->setPageMargins(QMarginsF(marginLeftSpin->value(), marginTopSpin->value(), marginRightSpin->value(), marginBottomSpin->value()), QPageLayout::Millimeter);
    //设置打印逐份打印和逐页打印的控制标志
    if (printOrderGroup->checkedId() == 1) {
        //判断若是逐页打印，是否为第一页在前
        bool isFirst = true;
        if (inorderCombo->currentIndex() == 1) {
            isFirst = false;
        }

        pview->isPageByPage(printer->copyCount(), isFirst);
        //由于手动设置逐页打印，这种情况下，输出打印机的打印份数为1
        printer->setCopyCount(1);
    }
}

void DPrintPreviewDialogPrivate::judgeSupportedAttributes(const QString &lastPaperSize)
{
    Q_Q(DPrintPreviewDialog);
    QPrinterInfo updateinfo(*printer);

    QStringList pageSizeList;
    int index = -1;
    for (int i = 0; i < updateinfo.supportedPageSizes().size(); i++) {
        pageSizeList.append(updateinfo.supportedPageSizes().at(i).name());
        if (index == -1 && updateinfo.supportedPageSizes().at(i).id() == QPageSize::PageSizeId::A4) {
            index = i;
        }
    }
    paperSizeCombo->addItems(pageSizeList);
    updateSubControlSettings(DPrintPreviewSettingInfo::PS_PaperSize);
    if (pageSizeList.contains(lastPaperSize)) {
        paperSizeCombo->setCurrentText(lastPaperSize);
    } else {
        //调用绘制预览
        paperSizeCombo->blockSignals(false);
        paperSizeCombo->setCurrentIndex(index == - 1 ? 0 : index);
    }

    //判断当前打印机是否支持双面打印，不支持禁用双面打印按钮，pdf不做判断
    QString lastDuplexComboText = duplexCombo->currentText();
    duplexCombo->clear();
    if (updateinfo.supportedDuplexModes().contains(QPrinter::DuplexLongSide) || updateinfo.supportedDuplexModes().contains(QPrinter::DuplexShortSide)) {
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_DuplexWidget, true);
        if (!updateinfo.supportedDuplexModes().contains(QPrinter::DuplexLongSide)) {
            duplexCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Flip on short edge"));
            updateSubControlSettings(DPrintPreviewSettingInfo::PS_PrintDuplex);
            supportedDuplexFlag = false;
        } else if (!updateinfo.supportedDuplexModes().contains(QPrinter::DuplexShortSide)) {
            duplexCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Flip on long edge"));
            updateSubControlSettings(DPrintPreviewSettingInfo::PS_PrintDuplex);
            supportedDuplexFlag = true;
        } else if (updateinfo.supportedDuplexModes().contains(QPrinter::DuplexLongSide) && updateinfo.supportedDuplexModes().contains(QPrinter::DuplexShortSide)) {
            duplexCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Flip on long edge"));
            duplexCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Flip on short edge"));
            updateSubControlSettings(DPrintPreviewSettingInfo::PS_PrintDuplex);
            duplexCombo->setCurrentText(lastDuplexComboText);
        }
    } else {
        duplexCheckBox->setChecked(false);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_DuplexWidget, false);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Duplex_TypeControl, false);
    }
}

/*!
  DPrintPreviewDialogPrivate::setMininumMargins 如果边距小于最小边距,设置成最小边距
 */
void DPrintPreviewDialogPrivate::setMininumMargins()
{
    if (marginLeftSpin->value() < minnumMargins.first())
        marginLeftSpin->setValue(minnumMargins.first());
    if (marginTopSpin->value() < minnumMargins.at(1))
        marginTopSpin->setValue(minnumMargins.at(1));
    if (marginRightSpin->value() < minnumMargins.at(2))
        marginRightSpin->setValue(minnumMargins.at(2));
    if (marginBottomSpin->value() < minnumMargins.last())
        marginBottomSpin->setValue(minnumMargins.last());
}

/*!
  \brief DPrintPreviewDialogPrivate::marginsUpdate 切换打印机和纸张切换对于边距的刷新和预览页面的刷新
  \a isPrinterchanged 判断是打印机切换或者是纸张切换
 */
void DPrintPreviewDialogPrivate::marginsUpdate(bool isPrinterChanged)
{
    //切换打印机,如果无属性变换,不刷新预览界面,如果属性变换,刷新一次预览界面,如果是纸张大小变换,刷新一次
    minnumMargins.clear();
    QMarginsF minumMargin = printer->pageLayout().minimumMargins();
    minnumMargins << minumMargin.left() << minumMargin.top() << minumMargin.right() << minumMargin.bottom();
    //若是窄边距,切换变成当前打印机最小边据,刷新预览界面
    if ((marginsCombo->currentIndex() == 0)) {
        _q_pageMarginChanged(0);
    } else if (marginsCombo->currentIndex() == marginsCombo->count() - 1) {
        //当边距选项为自定义选项,若切换之前的边距有小于当前printer的最小边距,刷新边距数据,刷新预览界面,反之,若全部边距大于最小边距,不刷新数据,若是纸张变化,刷新预览界面
        if (marginLeftSpin->value() < minumMargin.left() || marginTopSpin->value() < minumMargin.top() || marginRightSpin->value() < minumMargin.right() || marginBottomSpin->value() < minumMargin.bottom()) {
            setMininumMargins();
            QMarginsF currentMargin = QMarginsF(marginLeftSpin->value(), marginTopSpin->value(), marginRightSpin->value(), marginBottomSpin->value());
            printer->setPageMargins(currentMargin, QPageLayout::Millimeter);
            pview->updatePreview();
        } else {
            if (!isPrinterChanged)
                pview->updatePreview();
        }
    } else {
        //不是窄或者自定义选项,若是纸张切换,刷新预览界面
        if (!isPrinterChanged)
            pview->updatePreview();
    }
}

void DPrintPreviewDialogPrivate::themeTypeChange(DGuiApplicationHelper::ColorType themeType)
{
    Q_Q(DPrintPreviewDialog);
    QList<DFrame *> m_frameList = q->findChildren<DFrame *>();
    QList<DBackgroundGroup *> m_back = q->findChildren<DBackgroundGroup *>();

    DPalette pa;
    if (themeType == DGuiApplicationHelper::LightType) {
        pa.setBrush(DPalette::Base, QColor(0, 0, 0, 8));
    } else if (themeType == DGuiApplicationHelper::DarkType) {
        pa.setBrush(DPalette::Base, QColor(255, 255, 255, 10));
    }
    pa.setBrush(DPalette::FrameBorder, pa.base());
    for (int i = 1; i < m_frameList.size(); i++) {
        DPaletteHelper::instance()->setPalette(m_frameList.at(i), pa);
    }
    for (int i = 0; i < m_back.size(); i++) {
        if (m_back.at(i)->objectName() == "backGround")
            continue;
        DPaletteHelper::instance()->setPalette(m_back.at(i), pa);
    }
}

/*!
  DPrintPreviewDialogPrivate::setPageIsLegal 检查自定义页码是否合法,不合法的话设置警告颜色,打印保存按钮置灰
 */
void DPrintPreviewDialogPrivate::setPageIsLegal(bool islegal)
{
    printBtn->setEnabled(islegal);
    pageRangeEdit->setAlert(!islegal);
}

void DPrintPreviewDialogPrivate::tipSelected(TipsNum tipNum)
{
    switch (tipNum) {
    case NullTip:
        pageRangeEdit->showAlertMessage(qApp->translate("DPrintPreviewDialogPrivate", "Input page numbers please"), static_cast<DFrame *>(pageRangeEdit->parent()), 2000);
        break;
    case MaxTip:
        pageRangeEdit->showAlertMessage(qApp->translate("DPrintPreviewDialogPrivate", "Maximum page number reached"), static_cast<DFrame *>(pageRangeEdit->parent()), 2000);
        break;
    case CommaTip:
        pageRangeEdit->showAlertMessage(qApp->translate("DPrintPreviewDialogPrivate", "Input English comma please"), static_cast<DFrame *>(pageRangeEdit->parent()), 2000);
        break;
    case FormatTip:
        pageRangeEdit->showAlertMessage(qApp->translate("DPrintPreviewDialogPrivate", "Input page numbers like this: 1,3,5-7,11-15,18,21"), static_cast<DFrame *>(pageRangeEdit->parent()), 2000);
        break;
    default:
        break;
    }
}

QVector<int> DPrintPreviewDialogPrivate::checkDuplication(QVector<int> data)
{
    for (int i = 0; i < data.size(); i++) {
        for (int j = i + 1; j < data.size(); j++) {
            if (data.at(i) == data.at(j)) {
                data.remove(j);
                j--;
            }
        }
    }
    std::sort(data.begin(), data.end());
    return data;
}

void DPrintPreviewDialogPrivate::updateAllControlSettings()
{
    if (!settingUpdateTimer.isActive())
        settingUpdateTimer.start(0, q_func());
}

void DPrintPreviewDialogPrivate::updateAllContentSettings_impl()
{
    for (int i = DPrintPreviewSettingInfo::PS_Printer; i <= DPrintPreviewSettingInfo::PS_Watermark; ++i)
        updateSubControlSettings(static_cast<DPrintPreviewSettingInfo::SettingType>(i));
}

void DPrintPreviewDialogPrivate::updateAllControlStatus()
{
    for (int i = 0; i < DPrintPreviewSettingInterface::SC_ControlCount; ++i)
        settingHelper->updateSettingStatus(static_cast<DPrintPreviewSettingInterface::SettingSubControl>(i));
}

void DPrintPreviewDialogPrivate::updateSubControlSettings(DPrintPreviewSettingInfo::SettingType setting)
{
    DPrintPreviewSettingInfo *info = settingHelper->loadInfo(setting);
    settingHelper->updateSettingInfo(info);
    if (info) {
        delete info;
    }
}

/*!
  \brief DPrintPreviewDialogPrivate::setEnable 设置纸张范围自定义可输入状态
  \a value 判断选择的范围类型
  \a combox 选择的具体的combox
 */
void DPrintPreviewDialogPrivate::setEnable(const int &value, DComboBox *combox)
{
    if (combox == pageRangeCombo) {
        if (value != pageRangeCombo->count() - 1) {
            settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_PageRange_SelectEdit, false);
        } else {
            settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_PageRange_SelectEdit, true);
        }
    }
}

/*!
  \brief DPrintPreviewDialogPrivate::setEnable 设置翻页按钮状态
 */
void DPrintPreviewDialogPrivate::setTurnPageBtnStatus()
{
    int currentPage = jumpPageEdit->value();
    int totalPage = totalPageLabel->text().toInt();
    if (currentPage > 1 && currentPage < totalPage) {
        firstBtn->setEnabled(true);
        prevPageBtn->setEnabled(true);
        nextPageBtn->setEnabled(true);
        lastBtn->setEnabled(true);
    } else if (currentPage == 1 && currentPage == totalPage) {
        firstBtn->setEnabled(false);
        prevPageBtn->setEnabled(false);
        nextPageBtn->setEnabled(false);
        lastBtn->setEnabled(false);
    } else if (currentPage == 1) {
        firstBtn->setEnabled(false);
        prevPageBtn->setEnabled(false);
        nextPageBtn->setEnabled(true);
        lastBtn->setEnabled(true);
    } else if (totalPage == 0) {
        firstBtn->setEnabled(false);
        prevPageBtn->setEnabled(false);
        nextPageBtn->setEnabled(false);
        lastBtn->setEnabled(false);
    } else if (currentPage == totalPage) {
        firstBtn->setEnabled(true);
        prevPageBtn->setEnabled(true);
        nextPageBtn->setEnabled(false);
        lastBtn->setEnabled(false);
    }
}

/*!
  \brief DPrintPreviewDialogPrivate::watermarkTypeChoosed 选取水印类型
  \a index 判断选取的水印类型
 */
void DPrintPreviewDialogPrivate::watermarkTypeChoosed(int index)
{
    D_Q(DPrintPreviewDialog);

    if (index == 0) {
        pview->refreshBegin();
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_TextType, true);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_TextFont, true);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_ImageEdit, false);
        if (colorModeCombo->count() == 2 &&
                colorModeCombo->currentText() == qApp->translate("DPrintPreviewDialogPrivate", "Color"))
            settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_TextColor, true);
        _q_textWaterMarkModeChanged(waterTextCombo->currentIndex());
        initWaterSettings();
        // 获取可支持的所有字体
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QFontDatabase fdb;
        QStringList fontList = fdb.families(QFontDatabase::Any);
#else
        QStringList fontList = QFontDatabase::families(QFontDatabase::Any);
#endif
        Q_FOREACH (const QString &font, fontList) {
            if (fontCombo->findText(font) != -1)
                continue;

            fontCombo->addItem(font);
        }

        if (!q->property("_d_print_waterIsInit").toBool()) {
            // 初始化才使用系统默认字体 下次切换时保留上一次字体
            //通过字体信息,当中文字体的情况下将英文转换为中文
            QFont font;
            QFontInfo fontName(font);
            QString defaultFontName = fontName.family();
            //默认初始化水印字体是系统当前字体
            for (QString itemName : fontList) {
                if (itemName == defaultFontName) {
                    fontCombo->setCurrentText(itemName);
                }
            }
            q->setProperty("_d_print_waterIsInit", true);
        }
        pview->setWaterMarkType(Type_Text);
        pview->refreshEnd();
    } else if (index == 1) {
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_TextType, false);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_TextFont, false);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_TextColor, false);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_CustomText, false);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_ImageEdit, true);
        pview->setWaterMarkType(Type_Image);
    }
    typeChoice = index;
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_printerChanged 根据选取不同的打印设备刷新界面和属性
  \a index 判断选取的打印设备
 */
void DPrintPreviewDialogPrivate::_q_printerChanged(int index)
{
    Q_Q(DPrintPreviewDialog);
    QString lastPaperSize = paperSizeCombo->currentText();
    paperSizeCombo->clear();
    paperSizeCombo->blockSignals(true);
    QString currentName = printDeviceCombo->itemText(index);
    colorModeCombo->blockSignals(true);
    colorModeCombo->clear();
    colorModeCombo->blockSignals(false);
    if (isActualPrinter(currentName)) {
        //actual printer
        if (printer) {
            if (q->printFromPath().isEmpty() && !sidebysideCheckBox->isChecked()) {
                settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_PageOrder_SequentialPrint, true);
            } else {
                settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_PageOrder_SequentialPrint, false);
            }
            settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_CopiesWidget, true);
            settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_PaperSizeWidget, true);
            settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_ColorModeWidget, true);
            printer->setPrinterName(currentName);
            printBtn->setText(qApp->translate("DPrintPreviewDialogPrivate", "Print", "button"));
            judgeSupportedAttributes(lastPaperSize);
        }
        //判断当前打印机是否支持彩色打印，不支持彩色打印删除彩色打印选择选项，pdf不做判断
        QPlatformPrinterSupport *ps = QPlatformPrinterSupportPlugin::get();
        QPrintDevice currentDevice = ps->createPrintDevice(printDeviceCombo->currentText());
        supportedColorMode = false;
        if (currentDevice.supportedColorModes().contains(QPrint::Color)) {
            if (!isInited) {
                waterColor = QColor("#6f6f6f");
                _q_selectColorButton(waterColor);
                pickColorWidget->convertColor(waterColor);
                pickColorWidget->setRgbEdit(waterColor);
            }
            colorModeCombo->blockSignals(true);
            colorModeCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Color"));
            // Ensure that the signal CurrentIndexChanged is triggered afterwards
            colorModeCombo->setCurrentIndex(-1);
            colorModeCombo->blockSignals(false);
            updateSubControlSettings(DPrintPreviewSettingInfo::PS_ColorMode);
            supportedColorMode = true;
        }
        if (currentDevice.supportedColorModes().contains(QPrint::GrayScale)) {
            colorModeCombo->blockSignals(true);
            colorModeCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Grayscale"));
            // Ensure that the signal CurrentIndexChanged is triggered afterwards
            colorModeCombo->setCurrentIndex(-1);
            colorModeCombo->blockSignals(false);
            updateSubControlSettings(DPrintPreviewSettingInfo::PS_ColorMode);
            waterColor = QColor("#6f6f6f");
            _q_selectColorButton(waterColor);
            pickColorWidget->convertColor(waterColor);
            pickColorWidget->setRgbEdit(waterColor);
        }
        if (supportedColorMode) {
            colorModeCombo->setCurrentText(qApp->translate("DPrintPreviewDialogPrivate", "Color"));
            settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_TextColor, true);
        } else {
            colorModeCombo->setCurrentText(qApp->translate("DPrintPreviewDialogPrivate", "Grayscale"));
            settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_TextColor, false);
        }
    } else {
        //pdf
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_CopiesWidget, false);
        copycountspinbox->setValue(1);
        duplexCheckBox->setCheckState(Qt::Unchecked);
        duplexCombo->clear();
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_DuplexWidget, false);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_TextColor, true);
        colorModeCombo->blockSignals(true);
        colorModeCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Color"));
        colorModeCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Grayscale"));
        // Ensure that the signal CurrentIndexChanged is triggered afterwards
        colorModeCombo->setCurrentIndex(-1);
        updateSubControlSettings(DPrintPreviewSettingInfo::PS_ColorMode);
        colorModeCombo->blockSignals(false);
        colorModeCombo->setCurrentIndex(0);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_ColorModeWidget, false);
        supportedColorMode = true;
        printBtn->setText(qApp->translate("DPrintPreviewDialogPrivate", "Save", "button"));
        paperSizeCombo->setCurrentIndex(1);
        QStringList pdfPaperSize = QStringList() << "A3"
                                                 << "A4"
                                                 << "A5"
                                                 << "B4"
                                                 << "B5"
                                                 << "8K"
                                                 << "16K";
        paperSizeCombo->addItems(pdfPaperSize);
        updateSubControlSettings(DPrintPreviewSettingInfo::PS_PaperSize);
        if (pdfPaperSize.contains(lastPaperSize))
            paperSizeCombo->setCurrentText(lastPaperSize);
        else {
            //调用绘制预览
            paperSizeCombo->blockSignals(false);
            paperSizeCombo->setCurrentIndex(1);
        }
        printer->setPrinterName("");
        printOrderGroup->button(0)->setChecked(true);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_PageOrder_SequentialPrint, false);
        if (!isInited) {
            waterColor = QColor("#6f6f6f");
            _q_selectColorButton(waterColor);
            pickColorWidget->convertColor(waterColor);
            pickColorWidget->setRgbEdit(waterColor);
        }
    }

    marginsUpdate(true);
    if (pview->pageRangeMode() == DPrintPreviewWidget::SelectPage && pageRangeCombo->isEnabled())
        _q_customPagesFinished();
    paperSizeCombo->blockSignals(false);
    if (isInited)
        updateAllControlSettings();
}

/*!
  \brief DPrintPreviewDialogPrivate::slotPageRangeCombox 根据选取的不同纸张范围类型设置纸张范围
  \a index 判断不同的范围类型
 */
void DPrintPreviewDialogPrivate::_q_pageRangeChanged(int index)
{
    setEnable(index, pageRangeCombo);
    pageRangeEdit->setVisible(index == DPrintPreviewWidget::SelectPage);
    pageRangeEdit->lineEdit()->setPlaceholderText("");
    pageRangeEdit->setText("");
    if (index == DPrintPreviewWidget::AllPage || index == DPrintPreviewWidget::CurrentPage) {
        pview->setPageRangeMode((DPrintPreviewWidget::PageRange)index);
        setPageIsLegal(true);
        if (totalPages != 0) {
            totalPageLabel->setNum(pview->targetPageCount(totalPages));
            if (isInited) {
                pview->setPageRange(FIRST_PAGE, pview->originPageCount());
            }
            if (index == DPrintPreviewWidget::AllPage)
                pview->setCurrentPage(FIRST_PAGE);
        }
    } else {
        pview->setPageRangeMode(DPrintPreviewWidget::SelectPage);
        if (lastPageRange.isEmpty()) {
            setPageIsLegal(false);

        } else {
            pageRangeEdit->setText(lastPageRange);
            _q_customPagesFinished();
        }
        if (pageRangeEdit->isAlert()) {
            pageRangeEdit->clear();
            pageRangeEdit->setAlert(false);
            pageRangeEdit->lineEdit()->setPlaceholderText(qApp->translate("DPrintPreviewDialogPrivate", "For example, 1,3,5-7,11-15,18,21"));
        }
    }
    setTurnPageBtnStatus();
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_pageMarginChanged 根据选择不同的设置边距类型设置边距
  \a index 判断选取的不同的设置边距的类型
 */
void DPrintPreviewDialogPrivate::_q_pageMarginChanged(int index)
{
    setEnable(index, marginsCombo);
    marginLeftSpin->blockSignals(true);
    marginTopSpin->blockSignals(true);
    marginRightSpin->blockSignals(true);
    marginBottomSpin->blockSignals(true);
    if (index == 1) {
        marginTopSpin->setValue(NORMAL_MODERATE_TOP_BOTTRM);
        marginLeftSpin->setValue(NORMAL_LEFT_RIGHT);
        marginRightSpin->setValue(NORMAL_LEFT_RIGHT);
        marginBottomSpin->setValue(NORMAL_MODERATE_TOP_BOTTRM);
        printer->setPageMargins(QMarginsF(NORMAL_LEFT_RIGHT, NORMAL_MODERATE_TOP_BOTTRM, NORMAL_LEFT_RIGHT, NORMAL_MODERATE_TOP_BOTTRM), QPageLayout::Millimeter);
    } else if (index == 2) {
        marginLeftSpin->setValue(MODERATE_LEFT_RIGHT);
        marginTopSpin->setValue(NORMAL_MODERATE_TOP_BOTTRM);
        marginRightSpin->setValue(MODERATE_LEFT_RIGHT);
        marginBottomSpin->setValue(NORMAL_MODERATE_TOP_BOTTRM);
        printer->setPageMargins(QMarginsF(MODERATE_LEFT_RIGHT, NORMAL_MODERATE_TOP_BOTTRM, MODERATE_LEFT_RIGHT, NORMAL_MODERATE_TOP_BOTTRM), QPageLayout::Millimeter);
    } else if (index == 3) {
        marginTopSpin->setValue(printer->pageLayout().minimumMargins().top());
        marginLeftSpin->setValue(printer->pageLayout().minimumMargins().left());
        marginRightSpin->setValue(printer->pageLayout().minimumMargins().right());
        marginBottomSpin->setValue(printer->pageLayout().minimumMargins().bottom());
        printer->setPageMargins(QMarginsF(marginLeftSpin->value(), marginTopSpin->value(), marginRightSpin->value(), marginBottomSpin->value()), QPageLayout::Millimeter);
    } else {
        marginTopSpin->setValue(printer->pageLayout().minimumMargins().top());
        marginLeftSpin->setValue(printer->pageLayout().minimumMargins().left());
        marginRightSpin->setValue(printer->pageLayout().minimumMargins().right());
        marginBottomSpin->setValue(printer->pageLayout().minimumMargins().bottom());
        printer->setPageMargins(QMarginsF(printer->pageLayout().minimumMargins().left(), printer->pageLayout().minimumMargins().top(), printer->pageLayout().minimumMargins().right(), printer->pageLayout().minimumMargins().bottom()), QPageLayout::Millimeter);
    }
    marginLeftSpin->blockSignals(false);
    marginTopSpin->blockSignals(false);
    marginRightSpin->blockSignals(false);
    marginBottomSpin->blockSignals(false);
    if (isInited) {
        pview->updatePreview();
    }

    if (pview->pageRangeMode() == DPrintPreviewWidget::SelectPage && pageRangeCombo->isEnabled())
        _q_customPagesFinished();

    if (marginOldValue.length() > 4)
        marginOldValue.clear();

    marginOldValue << marginTopSpin->value() << marginLeftSpin->value() << marginRightSpin->value() << marginBottomSpin->value();
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_ColorModeChange 根据色彩选项，调整预览图中的色彩为黑白或者彩色
  \a index 当前选择彩色预览还是黑白预览
 */
void DPrintPreviewDialogPrivate::_q_ColorModeChange(int index)
{
    setEnable(index, colorModeCombo);
    //如果打印机不支持彩打
    if (colorModeCombo->count() == 1) {
        pview->setColorMode(DPrinter::GrayScale);
        supportedColorMode = false;
        return;
    }
    if (index == 0) {
        // color
        pview->setColorMode(DPrinter::Color);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_TextColor, true);
        supportedColorMode = true;
    } else {
        // gray
        pview->setColorMode(DPrinter::GrayScale);
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_TextColor, false);
        supportedColorMode = false;
        waterColor = QColor("#6f6f6f");
    }
    _q_selectColorButton(waterColor);
    pickColorWidget->convertColor(waterColor);
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_orientationChanged 根据按钮索引切换打印方向为横向或者纵向
  \a index 当前按钮是横向按钮还是纵向按钮
 */
void DPrintPreviewDialogPrivate::_q_orientationChanged(int index)
{
    if (index == 0) {
        // 纵向按钮
        if (isInited) {
            pview->setOrientation(DPrinter::Portrait);
        }
    } else {
        // 横向按钮
        pview->setOrientation(DPrinter::Landscape);
    }
    if (pview->pageRangeMode() == DPrintPreviewWidget::SelectPage && pageRangeCombo->isEnabled())
        _q_customPagesFinished();
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_customPagesFinished 自定义页面时，页码处理
  \a index
 */
void DPrintPreviewDialogPrivate::_q_customPagesFinished()
{
    if (pageRangeCombo->currentIndex() != 2)
        return;
    QString cuspages = pageRangeEdit->text();
    lastPageRange = cuspages;
    QVector<int> pagesrange;
    setPageIsLegal(true);
    //输入框为空，失去焦点或回车给出相应提示
    if (!cuspages.isEmpty()) {
        //自定义页未输入完整，如“1-”“1,”，若不符合，失去焦点或回车给出相应提示
        if (pageRangeEdit->text().right(1) != "-" && pageRangeEdit->text().right(1) != ",") {
            QStringList list = cuspages.split(",");
            //处理输入的页码
            for (int i = 0; i < list.size(); i++) {
                //输入的页码中，含“-”，对前后数值进行判断以及处理
                if (list.at(i).contains("-")) {
                    QStringList list1 = list.at(i).split("-");
                    bool convertFirst = false;
                    bool convertSercond = false;
                    if (list1.at(0).toInt(&convertFirst) <= list1.at(1).toInt(&convertSercond) && convertFirst && convertSercond) {
                        for (int page = list1.at(0).toInt(); page <= list1.at(1).toInt(); page++) {
                            if (page != 0 && page <= totalPages) {
                                pagesrange.append(page);
                            } else {
                                pageRangeError(MaxTip);
                                return;
                            }
                        }
                    } else { //“-”后值大于前值，则回车自动格式化
                        if (!convertFirst || !convertSercond) {
                            pageRangeError(MaxTip);
                            return;
                        }
                        QString temp = "";
                        temp = list1.at(0);
                        list1.replace(0, list1.at(1));
                        list1.replace(1, temp);
                        QString str = list1.join("-");
                        list.replace(i, str);
                        pageRangeEdit->setText(list.join(","));
                        if (list1.at(0).toInt() > totalPages || list1.at(1).toInt() > totalPages) {
                            pageRangeError(MaxTip);
                            return;
                        } else {
                            for (int page = list1.at(0).toInt(); page <= list1.at(1).toInt(); page++) {
                                pagesrange.append(page);
                            }
                        }
                    }
                } else {
                    if (list.at(i).toInt() != 0 && list.at(i).toInt() <= totalPages) {
                        pagesrange.append(list.at(i).toInt());
                    } else {
                        pageRangeError(MaxTip);
                        return;
                    }
                }
            }
        } else {
            pageRangeError(FormatTip);
            return;
        }
    } else {
        pageRangeError(NullTip);
        return;
    }
    QVector<int> page = checkDuplication(pagesrange);
    pview->setPageRange(page);
}

/*!
  \brief DPrintPreviewDialogPrivate::adjustMargins 根据输入的边距数据调整边距
 */
void DPrintPreviewDialogPrivate::adjustMargins()
{
    setMininumMargins();
    qreal leftMarginF = this->marginLeftSpin->value();
    qreal topMarginF = this->marginTopSpin->value();
    qreal rightMarginF = this->marginRightSpin->value();
    qreal bottomMarginF = this->marginBottomSpin->value();
    if (qFuzzyCompare(topMarginF, marginOldValue[0]) && qFuzzyCompare(leftMarginF, marginOldValue[1]) && qFuzzyCompare(rightMarginF, marginOldValue[2]) && qFuzzyCompare(bottomMarginF, marginOldValue[3]))
        return;

    marginOldValue.clear();
    marginOldValue << topMarginF << leftMarginF << rightMarginF << bottomMarginF;
    this->printer->setPageMargins(QMarginsF(leftMarginF, topMarginF, rightMarginF, bottomMarginF), QPageLayout::Millimeter);
    this->pview->updatePreview();
    if (pview->pageRangeMode() == DPrintPreviewWidget::SelectPage && pageRangeCombo->isEnabled())
        _q_customPagesFinished();
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_marginspinChanged 自定义页边距spinbox值改变
  \a marginValue 改变的值的大小
 */
void DPrintPreviewDialogPrivate::_q_marginspinChanged(double marginValue)
{
    Q_UNUSED(marginValue);
    marginsCombo->blockSignals(true);
    marginsCombo->setCurrentIndex(marginsCombo->count() - 1);
    marginsCombo->blockSignals(false);
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_marginspinChanged 自定义页边距spinbox焦点改变输入结束
 */
void DPrintPreviewDialogPrivate::_q_marginEditFinished()
{
    Q_Q(DPrintPreviewDialog);
    if (DDoubleSpinBox *spinbox = qobject_cast<DDoubleSpinBox *>(q->sender())) {
        if (spinboxTextCaches.contains(spinbox->lineEdit()) && spinboxTextCaches.value(spinbox->lineEdit()).isEmpty()) {
            QVariant defalutVariant = spinbox->property("_d_printPreview_spinboxDefalutValue");
            if (defalutVariant.isValid()) {
                spinbox->setValue(defalutVariant.toDouble());
            }
        }
    }

    if (q->focusWidget() == marginTopSpin || q->focusWidget() == marginLeftSpin
            || q->focusWidget() == marginBottomSpin || q->focusWidget() == marginRightSpin)
        return;

    adjustMargins();
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_currentPageSpinChanged 根据当前页的页码的变化,变化翻页按钮的状态
  \a value 当前页码
 */
void DPrintPreviewDialogPrivate::_q_currentPageSpinChanged(int value)
{
    if (value == 1 && value != totalPageLabel->text().toInt()) {
        firstBtn->setEnabled(false);
        prevPageBtn->setEnabled(false);
        lastBtn->setEnabled(true);
        nextPageBtn->setEnabled(true);
    } else if (value == totalPageLabel->text().toInt() && value != 1) {
        lastBtn->setEnabled(false);
        nextPageBtn->setEnabled(false);
        firstBtn->setEnabled(true);
        prevPageBtn->setEnabled(true);
    } else if (value == 1 && value == totalPageLabel->text().toInt()) {
        lastBtn->setEnabled(false);
        nextPageBtn->setEnabled(false);
        firstBtn->setEnabled(false);
        prevPageBtn->setEnabled(false);
    } else {
        firstBtn->setEnabled(true);
        prevPageBtn->setEnabled(true);
        lastBtn->setEnabled(true);
        nextPageBtn->setEnabled(true);
    }
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_checkStateChanged 点击双面打印按钮
  \a state 判断是否选中双面打印按钮
 */
void DPrintPreviewDialogPrivate::_q_checkStateChanged(int state)
{
    if (!state)
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Duplex_TypeControl, false);
    else {
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Duplex_TypeControl, true);
    }
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_textWaterMarkModeChanged 文本水印的内容选择
  \a state 判断文本水印内容选择
 */
void DPrintPreviewDialogPrivate::_q_textWaterMarkModeChanged(int index)
{
    if (index != waterTextCombo->count() - 1) {
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_CustomText, false);
        pview->setTextWaterMark(waterTextCombo->currentText());
        if (!waterTextEdit->text().isEmpty())
            waterTextEdit->clear();
    } else {
        settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_Watermark_CustomText, true);
        if (!lastCusWatermarkText.isEmpty()) {
            waterTextEdit->setText(lastCusWatermarkText);
            pview->setTextWaterMark(lastCusWatermarkText);
        }
    }
    waterTextEdit->setVisible(index == waterTextCombo->count() - 1);
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_customTextWatermarkFinished 文本水印的自定义输入
  \a state 判断文本水印自定义
 */
void DPrintPreviewDialogPrivate::_q_customTextWatermarkFinished()
{
    QString cusText = waterTextEdit->text();
    pview->setTextWaterMark(cusText);
    lastCusWatermarkText = cusText;
}

/*!
  \brief DPrintPreviewDialogPrivate::customPictureWatermarkChoosed 图片水印
  \a state 图片水印路径
 */
void DPrintPreviewDialogPrivate::customPictureWatermarkChoosed(const QString &filename)
{
    QImage image;
    if (QFileInfo(filename).isFile())
        image.load(filename);

    if (!image.isNull()) {
        pview->refreshBegin();
        initWaterSettings();
        pview->refreshEnd();
    }

    pview->setWaterMargImage(image);
}

/*!
  \brief DPrintPreviewDialogPrivate::waterMarkBtnClicked 是否开启水印
  \a state 水印开启
 */
void DPrintPreviewDialogPrivate::waterMarkBtnClicked(bool checked)
{
    if (checked) {
        wmSpacer->changeSize(WIDTH_NORMAL, SPACER_HEIGHT_HIDE);
        settingHelper->setSubControlVisible(DPrintPreviewSettingInterface::SC_WatermarkContentWidget, true);
        waterTypeGroup->button(typeChoice)->setChecked(true);
        watermarkTypeChoosed(typeChoice);
        if (typeChoice == Type_Image - 1 && !picPathEdit->text().isEmpty())
            customPictureWatermarkChoosed(picPathEdit->text());
    } else {
        wmSpacer->changeSize(WIDTH_NORMAL, SPACER_HEIGHT_SHOW);
        settingHelper->setSubControlVisible(DPrintPreviewSettingInterface::SC_WatermarkContentWidget, false);
        pview->setWaterMarkType(Type_None);
    }
}

/*!
  \brief DPrintPreviewDialogPrivate::disablePrintSettings 如果按照路径打印文件，则禁用部分不能使用的功能
 */
void DPrintPreviewDialogPrivate::disablePrintSettings()
{
    Q_Q(DPrintPreviewDialog);

    if (pview->printFromPath().isEmpty())
        return;

    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_OrientationWidget, false);
    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_ScalingWidget, false);
    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_MarginWidget, false);
    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_WatermarkWidget, false);
    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_PageOrder_SequentialPrint, false);
    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_NPrintWidget, false);
    q->findChild<DWidget *>("CollateWidget")->setEnabled(false);
}

/*!
  \brief DPrintPreviewDialogPrivate::setPageLayoutEnable 并列打印控件是否可用
  \a checked 并列打印功能是否选中
 */
void DPrintPreviewDialogPrivate::setPageLayoutEnable(const bool &checked)
{
    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_NPrint_Layout, checked);
    settingHelper->setSubControlEnabled(DPrintPreviewSettingInterface::SC_NPrint_Numbers, checked);
}

void DPrintPreviewDialogPrivate::matchFitablePageSize()
{
    QPrinterInfo prInfo(*printer);
    if (isActualPrinter(printDeviceCombo->currentText())) {
        auto const &pageSizes = prInfo.supportedPageSizes();
        auto it = std::find_if(pageSizes.cbegin(), pageSizes.cend(), [&](const QPageSize &pageSize) {
            return pageSize.name() == paperSizeCombo->currentText();
        });

        if (it != pageSizes.end())
            printer->setPageSize(*it);
    } else {
        if (paperSizeCombo->currentText() == "A3")
            printer->setPageSize(QPageSize::A3);
        else if (paperSizeCombo->currentText() == "A4")
            printer->setPageSize(QPageSize::A4);
        else if (paperSizeCombo->currentText() == "A5")
            printer->setPageSize(QPageSize::A4);
        else if (paperSizeCombo->currentText() == "B4")
            printer->setPageSize(QPageSize::A4);
        else if (paperSizeCombo->currentText() == "B4")
            printer->setPageSize(QPageSize::B4);
        else if (paperSizeCombo->currentText() == "B5")
            printer->setPageSize(QPageSize::B5);
        else if (paperSizeCombo->currentText() == "8K") {
            printer->setPageSize(QPageSize::Custom);
            printer->setPageSize(QPageSize{QSize{EightK_Weight, EightK_Height}});
        } else if (paperSizeCombo->currentText() == "16K") {
            printer->setPageSize(QPageSize::Custom);
            printer->setPageSize(QPageSize{QSize{SixteenK_Weight, SixteenK_Height}});
        } else {
            printer->setPageSize(QPageSize::A4);
        }
    }
}

bool DPrintPreviewDialogPrivate::isActualPrinter(const QString &name)
{
    const QStringList &printerNames = QPrinterInfo::availablePrinterNames();
    return printerNames.contains(name);
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_colorButtonCliked 点击取色按钮显示取色窗口位置
 */
void DPrintPreviewDialogPrivate::_q_colorButtonCliked(bool cliked)
{
    Q_Q(DPrintPreviewDialog);
    Q_UNUSED(cliked)
    if (colorWidget->isHidden()) {
        isChecked = false;
    } else {
        isChecked = true;
    }
    if (!isChecked) {
        QPoint colorWidgetPoint;
        QPoint globalBtnPos = textWatermarkWdg->mapToGlobal(QPoint(0, 0));
        QPoint globalDialogPos = q->mapToGlobal(QPoint(0, 0));
        int waterBtnX = globalBtnPos.x() - globalDialogPos.x() + waterColorBtn->pos().x() - 28 - colorWidget->width();
        int waterBtnY = globalBtnPos.y() - globalDialogPos.y() + waterColorBtn->pos().y() + waterColorBtn->height() / 2;
        if (waterBtnY < colorWidget->height() / 2) {
            colorWidgetPoint = QPoint(waterBtnX, waterBtnY);
        } else if (waterBtnY > colorWidget->height() && q->height() - waterBtnY < colorWidget->height() / 2) {
            colorWidgetPoint = QPoint(waterBtnX, waterBtnY - colorWidget->height());
        } else {
            colorWidgetPoint = QPoint(waterBtnX, waterBtnY - colorWidget->height() / 2);
        }
        colorWidget->setGeometry(colorWidgetPoint.x(), colorWidgetPoint.y(), 314, 357);
        colorWidget->show();
    } else {
        colorWidget->hide();
    }
    isChecked = !isChecked;
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_selectColorButton 获取水印取色框获取到的颜色
  \a color 水印的颜色
 */
void DPrintPreviewDialogPrivate::_q_selectColorButton(QColor color)
{
    QPixmap pic(QSize(32, 32));
    pic.fill(Qt::transparent);
    QPainter painter(&pic);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    QBrush brush(color);
    painter.setBrush(brush);
    painter.drawRoundedRect(pic.rect(), PICKCOLOR_RADIUS, PICKCOLOR_RADIUS);
    waterColorBtn->setIcon(QIcon(pic));
    waterColorBtn->setIconSize(QSize(24, 24));
    waterColor = color;
    if (isInitBtnColor)
        pview->setWaterMarkColor(color);
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_printOrderComboIndexChanged 打印顺序
  \a index 打印顺序选择
 */
void DPrintPreviewDialogPrivate::_q_printOrderComboIndexChanged(int index)
{
    Q_UNUSED(index)
}

void DPrintPreviewDialogPrivate::_q_spinboxValueEmptyChecked(const QString &text)
{
    Q_Q(DPrintPreviewDialog);

    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(q->sender()))
        spinboxTextCaches.insert(lineEdit, text);
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_pagePersheetComboIndexChanged 并列打印选择
  \a index 并列打印选择
 */
void DPrintPreviewDialogPrivate::_q_pagePersheetComboIndexChanged(int index)
{
    pview->setImposition(DPrintPreviewWidget::Imposition(index + 1));
}

/*!
  \brief DPrintPreviewDialogPrivate::_q_startPrint 点击开始打印，设置属性
  \a clicked 判断按钮点击状态
 */
void DPrintPreviewDialogPrivate::_q_startPrint(bool clicked)
{
    Q_Q(DPrintPreviewDialog);
    if (!clicked) {
        setupPrinter();
    }

    bool isSavePicture = (printDeviceCombo->currentText() == qApp->translate("DPrintPreviewDialogPrivate", "Save as Image"));
    bool isSavePdf = (printDeviceCombo->currentText() == qApp->translate("DPrintPreviewDialogPrivate", "Print to PDF"));
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    desktopPath += QStringLiteral("/");
    if (isSavePdf) {
        /*外部通过setDocName设置，如果不做任何操作默认保存名称print.pdf*/
        if (printer == nullptr) {
            return;
        }
        if (q->docName().isEmpty()) {
            desktopPath += QStringLiteral("print.pdf");
        } else {
            desktopPath += q->docName();
        }
        if (desktopPath.right(4).compare(".pdf", Qt::CaseInsensitive)) {
            desktopPath += ".pdf";
        }
        QFileInfo file(desktopPath);
        QString path = desktopPath;
        if (file.isFile()) {
            int i = 1;
            do {
                QString stres("(%1).pdf");
                path = desktopPath.remove(path.right(4)) + stres.arg(i);
                file.setFile(path);
                i++;
            } while (file.isFile());
            desktopPath = path;
        }

        QString str = DFileDialog::getSaveFileName(q, qApp->translate("DPrintPreviewDialogPrivate", "Save as PDF"), desktopPath, qApp->translate("DPrintPreviewDialogPrivate", "*.pdf"));
        if (str.isEmpty())
            return;

        printer->setOutputFileName(str);
        pview->setPrintMode(DPrintPreviewWidget::PrintToPdf);
    } else if (isSavePicture) {
        if (printer == nullptr) {
            return;
        }

        if (q->docName().isEmpty()) {
            desktopPath += QStringLiteral("print");
        } else {
            desktopPath += q->docName();
        }

        QFileInfo file(desktopPath);
        QString suffix = file.suffix();
        QString stres("(%1)");

        if (!suffix.isEmpty()) {
            // 删除后缀名 + .
            desktopPath.remove(desktopPath.right(suffix.length() + 1));
        }

        desktopPath.append(QStringLiteral("/"));
        stres.append(QStringLiteral("/"));

        file.setFile(desktopPath);
        QString path = desktopPath;
        int i = 1;
        while (file.isDir()) {
            path = desktopPath.left(desktopPath.length() - 1) + stres.arg(i);
            file.setFile(path);
            i++;
        };

        desktopPath = path;
        QString str = DFileDialog::getSaveFileName(q, qApp->translate("DPrintPreviewDialogPrivate", "Save as image"),
                                                   desktopPath.left(desktopPath.length() - 1),
                                                   qApp->translate("DPrintPreviewDialogPrivate", "Images"));

        if (str.isEmpty())
            return;

        QDir savedDir(str);
        if (!savedDir.exists() && !savedDir.mkpath(str))
            return;

        QString imageSuffix = QFileInfo(q->docName()).suffix();
        imageSuffix = !imageSuffix.compare("jpeg", Qt::CaseInsensitive) ? "jpeg" : "png";
        QString imageName = QFileInfo(str).fileName();
        str.append("/").append(imageName).append(".").append(imageSuffix);
        printer->setOutputFileName(str);
        pview->setPrintMode(DPrintPreviewWidget::PrintToImage);
    } else {
        pview->setPrintMode(DPrintPreviewWidget::PrintToPrinter);
    }

    pview->print();

    q->accept();
}

void DPrintPreviewDialogPrivate::pageRangeError(TipsNum tipNum)
{
    setPageIsLegal(false);
    if (isOnFocus)
        tipSelected(tipNum);
    isOnFocus = false;
}

/*!
    \class Dtk::Widget::DPrintPreviewDialog
    \inmodule dtkwidget

    \brief Dtk 风格的打印预览页面.

    一个用于创建 Dtk 风格的打印预览页面，通常情况下，只需要
    构建一个 DPrintPreivewDialog 对象，并将原始数据绘制
    到连接 DPrintPreview::paintRequested 信号的槽函数
    中,最后以模态的形式显示就可以正常使用。

    dtkwidget 项目的 collection 示例中有对打印预览的简单
    示例。简单的讲：打印预览拥有两种预览模式，1. 同步预览模式
    ，适用于数据量较小，且更新不太频繁的原始数据中，这种模式下
    默认情况下的，打印预览仅会在页面大小或者打印机属性发生变化
    时发射 DPrintPreview::paintRequested 信号。2. 异步
    预览模式，该模式适用于数据量较大，且更新较频繁的原始数据，
    这种模式下，打印预览会频繁触发异步的 DPrintPreview::paintRequested
    信号，但每次接收的数据量较小，因此不用担心响应速度的问题。
    默认情况下的打印预览窗口为同步模式。

    \section \b 基本使用
    该代码在 example 中可以看到，这里简单贴出对应内容.
    \snippet mainwindow.cpp 0

    示例的预览效果如下：
    \image html DPrintPreviewExample.png

    上述代码中用宏 AsynPreview 控制是否使用异步模式，并且通过
    分别连接两种模式的 DPrintPreview::paintRequested 信号
    将原始数据绘制到打印机 printer 中。需要注意的是，异步模式下
    该信号会传递一个 pageRange 的页码数据，这个数据里面会放置
    本次重绘需要展示的页面，应用可以根据原始数据，动态提供对应
    页码的数据给打印预览。同样地，在打印预览最后打印的阶段，也会
    根据用户选择的页码多少，向外获取对应页码的原始数据。
    \warning pageRange 对应的数据为页码值，而非页面索引值！

    \section \b 其他接口
    除同步和异步接口之外，打印预览还提供给外部一些其他接口，例如
    setDocName 设置数据文件名，在输出 pdf 或者图片时可以看到
    效果。 setPrintFromPath 按照文件路径打印的接口，使用这个
    接口将文件的路径传递给打印机，打印预览会在用户点击打印按钮后
    将该路径和用户操作的属性直接传递给打印机，而不通过打印预览进行
    中间处理，需要注意的是，使用了这个接口后，仍需要通过DPrintPreview::paintRequested
    信号传递原始数据，并且，由于使用该接口后，属性操作完全取决于打印机
    ，因此部分打印预览的高级功能会禁用。
 */

DPrintPreviewDialog::DPrintPreviewDialog(QWidget *parent)
    : DDialog(*new DPrintPreviewDialogPrivate(this), parent)
{
    Q_D(DPrintPreviewDialog);
    setMinimumSize(851, 606);
    setWindowFlag(Qt::WindowMaximizeButtonHint);
    if (qApp->isDXcbPlatform()) {
        DPlatformWindowHandle *handle = new DPlatformWindowHandle(this, nullptr);
        handle->setEnableSystemResize(true);
        handle->deleteLater();
    }
    d->startup();
}

DPrintPreviewDialog::~DPrintPreviewDialog()
{
    Q_D(DPrintPreviewDialog);
    if (nullptr != d->printer)
        delete d->printer;
    delete d->settingHelper;
}

void DPrintPreviewDialog::setPluginMimeData(const QVariant &mimeData)
{
    PreviewSettingsPluginHelper::loadPlugin();
    PreviewSettingsPluginHelper::setPluginMimeData(mimeData);
}

QVariant DPrintPreviewDialog::pluginMimeData()
{
    PreviewSettingsPluginHelper::loadPlugin();
    return PreviewSettingsPluginHelper::pluginMimeData();
}

QStringList DPrintPreviewDialog::availablePlugins()
{
    PreviewSettingsPluginHelper::loadPlugin();
    return PreviewSettingsPluginHelper::availablePlugins();
}

bool DPrintPreviewDialog::setCurrentPlugin(const QString &pluginName)
{
    PreviewSettingsPluginHelper::loadPlugin();
    return PreviewSettingsPluginHelper::setCurrentPlugin(pluginName);
}

QString DPrintPreviewDialog::currentPlugin()
{
    PreviewSettingsPluginHelper::loadPlugin();
    return PreviewSettingsPluginHelper::currentPlugin();
}

bool DPrintPreviewDialog::event(QEvent *event)
{
    Q_D(DPrintPreviewDialog);
    if (event->type() == QEvent::ApplicationFontChange || d->fontSizeMore == true) {
        if (d->waterTypeGroup->button(0)->isChecked()) {
            d->watermarkTypeChoosed(0);
        }
        if (DFontSizeManager::fontPixelSize(qGuiApp->font()) <= 15)
            d->marginsLayout(true);
        else {
            d->marginsLayout(false);
        }
        d->fontSizeMore = false;
    } else if (event->type() == QEvent::Show) {
        d->pview->updatePreview();
        d->updateAllControlSettings();
        d->updateAllControlStatus();
    }
    return DDialog::event(event);
}

bool DPrintPreviewDialog::eventFilter(QObject *watched, QEvent *event)
{
    Q_D(DPrintPreviewDialog);

    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keye = dynamic_cast<QKeyEvent *>(event);
        if (keye->key() == Qt::Key_Enter || keye->key() == Qt::Key_Return) {
            if (watched == d->marginTopSpin || watched == d->marginLeftSpin || watched == d->marginRightSpin || watched == d->marginBottomSpin) {
                d->setMininumMargins();
                d->adjustMargins();
            } else if (watched == d->pageRangeEdit){
                d->_q_customPagesFinished();
                d->isOnFocus = true;
                return true;
            } else if (watched == d->scaleRateEdit){
                Q_EMIT d->scaleRateEdit->lineEdit()->editingFinished();
                return true;
            } else if (watched == d->waterTextEdit) {
                d->_q_customTextWatermarkFinished();
                return true;
            }
         }

         QString str = d->pageRangeEdit->text();
         int strLengthNow = str.length();
         if (watched == d->pageRangeEdit) {
             if ((keye->key() >= Qt::Key_A && keye->key() <= Qt::Key_Z) || (keye->key() >= Qt::Key_Space && keye->key() <= Qt::Key_Slash && keye->key() != Qt::Key_Comma) || keye->key() == Qt::Key_0) {
                 if (str.isEmpty() || (d->strLengths == strLengthNow)) {
                     d->tipSelected(DPrintPreviewDialogPrivate::TipsNum::FormatTip);
                 }
                 d->strLengths = strLengthNow;
                 return true;
             }
             if (keye->key() == Qt::Key_Comma) {
                 if (str.isEmpty() || ((d->strLengths == strLengthNow) && (d->pageRangeEdit->text().right(1) == "," || d->pageRangeEdit->text().right(1) == "-"))) {
                     d->tipSelected(DPrintPreviewDialogPrivate::TipsNum::FormatTip);
                 } else if (!str.isEmpty() && (d->strLengths == strLengthNow)) {
                     d->tipSelected(DPrintPreviewDialogPrivate::TipsNum::CommaTip);
                 }
                 d->strLengths = strLengthNow;
                 return true;
             }
             d->strLengths = strLengthNow;
         }

        return false;
    }
    //手动实现窗体popup属性
    if (event->type() == QEvent::MouseButtonPress) {
        QRect rect = QRect(d->colorWidget->x(), d->colorWidget->y(), d->colorWidget->width(), d->colorWidget->height());
        QMouseEvent *e = dynamic_cast<QMouseEvent *>(event);
        QPoint pos = mapFromGlobal(QCursor::pos());
        QPoint globalBtnPos = d->textWatermarkWdg->mapToGlobal(QPoint(0, 0));
        QPoint globalDialogPos = mapToGlobal(QPoint(0, 0));
        int waterBtnX = globalBtnPos.x() - globalDialogPos.x() + d->waterColorBtn->pos().x();
        int waterBtnY = globalBtnPos.y() - globalDialogPos.y() + d->waterColorBtn->pos().y();
        QRect btnRect = QRect(waterBtnX, waterBtnY, d->waterColorBtn->width(), d->waterColorBtn->height());
        if (e && !rect.contains(pos) && !btnRect.contains(pos)) {
            d->colorWidget->hide();
        }
    }

    if (event->type() == QEvent::FocusIn) {
        if (watched->inherits("QSpinBox")) {
            if (QSpinBox *spinbox = qobject_cast<QSpinBox *>(watched)) {
                spinbox->setProperty("_d_printPreview_spinboxDefalutValue", spinbox->value());
            }
        } else if (watched->inherits("QDoubleSpinBox")) {
            if (QDoubleSpinBox *spinbox = qobject_cast<QDoubleSpinBox *>(watched)) {
                spinbox->setProperty("_d_printPreview_spinboxDefalutValue", spinbox->value());
            }
        }
    }
    return DDialog::eventFilter(watched, event);
}

/*!
  \brief DPrintPreviewDialog::setDocName 设置保存PDF的文件名称
  \a str 文件名称
 */
void DPrintPreviewDialog::setDocName(const QString &str)
{
    Q_D(DPrintPreviewDialog);
    d->printer->setDocName(str);
}

/*!
  \brief DPrintPreviewDialog::docName 保存PDF的文件名称
  \return  文件名称
 */
QString DPrintPreviewDialog::docName() const
{
    D_DC(DPrintPreviewDialog);
    return d->printer->docName();
}

/*!
  \brief DPrintPreviewDialog::setPrintFromPath 根据路径的文件进行打印.
  \a path 文件路径
  \return 设置成功返回 true, 否则返回 false.
 */
bool DPrintPreviewDialog::setPrintFromPath(const QString &path)
{
    if (path.isEmpty() || !QFileInfo(path).isFile() || !QFileInfo(path).isReadable())
        return false;

    Q_D(DPrintPreviewDialog);

    d->pview->setPrintFromPath(path);
    d->disablePrintSettings();

    return true;
}

/*!
  \brief DPrintPreviewDialog::docName 路径文件的路径名
  \return  路径文件路径名
 */
QString DPrintPreviewDialog::printFromPath() const
{
    D_DC(DPrintPreviewDialog);

    return d->pview->printFromPath();
}

bool DPrintPreviewDialog::setAsynPreview(int totalPage)
{
    Q_D(DPrintPreviewDialog);

    if (totalPage < 0)
        return false;

    d->pview->setAsynPreview(totalPage);
    return true;
}

bool DPrintPreviewDialog::isAsynPreview() const
{
    D_DC(DPrintPreviewDialog);

    return d->pview->isAsynPreview();
}

DPrintPreviewSettingInfo *DPrintPreviewDialog::createDialogSettingInfo(DPrintPreviewSettingInfo::SettingType type)
{
    Q_D(DPrintPreviewDialog);
    return d->settingHelper->loadInfo(type, true);
}

void DPrintPreviewDialog::updateDialogSettingInfo(DPrintPreviewSettingInfo *info)
{
    Q_D(DPrintPreviewDialog);
    d->settingHelper->updateSettingInfo(info);
}

void DPrintPreviewDialog::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    Q_D(DPrintPreviewDialog);

    this->findChild<DWidget *>("mainwidget")->setGeometry(0, 0, this->width(), this->height());
    double per = static_cast<double>(this->width()) / static_cast<double>(851);
    if (per >= 1.2) {
        this->findChild<DWidget *>("rightWidget")->setMaximumWidth(452 * 1.2);
        this->findChild<DWidget *>("leftWidget")->setMaximumWidth(this->width() - 20 - 10 - 452 * 1.2);
        d->back->setItemSpacing(10);
    } else {
        this->findChild<DWidget *>("rightWidget")->setMaximumWidth(452 * per);
        this->findChild<DWidget *>("leftWidget")->setMaximumWidth(this->width() - 20 - 2 - 452 * per);
        d->back->setItemSpacing(2);
    }
}

void DPrintPreviewDialog::timerEvent(QTimerEvent *event)
{
    Q_D(DPrintPreviewDialog);
    if (event->timerId() == d->settingUpdateTimer.timerId()) {
        d->settingUpdateTimer.stop();
        d->updateAllContentSettings_impl();
    }

    return DDialog::timerEvent(event);
}

QVariant PreviewSettingsPluginHelper::m_printSettingData;
DPrintPreviewSettingInterface *PreviewSettingsPluginHelper::m_currentInterface = nullptr;
QList<DPrintPreviewSettingInterface *> PreviewSettingsPluginHelper::m_availablePlugins = {};
PreviewSettingsPluginHelper::PreviewSettingsPluginHelper(DPrintPreviewDialogPrivate *dd)
    : d(dd)
{
}

DPrintPreviewSettingInfo *PreviewSettingsPluginHelper::loadInfo(DPrintPreviewSettingInfo::SettingType type, bool manual)
{
    // Apps can manually get the current print dialog settings.
    if (!manual && !m_currentInterface) {
        return nullptr;
    }

    DPrintPreviewSettingInfo *info = nullptr;
    switch (type) {
    case DPrintPreviewSettingInfo::PS_Printer: {
        DPrintPreviewPrinterInfo *printerInfo = new DPrintPreviewPrinterInfo;
        for (int index = 0; index < d->printDeviceCombo->count(); ++index)
            printerInfo->printers << d->printDeviceCombo->itemText(index);
        info = printerInfo;
    }
        break;
    case DPrintPreviewSettingInfo::PS_Copies: {
        DPrintPreviewCopiesInfo *copiesInfo = new DPrintPreviewCopiesInfo;
        copiesInfo->copies = d->copycountspinbox->value();
        info = copiesInfo;
    }
        break;
    case DPrintPreviewSettingInfo::PS_PageRange: {
        DPrintPreviewPageRangeInfo *pageRangeInfo = new DPrintPreviewPageRangeInfo;
        pageRangeInfo->rangeType = d->pview->pageRangeMode();
        pageRangeInfo->selectPages = d->pageRangeEdit->text();
        info = pageRangeInfo;
    }
        break;
    case DPrintPreviewSettingInfo::PS_Orientation: {
        DPrintPreviewOrientationInfo *orientationInfo = new DPrintPreviewOrientationInfo;
        orientationInfo->orientationMode = static_cast<DPrinter::Orientation>(d->printer->pageLayout().orientation());
        info  = orientationInfo;
    }
        break;
    case DPrintPreviewSettingInfo::PS_PaperSize: {
        DPrintPreviewPaperSizeInfo *paperSizeInfo = new DPrintPreviewPaperSizeInfo;
        for (int index = 0; index < d->paperSizeCombo->count(); ++index)
            paperSizeInfo->pageSize << d->paperSizeCombo->itemText(index);
        info = paperSizeInfo;
    }
        break;
    case DPrintPreviewSettingInfo::PS_PrintDuplex: {
        DPrintPreviewPrintDuplexInfo *duplexInfo = new DPrintPreviewPrintDuplexInfo;
        duplexInfo->enable = d->duplexCheckBox->isChecked();
        duplexInfo->duplex = d->printer->duplex();
        info = duplexInfo;
    }
        break;
    case DPrintPreviewSettingInfo::PS_NUpPrinting: {
        DPrintPreviewNUpPrintInfo *nupInfo = new DPrintPreviewNUpPrintInfo;
        nupInfo->enable = d->sidebysideCheckBox->isChecked();
        nupInfo->imposition = d->pview->imposition();
        nupInfo->order = d->pview->order();
        info = nupInfo;
    }
        break;
    case DPrintPreviewSettingInfo::PS_PageOrder: {
        DPrintPreviewPageOrderInfo *orderInfo = new DPrintPreviewPageOrderInfo;
        orderInfo->pageOrder = (d->printOrderGroup->checkedId() == 0) ? DPrintPreviewPageOrderInfo::CollatePage : DPrintPreviewPageOrderInfo::InOrderPage;
        orderInfo->inOrdertype = (d->inorderCombo->currentIndex() == 0) ? DPrintPreviewPageOrderInfo::FrontToBack : DPrintPreviewPageOrderInfo::BackToFront;
        info = orderInfo;
    }
        break;
    case DPrintPreviewSettingInfo::PS_ColorMode: {
        DPrintPreviewColorModeInfo *colorInfo = new DPrintPreviewColorModeInfo;
        for (int index = 0; index < d->colorModeCombo->count(); ++index)
            colorInfo->colorMode << d->colorModeCombo->itemText(index);
        info = colorInfo;
    }
        break;
    case DPrintPreviewSettingInfo::PS_PaperMargins: {
        DPrintPreviewPaperMarginsInfo *marginInfo = new DPrintPreviewPaperMarginsInfo;
        switch (d->marginsCombo->currentIndex()) {
        case 0:
            marginInfo->marginType = DPrintPreviewPaperMarginsInfo::Narrow;
            break;
        case 1:
            marginInfo->marginType = DPrintPreviewPaperMarginsInfo::Normal;
            break;
        case 2:
            marginInfo->marginType = DPrintPreviewPaperMarginsInfo::Moderate;
            break;
        case 3:
            marginInfo->marginType = DPrintPreviewPaperMarginsInfo::Customize;
            break;
        default:
            break;
        }
        marginInfo->topMargin = d->marginTopSpin->value();
        marginInfo->bottomMargin = d->marginBottomSpin->value();
        marginInfo->leftMargin = d->marginBottomSpin->value();
        marginInfo->rightMargin = d->marginBottomSpin->value();
        info = marginInfo;
    }
        break;
    case DPrintPreviewSettingInfo::PS_Scaling: {
        DPrintPreviewScalingInfo *scalingInfo = new DPrintPreviewScalingInfo;
        scalingInfo->scalingType = (d->scaleGroup->checkedId() == ACTUAL_SIZE) ? DPrintPreviewScalingInfo::ActualSize
                                                                               : DPrintPreviewScalingInfo::ScaleSize;
        scalingInfo->scaleRatio = d->scaleRateEdit->value();
        info = scalingInfo;
    }
        break;
    case DPrintPreviewSettingInfo::PS_Watermark: {
        DPrintPreviewWatermarkInfo *watermarkInfo = new DPrintPreviewWatermarkInfo;
        watermarkInfo->currentWatermarkType = static_cast<DPrintPreviewWatermarkInfo::WatermarkType>(d->waterTypeGroup->checkedId());
        switch (watermarkInfo->currentWatermarkType) {
        case DPrintPreviewWatermarkInfo::TextWatermark: {
            switch (d->waterTextCombo->currentIndex()) {
            case 0:
                watermarkInfo->textType = DPrintPreviewWatermarkInfo::Confidential;
                break;
            case 1:
                watermarkInfo->textType = DPrintPreviewWatermarkInfo::Draft;
                break;
            case 2:
                watermarkInfo->textType = DPrintPreviewWatermarkInfo::Sample;
                break;
            case 3:
                watermarkInfo->textType = DPrintPreviewWatermarkInfo::Custom;
                break;
            }
            watermarkInfo->customText = d->waterTextEdit->text();
            for (int index = 0; index < d->fontCombo->count(); ++index)
                watermarkInfo->fontList << d->fontCombo->itemText(index);
            watermarkInfo->textColor = d->pview->waterMarkColor();
        }
            break;
        case DPrintPreviewWatermarkInfo::ImageWatermark: {
            watermarkInfo->imagePath = d->picPathEdit->text();
        }
            break;
        }

        watermarkInfo->opened = d->waterMarkBtn->isChecked();
        watermarkInfo->angle = d->inclinatBox->value();
        watermarkInfo->size = d->waterSizeSlider->value();
        watermarkInfo->transparency = d->wmOpaSlider->value();

        // TODO: Remove it.
        QVariant spacingProperty = d->pview->property("_d_print_waterMarkRowSpacing");
        if (spacingProperty.isValid()) {
            watermarkInfo->rowSpacing = spacingProperty.toDouble();
        } else {
            watermarkInfo->rowSpacing = -1;
        }

        spacingProperty = d->pview->property("_d_print_waterMarkColumnSpacing");
        if (spacingProperty.isValid()) {
            watermarkInfo->columnSpacing = spacingProperty.toDouble();
        } else {
            watermarkInfo->columnSpacing = -1;
        }
        info = watermarkInfo;
    }
        break;
    default:
        break;
    }

    // Apps can get setting info without the print plugin, otherwise need to be filter by plugin.
    if (!info || !m_currentInterface)
        return info;

    if (m_currentInterface->settingFilter(d->settingHelper->m_printSettingData, info))
        return info;
    delete info;
    return nullptr;
}

void PreviewSettingsPluginHelper::setSubControlVisible(DPrintPreviewSettingInterface::SettingSubControl subControlType, bool visible)
{
    QWidgetList sourceList = subControl(subControlType);
    for (QWidget *w : sourceList)
        doUpdateStatus(w, subControlType, visible, w->isEnabledTo(w->parentWidget()));
}

void PreviewSettingsPluginHelper::setSubControlEnabled(DPrintPreviewSettingInterface::SettingSubControl subControlType, bool enabled)
{
    QWidgetList sourceList = subControl(subControlType);
    for (QWidget *w : sourceList)
        doUpdateStatus(w, subControlType, w->isVisibleTo(w->parentWidget()), enabled);
}

void PreviewSettingsPluginHelper::updateSettingInfo(DPrintPreviewSettingInfo *info)
{
    if (!info)
        return;

    switch (info->type()) {
    case DPrintPreviewSettingInfo::PS_Printer: {
        DPrintPreviewPrinterInfo *printerInfo = static_cast<DPrintPreviewPrinterInfo *>(info);
        QStringList targetPrinters, sourcePrinters;
        for (int index = 0; index < d->printDeviceCombo->count(); ++index) {
            const QString &p = d->printDeviceCombo->itemText(index);
            sourcePrinters << p;
            if (printerInfo->printers.contains(p))
                targetPrinters << p;
        }
        if (targetPrinters.isEmpty() || targetPrinters == sourcePrinters)
            break;
        d->printDeviceCombo->blockSignals(true);
        d->printDeviceCombo->clear();
        d->printDeviceCombo->blockSignals(false);
        d->printDeviceCombo->addItems(targetPrinters);
    }
        break;
    case DPrintPreviewSettingInfo::PS_Copies: {
        DPrintPreviewCopiesInfo *copiesInfo = static_cast<DPrintPreviewCopiesInfo*>(info);
        d->copycountspinbox->setValue(copiesInfo->copies);
    }
        break;
    case DPrintPreviewSettingInfo::PS_PageRange: {
        DPrintPreviewPageRangeInfo *pageRangeInfo = static_cast<DPrintPreviewPageRangeInfo *>(info);
        d->pageRangeCombo->setCurrentIndex(pageRangeInfo->rangeType);
        d->pageRangeEdit->setText(pageRangeInfo->selectPages);
        d->_q_customPagesFinished();
    }
        break;
    case DPrintPreviewSettingInfo::PS_Orientation: {
        DPrintPreviewOrientationInfo *orientationInfo = static_cast<DPrintPreviewOrientationInfo *>(info);
        d->orientationgroup->button(orientationInfo->orientationMode)->setChecked(true);
        d->_q_orientationChanged(orientationInfo->orientationMode);
    }
        break;
    case DPrintPreviewSettingInfo::PS_PaperSize: {
        DPrintPreviewPaperSizeInfo *paperSizeInfo = static_cast<DPrintPreviewPaperSizeInfo *>(info);
        QStringList paperSizes, sourcePaperSizes;
        for (int index = 0; index < d->paperSizeCombo->count(); ++index) {
            const QString &paper = d->paperSizeCombo->itemText(index);
            sourcePaperSizes << paper;
            if (paperSizeInfo->pageSize.contains(paper))
                paperSizes << paper;
        }
        if (paperSizes.isEmpty() || paperSizes == sourcePaperSizes)
            break;
        d->paperSizeCombo->blockSignals(true);
        d->paperSizeCombo->clear();
        d->paperSizeCombo->blockSignals(false);
        d->paperSizeCombo->addItems(paperSizes);
        d->paperSizeCombo->setCurrentIndex(0);
    }
        break;
    case DPrintPreviewSettingInfo::PS_PrintDuplex: {
        DPrintPreviewPrintDuplexInfo *duplexInfo = static_cast<DPrintPreviewPrintDuplexInfo *>(info);
        d->duplexCheckBox->setChecked(duplexInfo->enable);
        d->duplexCombo->setCurrentIndex(duplexInfo->duplex == DPrinter::DuplexShortSide ? 1 : 0);
    }
        break;
    case DPrintPreviewSettingInfo::PS_NUpPrinting: {
        DPrintPreviewNUpPrintInfo *nupInfo = static_cast<DPrintPreviewNUpPrintInfo *>(info);
        d->sidebysideCheckBox->setChecked(nupInfo->enable);
        if (nupInfo->imposition == DPrintPreviewWidget::One) {
            d->sidebysideCheckBox->setChecked(false);
            break;
        }

        d->pagePerSheetCombo->setCurrentIndex(nupInfo->imposition - 1);
        d->directGroup->button(nupInfo->order)->click();
    }
        break;
    case DPrintPreviewSettingInfo::PS_PageOrder: {
        DPrintPreviewPageOrderInfo *orderInfo = static_cast<DPrintPreviewPageOrderInfo *>(info);
        d->printOrderGroup->button(orderInfo->pageOrder)->click();
        d->inorderCombo->setCurrentIndex(orderInfo->inOrdertype);
    }
        break;
    case DPrintPreviewSettingInfo::PS_ColorMode: {
        DPrintPreviewColorModeInfo *colorInfo = static_cast<DPrintPreviewColorModeInfo *>(info);
        QStringList targetColors, sourceColors;
        for (int index = 0; index < d->colorModeCombo->count(); ++index) {
            const QString &color = d->colorModeCombo->itemText(index);
            sourceColors << color;
            if (colorInfo->colorMode.contains(color))
                targetColors << color;
        }
        if (targetColors.isEmpty() || targetColors == sourceColors)
            break;
        d->colorModeCombo->blockSignals(true);
        d->colorModeCombo->clear();
        d->colorModeCombo->blockSignals(false);
        d->colorModeCombo->addItems(targetColors);
    }
        break;
    case DPrintPreviewSettingInfo::PS_PaperMargins: {
        DPrintPreviewPaperMarginsInfo *marginInfo = static_cast<DPrintPreviewPaperMarginsInfo *>(info);
        d->marginsCombo->setCurrentIndex(marginInfo->marginType);
        d->marginTopSpin->setValue(marginInfo->topMargin);
        d->marginBottomSpin->setValue(marginInfo->bottomMargin);
        d->marginLeftSpin->setValue(marginInfo->leftMargin);
        d->marginRightSpin->setValue(marginInfo->rightMargin);
        d->_q_marginEditFinished();
    }
        break;
    case DPrintPreviewSettingInfo::PS_Scaling: {
        DPrintPreviewScalingInfo *scalingInfo = static_cast<DPrintPreviewScalingInfo *>(info);
        d->scaleGroup->button(scalingInfo->scalingType + 1)->click();
        d->scaleRateEdit->setValue(scalingInfo->scaleRatio);
    }
        break;
    case DPrintPreviewSettingInfo::PS_Watermark: {
        DPrintPreviewWatermarkInfo *watermarkInfo = static_cast<DPrintPreviewWatermarkInfo *>(info);
        d->waterMarkBtn->setChecked(watermarkInfo->opened);
        if (!watermarkInfo->opened) {
            d->pview->setWaterMarkType(DPrintPreviewDialogPrivate::Type_None);
            break;  // Watermark button should not update any watermarking function when off
        }

        d->pview->refreshBegin();
        d->waterTypeGroup->button(int(watermarkInfo->currentWatermarkType))->setChecked(true);
        switch (watermarkInfo->currentWatermarkType) {
        case DPrintPreviewWatermarkInfo::TextWatermark: {
            d->waterTextCombo->setCurrentIndex(watermarkInfo->textType);
            d->waterTextEdit->setText(watermarkInfo->customText);
            d->_q_customTextWatermarkFinished();
            QStringList targetFonts, sourceFonts;
            for (int index = 0; index < d->fontCombo->count(); ++index) {
                const QString &font = d->fontCombo->itemText(index);
                sourceFonts << font;
                if (watermarkInfo->fontList.contains(font))
                   targetFonts << font;
            }

            if (!targetFonts.isEmpty() && targetFonts != sourceFonts) {
                d->fontCombo->blockSignals(true);
                d->fontCombo->clear();
                d->fontCombo->blockSignals(false);
                d->fontCombo->addItems(targetFonts);
            }

            if (d->supportedColorMode)
                d->_q_selectColorButton(watermarkInfo->textColor);
        }
            break;
        case DPrintPreviewWatermarkInfo::ImageWatermark: {
            d->picPathEdit->setText(watermarkInfo->imagePath);
        }
            break;
        }

        d->waterPosCombox->setCurrentIndex(watermarkInfo->layout == DPrintPreviewWatermarkInfo::Tiled ? 0 : 1);
        d->inclinatBox->setValue(watermarkInfo->angle);
        Q_EMIT d->inclinatBox->editingFinished();
        d->waterSizeSlider->setValue(watermarkInfo->size);
        d->wmOpaSlider->setValue(watermarkInfo->transparency);

        // TODO: Remove it.
        if (!qFuzzyCompare(watermarkInfo->rowSpacing, -1)) {
            qreal rowSpacing = watermarkInfo->rowSpacing;
            rowSpacing = qBound(0.0, rowSpacing, 10.0);
            d->pview->setProperty("_d_print_waterMarkRowSpacing", rowSpacing);
        }

        if (!qFuzzyCompare(watermarkInfo->columnSpacing, -1)) {
            qreal columnSpacing = watermarkInfo->columnSpacing;
            columnSpacing = qBound(0.0, columnSpacing, 2.0);
            d->pview->setProperty("_d_print_waterMarkColumnSpacing", columnSpacing);
        }
        d->pview->refreshEnd();
    }
        break;
    default:
        break;
    }
}

void PreviewSettingsPluginHelper::updateSettingStatus(DPrintPreviewSettingInterface::SettingSubControl subControlType)
{
    QWidgetList sourceList = subControl(subControlType);
    for (QWidget *w : sourceList)
        doUpdateStatus(w, subControlType, w->isVisibleTo(w->parentWidget()), w->isEnabledTo(w->parentWidget()));
}

void PreviewSettingsPluginHelper::doUpdateStatus(QWidget *source, DPrintPreviewSettingInterface::SettingSubControl subControlType, bool visible, bool enabled)
{
    if (!source)
        return;

    do {
        if (!m_currentInterface)
            break;

        DPrintPreviewSettingInterface::SettingStatus status = m_currentInterface->settingStatus(d->settingHelper->m_printSettingData, subControlType);
        switch (status) {
        case DPrintPreviewSettingInterface::Hidden: {
            source->setEnabled(enabled);
            source->setVisible(false);
        }
            return;
        case DPrintPreviewSettingInterface::Disabled:
            source->setEnabled(false);
            source->setVisible(visible);
            return;
        default:
            break;
        }
    } while (false);

    source->setVisible(visible);
    source->setEnabled(enabled);
}

QWidgetList PreviewSettingsPluginHelper::subControl(DPrintPreviewSettingInterface::SettingSubControl subControlType) const
{
    if (subControlType >= sizeof(_d_printSettingNameMap) / sizeof(_d_printSettingNameMap[0]))
        return {};
    return d->q_func()->findChildren<QWidget *>(_d_printSettingNameMap[subControlType]);
}

void PreviewSettingsPluginHelper::loadPlugin()
{
    static bool loaded = false;
    if (loaded)
        return;
    const QString &path = pluginPath();
    if (!QFileInfo(path).exists())
        return;

    QDir pluginDir(path);
    const QStringList &entryList = pluginDir.entryList(QDir::Files);
    for (const QString &fileName : entryList) {
        QPluginLoader loader(pluginDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (!plugin)
            continue;
        DPrintPreviewSettingInterface *interface = dynamic_cast<DPrintPreviewSettingInterface *>(plugin);
        if (!interface)
            continue;
        m_availablePlugins.append(interface);
    }
    loaded = true;
    if (!m_availablePlugins.isEmpty())
        m_currentInterface = m_availablePlugins.first();
}

void PreviewSettingsPluginHelper::setPluginMimeData(const QVariant &data)
{
    m_printSettingData = data;
}

QVariant PreviewSettingsPluginHelper::pluginMimeData()
{
    return m_printSettingData;
}

QStringList PreviewSettingsPluginHelper::availablePlugins()
{
    QStringList targets;
    std::for_each(m_availablePlugins.cbegin(), m_availablePlugins.cend(), [&](DPrintPreviewSettingInterface *interface) {
        targets << interface->name();
    });
    return targets;
}

QString PreviewSettingsPluginHelper::currentPlugin()
{
    return m_currentInterface ? m_currentInterface->name() : QLatin1String("");
}

bool PreviewSettingsPluginHelper::setCurrentPlugin(const QString &pluginName)
{
    auto it = std::find_if(m_availablePlugins.cbegin(), m_availablePlugins.cend(), [=](DPrintPreviewSettingInterface *interface) {
        if (interface->name() != pluginName)
            return false;
        return true;
    });

    if (it == m_availablePlugins.cend()) {
        qWarning() << "DPrintPreviewDialog: " << "No plugin named " << pluginName << " was found.";
        return false;
    }
    m_currentInterface = *it;
    return true;
}

QString PreviewSettingsPluginHelper::pluginPath()
{
#if defined(Q_OS_LINUX)
    return QLatin1String("/usr/share/deepin/dtk/plugins/printsupport");
#else
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QLatin1String("/printsupport") + QLatin1String("/plugins");
#endif
}

DWIDGET_END_NAMESPACE
#include "moc_dprintpreviewdialog.cpp"
