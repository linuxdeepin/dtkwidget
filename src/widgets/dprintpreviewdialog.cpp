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

#include <cups/ppd.h>
#include <cups/cups.h>
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

void DPrintPreviewDialogPrivate::startup(DPrinter *printer)
{
    if (nullptr == printer) {
        this->printer = new DPrinter;
    } else {
        ownPrinter = true;
        this->printer = printer;
    }
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
    jumpPageEdit = new DSpinBox;
    jumpPageEdit->setMaximumWidth(50);
    jumpPageEdit->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
    DLabel *spaceLabel = new DLabel("/");
    totalPageLabel = new DLabel("123");
    jumpPageEdit->setRange(1, totalPageLabel->text().toInt());
    nextPageBtn = new DIconButton(DStyle::SP_ArrowRight);
    lastBtn = new DIconButton(DStyle::SP_ArrowNext);
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

    Q_Q(DPrintPreviewDialog);
    QObject::connect(pview, &DPrintPreviewWidget::paintRequested, q, &DPrintPreviewDialog::paintRequested);
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

    //页码范围
    DFrame *pageFrame = new DFrame(basicsettingwdg);
    pageFrame->setObjectName("pageFrame");
    layout->addWidget(pageFrame);
    pageFrame->setFixedSize(422, 94);
    setfrmaeback(pageFrame);
    QVBoxLayout *pagelayout = new QVBoxLayout(pageFrame);
    DLabel *pagerangelabel = new DLabel(q->tr("Page range"), pageFrame);
    pageRangeCombo = new DComboBox(pageFrame);
    pageRangeCombo->setFixedSize(275, 36);
    pageRangeCombo->addItem(q->tr("All"));
    pageRangeCombo->addItem(q->tr("Current page"));
    pageRangeCombo->addItem(q->tr("Select pages"));
    QHBoxLayout *hrangebox = new QHBoxLayout();
    hrangebox->addWidget(pagerangelabel);
    hrangebox->addWidget(pageRangeCombo);

    fromLabel = new DLabel(q->tr("From"), pageFrame);
    fromeSpin = new DSpinBox(pageFrame);

    toLabel = new DLabel(q->tr("To"), pageFrame);
    toSpin = new DSpinBox(pageFrame);
    fromeSpin->setRange(1, totalPageLabel->text().toInt());
    toSpin->setRange(1, totalPageLabel->text().toInt());
    fromeSpin->setEnabledEmbedStyle(true);
    toSpin->setEnabledEmbedStyle(true);
    QHBoxLayout *hfromtolayout = new QHBoxLayout();
    fromLabel->setFixedWidth(40);
    fromeSpin->setFixedWidth(95);
    toLabel->setFixedWidth(30);
    toSpin->setFixedWidth(95);
    hfromtolayout->addStretch();
    hfromtolayout->addWidget(fromLabel);
    hfromtolayout->addWidget(fromeSpin);
    hfromtolayout->addWidget(toLabel);
    hfromtolayout->addWidget(toSpin);
    pagelayout->addLayout(hrangebox);
    pagelayout->addLayout(hfromtolayout);

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
    marginsframe->setFixedHeight(102);
    QVBoxLayout *marginslayout = new QVBoxLayout(marginsframe);
    marginslayout->setSpacing(10);
    QHBoxLayout *marginscombolayout = new QHBoxLayout;
    marginscombolayout->setContentsMargins(0, 0, 0, 0);
    DLabel *marginlabel = new DLabel(q->tr("Margins"));
    marginlabel->setFixedWidth(123);
    marginsCombo = new DComboBox;
    marginsCombo->addItems(QStringList() << q->tr("Ordinary") << q->tr("Narrow") << q->tr("Moderate") << q->tr("Customize"));
    marginsCombo->setFixedHeight(36);
    marginscombolayout->addWidget(marginlabel);
    marginscombolayout->addWidget(marginsCombo);
    marginslayout->addLayout(marginscombolayout);

    QHBoxLayout *marginsspinlayout = new QHBoxLayout;
    marginsspinlayout->setSpacing(5);
    DLabel *toplabel = new DLabel(q->tr("Top"));
    marginTopSpin = new DSpinBox;
    marginTopSpin->setEnabledEmbedStyle(true);
    marginTopSpin->setMinimum(0);
    DLabel *bottomlabel = new DLabel(q->tr("Bottom"));
    marginBottomSpin = new DSpinBox;
    marginBottomSpin->setEnabledEmbedStyle(true);
    marginBottomSpin->setMinimum(0);
    DLabel *leftlabel = new DLabel(q->tr("Left"));
    marginLeftSpin = new DSpinBox;
    marginLeftSpin->setEnabledEmbedStyle(true);
    marginLeftSpin->setMinimum(0);
    DLabel *rightlabel = new DLabel(q->tr("Right"));
    marginRightSpin = new DSpinBox;
    marginRightSpin->setEnabledEmbedStyle(true);
    marginRightSpin->setMinimum(0);
    marginsspinlayout->addWidget(toplabel);
    marginsspinlayout->addWidget(marginTopSpin);
    marginsspinlayout->addWidget(bottomlabel);
    marginsspinlayout->addWidget(marginBottomSpin);
    marginsspinlayout->addWidget(leftlabel);
    marginsspinlayout->addWidget(marginLeftSpin);
    marginsspinlayout->addWidget(rightlabel);
    marginsspinlayout->addWidget(marginRightSpin);
    marginslayout->addLayout(marginsspinlayout);

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

    scaleGroup->addButton(actualSizeRadio, 1);
    actuallayout->addWidget(actualSizeRadio);
    DWidget *shrinkwdg = new DWidget;
    shrinkwdg->setFixedHeight(48);
    QHBoxLayout *shrinklayout = new QHBoxLayout(shrinkwdg);
    DRadioButton *shrinkPageRadio = new DRadioButton(q->tr("Shrink oversized pages"));
    scaleGroup->addButton(shrinkPageRadio, 2);
    shrinklayout->addWidget(shrinkPageRadio);
    DWidget *customscalewdg = new DWidget;
    customscalewdg->setFixedHeight(48);
    QHBoxLayout *customlayout = new QHBoxLayout(customscalewdg);
    colorlayout->setContentsMargins(10, 1, 10, 1);
    DRadioButton *customSizeRadio = new DRadioButton(q->tr("Scale"));
    scaleGroup->addButton(customSizeRadio, 3);
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
    scalingcontentlayout->addWidget(shrinkwdg);
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
    printDeviceCombo->addItems(QPrinterInfo::availablePrinterNames());
    printDeviceCombo->addItem(q->tr("Print to PDF"));
    updateSetteings(0);
}

void DPrintPreviewDialogPrivate::initconnections()
{
    Q_Q(DPrintPreviewDialog);

    QObject::connect(advanceBtn, &QPushButton::clicked, q, [this] { this->showadvancesetting(); });
    QObject::connect(printDeviceCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(printerChanged(int)));
    QObject::connect(cancelBtn, &DPushButton::clicked, q, &DPrintPreviewDialog::close);
    //    QObject::connect(advanceBtn, &QPushButton::clicked, q, &DPrintPreviewDialog::showAdvanceSetting);
    QObject::connect(pageRangeCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(slotPageRangeCombox(int)));
    QObject::connect(marginsCombo, SIGNAL(currentIndexChanged(int)), q, SLOT(slotPageMarginCombox(int)));
    QObject::connect(printBtn, &DPushButton::clicked, q, &DPrintPreviewDialog::slotStartPrint);
}

void DPrintPreviewDialogPrivate::setfrmaeback(DWidget *frame)
{
    DPalette pa = DApplicationHelper::instance()->palette(frame);
    pa.setBrush(DPalette::Base, pa.itemBackground());
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

void DPrintPreviewDialogPrivate::setupPrinter() //设置打印属性
{
    Q_Q(DPrintPreviewDialog);
    //--基础设置--
    printer->setNumCopies(copycountspinbox->value()); //设置打印份数
    q->slotPageRangeCombox(pageRangeCombo->currentIndex()); //设置打印范围
    if (orientationgroup->checkedId() == 0)
        printer->setOrientation(QPrinter::Portrait);
    else {
        printer->setOrientation(QPrinter::Landscape);
    } //设置打印方向
    //--高级设置--
    QPrinterInfo prInfo(*printer);
    qDebug() << prInfo.supportedPageSizes() << paperSizeCombo->currentIndex() << paperSizeCombo->count() - 1;
    if (printDeviceCombo->currentIndex() != printDeviceCombo->count() - 1) {
        printer->setPageSize(prInfo.supportedPageSizes().at(paperSizeCombo->currentIndex())); //设置纸张大小
    } else {
        if (paperSizeCombo->currentIndex() == 0) {
            printer->setPageSize(QPrinter::A3);
        } else if (paperSizeCombo->currentIndex() == 1) {
            printer->setPageSize(QPrinter::A4);
        } else if (paperSizeCombo->currentIndex() == 2) {
            printer->setPageSize(QPrinter::A5);
        } else if (paperSizeCombo->currentIndex() == 3) {
            printer->setPageSize(QPrinter::B4);
        } else if (paperSizeCombo->currentIndex() == 4) {
            printer->setPageSize(QPrinter::B5);
        } else if (paperSizeCombo->currentIndex() == 5) {
            printer->setPageSize(QPrinter::Prc16K);
        } else {
            printer->setPageSize(QPrinter::Prc16K);
        }
    }

    if (duplexSwitchBtn->isChecked())
        printer->setDuplex(QPrinter::DuplexAuto);
    else {
        printer->setDuplex(QPrinter::DuplexNone);
    } //设置双面打印
    if (colorModeCombo->currentIndex() == 0)
        printer->setColorMode(QPrinter::Color);
    else {
        printer->setColorMode(QPrinter::GrayScale);
    } //设置色彩打印
    q->slotPageMarginCombox(marginsCombo->currentIndex()); //设置纸张打印边距
    qDebug() << printer->resolution() << __func__;
    if (scaleGroup->checkedId() == 1)
        printer->setResolution(static_cast<int>(printer->resolution() * 1.5));
    else if (scaleGroup->checkedId() == 2) {
        printer->setResolution(static_cast<int>(printer->resolution() * (1 / (scaleRateEdit->value() / 100))));
    } else {
        return;
    } //设置缩放比例
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

void DPrintPreviewDialogPrivate::updateSetteings(int index) //刷新页面属性
{
    Q_Q(DPrintPreviewDialog);
    QPrinterInfo updateinfo(*printer);
    QStringList pageSizeList;
    copycountspinbox->setValue(1);
    paperSizeCombo->setCurrentIndex(0);
    q->slotPageRangeCombox(0);
    q->slotPageMarginCombox(0);
    scaleGroup->button(1)->setChecked(true);
    orientationgroup->button(0)->setChecked(true);
    if (index != printDeviceCombo->count() - 1) {
        //        qDebug() << QPrinterInfo::availablePrinterNames() << updateinfo.supportedDuplexModes()
        //                 << updateinfo.supportedPageSizes() << updateinfo.supportsCustomPageSizes();
        for (int i = 0; i < updateinfo.supportedPageSizes().size(); i++) {
            pageSizeList.append(updateinfo.supportedPageSizes().at(i).key());
        }
        paperSizeCombo->addItems(pageSizeList);
        paperSizeCombo->setCurrentText("A4");
        qDebug() << updateinfo.supportedDuplexModes() << __func__;
        if (updateinfo.supportedDuplexModes().contains(QPrinter::DuplexLongSide) || updateinfo.supportedDuplexModes().contains(QPrinter::DuplexShortSide)) {
            duplexSwitchBtn->setEnabled(true);
        } else {
            duplexSwitchBtn->setEnabled(false);
        } //判断当前打印机是否支持双面打印，不支持禁用双面打印按钮，pdf不做判断
        QPlatformPrinterSupport *ps = QPlatformPrinterSupportPlugin::get();
        QPrintDevice currentDevice = ps->createPrintDevice(printDeviceCombo->currentText());
        qDebug() << currentDevice.supportedColorModes() << __func__;
        if (!currentDevice.supportedColorModes().contains(QPrint::Color)) {
            if (colorModeCombo->count() != 1)
                colorModeCombo->removeItem(0);
        } else {
            if (colorModeCombo->count() == 1)
                colorModeCombo->insertItem(0, q->tr("Color"));
        }
        //判断当前打印机是否支持彩色打印，不支持彩色打印删除彩色打印选择选项，pdf不做判断
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

void DPrintPreviewDialogPrivate::setEnable(const int &value, DComboBox *combox)
{
    if (combox == pageRangeCombo) {
        if (value != pageRangeCombo->count() - 1) {
            fromeSpin->setEnabled(false);
            toSpin->setEnabled(false);
            fromLabel->setEnabled(false);
            toLabel->setEnabled(false);
        } else {
            fromeSpin->setEnabled(true);
            toSpin->setEnabled(true);
            fromLabel->setEnabled(true);
            toLabel->setEnabled(true);
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

DPrintPreviewDialog::DPrintPreviewDialog(QWidget *parent)
    : DDialog(*new DPrintPreviewDialogPrivate(this), parent)
{
    Q_D(DPrintPreviewDialog);
    setFixedSize(851, 606);
    d->startup();
}

DPrintPreviewDialog::DPrintPreviewDialog(DPrinter *printer, QWidget *parent)
    : DDialog(*new DPrintPreviewDialogPrivate(this), parent)
{
    Q_D(DPrintPreviewDialog);
    setFixedSize(851, 606);
    d->startup(printer);
}

DPrintPreviewDialog::~DPrintPreviewDialog()
{
    Q_D(DPrintPreviewDialog);
    if (d->ownPrinter)
        delete d->printer;
}

void DPrintPreviewDialog::printerChanged(int index)
{
    Q_D(DPrintPreviewDialog);
    qDebug() << d->printDeviceCombo->itemText(index);
    d->printer->setPrinterName(d->printDeviceCombo->itemText(index));
    QPrinterInfo info(*d->printer);
    //    qDebug() << info.supportedDuplexModes() << info.supportedPageSizes() << __func__;
    if (index == d->printDeviceCombo->count() - 1) {
        //pdf
        d->copycountspinbox->setDisabled(true);
        d->copycountspinbox->setValue(1);
        d->paperSizeCombo->clear();
        d->duplexSwitchBtn->setChecked(false);
        d->duplexSwitchBtn->setEnabled(false);
        d->printer->setOutputFormat(QPrinter::PdfFormat);
        d->colorModeCombo->setEnabled(false);
    } else {
        //actual printer
        if (d->printer) {
            d->copycountspinbox->setDisabled(false);
            d->paperSizeCombo->clear();
            d->paperSizeCombo->setEnabled(true);
            d->duplexSwitchBtn->setEnabled(true);
            d->colorModeCombo->setEnabled(true);
        }
    }
    d->updateSetteings(index);
}

void DPrintPreviewDialog::slotPageRangeCombox(int value)
{
    qDebug() << value;
    Q_D(DPrintPreviewDialog);
    d->setEnable(value, d->pageRangeCombo);
    if (value == 0) {
        d->fromeSpin->setValue(1);
        d->toSpin->setValue(d->totalPageLabel->text().toInt());
        d->printer->setPrintRange(QPrinter::AllPages);
        d->printer->setFromTo(1, d->totalPageLabel->text().toInt());
    } else if (value == 1) {
        d->fromeSpin->setValue(d->jumpPageEdit->value());
        d->toSpin->setValue(d->jumpPageEdit->value());
        d->printer->setPrintRange(QPrinter::CurrentPage);
        d->printer->setFromTo(d->jumpPageEdit->value(), d->jumpPageEdit->value());
    } else {
        d->fromeSpin->setValue(d->jumpPageEdit->value());
        d->toSpin->setValue(d->totalPageLabel->text().toInt());
        d->printer->setPrintRange(QPrinter::PageRange);
        d->printer->setFromTo(d->fromeSpin->value(), d->toSpin->value());
    }
    qDebug() << d->jumpPageEdit->text().toInt();
}

void DPrintPreviewDialog::slotPageMarginCombox(int value) //设置纸张边距
{
    Q_D(DPrintPreviewDialog);
    d->setEnable(value, d->marginsCombo);
    if (value == 1) {
        d->printer->setPageMargins(QMarginsF(12.7, 12.7, 12.7, 12.7));
    } else if (value == 2) {
        d->printer->setPageMargins(QMarginsF(19.1, 25.4, 19.1, 25.4));
    } else if (value == 3) {
        d->printer->setPageMargins(QMarginsF(d->marginLeftSpin->value(), d->marginTopSpin->value(), d->marginRightSpin->value(), d->marginBottomSpin->value()));
    } else {
        d->printer->setPageMargins(QMarginsF(31.8, 25.4, 31.8, 25.4));
    }
}

void DPrintPreviewDialog::slotStartPrint(bool clicked) //开始打印
{
    Q_D(DPrintPreviewDialog);
    if (!clicked) {
        d->setupPrinter();
    }
    if (d->printDeviceCombo->currentIndex() == d->printDeviceCombo->count() - 1) {
        //----设置pdf保存文本信息，可以外部通过setDocName设置，如果不做任何操作默认保存名称print.pdf-----
        QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        desktopPath += QStringLiteral("/");
        if (d->printer == nullptr) {
            return;
        }
        if (d->printer->outputFileName().isEmpty()) {
            if (d->printer->docName().isEmpty())
                desktopPath += QStringLiteral("print.pdf");
            else {
                desktopPath += d->printer->docName();
            }
        } else {
            desktopPath = d->printer->outputFileName();
        }
        qDebug() << desktopPath;
        QString str = DFileDialog::getSaveFileName(this, tr("Save Text"), desktopPath, tr("Text Files (*.pdf)"));
        d->printer->setOutputFileName(str);
    }
    Q_EMIT paintRequested(d->printer);

    done(0);
}

DWIDGET_END_NAMESPACE
