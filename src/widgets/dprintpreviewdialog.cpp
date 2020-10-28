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
#include "dapplicationhelper.h"
#include "dstyleoption.h"
#include "dapplication.h"

#include <DScrollArea>
#include <DScrollBar>
#include <DPlatformWindowHandle>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QtPrintSupport/QPrinterInfo>
#include <QPrinter>
#include <QDebug>
#include <QTime>
#include <QDesktopServices>
#include <QRegExp>
#include <QRegExpValidator>
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

#define MARGIN_SIZE_LESS 90
#define MARGIN_SIZE_MORE 169

#define PAGERANGE_ALL 0
#define PAGERANGE_CURRENT 1
#define PAGERANGE_SELECT 2
#define FIRST_PAGE 1

#define ACTUAL_SIZE 1
#define SCALE 2

DWIDGET_BEGIN_NAMESPACE

void setwidgetfont(QWidget *widget, DFontSizeManager::SizeType type = DFontSizeManager::T5)
{
    QFont font = widget->font();
    font.setBold(true);
    widget->setFont(font);
    DFontSizeManager::instance()->bind(widget, type);
}

DPrintPreviewDialogPrivate::DPrintPreviewDialogPrivate(DPrintPreviewDialog *qq)
    : DDialogPrivate(qq)
{
}

void DPrintPreviewDialogPrivate::startup()
{
    Q_Q(DPrintPreviewDialog);

    this->printer = new DPrinter;

    initui();
    initdata();
    initconnections();
}

void DPrintPreviewDialogPrivate::initui()
{
    Q_Q(DPrintPreviewDialog);
    DWidget *mainWidget = new DWidget(q);
    mainWidget->setObjectName("mainwidget");

    DWidget *titleWidget = new DWidget(q);
    titleWidget->setObjectName("titlewidget");

    mainWidget->setAutoFillBackground(true);

    DPalette pa = DApplicationHelper::instance()->palette(titleWidget);
    pa.setBrush(DPalette::Background, pa.base());
    DApplicationHelper::instance()->setPalette(titleWidget, pa);
    titleWidget->setAutoFillBackground(true);

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
    DBackgroundGroup *back = new DBackgroundGroup(mainlayout);
    back->setObjectName("backGround");
    back->setItemSpacing(2);

    q->addSpacing(5);
    q->addContent(back);
}

void DPrintPreviewDialogPrivate::initleft(QVBoxLayout *layout)
{
    pview = new DPrintPreviewWidget(this->printer);
    pview->setLineWidth(0);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(pview);

    QHBoxLayout *pbottomlayout = new QHBoxLayout;
    pbottomlayout->setContentsMargins(0, 10, 0, 0);
    layout->addLayout(pbottomlayout);
    firstBtn = new DIconButton(DStyle::SP_ArrowPrev);
    prevPageBtn = new DIconButton(DStyle::SP_ArrowLeft);
    firstBtn->setIcon(QIcon::fromTheme("printer_original"));
    firstBtn->setEnabled(false);
    prevPageBtn->setEnabled(false);
    jumpPageEdit = new DSpinBox;
    jumpPageEdit->setMaximumWidth(50);
    jumpPageEdit->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
    DLabel *spaceLabel = new DLabel("/");
    totalPageLabel = new DLabel;
    nextPageBtn = new DIconButton(DStyle::SP_ArrowRight);
    lastBtn = new DIconButton(DStyle::SP_ArrowNext);
    lastBtn->setIcon(QIcon::fromTheme("printer_final"));
    pbottomlayout->addStretch();
    pbottomlayout->addWidget(firstBtn);
    pbottomlayout->addSpacing(10);
    pbottomlayout->addWidget(prevPageBtn);
    pbottomlayout->addSpacing(55);
    pbottomlayout->addWidget(jumpPageEdit);
    pbottomlayout->addWidget(spaceLabel);
    pbottomlayout->addWidget(totalPageLabel);
    pbottomlayout->addSpacing(55);
    pbottomlayout->addWidget(nextPageBtn);
    pbottomlayout->addSpacing(10);
    pbottomlayout->addWidget(lastBtn);
    pbottomlayout->addStretch();

    QRegExp reg("^([1-9][0-9]*)");
    QRegExpValidator *val = new QRegExpValidator(reg);
    jumpPageEdit->lineEdit()->setValidator(val);

    DPalette m_pa = DApplicationHelper::instance()->palette(pview);
    m_pa.setBrush(DPalette::Base, m_pa.itemBackground());
    DApplicationHelper::instance()->setPalette(pview, m_pa);
}

void DPrintPreviewDialogPrivate::initright(QVBoxLayout *layout)
{
    Q_Q(DPrintPreviewDialog);
    //top
    QVBoxLayout *ptoplayout = new QVBoxLayout;
    ptoplayout->setContentsMargins(0, 0, 0, 0);
    DWidget *ptopwidget = new DWidget;
    ptopwidget->setMinimumWidth(442);
    ptopwidget->setLayout(ptoplayout);

    basicsettingwdg = new DWidget;
    advancesettingwdg = new DWidget;
    basicsettingwdg->setFixedHeight(415);
    scrollarea = new DScrollArea;
    scrollarea->setWidget(ptopwidget);
    scrollarea->setWidgetResizable(true);
    scrollarea->setFrameShape(QFrame::NoFrame);
    scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    advancesettingwdg->hide();

    advanceBtn = new DPushButton(qApp->translate("DPrintPreviewDialogPrivate", "Advanced"));
    advanceBtn->setLayoutDirection(Qt::RightToLeft);
    advanceBtn->setIcon(QIcon::fromTheme("printer_dropdown"));
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
    cancelBtn = new DPushButton(qApp->translate("DPrintPreviewDialogPrivate", "Cancel"));
    printBtn = new DSuggestButton(qApp->translate("DPrintPreviewDialogPrivate", "Print"));

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
    layout->addWidget(printerFrame);
    printerFrame->setMinimumSize(422, 48);
    setfrmaeback(printerFrame);
    QHBoxLayout *printerlayout = new QHBoxLayout(printerFrame);
    printerlayout->setContentsMargins(10, 0, 10, 0);
    DLabel *printerlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Printer"), printerFrame);
    printerlabel->setFixedWidth(123);
    printDeviceCombo = new DComboBox(basicsettingwdg);
    printDeviceCombo->setMinimumSize(275, 36);
    printerlayout->addWidget(printerlabel);
    printerlayout->addWidget(printDeviceCombo);
    printerlayout->setAlignment(printDeviceCombo, Qt::AlignVCenter);

    //打印份数
    DFrame *copycountFrame = new DFrame(basicsettingwdg);
    copycountFrame->setObjectName("copucountframe");
    layout->addWidget(copycountFrame);
    copycountFrame->setMinimumSize(422, 48);
    setfrmaeback(copycountFrame);
    QHBoxLayout *copycountlayout = new QHBoxLayout(copycountFrame);
    copycountlayout->setContentsMargins(10, 0, 10, 0);
    DLabel *copycountlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Copies"), copycountFrame);
    copycountlabel->setFixedWidth(123);
    copycountspinbox = new DSpinBox(copycountFrame);
    copycountspinbox->setEnabledEmbedStyle(true);
    copycountspinbox->setRange(1, 999);
    copycountspinbox->setMinimumSize(275, 36);
    copycountlayout->addWidget(copycountlabel);
    copycountlayout->addWidget(copycountspinbox);

    QRegExp re("^[1-9][0-9][0-9]$");
    QRegExpValidator *va = new QRegExpValidator(re);
    copycountspinbox->lineEdit()->setValidator(va);

    //页码范围
    DFrame *pageFrame = new DFrame(basicsettingwdg);
    pageFrame->setObjectName("pageFrame");
    layout->addWidget(pageFrame);
    pageFrame->setMinimumSize(422, 94);
    setfrmaeback(pageFrame);
    QVBoxLayout *pagelayout = new QVBoxLayout(pageFrame);
    pagelayout->setContentsMargins(10, 5, 10, 5);
    DLabel *pagerangelabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Page range"), pageFrame);
    pagerangelabel->setFixedWidth(123);
    pageRangeCombo = new DComboBox(pageFrame);
    pageRangeCombo->setMinimumSize(275, 36);
    pageRangeCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "All"));
    pageRangeCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Current page"));
    pageRangeCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Select pages"));
    QHBoxLayout *hrangebox = new QHBoxLayout();
    hrangebox->addWidget(pagerangelabel);
    hrangebox->addWidget(pageRangeCombo);

    pageRangeEdit = new DLineEdit;
    pagelayout->addLayout(hrangebox);
    pagelayout->addWidget(pageRangeEdit);
    pageRangeEdit->installEventFilter(q);

    QRegExp reg("^(([1-9][0-9]*)+(\\,)?|([1-9][0-9]*-[1-9][0-9]*)+(\\,)?)*");
    QRegExpValidator *val = new QRegExpValidator(reg);
    pageRangeEdit->lineEdit()->setValidator(val);

    //打印方向
    DLabel *orientationLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Orientation"), basicsettingwdg);
    setwidgetfont(orientationLabel);
    QHBoxLayout *orientationtitlelayout = new QHBoxLayout;
    orientationtitlelayout->setContentsMargins(10, 0, 0, 0);
    orientationtitlelayout->addWidget(orientationLabel);
    orientationtitlelayout->setAlignment(orientationLabel, Qt::AlignLeft | Qt::AlignBottom);
    layout->addLayout(orientationtitlelayout);

    QVBoxLayout *orientationlayout = new QVBoxLayout;
    orientationlayout->setContentsMargins(0, 0, 0, 0);
    DRadioButton *verRadio = new DRadioButton;
    verRadio->setIcon(QIcon::fromTheme("printer_portrait"));
    verRadio->setIconSize(QSize(36, 36));
    DRadioButton *horRadio = new DRadioButton;
    horRadio->setIcon(QIcon::fromTheme("printer_landscape"));
    horRadio->setIconSize(QSize(36, 36));
    orientationgroup = new QButtonGroup(q);
    orientationgroup->addButton(verRadio, 0);
    orientationgroup->addButton(horRadio, 1);

    //纵向
    DWidget *portraitwdg = new DWidget;
    portraitwdg->setMinimumSize(422, 48);
    QHBoxLayout *portraitlayout = new QHBoxLayout;
    DLabel *orientationTextLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Portrait"), portraitwdg);
    portraitlayout->addWidget(verRadio);
    portraitlayout->addWidget(orientationTextLabel);
    portraitlayout->addStretch();
    portraitwdg->setLayout(portraitlayout);

    //横向
    DWidget *landscapewdg = new DWidget;
    landscapewdg->setMinimumSize(422, 48);
    QHBoxLayout *landscapelayout = new QHBoxLayout;
    DLabel *landscapeTextLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Landscape"), portraitwdg);
    landscapelayout->addWidget(horRadio);
    landscapelayout->addWidget(landscapeTextLabel);
    landscapelayout->addStretch();
    landscapewdg->setLayout(landscapelayout);

    orientationlayout->addWidget(portraitwdg);
    orientationlayout->addWidget(landscapewdg);
    DBackgroundGroup *back = new DBackgroundGroup(orientationlayout);
    back->setItemSpacing(2);
    DPalette pa = DApplicationHelper::instance()->palette(back);
    pa.setBrush(DPalette::Base, pa.itemBackground());
    DApplicationHelper::instance()->setPalette(back, pa);
    layout->addWidget(back);
}

void DPrintPreviewDialogPrivate::initadvanceui()
{
    Q_Q(DPrintPreviewDialog);
    QVBoxLayout *layout = new QVBoxLayout(advancesettingwdg);
    layout->setContentsMargins(0, 0, 0, 0);
    advancesettingwdg->setMinimumWidth(442);

    //页面设置
    QVBoxLayout *pagelayout = new QVBoxLayout;
    pagelayout->setSpacing(10);
    pagelayout->setContentsMargins(10, 0, 10, 0);
    DLabel *pagesLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Pages"), advancesettingwdg);
    setwidgetfont(pagesLabel, DFontSizeManager::T5);
    QHBoxLayout *pagestitlelayout = new QHBoxLayout;
    pagestitlelayout->setContentsMargins(10, 20, 0, 0);
    pagestitlelayout->addWidget(pagesLabel, Qt::AlignLeft | Qt::AlignBottom);

    DFrame *colorframe = new DFrame;
    setfrmaeback(colorframe);
    colorframe->setFixedHeight(48);
    QHBoxLayout *colorlayout = new QHBoxLayout(colorframe);
    DLabel *colorlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Color mode"));
    colorlabel->setFixedWidth(123);
    colorModeCombo = new DComboBox;
    colorModeCombo->addItems(QStringList() << qApp->translate("DPrintPreviewDialogPrivate", "Color") << qApp->translate("DPrintPreviewDialogPrivate", "Grayscale"));
    colorlayout->addWidget(colorlabel);
    colorlayout->addWidget(colorModeCombo);
    colorlayout->setContentsMargins(10, 4, 10, 4);

    DFrame *marginsframe = new DFrame;
    marginsframe->setObjectName("marginsFrame");
    setfrmaeback(marginsframe);
    QVBoxLayout *marginslayout = new QVBoxLayout(marginsframe);
    marginslayout->setContentsMargins(10, 5, 10, 5);
    QHBoxLayout *marginscombolayout = new QHBoxLayout;
    DLabel *marginlabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Margins"));
    marginlabel->setFixedSize(123, 36);
    marginsCombo = new DComboBox;
    marginsCombo->addItems(QStringList() << qApp->translate("DPrintPreviewDialogPrivate", "Narrow(mm)") << qApp->translate("DPrintPreviewDialogPrivate", "Normal(mm)") << qApp->translate("DPrintPreviewDialogPrivate", "Moderate(mm)") << qApp->translate("DPrintPreviewDialogPrivate", "Customize(mm)"));
    marginscombolayout->addWidget(marginlabel);
    marginscombolayout->addWidget(marginsCombo);

    QHBoxLayout *marginsspinlayout = new QHBoxLayout;
    marginsspinlayout->setContentsMargins(0, 0, 0, 0);
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
    QVBoxLayout *marginslabellayout2 = new QVBoxLayout;
    marginslabellayout2->addWidget(bottomlabel);
    marginslabellayout2->addWidget(rightlabel);
    QVBoxLayout *marginsspinboxlayout2 = new QVBoxLayout;
    marginsspinboxlayout2->addWidget(marginBottomSpin);
    marginsspinboxlayout2->addWidget(marginRightSpin);
    marginsspinlayout->addStretch();
    marginsLayout(true);

    marginsspinlayout->addLayout(marginslabellayout1);
    marginsspinlayout->addLayout(marginsspinboxlayout1);
    marginsspinlayout->addLayout(marginslabellayout2);
    marginsspinlayout->addLayout(marginsspinboxlayout2);
    marginslayout->addLayout(marginscombolayout);
    marginslayout->addLayout(marginsspinlayout);

    QRegExp reg("^([5-5][0-4]|[1-4][0-9]|[0-9])(\\.[0-9][0-9])|55(\\.[8-8][0-8])|55(\\.[0-7][0-9])");
    QRegExpValidator *val = new QRegExpValidator(reg);
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
    QHBoxLayout *scalingtitlelayout = new QHBoxLayout;
    scalingtitlelayout->setContentsMargins(10, 20, 0, 0);
    scalingtitlelayout->addWidget(scalingLabel, Qt::AlignLeft | Qt::AlignBottom);
    setwidgetfont(scalingLabel, DFontSizeManager::T5);

    scaleGroup = new QButtonGroup(q);
    QVBoxLayout *scalingcontentlayout = new QVBoxLayout;
    scalingcontentlayout->setContentsMargins(0, 0, 0, 0);

    DWidget *actualwdg = new DWidget;
    actualwdg->setFixedHeight(48);
    QHBoxLayout *actuallayout = new QHBoxLayout(actualwdg);
    actuallayout->setContentsMargins(10, 0, 10, 0);
    DRadioButton *actualSizeRadio = new DRadioButton(qApp->translate("DPrintPreviewDialogPrivate", "Actual size"));

    scaleGroup->addButton(actualSizeRadio, ACTUAL_SIZE);
    actuallayout->addWidget(actualSizeRadio);

    DWidget *customscalewdg = new DWidget;
    customscalewdg->setFixedHeight(48);
    QHBoxLayout *customlayout = new QHBoxLayout(customscalewdg);
    customlayout->setContentsMargins(10, 0, 10, 0);
    DRadioButton *customSizeRadio = new DRadioButton(qApp->translate("DPrintPreviewDialogPrivate", "Scale"));
    scaleGroup->addButton(customSizeRadio, SCALE);
    scaleRateEdit = new DSpinBox;
    QRegExp scaleReg("^([1-9][0-9]?|[1][0-9]{2}|200)$");
    QRegExpValidator *scaleVal = new QRegExpValidator(scaleReg);
    scaleRateEdit->lineEdit()->setValidator(scaleVal);
    scaleRateEdit->setEnabledEmbedStyle(true);
    scaleRateEdit->setRange(1, 200);
    scaleRateEdit->setFixedWidth(78);
    scaleRateEdit->installEventFilter(q);
    DLabel *scaleLabel = new DLabel("%");
    customlayout->addWidget(customSizeRadio);
    customlayout->addWidget(scaleRateEdit);
    customlayout->addWidget(scaleLabel);
    customlayout->addStretch(1);

    scalingcontentlayout->addWidget(actualwdg);
    scalingcontentlayout->addWidget(customscalewdg);

    DBackgroundGroup *back = new DBackgroundGroup(scalingcontentlayout);
    back->setItemSpacing(1);
    DPalette pa = DApplicationHelper::instance()->palette(back);
    pa.setBrush(DPalette::Base, pa.itemBackground());
    DApplicationHelper::instance()->setPalette(back, pa);
    scalinglayout->addLayout(scalingtitlelayout);
    scalinglayout->addWidget(back);

    //纸张
    QVBoxLayout *paperlayout = new QVBoxLayout;
    paperlayout->setContentsMargins(10, 0, 10, 0);
    DLabel *paperLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Paper"), advancesettingwdg);
    setwidgetfont(paperLabel, DFontSizeManager::T5);
    QHBoxLayout *papertitlelayout = new QHBoxLayout;
    papertitlelayout->setContentsMargins(10, 0, 0, 0);
    papertitlelayout->addWidget(paperLabel, Qt::AlignLeft | Qt::AlignBottom);

    DFrame *paperframe = new DFrame;
    setfrmaeback(paperframe);
    paperframe->setFixedHeight(48);
    QHBoxLayout *paperframelayout = new QHBoxLayout(paperframe);
    DLabel *papersizelabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Paper size"));
    papersizelabel->setFixedWidth(123);
    paperSizeCombo = new DComboBox;
    paperSizeCombo->setFixedHeight(36);
    paperframelayout->addWidget(papersizelabel);
    paperframelayout->addWidget(paperSizeCombo);
    paperframelayout->setContentsMargins(10, 4, 10, 4);
    paperlayout->addLayout(papertitlelayout);
    paperlayout->addWidget(paperframe);

    //打印方式
    QVBoxLayout *drawinglayout = new QVBoxLayout;
    drawinglayout->setSpacing(10);
    drawinglayout->setContentsMargins(10, 0, 10, 0);
    DLabel *drawingLabel = new DLabel(qApp->translate("DPrintPreviewDialogPrivate", "Layout"), advancesettingwdg);
    setwidgetfont(drawingLabel, DFontSizeManager::T5);
    QHBoxLayout *drawingtitlelayout = new QHBoxLayout;
    drawingtitlelayout->setContentsMargins(10, 20, 0, 0);
    drawingtitlelayout->addWidget(drawingLabel, Qt::AlignLeft | Qt::AlignBottom);

    DFrame *duplexframe = new DFrame;
    setfrmaeback(duplexframe);
    duplexframe->setFixedHeight(48);
    QHBoxLayout *duplexlayout = new QHBoxLayout(duplexframe);
    duplexCombo = new DComboBox;
    duplexCheckBox = new DCheckBox(qApp->translate("DPrintPreviewDialogPrivate", "Duplex"));
    duplexCheckBox->setFixedWidth(123);
    duplexCombo->setFixedHeight(36);
    duplexlayout->setContentsMargins(14, 4, 10, 4);
    duplexlayout->addWidget(duplexCheckBox);
    duplexlayout->addWidget(duplexCombo);

    drawinglayout->addLayout(drawingtitlelayout);
    drawinglayout->addWidget(duplexframe);

    layout->addLayout(paperlayout);
    layout->addLayout(drawinglayout);
    layout->addLayout(pagelayout);
    layout->addLayout(scalinglayout);
}

void DPrintPreviewDialogPrivate::marginsLayout(bool adapted)
{
    if (adapted) {
        marginTopSpin->setMaximumWidth(MARGIN_SIZE_LESS);
        marginLeftSpin->setMaximumWidth(MARGIN_SIZE_LESS);
        marginRightSpin->setMaximumWidth(MARGIN_SIZE_LESS);
        marginBottomSpin->setMaximumWidth(MARGIN_SIZE_LESS);
    } else {
        marginTopSpin->setMaximumWidth(MARGIN_SIZE_MORE);
        marginLeftSpin->setMaximumWidth(MARGIN_SIZE_MORE);
        marginRightSpin->setMaximumWidth(MARGIN_SIZE_MORE);
        marginBottomSpin->setMaximumWidth(MARGIN_SIZE_MORE);
    }
}

void DPrintPreviewDialogPrivate::initdata()
{
    Q_Q(DPrintPreviewDialog);
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
    duplexCombo->setEnabled(false);
    isInited = true;
    fontSizeMore = true;
}

void DPrintPreviewDialogPrivate::initconnections()
{
    Q_Q(DPrintPreviewDialog);

    QObject::connect(pview, &DPrintPreviewWidget::paintRequested, q, &DPrintPreviewDialog::paintRequested);

    QObject::connect(advanceBtn, &QPushButton::clicked, q, [this] { this->showadvancesetting(); });
    QObject::connect(printDeviceCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_printerChanged(int)));
    QObject::connect(cancelBtn, &DPushButton::clicked, q, &DPrintPreviewDialog::close);
    QObject::connect(pageRangeCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_pageRangeChanged(int)));
    QObject::connect(marginsCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_pageMarginChanged(int)));
    QObject::connect(printBtn, SIGNAL(clicked(bool)), q, SLOT(_q_startPrint(bool)));
    QObject::connect(colorModeCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_ColorModeChange(int)));
    QObject::connect(orientationgroup, SIGNAL(buttonClicked(int)), q, SLOT(_q_orientationChanged(int)));
    QObject::connect(pageRangeEdit, &DLineEdit::editingFinished, [=] {
        _q_customPagesFinished();
    });
    QObject::connect(pageRangeEdit, &DLineEdit::focusChanged, q, [this](bool onFocus) {
        isOnFocus = true;
        if (pageRangeEdit->text().right(1) == "-" && !onFocus) {
            this->_q_customPagesFinished();
        }
    });
    QObject::connect(jumpPageEdit->lineEdit(), &QLineEdit::textChanged, q, [ = ](QString str) {
        if (str.toInt() > totalPageLabel->text().toInt())
            jumpPageEdit->lineEdit()->setText(totalPageLabel->text());
    });

    QObject::connect(pview, &DPrintPreviewWidget::totalPages, [this](int pages) {
        jumpPageEdit->setRange(1, pages);
        totalPageLabel->setText(QString::number(pages));
        totalPages = pages;
        jumpPageEdit->setMaximum(totalPages);
        setTurnPageBtnStatus();
    });
    QObject::connect(pview, &DPrintPreviewWidget::pagesCountChanged, [this](int pages) {
        totalPageLabel->setNum(pages);
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
    QObject::connect(jumpPageEdit->lineEdit(), &QLineEdit::editingFinished, [this]() {
        pview->setCurrentPage(jumpPageEdit->value());
        setTurnPageBtnStatus();
    });
    QObject::connect(paperSizeCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), q, [this](int index) {
        QPrinterInfo prInfo(*printer);
        if (paperSizeCombo->count() == 0) {
            printer->setPageSize(QPrinter::A4);
            return ;
        }
        if (printDeviceCombo->currentIndex() != printDeviceCombo->count() - 1 && printDeviceCombo->currentIndex() != printDeviceCombo->count() - 2) {
            printer->setPageSize(prInfo.supportedPageSizes().at(paperSizeCombo->currentIndex()));
        } else {
            switch (paperSizeCombo->currentIndex()) {
            case 0:
                printer->setPageSize(QPrinter::A3);
                break;
            case 1:
                printer->setPageSize(QPrinter::A4);
                break;
            case 2:
                printer->setPageSize(QPrinter::A5);
                break;
            case 3:
                printer->setPageSize(QPrinter::B4);
                break;
            case 4:
                printer->setPageSize(QPrinter::B5);
                break;
            case 5:
                printer->setPageSize(QPrinter::Custom);
                printer->setPageSizeMM(QSizeF(EightK_Weight, EightK_Height));
                break;
            case 6:
                printer->setPageSize(QPrinter::Custom);
                printer->setPageSizeMM(QSizeF(SixteenK_Weight, SixteenK_Height));
                break;
            }
        }
        if (isInited) {
            this->marginsUpdate(false);
        }
        if (pview->pageRangeMode() == DPrintPreviewWidget::SelectPage)
            pageRangeCombo->setCurrentIndex(PAGERANGE_ALL);

    });

    QObject::connect(scaleRateEdit->lineEdit(), &QLineEdit::editingFinished, q, [=] {
        if (scaleGroup->checkedId() == SCALE) {
            if (scaleRateEdit->value() < 10)
                scaleRateEdit->setValue(10);
            qreal scale = scaleRateEdit->value() / 100.0;
            pview->setScale(scale);
            pview->updateView();
        }
    });
    QObject::connect(scaleGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), q, [this](int id) {
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

    QObject::connect(marginTopSpin, SIGNAL(valueChanged(double)), q, SLOT(_q_marginspinChanged(double)));
    QObject::connect(marginRightSpin, SIGNAL(valueChanged(double)), q, SLOT(_q_marginspinChanged(double)));
    QObject::connect(marginLeftSpin, SIGNAL(valueChanged(double)), q, SLOT(_q_marginspinChanged(double)));
    QObject::connect(marginBottomSpin, SIGNAL(valueChanged(double)), q, SLOT(_q_marginspinChanged(double)));
    QObject::connect(duplexCheckBox, SIGNAL(stateChanged(int)), q, SLOT(_q_checkStateChanged(int)));
    QObject::connect(DApplicationHelper::instance(), &DApplicationHelper::themeTypeChanged, pview, &DPrintPreviewWidget::themeTypeChanged);
    QObject::connect(marginTopSpin, SIGNAL(editingFinished()), q, SLOT(_q_marginEditFinished()));
    QObject::connect(marginRightSpin, SIGNAL(editingFinished()), q, SLOT(_q_marginEditFinished()));
    QObject::connect(marginLeftSpin, SIGNAL(editingFinished()), q, SLOT(_q_marginEditFinished()));
    QObject::connect(marginBottomSpin, SIGNAL(editingFinished()), q, SLOT(_q_marginEditFinished()));
    QObject::connect(DApplicationHelper::instance(), &DApplicationHelper::themeTypeChanged, q, [this](DGuiApplicationHelper::ColorType themeType) { this->themeTypeChange(themeType); });
}

void DPrintPreviewDialogPrivate::setfrmaeback(DFrame *frame)
{
    frame->setLineWidth(0);
    DPalette pa = DApplicationHelper::instance()->palette(frame);
    pa.setBrush(DPalette::Base, pa.itemBackground());
    DApplicationHelper::instance()->setPalette(frame, pa);
}

void DPrintPreviewDialogPrivate::showadvancesetting()
{
    if (advancesettingwdg->isHidden()) {
        advancesettingwdg->show();
        advanceBtn->setText(qApp->translate("DPrintPreviewDialogPrivate", "Collapse"));
        advanceBtn->setIcon(QIcon::fromTheme("printer_dropup"));
    } else {
        advancesettingwdg->hide();
        advanceBtn->setText(qApp->translate("DPrintPreviewDialogPrivate", "Advanced"));
        advanceBtn->setIcon(QIcon::fromTheme("printer_dropdown"));
    }
}

/*!
 * \~chinese DPrintPreviewDialogPrivate::setupPrinter 设置打印的相关基础属性和高级属性
 */
void DPrintPreviewDialogPrivate::setupPrinter()
{
    //基础设置
    //设置打印份数
    printer->setNumCopies(copycountspinbox->value());
    //设置打印方向
    if (orientationgroup->checkedId() == 0)
        printer->setOrientation(QPrinter::Portrait);
    else {
        printer->setOrientation(QPrinter::Landscape);
    }
    //高级设置
    //设置纸张大小
    QPrinterInfo prInfo(*printer);
    if (printDeviceCombo->currentIndex() != printDeviceCombo->count() - 1
        && printDeviceCombo->currentIndex() != printDeviceCombo->count() - 2) {
        printer->setPageSize(prInfo.supportedPageSizes().at(paperSizeCombo->currentIndex()));
    } else {
        switch (paperSizeCombo->currentIndex()) {
        case 0:
            printer->setPageSize(QPrinter::A3);
            break;
        case 1:
            printer->setPageSize(QPrinter::A4);
            break;
        case 2:
            printer->setPageSize(QPrinter::A5);
            break;
        case 3:
            printer->setPageSize(QPrinter::B4);
            break;
        case 4:
            printer->setPageSize(QPrinter::B5);
            break;
        case 5:
            printer->setPageSize(QPrinter::Custom);
            printer->setPageSizeMM(QSizeF(EightK_Weight, EightK_Height));
            break;
        case 6:
            printer->setPageSize(QPrinter::Custom);
            printer->setPageSizeMM(QSizeF(SixteenK_Weight, SixteenK_Height));
            break;
        }
    }
    //设置双面打印
    if (duplexCheckBox->isChecked()) {
        if (duplexCombo->count() == 1) {
            if (supportedDuplexFlag)
                printer->setDuplex(QPrinter::DuplexLongSide);
            else
                printer->setDuplex(QPrinter::DuplexShortSide);
        } else {
            switch (duplexCombo->currentIndex()) {
            case 0:
                printer->setDuplex(QPrinter::DuplexLongSide);
                break;
            case 1:
                printer->setDuplex(QPrinter::DuplexShortSide);
                break;
            }
        }
    } else {
        printer->setDuplex(QPrinter::DuplexNone);
    }
    //设置色彩打印
    if (supportedColorMode)
        QCUPSSupport::setCupsOption(printer, "ColorModel", "RGB");
    else {
        QCUPSSupport::setCupsOption(printer, "ColorModel", "Gray");
    }
    //设置纸张打印边距
    printer->setPageMargins(QMarginsF(marginLeftSpin->value(), marginTopSpin->value(), marginRightSpin->value(), marginBottomSpin->value()), QPageLayout::Millimeter);
}

void DPrintPreviewDialogPrivate::judgeSupportedAttributes(const QString &lastPaperSize)
{
    Q_Q(DPrintPreviewDialog);
    QPrinterInfo updateinfo(*printer);

    QStringList pageSizeList;
    for (int i = 0; i < updateinfo.supportedPageSizes().size(); i++) {
        pageSizeList.append(updateinfo.supportedPageSizes().at(i).key());
    }
    paperSizeCombo->addItems(pageSizeList);
    if (pageSizeList.contains(lastPaperSize)) {
        paperSizeCombo->setCurrentText(lastPaperSize);
    } else {
        //调用绘制预览
        paperSizeCombo->blockSignals(false);
        paperSizeCombo->setCurrentText("A4");
    }

    //判断当前打印机是否支持双面打印，不支持禁用双面打印按钮，pdf不做判断
    QString lastDuplexComboText = duplexCombo->currentText();
    duplexCombo->clear();
    if (updateinfo.supportedDuplexModes().contains(QPrinter::DuplexLongSide) || updateinfo.supportedDuplexModes().contains(QPrinter::DuplexShortSide)) {
        duplexCheckBox->setEnabled(true);
        if (!updateinfo.supportedDuplexModes().contains(QPrinter::DuplexLongSide)) {
            duplexCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Flip on short edge"));
            supportedDuplexFlag = false;
        } else if (!updateinfo.supportedDuplexModes().contains(QPrinter::DuplexShortSide)) {
            duplexCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Flip on long edge"));
            supportedDuplexFlag = true;
        } else if (updateinfo.supportedDuplexModes().contains(QPrinter::DuplexLongSide) && updateinfo.supportedDuplexModes().contains(QPrinter::DuplexShortSide)) {
            duplexCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Flip on long edge"));
            duplexCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Flip on short edge"));
            duplexCombo->setCurrentText(lastDuplexComboText);
        }
    } else {
        duplexCheckBox->setChecked(false);
        duplexCheckBox->setEnabled(false);
        duplexCombo->setEnabled(false);
    }
}

/*!
 * \~chinese DPrintPreviewDialogPrivate::setMininumMargins 如果边距小于最小边距,设置成最小边距
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
 * \~chinese \brief DPrintPreviewDialogPrivate::marginsUpdate 切换打印机和纸张切换对于边距的刷新和预览页面的刷新
 * \~chinese \param isPrinterchanged 判断是打印机切换或者是纸张切换
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
    DWidget *titleWidget = q->findChild<DWidget *>("titlewidget");
    DPalette m_pa = DApplicationHelper::instance()->palette(titleWidget);
    m_pa.setBrush(DPalette::Background, m_pa.base());
    DApplicationHelper::instance()->setPalette(titleWidget, m_pa);
    titleWidget->setAutoFillBackground(true);

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
        DApplicationHelper::instance()->setPalette(m_frameList.at(i), pa);
    }
    for (int i = 0; i < m_back.size(); i++) {
        DApplicationHelper::instance()->setPalette(m_back.at(i), pa);
    }
}

/*!
 * \~chinese DPrintPreviewDialogPrivate::setPageIsLegal 检查自定义页码是否合法,不合法的话设置警告颜色,打印保存按钮置灰
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
    qSort(data.begin(), data.end());
    return data;
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::setEnable 设置纸张范围自定义可输入状态
 * \~chinese \param value 判断选择的范围类型
 * \~chinese \param combox 选择的具体的combox
 */
void DPrintPreviewDialogPrivate::setEnable(const int &value, DComboBox *combox)
{
    if (combox == pageRangeCombo) {
        if (value != pageRangeCombo->count() - 1) {
            pageRangeEdit->setEnabled(false);
        } else {
            pageRangeEdit->setEnabled(true);
        }
    }
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::setEnable 设置翻页按钮状态
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
    } else if (currentPage == totalPage) {
        firstBtn->setEnabled(true);
        prevPageBtn->setEnabled(true);
        nextPageBtn->setEnabled(false);
        lastBtn->setEnabled(false);
    }
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_printerChanged 根据选取不同的打印设备刷新界面和属性
 * \~chinese \param index 判断选取的打印设备
 */
void DPrintPreviewDialogPrivate::_q_printerChanged(int index)
{
    Q_Q(DPrintPreviewDialog);
    QString lastPaperSize = paperSizeCombo->currentText();
    QString lastColormode = colorModeCombo->currentText();
    paperSizeCombo->clear();
    paperSizeCombo->blockSignals(true);
    colorModeCombo->blockSignals(true);
    if (index == printDeviceCombo->count() - 1
        || index == printDeviceCombo->count() - 2) {
        //pdf
        copycountspinbox->setDisabled(true);
        copycountspinbox->setValue(1);
        duplexCheckBox->setCheckState(Qt::Unchecked);
        duplexCheckBox->setEnabled(false);
        duplexCombo->clear();
        duplexCombo->setEnabled(false);
        if (colorModeCombo->count() == 1)
            colorModeCombo->insertItem(0, qApp->translate("DPrintPreviewDialogPrivate", "Color"));
        colorModeCombo->blockSignals(false);
        colorModeCombo->setCurrentIndex(0);
        colorModeCombo->setEnabled(false);
        supportedColorMode = true;
        printBtn->setText(qApp->translate("DPrintPreviewDialogPrivate", "Save"));
        paperSizeCombo->setCurrentIndex(1);
        QStringList pdfPaperSize = QStringList() << "A3"
                                                 << "A4"
                                                 << "A5"
                                                 << "B4"
                                                 << "B5"
                                                 << "8K"
                                                 << "16K";
        paperSizeCombo->addItems(pdfPaperSize);
        if (pdfPaperSize.contains(lastPaperSize))
            paperSizeCombo->setCurrentText(lastPaperSize);
        else {
            //调用绘制预览
            paperSizeCombo->blockSignals(false);
            paperSizeCombo->setCurrentIndex(1);
        }
        printer->setPrinterName("");
    } else {
        //actual printer
        if (printer) {
            copycountspinbox->setDisabled(false);
            paperSizeCombo->setEnabled(true);
            colorModeCombo->setEnabled(true);
            printer->setPrinterName(printDeviceCombo->itemText(index));
            printBtn->setText(qApp->translate("DPrintPreviewDialogPrivate", "Print"));
            judgeSupportedAttributes(lastPaperSize);
        }
        //判断当前打印机是否支持彩色打印，不支持彩色打印删除彩色打印选择选项，pdf不做判断
        QPlatformPrinterSupport *ps = QPlatformPrinterSupportPlugin::get();
        QPrintDevice currentDevice = ps->createPrintDevice(printDeviceCombo->currentText());
        colorModeCombo->clear();
        if (!currentDevice.supportedColorModes().contains(QPrint::Color)) {
            colorModeCombo->blockSignals(false);
            colorModeCombo->addItem(qApp->translate("DPrintPreviewDialogPrivate", "Grayscale"));
            supportedColorMode = false;
        } else {
            colorModeCombo->addItems(QStringList() << qApp->translate("DPrintPreviewDialogPrivate", "Color") << qApp->translate("DPrintPreviewDialogPrivate", "Grayscale"));
            colorModeCombo->blockSignals(false);
            if (colorModeCombo->currentText() == lastColormode) {
                colorModeCombo->setCurrentIndex(0);
                supportedColorMode = true;
            } else {
                colorModeCombo->setCurrentIndex(1);
                supportedColorMode = false;
            }
        }
    }
    marginsUpdate(true);
    if (pview->pageRangeMode() == DPrintPreviewWidget::SelectPage)
        pageRangeCombo->setCurrentIndex(PAGERANGE_ALL);
    paperSizeCombo->blockSignals(false);
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::slotPageRangeCombox 根据选取的不同纸张范围类型设置纸张范围
 * \~chinese \param index 判断不同的范围类型
 */
void DPrintPreviewDialogPrivate::_q_pageRangeChanged(int index)
{
    Q_Q(DPrintPreviewDialog);
    setEnable(index, pageRangeCombo);
    pageRangeEdit->lineEdit()->setPlaceholderText("");
    pageRangeEdit->setText("");
    if (index == DPrintPreviewWidget::AllPage || index == DPrintPreviewWidget::CurrentPage) {
        pview->setPageRangeMode((DPrintPreviewWidget::PageRange)index);
        setPageIsLegal(true);
        if (totalPages != 0) {
            totalPageLabel->setNum(totalPages);
            if (isInited)
                pview->setPageRange(FIRST_PAGE, totalPages);
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
            pageRangeEdit->lineEdit()->setPlaceholderText(qApp->translate("DPrintPreviewDialogPrivate", "For example, 1,3,5-7,11-15,18,21"));
        }
    }
    setTurnPageBtnStatus();
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_pageMarginChanged 根据选择不同的设置边距类型设置边距
 * \~chinese \param index 判断选取的不同的设置边距的类型
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

    if (pview->pageRangeMode() == DPrintPreviewWidget::SelectPage)
        pageRangeCombo->setCurrentIndex(PAGERANGE_ALL);

    if (marginOldValue.length() > 4)
        marginOldValue.clear();

    marginOldValue << marginTopSpin->value() << marginLeftSpin->value() << marginRightSpin->value() << marginBottomSpin->value();
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_ColorModeChange 根据色彩选项，调整预览图中的色彩为黑白或者彩色
 * \~chinese \param index 当前选择彩色预览还是黑白预览
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
        supportedColorMode = true;
    } else {
        // gray
        pview->setColorMode(DPrinter::GrayScale);
        supportedColorMode = false;
    }
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_orientationChanged 根据按钮索引切换打印方向为横向或者纵向
 * \~chinese \param index 当前按钮是横向按钮还是纵向按钮
 */
void DPrintPreviewDialogPrivate::_q_orientationChanged(int index)
{
    if (index == 0) {
        // 纵向按钮
        if (isInited) {
            pview->setOrientation(DPrinter::Portrait);
            pview->reviewChange(true);
        }
    } else {
        // 横向按钮
        pview->setOrientation(DPrinter::Landscape);
        pview->reviewChange(true);
    }
    if (pview->pageRangeMode() == DPrintPreviewWidget::SelectPage)
        pageRangeCombo->setCurrentIndex(PAGERANGE_ALL);
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_customPagesFinished 自定义页面时，页码处理
 * \~chinese \param index
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
                    if (list1.at(0).toInt() <= list1.at(1).toInt()) {
                        for (int page = list1.at(0).toInt(); page <= list1.at(1).toInt(); page++) {
                            if (page != 0 && page <= totalPages) {
                                pagesrange.append(page);
                            } else {
                                setPageIsLegal(false);
                                if (isOnFocus)
                                    tipSelected(MaxTip);
                                isOnFocus = false;
                                return;
                            }
                        }
                    } else { //“-”后值大于前值，则回车自动格式化
                        QString temp = "";
                        temp = list1.at(0);
                        list1.replace(0, list1.at(1));
                        list1.replace(1, temp);
                        QString str = list1.join("-");
                        list.replace(i, str);
                        pageRangeEdit->setText(list.join(","));
                        if (list1.at(0).toInt() > totalPages || list1.at(1).toInt() > totalPages) {
                            setPageIsLegal(false);
                            if (isOnFocus)
                                tipSelected(MaxTip);
                            isOnFocus = false;
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
                        setPageIsLegal(false);
                        if (isOnFocus)
                            tipSelected(MaxTip);
                        isOnFocus = false;
                        return;
                    }
                }
            }
        } else {
            setPageIsLegal(false);
            if (isOnFocus)
                tipSelected(FormatTip);
            isOnFocus = false;
            return;
        }
    } else {
        setPageIsLegal(false);
        if (isOnFocus)
            tipSelected(NullTip);
        isOnFocus = false;
        return;
    }
    jumpPageEdit->setValue(1);
    QVector<int> page = checkDuplication(pagesrange);
    pview->setPageRange(page);
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::adjustMargins 根据输入的边距数据调整边距
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
    if (pview->pageRangeMode() == DPrintPreviewWidget::SelectPage)
        pageRangeCombo->setCurrentIndex(PAGERANGE_ALL);
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_marginspinChanged 自定义页边距spinbox值改变
 * \~chinese \param marginValue 改变的值的大小
 */
void DPrintPreviewDialogPrivate::_q_marginspinChanged(double marginValue)
{
    Q_UNUSED(marginValue);
    marginsCombo->blockSignals(true);
    marginsCombo->setCurrentIndex(marginsCombo->count() - 1);
    marginsCombo->blockSignals(false);
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_marginspinChanged 自定义页边距spinbox焦点改变输入结束
 */
void DPrintPreviewDialogPrivate::_q_marginEditFinished()
{
    Q_Q(DPrintPreviewDialog);
    if (q->focusWidget() == marginTopSpin || q->focusWidget() == marginLeftSpin
            || q->focusWidget() == marginBottomSpin || q->focusWidget() == marginRightSpin)
        return;
    adjustMargins();
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_currentPageSpinChanged 根据当前页的页码的变化,变化翻页按钮的状态
 * \~chinese \param value 当前页码
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
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_checkStateChanged 点击双面打印按钮
 * \~chinese \param state 判断是否选中双面打印按钮
 */
void DPrintPreviewDialogPrivate::_q_checkStateChanged(int state)
{
    if (!state)
        duplexCombo->setEnabled(false);
    else {
        duplexCombo->setEnabled(true);
    }
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_startPrint 点击开始打印，设置属性
 * \~chinese \param clicked 判断按钮点击状态
 */
void DPrintPreviewDialogPrivate::_q_startPrint(bool clicked)
{
    Q_Q(DPrintPreviewDialog);
    if (!clicked) {
        setupPrinter();
    }

    bool isSavePicture = (printDeviceCombo->currentIndex() == printDeviceCombo->count() - 1);
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    desktopPath += QStringLiteral("/");
    if (printDeviceCombo->currentIndex() == printDeviceCombo->count() - 2) {
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

        QString str = DFileDialog::getSaveFileName(q, qApp->translate("DPrintPreviewDialogPrivate", "Save as PDF"), desktopPath, qApp->translate("DPrintPreviewDialogPrivate", "PDF file"));
        if (str.isEmpty())
            return;
        printer->setOutputFileName(str);
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
                                                   qApp->translate("DPrintPreviewDialogPrivate", "Images"),
                                                   nullptr, QFileDialog::ShowDirsOnly);

        QDir savedDir(str);
        if (!savedDir.exists() && !savedDir.mkpath(str))
            return;

        QString imageSuffix = QFileInfo(q->docName()).suffix();
        imageSuffix = !imageSuffix.compare("jpeg", Qt::CaseInsensitive) ? "jpeg" : "png";
        QString imageName = QFileInfo(str).fileName();
        str.append("/").append(imageName).append(".").append(imageSuffix);

        if (str.isEmpty())
            return;
        printer->setOutputFileName(str);
    }

    pview->print(isSavePicture);

    q->done(0);
}

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
    if (nullptr == d->printer)
        delete d->printer;
}

bool DPrintPreviewDialog::event(QEvent *event)
{
    Q_D(DPrintPreviewDialog);
    if (event->type() == QEvent::ApplicationFontChange || d->fontSizeMore == true) {
        if (DFontSizeManager::fontPixelSize(qGuiApp->font()) <= 15)
            d->marginsLayout(true);
        else {
            d->marginsLayout(false);
        }
        d->fontSizeMore = false;
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

    return DDialog::eventFilter(watched, event);
}

/*!
 * \~chinese \brief DPrintPreviewDialog::setDocName 设置保存PDF的文件名称
 * \~chinese \param str 文件名称
 */
void DPrintPreviewDialog::setDocName(const QString &str)
{
    Q_D(DPrintPreviewDialog);
    d->printer->setDocName(str);
}

/*!
 * \~chinese \brief DPrintPreviewDialog::docName 保存PDF的文件名称
 * \~chinese \return  文件名称
 */
QString DPrintPreviewDialog::docName() const
{
    D_DC(DPrintPreviewDialog);
    return d->printer->docName();
}

void DPrintPreviewDialog::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    Q_D(DPrintPreviewDialog);
    this->findChild<DWidget *>("titlewidget")->setGeometry(0, 0, this->width(), 50);
    this->findChild<DWidget *>("mainwidget")->setGeometry(0, 0, this->width(), this->height());
    double per = static_cast<double>(this->width()) / static_cast<double>(851);
    if (per >= 1.2) {
        this->findChild<DWidget *>("rightWidget")->setMaximumWidth(452 * 1.2);
        this->findChild<DWidget *>("leftWidget")->setMaximumWidth(this->width() - 20 - 10 - 452 * 1.2);
        this->findChild<DBackgroundGroup *>("backGround")->setItemSpacing(10);
        d->marginsLayout(false);
    } else {
        this->findChild<DWidget *>("rightWidget")->setMaximumWidth(452 * per);
        this->findChild<DWidget *>("leftWidget")->setMaximumWidth(this->width() - 20 - 2 - 452 * per);
        this->findChild<DBackgroundGroup *>("backGround")->setItemSpacing(2);
        d->marginsLayout(true);
    }
}
DWIDGET_END_NAMESPACE
#include "moc_dprintpreviewdialog.cpp"
