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
#include <DScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QtPrintSupport/QPrinterInfo>
#include <QPrinter>
#include <QDebug>
#include <private/qprint_p.h>
#include <qpa/qplatformprintplugin.h>
#include <qpa/qplatformprintersupport.h>
#include <QTime>
#include <QDesktopServices>
#include <QRegExp>
#include <QRegExpValidator>
#include <QTimer>

#include <cups/ppd.h>
#include <cups/cups.h>
#define NORMAL_LEFT_RIGHT 31.8
#define NORMAL_MODERATE_TOP_BOTTRM 25.4
#define NARROW_ALL 12.7
#define MODERATE_LEFT_RIGHT 19.1

#define EightK_Weight 270
#define EightK_Height 390
#define SixteenK_Weight 195
#define SixteenK_Height 270

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
    this->marginTimer = new QTimer(q);
    this->marginTimer->setSingleShot(true);
    QTime tm;
    tm.start();
    //临时测试方法后期移除
    test();
    qDebug() << __FUNCTION__ << "test cost time******" << tm.elapsed();
    tm.restart();
    initui();
    qDebug() << __FUNCTION__ << "initui cost time******" << tm.elapsed();
    tm.restart();
    initdata();
    qDebug() << __FUNCTION__ << "initdata cost time******" << tm.elapsed();
    tm.restart();
    initconnections();
    qDebug() << __FUNCTION__ << "initconnections cost time******" << tm.elapsed();
}

void DPrintPreviewDialogPrivate::initui()
{
    Q_Q(DPrintPreviewDialog);
    DWidget *titleWidget = new DWidget(q);
    titleWidget->setGeometry(0, 0, q->width(), 50);
    DPalette pa = DApplicationHelper::instance()->palette(titleWidget);
    pa.setBrush(DPalette::Base, pa.base());
    DApplicationHelper::instance()->setPalette(titleWidget, pa);
    titleWidget->setAutoFillBackground(true);

    QHBoxLayout *mainlayout = new QHBoxLayout();
    mainlayout->setContentsMargins(QMargins(0, 0, 0, 0));
    mainlayout->setSpacing(0);
    DFrame *pframe = new DFrame;
    pframe->setLayout(mainlayout);
    pframe->setFixedHeight(536);
    QVBoxLayout *pleftlayout = new QVBoxLayout;
    initleft(pleftlayout);
    DVerticalLine *pvline = new DVerticalLine;
    QVBoxLayout *prightlayout = new QVBoxLayout;
    initright(prightlayout);
    mainlayout->addLayout(pleftlayout);
    mainlayout->addWidget(pvline);
    mainlayout->addLayout(prightlayout);
    q->addContent(pframe);
}

void DPrintPreviewDialogPrivate::initleft(QVBoxLayout *layout)
{
    pview = new DPrintPreviewWidget(this->printer);
    //    pview->setFixedSize(364, 470);
    setfrmaeback(pview);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(pview);
    //    layout->setAlignment(pview, Qt::AlignCenter);
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
    totalPageLabel = new DLabel("123");
    jumpPageEdit->setRange(1, totalPageLabel->text().toInt());
    nextPageBtn = new DIconButton(DStyle::SP_ArrowRight);
    lastBtn = new DIconButton(DStyle::SP_ArrowNext);
    lastBtn->setIcon(QIcon::fromTheme("printer_final"));
    pbottomlayout->addWidget(firstBtn);
    pbottomlayout->addSpacing(10);
    pbottomlayout->addWidget(prevPageBtn);
    pbottomlayout->addStretch();
    pbottomlayout->addWidget(jumpPageEdit);
    pbottomlayout->addWidget(spaceLabel);
    pbottomlayout->addWidget(totalPageLabel);
    pbottomlayout->addStretch();
    pbottomlayout->addWidget(nextPageBtn);
    pbottomlayout->addSpacing(10);
    pbottomlayout->addWidget(lastBtn);
}

void DPrintPreviewDialogPrivate::initright(QVBoxLayout *layout)
{
    Q_Q(DPrintPreviewDialog);
    //top
    QVBoxLayout *ptoplayout = new QVBoxLayout;
    ptoplayout->setContentsMargins(0, 0, 0, 0);
    DWidget *ptopwidget = new DWidget;
    ptopwidget->setFixedWidth(442);
    ptopwidget->setLayout(ptoplayout);

    basicsettingwdg = new DWidget;
    advancesettingwdg = new DWidget;
    scrollarea = new DScrollArea;
    scrollarea->setWidget(advancesettingwdg);
    scrollarea->setWidgetResizable(true);
    scrollarea->setFrameShape(QFrame::NoFrame);
    scrollarea->hide();

    advanceBtn = new DPushButton(q->tr("Advanced"));
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
    ptoplayout->addWidget(scrollarea);

    initbasicui();
    initadvanceui();
    //bottom
    QHBoxLayout *pbottomlayout = new QHBoxLayout;
    pbottomlayout->setContentsMargins(0, 10, 0, 10);
    cancelBtn = new DPushButton(q->tr("Cancel"));
    printBtn = new DSuggestButton(q->tr("Print"));

    cancelBtn->setFixedSize(170, 36);
    printBtn->setFixedSize(170, 36);
    pbottomlayout->addWidget(cancelBtn);
    pbottomlayout->addWidget(printBtn);

    layout->addWidget(ptopwidget);
    layout->addLayout(pbottomlayout);
}

void DPrintPreviewDialogPrivate::initbasicui()
{
    Q_Q(DPrintPreviewDialog);
    QVBoxLayout *layout = new QVBoxLayout(basicsettingwdg);
    layout->setSpacing(10);
    DLabel *basicLabel = new DLabel(q->tr("Basic"), basicsettingwdg);
    setwidgetfont(basicLabel);
    QHBoxLayout *basictitlelayout = new QHBoxLayout;
    layout->addLayout(basictitlelayout);
    basictitlelayout->setContentsMargins(0, 0, 0, 0);
    basictitlelayout->addWidget(basicLabel);
    basictitlelayout->setAlignment(basicLabel, Qt::AlignLeft | Qt::AlignBottom);

    //打印机选择
    DFrame *printerFrame = new DFrame(basicsettingwdg);
    layout->addWidget(printerFrame);
    printerFrame->setFixedSize(422, 48);
    setfrmaeback(printerFrame);
    QHBoxLayout *printerlayout = new QHBoxLayout(printerFrame);
    printerlayout->setContentsMargins(10, 0, 10, 0);
    DLabel *printerlabel = new DLabel(q->tr("Printer"), printerFrame);
    printDeviceCombo = new DComboBox(basicsettingwdg);
    printDeviceCombo->setFixedSize(275, 36);
    printerlayout->addWidget(printerlabel);
    printerlayout->addWidget(printDeviceCombo);
    printerlayout->setAlignment(printDeviceCombo, Qt::AlignVCenter);

    //打印份数
    DFrame *copycountFrame = new DFrame(basicsettingwdg);
    copycountFrame->setObjectName("copucountframe");
    layout->addWidget(copycountFrame);
    copycountFrame->setFixedSize(422, 48);
    setfrmaeback(copycountFrame);
    QHBoxLayout *copycountlayout = new QHBoxLayout(copycountFrame);
    copycountlayout->setContentsMargins(10, 0, 10, 0);
    DLabel *copycountlabel = new DLabel(q->tr("Copies"), copycountFrame);
    copycountspinbox = new DSpinBox(copycountFrame);
    copycountspinbox->setEnabledEmbedStyle(true);
    copycountspinbox->setRange(1, 999);
    copycountspinbox->setFixedSize(275, 36);
    copycountlayout->addWidget(copycountlabel);
    copycountlayout->addWidget(copycountspinbox);

    QRegExp re("^[1-9][0-9][0-9]$");
    QRegExpValidator *va = new QRegExpValidator(re);
    copycountspinbox->lineEdit()->setValidator(va);

    //页码范围
    DFrame *pageFrame = new DFrame(basicsettingwdg);
    pageFrame->setObjectName("pageFrame");
    layout->addWidget(pageFrame);
    pageFrame->setFixedSize(422, 94);
    setfrmaeback(pageFrame);
    QVBoxLayout *pagelayout = new QVBoxLayout(pageFrame);
    pagelayout->setContentsMargins(10, 5, 10, 5);
    DLabel *pagerangelabel = new DLabel(q->tr("Page range"), pageFrame);
    pageRangeCombo = new DComboBox(pageFrame);
    pageRangeCombo->setFixedSize(275, 36);
    pageRangeCombo->addItem(q->tr("All"));
    pageRangeCombo->addItem(q->tr("Current page"));
    pageRangeCombo->addItem(q->tr("Select pages"));
    QHBoxLayout *hrangebox = new QHBoxLayout();
    hrangebox->addWidget(pagerangelabel);
    hrangebox->addWidget(pageRangeCombo);

    pageRangeEdit = new DLineEdit;
    pagelayout->addLayout(hrangebox);
    pagelayout->addWidget(pageRangeEdit);

    QRegExp reg("^(([1-9][0-9]*)+(\\,|\\-)?)*");
    QRegExpValidator *val = new QRegExpValidator(reg);
    pageRangeEdit->lineEdit()->setValidator(val);

    //打印方向
    DLabel *orientationLabel = new DLabel(q->tr("Orientation"), basicsettingwdg);
    setwidgetfont(orientationLabel);
    QHBoxLayout *orientationtitlelayout = new QHBoxLayout;
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
    portraitwdg->setFixedSize(422, 48);
    QHBoxLayout *portraitlayout = new QHBoxLayout;
    DLabel *orientationTextLabel = new DLabel(q->tr("Portrait"), portraitwdg);
    portraitlayout->addWidget(verRadio);
    portraitlayout->addWidget(orientationTextLabel);
    portraitlayout->addStretch();
    portraitwdg->setLayout(portraitlayout);

    //横向
    DWidget *landscapewdg = new DWidget;
    landscapewdg->setFixedSize(422, 48);
    QHBoxLayout *landscapelayout = new QHBoxLayout;
    DLabel *landscapeTextLabel = new DLabel(q->tr("Landscape"), portraitwdg);
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
    advancesettingwdg->setFixedWidth(442);

    //页面设置
    QVBoxLayout *pagelayout = new QVBoxLayout;
    pagelayout->setSpacing(10);
    pagelayout->setContentsMargins(10, 0, 10, 0);
    DLabel *pagesLabel = new DLabel(q->tr("Pages"), advancesettingwdg);
    setwidgetfont(pagesLabel, DFontSizeManager::T5);
    QHBoxLayout *pagestitlelayout = new QHBoxLayout;
    pagestitlelayout->setContentsMargins(0, 20, 0, 0);
    pagestitlelayout->addWidget(pagesLabel, Qt::AlignLeft | Qt::AlignBottom);

    DFrame *colorframe = new DFrame;
    setfrmaeback(colorframe);
    colorframe->setFixedHeight(48);
    QHBoxLayout *colorlayout = new QHBoxLayout(colorframe);
    DLabel *colorlabel = new DLabel(q->tr("Color mode"));
    colorlabel->setFixedWidth(123);
    colorModeCombo = new DComboBox;
    colorModeCombo->addItems(QStringList() << q->tr("Color") << q->tr("Grayscale"));
    colorlayout->addWidget(colorlabel);
    colorlayout->addWidget(colorModeCombo);
    colorlayout->setContentsMargins(10, 4, 10, 4);

    DFrame *marginsframe = new DFrame;
    marginsframe->setObjectName("marginsFrame");
    setfrmaeback(marginsframe);
    QHBoxLayout *marginslayout = new QHBoxLayout(marginsframe);
    marginslayout->setContentsMargins(10, 5, 10, 5);
    QVBoxLayout *marginscombolayout = new QVBoxLayout;
    DLabel *marginlabel = new DLabel(q->tr("Margins"));
    marginlabel->setFixedSize(123, 36);
    marginsCombo = new DComboBox;
    marginsCombo->addItems(QStringList() << q->tr("Ordinary") << q->tr("Narrow") << q->tr("Moderate") << q->tr("Customize"));
    //    marginsCombo->setFixedSize(275, 36);
    marginscombolayout->addWidget(marginlabel);
    marginscombolayout->addStretch();
    marginslayout->addLayout(marginscombolayout);

    QVBoxLayout *marginlayout = new QVBoxLayout;
    QHBoxLayout *marginsspinlayout = new QHBoxLayout;
    marginsspinlayout->setContentsMargins(0, 0, 0, 0);
    DLabel *toplabel = new DLabel(q->tr("Top"));
    marginTopSpin = new DDoubleSpinBox;
    DLabel *leftlabel = new DLabel(q->tr("Left"));
    marginLeftSpin = new DDoubleSpinBox;
    DLabel *bottomlabel = new DLabel(q->tr("Bottom"));
    marginBottomSpin = new DDoubleSpinBox;
    DLabel *rightlabel = new DLabel(q->tr("Right"));
    marginRightSpin = new DDoubleSpinBox;
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

    marginsspinlayout->addLayout(marginslabellayout1);
    marginsspinlayout->addLayout(marginsspinboxlayout1);
    marginsspinlayout->addLayout(marginslabellayout2);
    marginsspinlayout->addLayout(marginsspinboxlayout2);
    marginlayout->addWidget(marginsCombo);
    marginlayout->addLayout(marginsspinlayout);
    marginslayout->addLayout(marginlayout);

    QList<DDoubleSpinBox *> list = marginsframe->findChildren<DDoubleSpinBox *>();
    for (int i = 0; i < list.size(); i++) {
        list.at(i)->setProperty("_d_dtk_spinBox", true);
        DLineEdit *edit = list.at(i)->findChild<DLineEdit *>();
        edit->setClearButtonEnabled(false);
        list.at(i)->setRange(0, 58.88);
    }

    pagelayout->addLayout(pagestitlelayout);
    pagelayout->addWidget(colorframe);
    pagelayout->addWidget(marginsframe);

    //缩放
    QVBoxLayout *scalinglayout = new QVBoxLayout;
    scalinglayout->setContentsMargins(10, 0, 10, 0);
    DLabel *scalingLabel = new DLabel(q->tr("Scaling"), advancesettingwdg);
    QHBoxLayout *scalingtitlelayout = new QHBoxLayout;
    scalingtitlelayout->setContentsMargins(0, 20, 0, 0);
    scalingtitlelayout->addWidget(scalingLabel, Qt::AlignLeft | Qt::AlignBottom);
    setwidgetfont(scalingLabel, DFontSizeManager::T5);

    scaleGroup = new QButtonGroup(q);
    QVBoxLayout *scalingcontentlayout = new QVBoxLayout;
    scalingcontentlayout->setContentsMargins(0, 0, 0, 0);
    //    DWidget *fitwdg = new DWidget;
    //    fitwdg->setFixedHeight(48);
    //    QHBoxLayout *fitlayout = new QHBoxLayout(fitwdg);
    //    DRadioButton *fitPaperRadio = new DRadioButton(q->tr("Fit to paper size"));
    //    scaleGroup->addButton(fitPaperRadio, 0);
    //    fitlayout->addWidget(fitPaperRadio);
    DWidget *actualwdg = new DWidget;
    actualwdg->setFixedHeight(48);
    QHBoxLayout *actuallayout = new QHBoxLayout(actualwdg);
    DRadioButton *actualSizeRadio = new DRadioButton(q->tr("Actual size"));

    scaleGroup->addButton(actualSizeRadio, ACTUAL_SIZE);
    actuallayout->addWidget(actualSizeRadio);
    //    DWidget *shrinkwdg = new DWidget;
    //    shrinkwdg->setFixedHeight(48);
    //    QHBoxLayout *shrinklayout = new QHBoxLayout(shrinkwdg);
    //    DRadioButton *shrinkPageRadio = new DRadioButton(q->tr("Shrink oversized pages"));
    //    scaleGroup->addButton(shrinkPageRadio, 2);
    //    shrinklayout->addWidget(shrinkPageRadio);
    DWidget *customscalewdg = new DWidget;
    customscalewdg->setFixedHeight(48);
    QHBoxLayout *customlayout = new QHBoxLayout(customscalewdg);
    customlayout->setContentsMargins(10, 0, 10, 0);
    DRadioButton *customSizeRadio = new DRadioButton(q->tr("Scale"));
    scaleGroup->addButton(customSizeRadio, SCALE);
    scaleRateEdit = new DSpinBox;
    scaleRateEdit->setSuffix("%");
    scaleRateEdit->setRange(10, 200);
    qDebug() << scaleRateEdit->value();
    //    scaleRateEdit->lineEdit()->setText("");
    //    scaleRateEdit->lineEdit()->setPlaceholderText("90%");
    scaleRateEdit->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
    scaleRateEdit->setFixedWidth(78);
    customlayout->addWidget(customSizeRadio);
    customlayout->addWidget(scaleRateEdit);
    customlayout->addStretch(1);
    //    scalingcontentlayout->addWidget(fitwdg);
    scalingcontentlayout->addWidget(actualwdg);
    //    scalingcontentlayout->addWidget(shrinkwdg);
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
    DLabel *paperLabel = new DLabel(q->tr("Paper"), advancesettingwdg);
    setwidgetfont(paperLabel, DFontSizeManager::T5);
    QHBoxLayout *papertitlelayout = new QHBoxLayout;
    papertitlelayout->setContentsMargins(0, 0, 0, 0);
    papertitlelayout->addWidget(paperLabel, Qt::AlignLeft | Qt::AlignBottom);

    DFrame *paperframe = new DFrame;
    setfrmaeback(paperframe);
    paperframe->setFixedHeight(48);
    QHBoxLayout *paperframelayout = new QHBoxLayout(paperframe);
    DLabel *papersizelabel = new DLabel(q->tr("Paper size"));
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
    DLabel *drawingLabel = new DLabel(q->tr("Layout"), advancesettingwdg);
    setwidgetfont(drawingLabel, DFontSizeManager::T5);
    QHBoxLayout *drawingtitlelayout = new QHBoxLayout;
    drawingtitlelayout->setContentsMargins(0, 20, 0, 0);
    drawingtitlelayout->addWidget(drawingLabel, Qt::AlignLeft | Qt::AlignBottom);

    DFrame *duplexframe = new DFrame;
    setfrmaeback(duplexframe);
    duplexframe->setFixedHeight(48);
    QHBoxLayout *duplexlayout = new QHBoxLayout(duplexframe);
    DLabel *duplexlabel = new DLabel(q->tr("Duplex"));
    duplexSwitchBtn = new DSwitchButton;
    duplexlayout->addWidget(duplexlabel);
    duplexlayout->addStretch(1);
    duplexlayout->addWidget(duplexSwitchBtn, Qt::AlignRight);
    /*
        DFrame  *drawingframe = new DFrame;
        setfrmaeback(drawingframe);
        drawingframe->setFixedHeight(94);
        QVBoxLayout *drawingframelayout = new QVBoxLayout(drawingframe);
        drawingframelayout->setSpacing(10);
        QHBoxLayout *pagepersheetlayout = new QHBoxLayout;
        pagepersheetlayout->setContentsMargins(0, 0, 0, 0);
        DLabel *sheetlabel = new DLabel(q->tr("Pages per sheet"));
        sheetlabel->setFixedWidth(123);
        pagePerSheetCombo = new DComboBox;
        pagePerSheetCombo->setFixedHeight(36);
        pagepersheetlayout->addWidget(sheetlabel);
        pagepersheetlayout->addWidget(pagePerSheetCombo);
        drawingframelayout->addLayout(pagepersheetlayout);

        QHBoxLayout *printdirectlayout = new QHBoxLayout;
        printdirectlayout->setContentsMargins(0, 0, 0, 0);
        printdirectlayout->setSpacing(10);
        DLabel *directlabel = new DLabel(q->tr("Layout direction"));
        directlabel->setFixedWidth(123);
        lrtbBtn = new DIconButton(DStyle::SP_IncreaseElement);
        rltbBtn = new DIconButton(DStyle::SP_IncreaseElement);
        tblrBtn = new DIconButton(DStyle::SP_IncreaseElement);
        tbrlBtn = new DIconButton(DStyle::SP_IncreaseElement);
        printdirectlayout->addWidget(directlabel);
        printdirectlayout->addWidget(lrtbBtn);
        printdirectlayout->addWidget(rltbBtn);
        printdirectlayout->addWidget(tblrBtn);
        printdirectlayout->addWidget(tbrlBtn);
        printdirectlayout->addStretch(1);
        drawingframelayout->addLayout(printdirectlayout);
    */
    drawinglayout->addLayout(drawingtitlelayout);
    drawinglayout->addWidget(duplexframe);
    //        drawinglayout->addWidget(drawingframe);

    //打印顺序
    /*
    QVBoxLayout *orderlayout = new QVBoxLayout;
    orderlayout->setContentsMargins(10, 0, 10, 0);
    DLabel *orderLabel = new DLabel(q->tr("Page Order"), advancesettingwdg);
    setwidgetfont(orderLabel, DFontSizeManager::T5);
    QHBoxLayout *ordertitlelayout = new QHBoxLayout;
    ordertitlelayout->setContentsMargins(0, 20, 0, 0);
    ordertitlelayout->addWidget(orderLabel, Qt::AlignLeft | Qt::AlignBottom);

    QVBoxLayout *ordercontentlayout = new QVBoxLayout;
    ordercontentlayout->setContentsMargins(0, 0, 0, 0);
    DWidget *collatewdg = new DWidget;
    collatewdg->setFixedHeight(36);
    QHBoxLayout *collatelayout = new QHBoxLayout(collatewdg);
    printOrderGroup = new QButtonGroup(q);
    DRadioButton *printCollateRadio = new DRadioButton(q->tr("Collate pages")); //逐份打印
    printOrderGroup->addButton(printCollateRadio, 0);
    collatelayout->addWidget(printCollateRadio);

    DWidget *inorderwdg = new DWidget;
    inorderwdg->setFixedHeight(48);
    QHBoxLayout *inorderlayout = new QHBoxLayout(inorderwdg);
    DRadioButton *printInOrderRadio = new DRadioButton(q->tr("Print pages in order")); //按顺序打印
    printInOrderRadio->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    printOrderGroup->addButton(printCollateRadio, 1);
    inorderCombo = new DComboBox;
    inorderlayout->addWidget(printInOrderRadio);
    inorderlayout->addWidget(inorderCombo);

    ordercontentlayout->addWidget(collatewdg);
    ordercontentlayout->addWidget(inorderwdg);
    DBackgroundGroup *backorder = new DBackgroundGroup(ordercontentlayout);
    backorder->setItemSpacing(1);
    pa = DApplicationHelper::instance()->palette(backorder);
    pa.setBrush(DPalette::Base, pa.itemBackground());
    DApplicationHelper::instance()->setPalette(backorder, pa);

    orderlayout->addLayout(ordertitlelayout);
    orderlayout->addWidget(backorder);
    */

    //水印
    /*QVBoxLayout *watermarklayout = new QVBoxLayout;
    watermarklayout->setContentsMargins(10, 0, 10, 0);
    DLabel *watermarkLabel = new DLabel(q->tr("Watermark"), advancesettingwdg);
    QHBoxLayout *watermarktitlelayout = new QHBoxLayout;
    watermarktitlelayout->setContentsMargins(0, 20, 0, 0);
    watermarktitlelayout->addWidget(watermarkLabel, Qt::AlignLeft | Qt::AlignBottom);
    setwidgetfont(watermarkLabel, DFontSizeManager::T5);

    DFrame *watermarkframe = new DFrame;
    watermarkframe->setFixedHeight(94);
    setfrmaeback(watermarkframe);
    QVBoxLayout *watercontentlayout = new QVBoxLayout(watermarkframe);
    watercontentlayout->setSpacing(10);
    watercontentlayout->setContentsMargins(14, 10, 10, 10);
    QHBoxLayout *texttypelayout = new QHBoxLayout;
    texttypelayout->setContentsMargins(0, 0, 0, 0);
    DLabel *addlabel = new DLabel(q->tr("Add watermark"));
    addlabel->setFixedWidth(113);
    waterTypeCombo = new DComboBox;
    waterTypeCombo->setFixedHeight(36);
    waterColorBtn = new DIconButton(DStyle::SP_IncreaseElement);
    texttypelayout->addWidget(addlabel);
    texttypelayout->addWidget(waterTypeCombo);
    texttypelayout->addWidget(waterColorBtn);
    QHBoxLayout *watereditlayout = new QHBoxLayout;
    watereditlayout->setContentsMargins(0, 0, 0, 0);
    waterTextEdit = new DLineEdit;
    waterTextEdit->setFixedWidth(275);
    waterTextEdit->setFixedHeight(36);
    watereditlayout->addSpacerItem(new QSpacerItem(127, 2, QSizePolicy::Maximum));
    watereditlayout->addWidget(waterTextEdit);
    watercontentlayout->addLayout(texttypelayout);
    watercontentlayout->addLayout(watereditlayout);

    watermarklayout->addLayout(watermarktitlelayout);
    watermarklayout->addWidget(watermarkframe);
    */

    layout->addLayout(paperlayout);
    layout->addLayout(drawinglayout);
    layout->addLayout(pagelayout);
    layout->addLayout(scalinglayout);

    //    layout->addLayout(orderlayout);
    //    layout->addLayout(watermarklayout);
}

void DPrintPreviewDialogPrivate::initdata()
{
    Q_Q(DPrintPreviewDialog);
    QStringList itemlist;
    itemlist << QPrinterInfo::availablePrinterNames() << q->tr("Print to PDF");
    printDeviceCombo->addItems(itemlist);
    QString defauledevice = QPrinterInfo::defaultPrinterName();
    for (int i = 0; i < itemlist.size(); ++i) {
        if (defauledevice.count(itemlist.at(i)) > 0) {
            printDeviceCombo->setCurrentIndex(i);
            break;
        }
    }
    updateSetteings(0, true);
}

void DPrintPreviewDialogPrivate::initconnections()
{
    Q_Q(DPrintPreviewDialog);

    QObject::connect(pview, &DPrintPreviewWidget::paintRequested, q, &DPrintPreviewDialog::paintRequested);

    QObject::connect(advanceBtn, &QPushButton::clicked, q, [this] { this->showadvancesetting(); });
    QObject::connect(printDeviceCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_printerChanged(int)));
    QObject::connect(cancelBtn, &DPushButton::clicked, q, &DPrintPreviewDialog::close);
    //    QObject::connect(advanceBtn, &QPushButton::clicked, q, &DPrintPreviewDialog::showAdvanceSetting);
    QObject::connect(pageRangeEdit, SIGNAL(editingFinished()), q, SLOT(_q_customPagesFinished()));
    QObject::connect(pageRangeCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_pageRangeChanged(int)));
    QObject::connect(marginsCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_pageMarginChanged(int)));
    QObject::connect(printBtn, SIGNAL(clicked(bool)), q, SLOT(_q_startPrint(bool)));
    QObject::connect(colorModeCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_ColorModeChange(int)));
    QObject::connect(orientationgroup, SIGNAL(buttonClicked(int)), q, SLOT(_q_orientationChanged(int)));
    QObject::connect(jumpPageEdit, SIGNAL(valueChanged(int)), q, SLOT(_q_currentPageSpinChanged(int)));

    QObject::connect(pview, &DPrintPreviewWidget::totalPages, [this](int pages) {
        totalPageLabel->setText(QString::number(pages));
        totalPages = pages;
    });
    QObject::connect(pview, &DPrintPreviewWidget::pagesCountChanged, totalPageLabel, static_cast<void (QLabel::*)(int)>(&QLabel::setNum));
    QObject::connect(firstBtn, &DIconButton::clicked, pview, &DPrintPreviewWidget::turnBegin);
    QObject::connect(prevPageBtn, &DIconButton::clicked, pview, &DPrintPreviewWidget::turnFront);
    QObject::connect(nextPageBtn, &DIconButton::clicked, pview, &DPrintPreviewWidget::turnBack);
    QObject::connect(lastBtn, &DIconButton::clicked, pview, &DPrintPreviewWidget::turnEnd);
    QObject::connect(pview, &DPrintPreviewWidget::currentPageChanged, jumpPageEdit, &QSpinBox::setValue);
    QObject::connect(jumpPageEdit, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), pview, &DPrintPreviewWidget::setCurrentPage);

    QObject::connect(paperSizeCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), q, [this](int index) {
        QPrinterInfo prInfo(*printer);
        if (paperSizeCombo->count() == 0) {
            printer->setPageSize(QPrinter::A4);
            return ;
        }
        if (printDeviceCombo->currentIndex() != printDeviceCombo->count() - 1) {
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
        pview->updatePreview();
    });

    QObject::connect(scaleRateEdit, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), q, [this](int value) {
        if (scaleGroup->checkedId() == SCALE) {
            qreal scale = scaleRateEdit->value() / 100.0;
            pview->setScale(scale);
            pview->updateView();
        }
    });
    QObject::connect(scaleGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), q, [this](int id) {
        if (id == ACTUAL_SIZE) {
            pview->setScale(1);
        } else if (id == SCALE) {
            qreal scale = scaleRateEdit->value() / 100.0;
            pview->setScale(scale);
        }
        pview->updateView();
    });

    QObject::connect(marginTimer, SIGNAL(timeout()), q, SLOT(_q_marginTimerOut()));
    QObject::connect(marginTopSpin, SIGNAL(valueChanged(double)), q, SLOT(_q_marginspinChanged(double)));
    QObject::connect(marginRightSpin, SIGNAL(valueChanged(double)), q, SLOT(_q_marginspinChanged(double)));
    QObject::connect(marginLeftSpin, SIGNAL(valueChanged(double)), q, SLOT(_q_marginspinChanged(double)));
    QObject::connect(marginBottomSpin, SIGNAL(valueChanged(double)), q, SLOT(_q_marginspinChanged(double)));
}

void DPrintPreviewDialogPrivate::setfrmaeback(DWidget *frame)
{
    DPalette pa = DApplicationHelper::instance()->palette(frame);
    pa.setBrush(DPalette::Base, pa.itemBackground());
    pa.setBrush(DPalette::FrameBorder, pa.base());
    DApplicationHelper::instance()->setPalette(frame, pa);
    //frame->setAutoFillBackground(true);
}

void DPrintPreviewDialogPrivate::showadvancesetting()
{
    if (scrollarea->isHidden()) {
        basicsettingwdg->hide();
        scrollarea->show();
        advanceBtn->setIcon(QIcon::fromTheme("printer_dropup"));
    } else {
        basicsettingwdg->show();
        scrollarea->hide();
        advanceBtn->setIcon(QIcon::fromTheme("printer_dropdown"));
    }
}

/*!
 * \~chinese DPrintPreviewDialogPrivate::setupPrinter 设置打印的相关基础属性和高级属性
 */
void DPrintPreviewDialogPrivate::setupPrinter()
{
    Q_Q(DPrintPreviewDialog);
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
    if (printDeviceCombo->currentIndex() != printDeviceCombo->count() - 1) {
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
    if (duplexSwitchBtn->isChecked())
        printer->setDuplex(QPrinter::DuplexAuto);
    else {
        printer->setDuplex(QPrinter::DuplexNone);
    }
    //设置色彩打印
    if (colorModeCombo->currentIndex() == 0)
        printer->setColorMode(QPrinter::Color);
    else {
        printer->setColorMode(QPrinter::GrayScale);
    }
    //设置纸张打印边距
    printer->setPageMargins(QMarginsF(marginLeftSpin->value(), marginTopSpin->value(), marginRightSpin->value(), marginBottomSpin->value()));
}

void DPrintPreviewDialogPrivate::test()
{
    //"Canon-iR2520", "HP_LaserJet_Professional_M1213nf_MFP_052BD1_"
    QString m_cupsName = "Canon-iR2520";
    cups_dest_t *m_cupsDest = cupsGetNamedDest(CUPS_HTTP_DEFAULT, m_cupsName.toStdString().c_str(), nullptr);
    const char *ppdFile = cupsGetPPD("Canon-iR2520");
    ppd_file_t *m_ppd = nullptr;
    if (ppdFile) {
        m_ppd = ppdOpenFile(ppdFile);
        unlink(ppdFile);
    }

    ppd_option_t *duplexModes = ppdFindOption(m_ppd, "Duplex");
    bool find = false;
    if (duplexModes) {
        for (int i = 0; i < duplexModes->num_choices; ++i) {
            qDebug() << QPrintUtils::ppdChoiceToDuplexMode(duplexModes->choices[i].choice);
            find = true;
        }
        if (find) {
            duplexModes = ppdFindOption(m_ppd, "DefaultDuplex");
            if (duplexModes)
                qDebug() << QPrintUtils::ppdChoiceToDuplexMode(duplexModes->choices[0].choice);
        }
    }
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::updateSetteings 具体对每个控件和属性进行更新
 * \~chinese \param index 判断当前的打印设备
 */
void DPrintPreviewDialogPrivate::updateSetteings(int index, bool isInit)
{
    Q_Q(DPrintPreviewDialog);
    QPrinterInfo updateinfo(*printer);
    if (isInit) {
        copycountspinbox->setValue(1);
        paperSizeCombo->setCurrentIndex(0);
        _q_pageRangeChanged(0);
        _q_pageMarginChanged(0);
        _q_orientationChanged(0);
        scaleGroup->button(1)->setChecked(true);
        orientationgroup->button(0)->setChecked(true);
        scaleRateEdit->setValue(100);
        marginsCombo->setCurrentIndex(0);
        pageRangeCombo->setCurrentIndex(0);
    }

    if (index != printDeviceCombo->count() - 1) {
        QStringList pageSizeList;
        for (int i = 0; i < updateinfo.supportedPageSizes().size(); i++) {
            pageSizeList.append(updateinfo.supportedPageSizes().at(i).key());
        }
        paperSizeCombo->addItems(pageSizeList);
        paperSizeCombo->setCurrentText("A4");
        //判断当前打印机是否支持双面打印，不支持禁用双面打印按钮，pdf不做判断
        if (updateinfo.supportedDuplexModes().contains(QPrinter::DuplexLongSide) || updateinfo.supportedDuplexModes().contains(QPrinter::DuplexShortSide)) {
            duplexSwitchBtn->setEnabled(true);
        } else {
            duplexSwitchBtn->setChecked(false);
            duplexSwitchBtn->setEnabled(false);
        }
        //判断当前打印机是否支持彩色打印，不支持彩色打印删除彩色打印选择选项，pdf不做判断
        QPlatformPrinterSupport *ps = QPlatformPrinterSupportPlugin::get();
        QPrintDevice currentDevice = ps->createPrintDevice(printDeviceCombo->currentText());
        if (!currentDevice.supportedColorModes().contains(QPrint::Color)) {
            if (colorModeCombo->count() != 1)
                colorModeCombo->removeItem(0);
        } else {
            if (colorModeCombo->count() == 1)
                colorModeCombo->insertItem(0, q->tr("Color"));
        }

    } else {
        if (colorModeCombo->count() == 1)
            colorModeCombo->insertItem(0, q->tr("Color"));
        copycountspinbox->setDisabled(true);
        copycountspinbox->setValue(1);
        paperSizeCombo->clear();
        paperSizeCombo->addItems(QStringList() << "A3"
                                               << "A4"
                                               << "A5"
                                               << "B4"
                                               << "B5"
                                               << "8K"
                                               << "16K");
        paperSizeCombo->setCurrentIndex(1);
        duplexSwitchBtn->setEnabled(false);
    }
    colorModeCombo->setCurrentIndex(0);
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
    DFrame *marginframe = advancesettingwdg->findChild<DFrame *>("marginsFrame");
    QList<DLabel *> marginlist = marginframe->findChildren<DLabel *>();
    if (combox == marginsCombo) {
        if (value != marginsCombo->count() - 1) {
            marginTopSpin->setDisabled(true);
            marginLeftSpin->setDisabled(true);
            marginRightSpin->setDisabled(true);
            marginBottomSpin->setDisabled(true);
            qDebug() << marginsCombo->count();
            for (int i = 1; i < marginlist.size(); i++) {
                marginlist.at(i)->setEnabled(false);
            }
        } else {
            marginTopSpin->setDisabled(false);
            marginLeftSpin->setDisabled(false);
            marginRightSpin->setDisabled(false);
            marginBottomSpin->setDisabled(false);
            for (int i = 1; i < marginlist.size(); i++) {
                marginlist.at(i)->setEnabled(true);
            }
        }
    }
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_printerChanged 根据选取不同的打印设备刷新界面和属性
 * \~chinese \param index 判断选取的打印设备
 */
void DPrintPreviewDialogPrivate::_q_printerChanged(int index)
{
    qDebug() << printDeviceCombo->itemText(index);
    if (index == printDeviceCombo->count() - 1) {
        //pdf
        copycountspinbox->setDisabled(true);
        copycountspinbox->setValue(1);
        duplexSwitchBtn->setChecked(false);
        duplexSwitchBtn->setEnabled(false);
        colorModeCombo->setEnabled(false);
        paperSizeCombo->clear();
        printer->setPrinterName("");
    } else {
        //actual printer
        if (printer) {
            paperSizeCombo->clear();
            copycountspinbox->setDisabled(false);
            paperSizeCombo->setEnabled(true);
            duplexSwitchBtn->setEnabled(true);
            colorModeCombo->setEnabled(true);
            printer->setPrinterName(printDeviceCombo->itemText(index));
        }
    }
    updateSetteings(index, false);
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::slotPageRangeCombox 根据选取的不同纸张范围类型设置纸张范围
 * \~chinese \param index 判断不同的范围类型
 */
void DPrintPreviewDialogPrivate::_q_pageRangeChanged(int index)
{
    setEnable(index, pageRangeCombo);
    pageRangeEdit->lineEdit()->setPlaceholderText("");
    pageRangeEdit->setText("");
    QVector<int> pagesrange;
    if (index == PAGERANGE_ALL) {
        qDebug() << totalPages;
        if (totalPages != 0) {
            totalPageLabel->setNum(totalPages);
            printer->setPrintRange(DPrinter::AllPages);
            printer->setFromTo(FIRST_PAGE, totalPages);
            pview->setPageRange(FIRST_PAGE, totalPages);
        }
    } else if (index == PAGERANGE_CURRENT) {
        pagesrange.clear();
        printer->setPrintRange(DPrinter::CurrentPage);
        pview->setPageRange(FIRST_PAGE, totalPages);
    } else {
        printer->setPrintRange(DPrinter::Selection);
        jumpPageEdit->setValue(1);
        pageRangeEdit->lineEdit()->setPlaceholderText("1,3,5-7,11-15,18,21");
    }
    pview->setPageRange(pagesrange);
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_pageMarginChanged 根据选择不同的设置边距类型设置边距
 * \~chinese \param index 判断选取的不同的设置边距的类型
 */
void DPrintPreviewDialogPrivate::_q_pageMarginChanged(int index)
{
    setEnable(index, marginsCombo);
    if (index == 1) {
        if (marginTimer->isActive())
            marginTimer->stop();

        marginLeftSpin->blockSignals(true);
        marginTopSpin->blockSignals(true);
        marginRightSpin->blockSignals(true);
        marginBottomSpin->blockSignals(true);

        marginLeftSpin->setValue(NARROW_ALL);
        marginTopSpin->setValue(NARROW_ALL);
        marginRightSpin->setValue(NARROW_ALL);
        marginBottomSpin->setValue(NARROW_ALL);
        printer->setPageMargins(QMarginsF(NARROW_ALL, NARROW_ALL, NARROW_ALL, NARROW_ALL));

        pview->updatePreview();
    } else if (index == 2) {
        if (marginTimer->isActive())
            marginTimer->stop();

        marginLeftSpin->blockSignals(true);
        marginTopSpin->blockSignals(true);
        marginRightSpin->blockSignals(true);
        marginBottomSpin->blockSignals(true);

        marginLeftSpin->setValue(MODERATE_LEFT_RIGHT);
        marginTopSpin->setValue(NORMAL_MODERATE_TOP_BOTTRM);
        marginRightSpin->setValue(MODERATE_LEFT_RIGHT);
        marginBottomSpin->setValue(NORMAL_MODERATE_TOP_BOTTRM);
        printer->setPageMargins(QMarginsF(MODERATE_LEFT_RIGHT, NORMAL_MODERATE_TOP_BOTTRM, MODERATE_LEFT_RIGHT, NORMAL_MODERATE_TOP_BOTTRM));

        pview->updatePreview();
    } else if (index == 3) {
        marginLeftSpin->blockSignals(true);
        marginTopSpin->blockSignals(true);
        marginRightSpin->blockSignals(true);
        marginBottomSpin->blockSignals(true);

        marginTopSpin->setValue(NORMAL_MODERATE_TOP_BOTTRM);
        marginLeftSpin->setValue(NORMAL_LEFT_RIGHT);
        marginRightSpin->setValue(NORMAL_LEFT_RIGHT);
        marginBottomSpin->setValue(NORMAL_MODERATE_TOP_BOTTRM);
        printer->setPageMargins(QMarginsF(marginLeftSpin->value(), marginTopSpin->value(), marginRightSpin->value(), marginBottomSpin->value()));

        pview->updatePreview();

        marginLeftSpin->blockSignals(false);
        marginTopSpin->blockSignals(false);
        marginRightSpin->blockSignals(false);
        marginBottomSpin->blockSignals(false);
    } else {
        if (marginTimer->isActive())
            marginTimer->stop();

        marginLeftSpin->blockSignals(true);
        marginTopSpin->blockSignals(true);
        marginRightSpin->blockSignals(true);
        marginBottomSpin->blockSignals(true);

        marginTopSpin->setValue(NORMAL_MODERATE_TOP_BOTTRM);
        marginLeftSpin->setValue(NORMAL_LEFT_RIGHT);
        marginRightSpin->setValue(NORMAL_LEFT_RIGHT);
        marginBottomSpin->setValue(NORMAL_MODERATE_TOP_BOTTRM);

        printer->setPageMargins(QMarginsF(NORMAL_LEFT_RIGHT, NORMAL_MODERATE_TOP_BOTTRM, NORMAL_LEFT_RIGHT, NORMAL_MODERATE_TOP_BOTTRM));
        pview->updatePreview();
    }
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
        return;
    }
    if (index == 0) {
        // color
        pview->setColorMode(DPrinter::Color);
    } else {
        // gray
        pview->setColorMode(DPrinter::GrayScale);
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
        pview->setOrientation(DPrinter::Portrait);
    } else {
        // 横向按钮
        pview->setOrientation(DPrinter::Landscape);
    }
}

void DPrintPreviewDialogPrivate::_q_customPagesFinished()
{
    QString cuspages = pageRangeEdit->text();
    QVector<int> pagesrange;
    QStringList list = cuspages.split(",");
    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).contains("-")) {
            QStringList list1 = list.at(i).split("-");
            if (list1.at(0).toInt() <= list1.at(1).toInt()) {
                for (int page = list1.at(0).toInt(); page <= list1.at(1).toInt(); page++) {
                    if (page != 0 && page <= totalPages)
                        pagesrange.append(page);
                    else
                        return;
                }
            }
        } else {
            if (list.at(i).toInt() != 0 && list.at(i).toInt() <= totalPages)
                pagesrange.append(list.at(i).toInt());
            else
                return;
        }
    }
    jumpPageEdit->setValue(1);
    QVector<int> page = checkDuplication(pagesrange);
    qDebug() << page << __func__;
    pview->setPageRange(page);
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_marginTimerOut 自定义边距计时器时间结束的槽函数
 * \~chinese \param
 */
void DPrintPreviewDialogPrivate::_q_marginTimerOut()
{
    qreal leftMarginF = this->marginLeftSpin->value();
    qreal topMarginF = this->marginTopSpin->value();
    qreal rightMarginF = this->marginRightSpin->value();
    qreal bottomMarginF = this->marginBottomSpin->value();

    this->printer->setPageMargins(QMarginsF(leftMarginF, topMarginF, rightMarginF, bottomMarginF));
    this->pview->updatePreview();
}

/*!
 * \~chinese \brief DPrintPreviewDialogPrivate::_q_marginspinChanged 自定义页边距spinbox值改变
 * \~chinese \param clicked 判断按钮点击状态
 */
void DPrintPreviewDialogPrivate::_q_marginspinChanged(double)
{
    if (marginTimer->isActive())
        marginTimer->stop();

    // 默认1秒的定时器，时间到了就刷新预览页面
    marginTimer->start(1000);
}

void DPrintPreviewDialogPrivate::_q_currentPageSpinChanged(int value)
{
    if (value == 1) {
        firstBtn->setEnabled(false);
        prevPageBtn->setEnabled(false);
        lastBtn->setEnabled(true);
        nextPageBtn->setEnabled(true);
    } else if (value == totalPageLabel->text().toInt()) {
        lastBtn->setEnabled(false);
        nextPageBtn->setEnabled(false);
        firstBtn->setEnabled(true);
        prevPageBtn->setEnabled(true);
    } else {
        firstBtn->setEnabled(true);
        prevPageBtn->setEnabled(true);
        lastBtn->setEnabled(true);
        nextPageBtn->setEnabled(true);
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
    if (printDeviceCombo->currentIndex() == printDeviceCombo->count() - 1) {
        //设置pdf保存文本信息，可以外部通过setDocName设置，如果不做任何操作默认保存名称print.pdf
        QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        desktopPath += QStringLiteral("/");
        if (printer == nullptr) {
            return;
        }
        if (printer->outputFileName().isEmpty()) {
            if (printer->docName().isEmpty())
                desktopPath += QStringLiteral("print.pdf");
            QFileInfo file(desktopPath);
            QString path = desktopPath;
            if (file.isFile()) {
                int i = 1;
                do {
                    QString stres("(%1).pdf");
                    path = desktopPath.remove(".pdf") + stres.arg(i);
                    file.setFile(path);
                    i++;
                } while (file.isFile());
                desktopPath = path;
            } else {
                desktopPath += printer->docName();
            }
        } else {
            desktopPath = printer->outputFileName();
        }
        QString str = DFileDialog::getSaveFileName(q, q->tr("Save as PDF"), desktopPath, q->tr("PDF file"));
        if (str.isEmpty())
            return;
        printer->setOutputFileName(str);
    }
    //    Q_EMIT q->paintRequested(printer);
    pview->print();

    q->done(0);
}

DPrintPreviewDialog::DPrintPreviewDialog(QWidget *parent)
    : DDialog(*new DPrintPreviewDialogPrivate(this), parent)
{
    Q_D(DPrintPreviewDialog);
    setFixedSize(851, 606);
    d->startup();
}

DPrintPreviewDialog::~DPrintPreviewDialog()
{
    Q_D(DPrintPreviewDialog);
    if (nullptr == d->printer)
        delete d->printer;
}

DWIDGET_END_NAMESPACE
#include "moc_dprintpreviewdialog.cpp"
