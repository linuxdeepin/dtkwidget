#include "dprintpreviewdialog.h"
#include "private/dprintpreviewdialog_p.h"
#include "dframe.h"
#include "diconbutton.h"
#include "dlabel.h"
#include "dlineedit.h"
#include "dfloatingbutton.h"
#include "dswitchbutton.h"
#include <DSpinBox>
#include <DBackgroundGroup>
#include <DApplicationHelper>
#include <DFontSizeManager>
#include <DScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QtPrintSupport/QPrinterInfo>
#include <QDebug>

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
    initui();
    initdata();
    initconnections();
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
    pview = new DFrame;
    pview->setFixedSize(364, 470);
    setfrmaeback(pview);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(pview);
    layout->setAlignment(pview, Qt::AlignCenter);
    QHBoxLayout *pbottomlayout = new QHBoxLayout;
    pbottomlayout->setContentsMargins(0, 10, 0, 0);
    layout->addLayout(pbottomlayout);
    firstBtn = new DIconButton(DStyle::SP_ArrowPrev);
    prevPageBtn = new DIconButton(DStyle::SP_ArrowLeft);
    jumpPageEdit = new DLineEdit();
    jumpPageEdit->setMaximumWidth(50);
    jumpPageEdit->setClearButtonEnabled(false);
    totalPageLabel = new DLabel("/123");
    nextPageBtn = new DIconButton(DStyle::SP_ArrowRight);
    lastBtn = new DIconButton(DStyle::SP_ArrowNext);
    pbottomlayout->addWidget(firstBtn);
    pbottomlayout->addWidget(prevPageBtn);
    pbottomlayout->addStretch();
    pbottomlayout->addWidget(jumpPageEdit);
    pbottomlayout->addWidget(totalPageLabel);
    pbottomlayout->addStretch();
    pbottomlayout->addWidget(nextPageBtn);
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
    DPalette pa = advanceBtn->palette();
    pa.setColor(DPalette::ButtonText, pa.link().color());
    advanceBtn->setPalette(pa);
    advanceBtn->setFlat(true);
    QHBoxLayout *advancelayout = new QHBoxLayout;
    advancelayout->addWidget(advanceBtn);

    ptoplayout->addWidget(basicsettingwdg);
    ptoplayout->addLayout(advancelayout);
    ptoplayout->addWidget(scrollarea);

    initbasicui();
    initadvanceui();
    //bottom
    QHBoxLayout *pbottomlayout = new QHBoxLayout;
    pbottomlayout->setContentsMargins(0, 10, 0, 10);
    cancelBtn = new DPushButton(q->tr("Cancel"));
    printBtn = new DPushButton(q->tr("Print"));
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
    //    printerlayout->setAlignment(printerlabel, Qt::AlignVCenter);
    printerlayout->setAlignment(printDeviceCombo, Qt::AlignVCenter);

    //打印份数
    DFrame *copycountFrame = new DFrame(basicsettingwdg);
    layout->addWidget(copycountFrame);
    copycountFrame->setFixedSize(422, 48);
    setfrmaeback(copycountFrame);
    QHBoxLayout *copycountlayout = new QHBoxLayout(copycountFrame);
    copycountlayout->setContentsMargins(10, 0, 10, 0);
    DLabel *copycountlabel = new DLabel(q->tr("Copies"), copycountFrame);
    copycountspinbox = new DSpinBox(copycountFrame);
    copycountspinbox->setEnabledEmbedStyle(true);
    copycountspinbox->setMinimum(1);
    copycountspinbox->setFixedSize(275, 36);
    copycountlayout->addWidget(copycountlabel);
    copycountlayout->addWidget(copycountspinbox);

    //页码范围
    DFrame *pageFrame = new DFrame(basicsettingwdg);
    layout->addWidget(pageFrame);
    pageFrame->setFixedSize(422, 94);
    setfrmaeback(pageFrame);
    QVBoxLayout *pagelayout = new QVBoxLayout(pageFrame);
    DLabel *pagerangelabel = new DLabel(q->tr("Page range"), pageFrame);
    pageRangeCombo = new DComboBox(pageFrame);
    pageRangeCombo->addItem(q->tr("All"));
    pageRangeCombo->addItem(q->tr("Current page"));
    pageRangeCombo->addItem(q->tr("Select pages"));
    QHBoxLayout *hrangebox = new QHBoxLayout();
    hrangebox->addWidget(pagerangelabel);
    hrangebox->addWidget(pageRangeCombo);

    DLabel *fromLabel = new DLabel(q->tr("From"), pageFrame);
    fromeSpin = new DSpinBox(pageFrame);
    DLabel *toLabel = new DLabel(q->tr("To"), pageFrame);
    toSpin = new DSpinBox(pageFrame);
    fromeSpin->setEnabledEmbedStyle(true);
    toSpin->setEnabledEmbedStyle(true);
    QHBoxLayout *hfromtolayout = new QHBoxLayout();
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
    verRadio->setText("sbkebcmj");
    DRadioButton *horRadio = new DRadioButton;
    orientationgroup = new QButtonGroup(q);
    orientationgroup->addButton(verRadio, 0);
    orientationgroup->addButton(horRadio, 1);

    //纵向
    DWidget *portraitwdg = new DWidget;
    portraitwdg->setFixedSize(422, 48);
    QHBoxLayout *portraitlayout = new QHBoxLayout;
    DLabel *orientationlabel = new DLabel;
    DLabel *orientationTextLabel = new DLabel(q->tr("Portrait"), portraitwdg);
    portraitlayout->addWidget(verRadio);
    portraitlayout->addWidget(orientationlabel);
    portraitlayout->addWidget(orientationTextLabel);
    portraitwdg->setLayout(portraitlayout);

    //横向
    DWidget *landscapewdg = new DWidget;
    landscapewdg->setFixedSize(422, 48);
    QHBoxLayout *landscapelayout = new QHBoxLayout;
    DLabel *landscapelabel = new DLabel;
    DLabel *landscapeTextLabel = new DLabel(q->tr("Landscape"), portraitwdg);
    landscapelayout->addWidget(horRadio);
    landscapelayout->addWidget(landscapelabel);
    landscapelayout->addWidget(landscapeTextLabel);
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
    pagestitlelayout->setContentsMargins(0, 0, 0, 0);
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
    setfrmaeback(marginsframe);
    marginsframe->setFixedHeight(102);
    QVBoxLayout *marginslayout = new QVBoxLayout(marginsframe);
    marginslayout->setSpacing(10);
    QHBoxLayout *marginscombolayout = new QHBoxLayout;
    marginscombolayout->setContentsMargins(0, 0, 0, 0);
    DLabel *marginlabel = new DLabel(q->tr("Margins"));
    marginlabel->setFixedWidth(123);
    marginsCombo = new DComboBox;
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
    //marginTopSpin->setFixedSize(71, 36);
    DLabel *bottomlabel = new DLabel(q->tr("Bottom"));
    marginBottomSpin = new DSpinBox;
    marginBottomSpin->setEnabledEmbedStyle(true);
    marginBottomSpin->setMinimum(0);
    // marginBottomSpin->setFixedSize(71, 36);
    DLabel *leftlabel = new DLabel(q->tr("Left"));
    marginLeftSpin = new DSpinBox;
    marginLeftSpin->setEnabledEmbedStyle(true);
    marginLeftSpin->setMinimum(0);
    // marginLeftSpin->setFixedSize(71, 36);
    DLabel *rightlabel = new DLabel(q->tr("Right"));
    marginRightSpin = new DSpinBox;
    marginRightSpin->setEnabledEmbedStyle(true);
    marginRightSpin->setMinimum(0);
    //marginRightSpin->setFixedSize(71, 36);
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
    layout->addLayout(pagelayout);

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
    DWidget *fitwdg = new DWidget;
    fitwdg->setFixedHeight(48);
    QHBoxLayout *fitlayout = new QHBoxLayout(fitwdg);
    DRadioButton *fitPaperRadio = new DRadioButton(q->tr("Fit to paper size"));
    scaleGroup->addButton(fitPaperRadio, 0);
    fitlayout->addWidget(fitPaperRadio);
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
    scaleRateEdit = new DLineEdit;
    scaleRateEdit->setFixedWidth(78);
    customlayout->addWidget(customSizeRadio);
    customlayout->addWidget(scaleRateEdit);
    customlayout->addStretch(1);
    scalingcontentlayout->addWidget(fitwdg);
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
    layout->addLayout(scalinglayout);

    //纸张
    QVBoxLayout *paperlayout = new QVBoxLayout;
    paperlayout->setContentsMargins(10, 0, 10, 0);
    DLabel *paperLabel = new DLabel(q->tr("Paper"), advancesettingwdg);
    setwidgetfont(paperLabel, DFontSizeManager::T5);
    QHBoxLayout *papertitlelayout = new QHBoxLayout;
    papertitlelayout->setContentsMargins(0, 20, 0, 0);
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
    layout->addLayout(paperlayout);

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

    DFrame *drawingframe = new DFrame;
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

    drawinglayout->addLayout(drawingtitlelayout);
    drawinglayout->addWidget(duplexframe);
    drawinglayout->addWidget(drawingframe);
    layout->addLayout(drawinglayout);

    //打印顺序
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
    layout->addLayout(orderlayout);

    //水印
    QVBoxLayout *watermarklayout = new QVBoxLayout;
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
    layout->addLayout(watermarklayout);
}

void DPrintPreviewDialogPrivate::initdata()
{
    Q_Q(DPrintPreviewDialog);
    qDebug() << QPrinterInfo::availablePrinterNames();
    printDeviceCombo->addItems(QPrinterInfo::availablePrinterNames());
    printDeviceCombo->addItem(q->tr("Print to PDF"));
}

void DPrintPreviewDialogPrivate::initconnections()
{
    Q_Q(DPrintPreviewDialog);
    QObject::connect(advanceBtn, &QPushButton::clicked, q, &DPrintPreviewDialog::showAdvanceSetting);
}

void DPrintPreviewDialogPrivate::setfrmaeback(DWidget *frame)
{
    Q_Q(DPrintPreviewDialog);
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
    } else {
        basicsettingwdg->show();
        scrollarea->hide();
    }
}

DPrintPreviewDialog::DPrintPreviewDialog(QWidget *parent)
    : DDialog(*new DPrintPreviewDialogPrivate(this), parent)
{
    Q_D(DPrintPreviewDialog);
    setFixedSize(851, 606);
    d->startup();
}

void DPrintPreviewDialog::showAdvanceSetting()
{
    Q_D(DPrintPreviewDialog);
    d->showadvancesetting();
}

DWIDGET_END_NAMESPACE
