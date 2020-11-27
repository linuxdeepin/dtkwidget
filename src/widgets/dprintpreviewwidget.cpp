#include "dprintpreviewwidget.h"
#include "private/dprintpreviewwidget_p.h"
#include <QVBoxLayout>
#include <private/qprinter_p.h>
#include <QPicture>
#include <QFileInfo>
#include <QtConcurrent>

#include <cups/cups.h>

#define FIRST_PAGE 1
#define FIRST_INDEX 0
#define WATER_DEFAULTFONTSIZE 65
#define WATER_TEXTSPACE WATER_DEFAULTFONTSIZE

DWIDGET_BEGIN_NAMESPACE
// 取自Qt源码qpixmapfilter.cpp 945行
static void grayscale(const QImage &image, QImage &dest, const QRect &rect = QRect())
{
    QRect destRect = rect;
    QRect srcRect = rect;
    if (rect.isNull()) {
        srcRect = dest.rect();
        destRect = dest.rect();
    }
    if (&image != &dest) {
        destRect.moveTo(QPoint(0, 0));
    }

    const unsigned int *data = reinterpret_cast<const unsigned int *>(image.bits());
    unsigned int *outData = reinterpret_cast<unsigned int *>(dest.bits());

    if (dest.size() == image.size() && image.rect() == srcRect) {
        // a bit faster loop for grayscaling everything
        int pixels = dest.width() * dest.height();
        for (int i = 0; i < pixels; ++i) {
            int val = qGray(data[i]);
            outData[i] = qRgba(val, val, val, qAlpha(data[i]));
        }
    } else {
        int yd = destRect.top();
        for (int y = srcRect.top(); y <= srcRect.bottom() && y < image.height(); y++) {
            data = reinterpret_cast<const unsigned int *>(image.scanLine(y));
            outData = reinterpret_cast<unsigned int *>(dest.scanLine(yd++));
            int xd = destRect.left();
            for (int x = srcRect.left(); x <= srcRect.right() && x < image.width(); x++) {
                int val = qGray(data[x]);
                outData[xd++] = qRgba(val, val, val, qAlpha(data[x]));
            }
        }
    }
}

DPrintPreviewWidgetPrivate::DPrintPreviewWidgetPrivate(DPrintPreviewWidget *qq)
    : DFramePrivate(qq)
    , imposition(DPrintPreviewWidget::One)
    , order(DPrintPreviewWidget::L2R_T2B)
    , refreshMode(DPrintPreviewWidgetPrivate::RefreshImmediately)
    , printMode(DPrintPreviewWidget::PrintToPrinter)
    , isAsynPreview(false)
    , asynPreviewNeedUpdate(false)
{
}

void DPrintPreviewWidgetPrivate::init()
{
    Q_Q(DPrintPreviewWidget);

    graphicsView = new GraphicsView;
    graphicsView->setInteractive(true);
    graphicsView->setDragMode(QGraphicsView::NoDrag);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    graphicsView->setLineWidth(0);

    scene = new QGraphicsScene(graphicsView);
    q->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());
    graphicsView->setScene(scene);

    background = new QGraphicsRectItem();
    background->setZValue(-1);
    scene->addItem(background);

    waterMark = new WaterMark;
    scene->addItem(waterMark);
    waterMark->setZValue(0);

    QVBoxLayout *layout = new QVBoxLayout(q);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(graphicsView);

    colorMode = previewPrinter->colorMode();
}

void DPrintPreviewWidgetPrivate::populateScene()
{
    QSize paperSize = previewPrinter->pageLayout().fullRectPixels(previewPrinter->resolution()).size();
    QRect pageRect = previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution());

    QRectF rect(pageRect);
    background->setRect(rect);
    background->setBrush(Qt::white);
    background->setPen(Qt::NoPen);

    int page = 1;
    //todo 多页显示接口添加
    for (int i = 0; i < pictures.size(); i++) {
        PageItem *item = new PageItem(page++, pictures[i], paperSize, pageRect);
        item->setVisible(false);
        scene->addItem(item);
        pages.append(item);
    }

    waterMark->setBoundingRect(pageRect);

    if (!pages.isEmpty()) {
        if (currentPageNumber == 0)
            currentPageNumber = FIRST_PAGE;
        setCurrentPage(currentPageNumber);
    }

    scene->setSceneRect(QRect(QPoint(0, 0), paperSize));
}

void DPrintPreviewWidgetPrivate::generatePreview()
{
    if (refreshMode == RefreshDelay)
        return;

    if (isAsynPreview)
        previewPages = {currentPageNumber == 0 ? FIRST_PAGE : index2page(currentPageNumber - 1)};

    generatePreviewPicture();
    setPageRangeAll();
    populateScene();
    impositionPages();
    fitView();
}

void DPrintPreviewWidgetPrivate::fitView()
{
    QRectF target = scene->sceneRect();
    graphicsView->fitInView(target, Qt::KeepAspectRatio);
    graphicsView->resetScale();
}

void DPrintPreviewWidgetPrivate::print(bool printAsPicture)
{
    QRect pageRect = previewPrinter->pageRect();
    QSize paperSize = previewPrinter->pageLayout().fullRectPixels(previewPrinter->resolution()).size();
    QMargins pageMargins = previewPrinter->pageLayout().marginsPixels(previewPrinter->resolution());
    QImage savedImages(paperSize, QImage::Format_ARGB32);
    QString outPutFileName = previewPrinter->outputFileName();
    QString suffix = QFileInfo(outPutFileName).suffix();
    bool isJpegImage = !suffix.compare(QLatin1String("jpeg"), Qt::CaseInsensitive);
    const QImage &waterMarkImage = generateWaterMarkImage();
    QPainter painter;

    QVector<int> pageVector;
    if (pageRangeMode == DPrintPreviewWidget::CurrentPage)
        pageVector.append(pageRange.at(currentPageNumber - 1));
    else {
        pageVector = pageRange;
    }

    if (isAsynPreview) {
        previewPages = pageVector;
        generatePreviewPicture();
    }

    if (printAsPicture) {
        painter.begin(&savedImages);
        painter.setClipRect(0, 0, paperSize.width(), paperSize.height());
    } else {
        painter.begin(previewPrinter);
        painter.setClipRect(0, 0, pageRect.width(), pageRect.height());
    }

    savedImages.fill(Qt::white);
    painter.scale(scale, scale);
    QPointF leftTopPoint;
    double lt_x, lt_y;
    if (scale >= 1.0) {
        lt_x = printAsPicture ? pageMargins.left() : 0.0;
        lt_y = printAsPicture ? pageMargins.top() : 0.0;
    } else {
        lt_x = printAsPicture ? (paperSize.width() * (1.0 - scale) / (2.0 * scale) + pageMargins.left())
                              : (pageRect.width() * (1.0 - scale) / (2.0 * scale));
        lt_y = printAsPicture ? (paperSize.height() * (1.0 - scale) / (2.0 * scale) + pageMargins.top())
                              : (pageRect.height() * (1.0 - scale) / (2.0 * scale));
    }

    leftTopPoint.setX(lt_x);
    leftTopPoint.setY(lt_y);

    for (int i = 0; i < pageVector.size(); i++) {
        if (0 != i && !printAsPicture)
            previewPrinter->newPage();

        painter.save();
        // 绘制水印
        painter.save();
        painter.resetTransform();
        painter.translate(paperSize.width() / 2, paperSize.height() / 2);
        painter.rotate(waterMark->rotation());
        painter.drawImage(-waterMarkImage.width() / 2, -waterMarkImage.height() / 2, waterMarkImage);
        painter.restore();
        //todo scale,black and white,watermarking,……
        // 绘制原始数据
        painter.drawPicture(leftTopPoint, *(isAsynPreview ? pictures[i] : pictures[pageVector.at(i) - 1]));
        painter.restore();

        if (printAsPicture) {
            // write image
            QString stres = outPutFileName.right(suffix.length() + 1);
            QString tmpString = outPutFileName.left(outPutFileName.length() - suffix.length() - 1) + QString("(%1)").arg(QString::number(i + 1)) + stres;

            // 多线程保存文件修复大文件卡顿问题
            QtConcurrent::run(QThreadPool::globalInstance(), [savedImages, tmpString, isJpegImage] {
                savedImages.save(tmpString, isJpegImage ? "JPEG" : "PNG");
            });

            savedImages.fill(Qt::white);
        }
    }

    painter.end();
}

void DPrintPreviewWidgetPrivate::setPageRangeAll()
{
    Q_Q(DPrintPreviewWidget);
    if (isAsynPreview) {
        int size;

        if ((pageRangeMode == DPrintPreviewWidget::AllPage) || (pageRangeMode == DPrintPreviewWidget::CurrentPage)) {
            size = asynPreviewTotalPage;
        } else {
            size = pageRange.count();
        }

        if (currentPageNumber == 0) {
            pageRange.clear();
            for (int i = FIRST_PAGE; i <= size; i++) {
                pageRange.append(i);
            }
        }

        if (pageRangeMode == DPrintPreviewWidget::SelectPage) {
            Q_EMIT q->pagesCountChanged(size);
        } else {
            Q_EMIT q->totalPages(size);
        }

    } else {
        int size = pictures.count();
        size = targetPage(size);
        pageRange.clear();
        for (int i = FIRST_PAGE; i <= size; i++) {
            pageRange.append(i);
        }

        Q_EMIT q->totalPages(size);
    }

    reviewChanged = false;
}

int DPrintPreviewWidgetPrivate::pagesCount()
{
    return pageRange.size();
}

void DPrintPreviewWidgetPrivate::setCurrentPage(int page)
{
    int pageCount = pagesCount();
    if (page < FIRST_PAGE)
        return;
    if (page > pageCount)
        page = pageCount;

    if (isAsynPreview) {
        currentPageNumber = page;

        // 如果单独使用这个函数 在异步预览下需要重绘一次 否则无法更新预览页面
        if (asynPreviewNeedUpdate) {
            asynPreviewNeedUpdate = false;
            generatePreview();
            return;
        } else {
            if (PageItem *currentPage = dynamic_cast<PageItem *>(pages.first())) {
                currentPage->setVisible(true);
            }
        }
    } else {
        int pageNumber = index2page(page - 1);
        if (pageNumber < 0)
            return;
        int lastPage = index2page(currentPageNumber - 1);
        if (lastPage > -1) {
            if (lastPage > pages.size())
                pages.back()->setVisible(false);
            else
                pages.at(lastPage - 1)->setVisible(false);
        }
        currentPageNumber = page;
        if (pageNumber > pages.size())
            return;

        if (PageItem *currentPage = dynamic_cast<PageItem *>(pages.at(pageNumber - 1))) {
            currentPage->setVisible(true);
        }
    }

    graphicsView->resetScale(false);
    Q_Q(DPrintPreviewWidget);
    Q_EMIT q->currentPageChanged(page);
}

int DPrintPreviewWidgetPrivate::targetPage(int page)
{
    int mod = 0;
    switch (imposition) {
    case DPrintPreviewWidget::Imposition::One:
        break;
    case DPrintPreviewWidget::Imposition::OneRowTwoCol:
        mod = page % 2;
        break;
    case DPrintPreviewWidget::Imposition::TwoRowTwoCol:
        mod = page % 4;
        break;
    case DPrintPreviewWidget::Imposition::TwoRowThreeCol:
        mod = page % 6;
        break;
    case DPrintPreviewWidget::Imposition::ThreeRowThreeCol:
        mod = page % 9;
        break;
    case DPrintPreviewWidget::Imposition::FourRowFourCol:
        mod = page % 16;
        break;
    default:
        break;
    }
    if (mod)
        page += 1;
    return page;
}

int DPrintPreviewWidgetPrivate::index2page(int index)
{
    if (index < 0 || index >= pageRange.size())
        return -1;
    return pageRange.at(index);
}

int DPrintPreviewWidgetPrivate::page2index(int page)
{
    return pageRange.indexOf(page);
}

void DPrintPreviewWidgetPrivate::impositionPages()
{
    // 快速切换会导致异常崩溃
    return;
    QSize paperSize = previewPrinter->pageLayout().fullRectPixels(previewPrinter->resolution()).size();
    QRect pageRect = previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution());

    switch (imposition) {
    case DPrintPreviewWidget::Imposition::One:
    {
        for (int i = 0, page = 1; i < pageRange.count(); i++) {
            PageItem *item = new PageItem(page++, pictures[i], paperSize, pageRect);
            item->setVisible(false);
            scene->addItem(item);
            pages.append(item);
        }
        if (!pages.isEmpty()) {
            if (currentPageNumber == 0)
                currentPageNumber = FIRST_PAGE;
            setCurrentPage(currentPageNumber);
        }
    }
        break;
    case DPrintPreviewWidget::Imposition::OneRowTwoCol:
        break;
    case DPrintPreviewWidget::Imposition::TwoRowTwoCol:
        break;
    case DPrintPreviewWidget::Imposition::TwoRowThreeCol:
        break;
    case DPrintPreviewWidget::Imposition::ThreeRowThreeCol:
        break;
    case DPrintPreviewWidget::Imposition::FourRowFourCol:
        break;
    default:
        break;
    }
}

QImage DPrintPreviewWidgetPrivate::generateWaterMarkImage() const
{
    QRectF itemMaxRect = waterMark->itemMaxPolygon().boundingRect();
    QImage originImage(itemMaxRect.size().toSize(), QImage::Format_ARGB32);
    originImage.fill(Qt::white);

    QPainter picPainter;
    picPainter.begin(&originImage);
    picPainter.setOpacity(waterMark->opacity());
    // 由于painter绘制此image中的位置是从0,0点开始的 但预览图的位置左移（上移）过 因此此处的绘图原点和预览图保持一致
    picPainter.translate(-itemMaxRect.topLeft());
    waterMark->updatePicture(&picPainter);
    picPainter.end();

    return originImage;
}

PrintOptions DPrintPreviewWidgetPrivate::printerOptions()
{
    PrintOptions options;

    options.append(QPair<QByteArray, QByteArray>(QStringLiteral("media").toLocal8Bit(), QPageSize(QPageSize::PageSizeId(previewPrinter->pageSize())).key().toLocal8Bit()));
    options.append(QPair<QByteArray, QByteArray>(QStringLiteral("copies").toLocal8Bit(), QString::number(previewPrinter->numCopies()).toLocal8Bit()));
    options.append(QPair<QByteArray, QByteArray>(QStringLiteral("fit-to-page").toLocal8Bit(), QStringLiteral("true").toLocal8Bit()));

    QString pageRangeString;
    if (pageRangeMode == DPrintPreviewWidget::CurrentPage) {
        pageRangeString = QString::number(pageRange.at(currentPageNumber - 1));
    } else {
        Q_FOREACH (int pageRangeCount, pageRange) {
            pageRangeString.append(QString::number(pageRangeCount).append(","));
        }

        pageRangeString.resize(pageRangeString.length() - 1);
    }

    options.append(QPair<QByteArray, QByteArray>(QStringLiteral("page-ranges").toLocal8Bit(), pageRangeString.toLocal8Bit()));

    switch (previewPrinter->duplex()) {
    case QPrinter::DuplexNone:
        options.append(QPair<QByteArray, QByteArray>(QStringLiteral("sides").toLocal8Bit(), QStringLiteral("one-sided").toLocal8Bit()));
        break;
    case QPrinter::DuplexAuto:
        if (previewPrinter->orientation() == QPrinter::Portrait) {
            options.append(QPair<QByteArray, QByteArray>(QStringLiteral("sides").toLocal8Bit(), QStringLiteral("two-sided-long-edge").toLocal8Bit()));
        } else {
            options.append(QPair<QByteArray, QByteArray>(QStringLiteral("sides").toLocal8Bit(), QStringLiteral("two-sided-short-edge").toLocal8Bit()));
        }

        break;
    case QPrinter::DuplexLongSide:
        options.append(QPair<QByteArray, QByteArray>(QStringLiteral("sides").toLocal8Bit(), QStringLiteral("two-sided-long-edge").toLocal8Bit()));
        break;
    case QPrinter::DuplexShortSide:
        options.append(QPair<QByteArray, QByteArray>(QStringLiteral("sides").toLocal8Bit(), QStringLiteral("two-sided-short-edge").toLocal8Bit()));
        break;
    }

    if (previewPrinter->colorMode() == QPrinter::GrayScale) {
        options.append(QPair<QByteArray, QByteArray>(QStringLiteral("ColorModel").toLocal8Bit(), QStringLiteral("Gray").toLocal8Bit()));
    } else {
        options.append(QPair<QByteArray, QByteArray>(QStringLiteral("ColorModel").toLocal8Bit(), QStringLiteral("RGB").toLocal8Bit()));
    }

    return options;
}

void DPrintPreviewWidgetPrivate::printByCups()
{
    //  libcups2-dev libcups2
    QLibrary cupsLibrary("cups", "2");
    if (!cupsLibrary.isLoaded()) {
        if (!cupsLibrary.load()) {
            qWarning() << "Cups not found";
            return;
        }
    }

    int (*cupsPrintFile)(const char *name, const char *filename,
                         const char *title, int num_options,
                         cups_option_t *options) = nullptr;
    cupsPrintFile = reinterpret_cast<decltype(cupsPrintFile)>(cupsLibrary.resolve("cupsPrintFile"));
    if (!cupsPrintFile) {
        qWarning() << "cupsPrintFile function load failed";
        return;
    }

    PrintOptions options = printerOptions();
    const int numOptions = options.size();

    QVector<cups_option_t> cupsOptStruct;
    cupsOptStruct.reserve(numOptions);

    for (int c = 0; c < numOptions; ++c) {
        cups_option_t opt;
        opt.name = options[c].first.data();
        opt.value = options[c].second.data();
        cupsOptStruct.append(opt);
    }

    QString printerName = previewPrinter->printerName();
    cups_option_t *optPtr = cupsOptStruct.size() ? &cupsOptStruct.first() : nullptr;

    cupsPrintFile(printerName.toLocal8Bit().constData(), printFromPath.toLocal8Bit().constData(),
                  previewPrinter->docName().toLocal8Bit().constData(), numOptions, optPtr);
}

void DPrintPreviewWidgetPrivate::generatePreviewPicture()
{
    for (auto *page : qAsConst(pages))
        scene->removeItem(page);
    qDeleteAll(pages);
    pages.clear();

    Q_Q(DPrintPreviewWidget);
    previewPrinter->setPreviewMode(true);
    if (isAsynPreview) {
        Q_EMIT q->paintRequested(previewPrinter, previewPages);
    } else {
        Q_EMIT q->paintRequested(previewPrinter);
    }
    previewPrinter->setPreviewMode(false);
    pictures = previewPrinter->getPrinterPages();
}

/*!
 * \~chinese \brief 构造一个 DPrintPreviewWidget。
 *
 * \~chinese \param printer 打印机
 * \~chinese \param parent 父控件
 */
DPrintPreviewWidget::DPrintPreviewWidget(DPrinter *printer, QWidget *parent)
    : DFrame(*new DPrintPreviewWidgetPrivate(this))
{
    Q_D(DPrintPreviewWidget);
    d->previewPrinter = printer;
    d->init();
}

/*!
 * \~chinese \brief 设置打印预览widget是否可见。
 *
 * \~chinese \param visible 是否可见
 */
void DPrintPreviewWidget::setVisible(bool visible)
{
    QWidget::setVisible(visible);
    if (visible) {
        updatePreview();
    }
}

/*!
 * \~chinese \brief 设置打印预览页面范围为所有页。
 */
void DPrintPreviewWidget::setPageRangeALL()
{
    Q_D(DPrintPreviewWidget);
    d->setPageRangeAll();
    if (!d->pageRange.isEmpty())
        d->setCurrentPage(FIRST_PAGE);
}

/*!
 * \~chinese \brief 设置是否需要重新生成页面内容。
 *
 * \~chinese \param generate 是否需要重新生成页面内容
 */
void DPrintPreviewWidget::setReGenerate(bool generate)
{
    reviewChange(generate);
}

/*!
 * \~chinese \brief 设置页面选择范围模式。
 *
 * \~chinese \param mode 页面选择范围模式，AllPage所有页，CurrentPage当前页，SelectPage选择页
 */
void DPrintPreviewWidget::setPageRangeMode(PageRange mode)
{
    Q_D(DPrintPreviewWidget);
    d->pageRangeMode = mode;
}

/*!
 * \~chinese \brief 获取页面选择范围模式。
 */
DPrintPreviewWidget::PageRange DPrintPreviewWidget::pageRangeMode()
{
    Q_D(DPrintPreviewWidget);
    return d->pageRangeMode;
}

/*!
 * \~chinese \brief 预览是否改变，当预览改变时需要调用。
 */
void DPrintPreviewWidget::reviewChange(bool generate)
{
    Q_D(DPrintPreviewWidget);
    d->reviewChanged = generate;
    d->impositionPages();
}

/*!
 * \~chinese \brief 设置预览页面范围。
 *
 * \~chinese \param rangePages 页码Vector
 */
void DPrintPreviewWidget::setPageRange(const QVector<int> &rangePages)
{
    Q_D(DPrintPreviewWidget);

    if (rangePages == d->pageRange)
        return;

    if (!d->isAsynPreview) {
        int currentPage = d->index2page(d->currentPageNumber - 1);
        if (currentPage > 0) {
            d->pages.at(currentPage - 1)->setVisible(false);
        }
    }
    d->pageRange = rangePages;
    Q_EMIT pagesCountChanged(d->pagesCount());
    setCurrentPage(d->currentPageNumber);
}

/*!
 * \~chinese \brief 设置预览页面范围。
 *
 * \~chinese \param from 起始页码
 * \~chinese \param to 终止页码
 */
void DPrintPreviewWidget::setPageRange(int from, int to)
{
    if (from > to)
        return;

    QVector<int> rangePages;
    for (int i = from; i <= to; i++)
        rangePages.append(i);

    setPageRange(rangePages);
}

/*!
 * \~chinese \brief 获取预览总页数。
 */
int DPrintPreviewWidget::pagesCount()
{
    Q_D(DPrintPreviewWidget);
    return d->pagesCount();
}

/*!
 * \~chinese \brief 获取当前页的页码。
 */
int DPrintPreviewWidget::currentPage()
{
    Q_D(DPrintPreviewWidget);
    return d->index2page(d->currentPageNumber - 1);
}

/*!
 * \~chinese \brief 获取是否可翻页。
 */
bool DPrintPreviewWidget::turnPageAble()
{
    Q_D(DPrintPreviewWidget);
    return pagesCount() > 1;
}

/*!
 * \~chinese \brief 设置色彩模式。
 *
 * \~chinese \param colorMode 色彩模式
 */
void DPrintPreviewWidget::setColorMode(const QPrinter::ColorMode &colorMode)
{
    Q_D(DPrintPreviewWidget);

    d->colorMode = colorMode;
    d->previewPrinter->setColorMode(colorMode);
    int page = d->index2page(d->currentPageNumber - 1);
    if (page > 0) {
        if (isAsynPreview()) {
            d->pages.first()->update();
        } else {
            d->pages.at(page - 1)->update();
        }

        d->graphicsView->resetScale(false);
    }
}

/*!
 * \~chinese \brief 设置页面方向。
 *
 * \~chinese \param pageOrientation 页面方向
 */
void DPrintPreviewWidget::setOrientation(const QPrinter::Orientation &pageOrientation)
{
    Q_D(DPrintPreviewWidget);

    d->previewPrinter->setOrientation(pageOrientation);
    reviewChange(true);
    d->generatePreview();
}

/*!
 * \~chinese \brief 获取色彩模式。
 */
DPrinter::ColorMode DPrintPreviewWidget::getColorMode()
{
    Q_D(DPrintPreviewWidget);
    return d->colorMode;
}

/*!
 * \~chinese \brief 设置页面缩放。
 *
 * \~chinese \param scale 缩放大小
 */
void DPrintPreviewWidget::setScale(qreal scale)
{
    Q_D(DPrintPreviewWidget);
    d->scale = scale;
}

/*!
 * \~chinese \brief 获取缩放大小。
 */
qreal DPrintPreviewWidget::getScale() const
{
    D_DC(DPrintPreviewWidget);
    return d->scale;
}

/*!
 * \~chinese \brief 刷新预览页面。
 */
void DPrintPreviewWidget::updateView()
{
    Q_D(DPrintPreviewWidget);
    if (d->currentPageNumber < 0 || d->currentPageNumber > d->pages.count() || d->pages.empty())
        return;

    if (d->isAsynPreview) {
        d->pages.first()->update();
    } else {
        d->pages.at(d->currentPageNumber - 1)->update();
    }

    d->graphicsView->resetScale(false);
}

/*!
 * \~chinese \brief 强制刷新水印效果
 */
void DPrintPreviewWidget::updateWaterMark()
{
    Q_D(DPrintPreviewWidget);

    if (d->refreshMode == DPrintPreviewWidgetPrivate::RefreshImmediately) {
        d->waterMark->update();
    }
}

/*!
 * \~chinese \brief 水印刷新开始，主要用于同时多次调用水印属性改变的接口时，只刷新一次
 */
void DPrintPreviewWidget::refreshBegin()
{
    Q_D(DPrintPreviewWidget);

    d->refreshMode = DPrintPreviewWidgetPrivate::RefreshDelay;
}

/*!
 * \~chinese \brief 水印刷新结束，刷新水印效果
 */
void DPrintPreviewWidget::refreshEnd()
{
    Q_D(DPrintPreviewWidget);

    d->refreshMode = DPrintPreviewWidgetPrivate::RefreshImmediately;
    updateWaterMark();
}

/*!
 * \~chinese \brief 设置水印类型。
 *
 * \~chinese \param type 水印类型
 */
void DPrintPreviewWidget::setWaterMarkType(int type)
{
    Q_D(DPrintPreviewWidget);

    d->waterMark->setType(static_cast<WaterMark::Type>(type));
    updateWaterMark();
}

/*!
 * \~chinese \brief 设置水印图片。
 *
 * \~chinese \param image 水印图片
 */
void DPrintPreviewWidget::setWaterMargImage(const QImage &image)
{
    Q_D(DPrintPreviewWidget);

    d->waterMark->setImage(image);
    updateWaterMark();
}

/*!
 * \~chinese \brief 设置水印旋转角度。
 *
 * \~chinese \param rotate 水印旋转角度
 */
void DPrintPreviewWidget::setWaterMarkRotate(qreal rotate)
{
    Q_D(DPrintPreviewWidget);

    d->waterMark->setRotation(rotate);
}

/*!
 * \~chinese \brief 设置水印缩放大小。
 *
 * \~chinese \param rotate 水印缩放大小
 */
void DPrintPreviewWidget::setWaterMarkScale(qreal scale)
{
    Q_D(DPrintPreviewWidget);

    d->waterMark->setScaleFactor(scale);
    updateWaterMark();
}

/*!
 * \~chinese \brief 设置水印透明度。
 *
 * \~chinese \param rotate 水印透明度
 */
void DPrintPreviewWidget::setWaterMarkOpacity(qreal opacity)
{
    Q_D(DPrintPreviewWidget);

    d->waterMark->setOpacity(opacity);
}

/*!
 * \~chinese \brief 设置“绝密”文字水印。
 */
void DPrintPreviewWidget::setConfidentialWaterMark()
{
    Q_D(DPrintPreviewWidget);

    d->waterMark->setText(qApp->translate("DPrintPreviewWidget", "Confidential"));
    updateWaterMark();
}

/*!
 * \~chinese \brief 设置“草稿”文字水印。
 */
void DPrintPreviewWidget::setDraftWaterMark()
{
    Q_D(DPrintPreviewWidget);

    d->waterMark->setText(qApp->translate("DPrintPreviewWidget", "Draft"));
    updateWaterMark();
}

/*!
 * \~chinese \brief 设置“样本”文字水印。
 */
void DPrintPreviewWidget::setSampleWaterMark()
{
    Q_D(DPrintPreviewWidget);

    d->waterMark->setText(qApp->translate("DPrintPreviewWidget", "Sample"));
    updateWaterMark();
}

/*!
 * \~chinese \brief 设置自定义文字水印。
 *
 * \~chinese \param text 自定义文字
 */
void DPrintPreviewWidget::setCustomWaterMark(const QString &text)
{
    Q_D(DPrintPreviewWidget);

    d->waterMark->setText(text);
    updateWaterMark();
}

/*!
 * \~chinese \brief 设置文字水印的文字内容。
 *
 * \~chinese \param text 文字水印的文字内容
 */
void DPrintPreviewWidget::setTextWaterMark(const QString &text)
{
    Q_D(DPrintPreviewWidget);

    d->waterMark->setText(text);
    updateWaterMark();
}

/*!
 * \~chinese \brief 设置文字水印的字体。
 *
 * \~chinese \param font 文字水印的字体
 */
void DPrintPreviewWidget::setWaterMarkFont(const QFont &font)
{
    Q_D(DPrintPreviewWidget);

    d->waterMark->setFont(font);
    updateWaterMark();
}

/*!
 * \~chinese \brief 设置文字水印的颜色。
 *
 * \~chinese \param color 文字水印的颜色
 */
void DPrintPreviewWidget::setWaterMarkColor(const QColor &color)
{
    Q_D(DPrintPreviewWidget);

    d->waterMark->setColor(color);
    updateWaterMark();
}

/*!
 * \~chinese \brief 设置水印的布局。
 *
 * \~chinese \param layout 水印的布局，Center居中，Tiled平铺
 */
void DPrintPreviewWidget::setWaterMarkLayout(int layout)
{
    Q_D(DPrintPreviewWidget);
    d->waterMark->setLayoutType(static_cast<WaterMark::Layout>(layout));
    updateWaterMark();
}

/*!
 * \~chinese \brief 设置并打的模式。
 *
 * \~chinese \param im 并打的模式
 */
void DPrintPreviewWidget::setImposition(Imposition im)
{
    Q_D(DPrintPreviewWidget);
    d->imposition = im;
    d->impositionPages();
}

/*!
 * \~chinese \brief 设置页面内并打的顺序。
 *
 * \~chinese \param order 页面内并打的顺序
 */
void DPrintPreviewWidget::setOrder(Order order)
{
    Q_D(DPrintPreviewWidget);
    d->order = order;
    d->impositionPages();
}

void DPrintPreviewWidget::setPrintFromPath(const QString &path)
{
    Q_D(DPrintPreviewWidget);

    d->printFromPath = path;
}

QString DPrintPreviewWidget::printFromPath() const
{
    D_DC(DPrintPreviewWidget);

    return d->printFromPath;
}

void DPrintPreviewWidget::setPrintMode(DPrintPreviewWidget::PrintMode pt)
{
    Q_D(DPrintPreviewWidget);

    d->printMode = pt;
}

void DPrintPreviewWidget::setAsynPreview(int totalPage)
{
    Q_D(DPrintPreviewWidget);

    d->isAsynPreview = true;
    d->asynPreviewTotalPage = totalPage;
}

bool DPrintPreviewWidget::isAsynPreview() const
{
    D_DC(DPrintPreviewWidget);

    return d->isAsynPreview;
}

/*!
 * \~chinese \brief 刷新预览。
 */
void DPrintPreviewWidget::updatePreview()
{
    Q_D(DPrintPreviewWidget);
    reviewChange(true);
    d->generatePreview();
    d->graphicsView->updateGeometry();
}

/*!
 * \~chinese \brief 前翻一页。
 */
void DPrintPreviewWidget::turnFront()
{
    Q_D(DPrintPreviewWidget);
    if (d->currentPageNumber < 2)
        return;
    setCurrentPage(d->currentPageNumber - 1);
}

/*!
 * \~chinese \brief 后翻一页。
 */
void DPrintPreviewWidget::turnBack()
{
    Q_D(DPrintPreviewWidget);
    if (d->currentPageNumber >= d->pagesCount())
        return;
    setCurrentPage(d->currentPageNumber + 1);
}

/*!
 * \~chinese \brief 第一页。
 */
void DPrintPreviewWidget::turnBegin()
{
    Q_D(DPrintPreviewWidget);
    if (d->pageRange.isEmpty())
        return;
    setCurrentPage(FIRST_PAGE);
}

/*!
 * \~chinese \brief 最后一页。
 */
void DPrintPreviewWidget::turnEnd()
{
    Q_D(DPrintPreviewWidget);
    if (d->pageRange.isEmpty())
        return;
    setCurrentPage(d->pageRange.size());
}

/*!
 * \~chinese \brief 设置当前页。
 *
 * \~chinese \param page 当前页
 */
void DPrintPreviewWidget::setCurrentPage(int page)
{
    Q_D(DPrintPreviewWidget);

    if (d->isAsynPreview)
        d->asynPreviewNeedUpdate = true;

    d->setCurrentPage(page);
}

void DPrintPreviewWidget::print(bool isSavedPicture)
{
    Q_D(DPrintPreviewWidget);
    Q_UNUSED(isSavedPicture);

    switch (d->printMode) {
    case PrintToPrinter:
        if (d->printFromPath.isEmpty()) {
            // 通过QPrinter打印
            d->print(false);
        } else {
            // 通过cups打印
            d->printByCups();
        }

        break;
    case PrintToImage:
        d->print(true);
        break;
    case PrintToPdf:
        d->print(false);
        break;
    }
}

void DPrintPreviewWidget::themeTypeChanged(DGuiApplicationHelper::ColorType themeType)
{
    Q_D(DPrintPreviewWidget);
    if (DGuiApplicationHelper::DarkType == themeType)
        d->scene->setBackgroundBrush(QColor(0, 0, 0, 3));
    else
        d->scene->setBackgroundBrush(QColor(255, 255, 255, 5));
}

DPrinter::DPrinter(QPrinter::PrinterMode mode)
    : QPrinter(mode)
{
}

void DPrinter::setPreviewMode(bool isPreview)
{
    d_ptr->setPreviewMode(isPreview);
}

QList<const QPicture *> DPrinter::getPrinterPages()
{
    return d_ptr->previewPages();
}

void PageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    QRectF paperRect(0, 0, paperSize.width(), paperSize.height());

    painter->setClipRect(paperRect & option->exposedRect);
    //painter->fillRect(paperRect, Qt::white);

    if (!pagePicture)
        return;

    content->setRect(QRectF(pageRect.topLeft(), pageRect.size()));
    content->update();
}

void PageItem::setVisible(bool isVisible)
{
    if (isVisible) {
        content->updateGrayContent();
    }

    QGraphicsItem::setVisible(isVisible);
}

void ContentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->setClipRect(brect & item->exposedRect);

    DPrintPreviewWidget *pwidget = qobject_cast<DPrintPreviewWidget *>(scene()->parent()->parent());
    qreal scale = pwidget->getScale();
    painter->scale(scale, scale);

    QPointF leftTopPoint;
    if (scale >= 1.0) {
        leftTopPoint = QPointF(0, 0);
    } else {
        leftTopPoint.setX(((pageRect.width() * (1.0 - scale) / 2.0)) / scale);
        leftTopPoint.setY(((pageRect.height() * (1.0 - scale) / 2.0)) / scale);
    }

    if (pwidget && (pwidget->getColorMode() == QPrinter::GrayScale)) {
        // 图像灰度处理
        painter->drawPicture(leftTopPoint, grayPicture);
    } else if (pwidget && (pwidget->getColorMode() == QPrinter::Color)) {
        painter->drawPicture(leftTopPoint, *pagePicture);
    }
}

void ContentItem::updateGrayContent()
{
    grayPicture = grayscalePaint(*pagePicture);
}

QPicture ContentItem::grayscalePaint(const QPicture &picture)
{
    QImage image(pageRect.size(), QImage::Format_ARGB32);
    QPainter imageP;

    image.fill(Qt::transparent);
    imageP.begin(&image);
    imageP.drawPicture(0, 0, picture);
    imageP.end();

    image = imageGrayscale(&image);

    QPicture temp;
    QPainter tempP;

    tempP.begin(&temp);
    tempP.drawImage(0, 0, image);
    tempP.end();

    return temp;
}

QImage ContentItem::imageGrayscale(const QImage *origin)
{
    int w = origin->width();
    int h = origin->height();
    QImage iGray(w, h, QImage::Format_ARGB32);

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            QRgb pixel = origin->pixel(i, j);
            int gray = qGray(pixel);
            QColor color(gray, gray, gray, qAlpha(pixel));
            iGray.setPixel(i, j, color.rgba());
        }
    }

    return iGray;
}

void WaterMark::setImage(const QImage &img)
{
    type = Image;
    sourceImage = img;
    graySourceImage = img;
    grayscale(img, graySourceImage, img.rect());
}

void WaterMark::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(item);
    Q_UNUSED(widget);
    QPainterPath path = itemClipPath();

    painter->setClipPath(path, Qt::IntersectClip);
    updatePicture(painter);
}

void WaterMark::updatePicture(QPainter *painter)
{
    DPrintPreviewWidget *pwidget = qobject_cast<DPrintPreviewWidget *>(scene()->parent()->parent());
    switch (type) {
    case Type::None:
        break;
    case Type::Text: {
        if (!(font.styleStrategy() & QFont::PreferAntialias))
            font.setStyleStrategy(QFont::PreferAntialias);

        font.setPointSize(qRound(WATER_DEFAULTFONTSIZE * mScaleFactor));

        if (layout == Center) {
            painter->save();
            painter->setRenderHint(QPainter::TextAntialiasing);
            painter->setFont(font);
            painter->setPen(color);

            painter->drawText(twoPolygon.boundingRect(), Qt::AlignCenter, text);
            painter->restore();
            break;
        }

        QFontMetrics fm(font);
        QSize textSize = fm.size(Qt::TextSingleLine, text);
        int space = qMin(textSize.width(), textSize.height());
        QSize spaceSize = QSize(WATER_TEXTSPACE, space);
        QImage textImage(textSize + spaceSize, QImage::Format_ARGB32);
        textImage.fill(Qt::transparent);
        QPainter tp;
        tp.begin(&textImage);

        tp.setFont(font);
        tp.setPen(color);
        tp.setBrush(Qt::NoBrush);
        tp.setRenderHint(QPainter::TextAntialiasing);
        tp.drawText(textImage.rect(), Qt::AlignBottom | Qt::AlignRight, text);
        tp.end();

        painter->save();
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        QBrush b;
        b.setTextureImage(textImage);
        painter->setBrush(b);
        painter->drawRect(twoPolygon.boundingRect());
        painter->restore();
    }
        break;
    case Type::Image: {
        if (sourceImage.isNull() || graySourceImage.isNull() || qFuzzyCompare(mScaleFactor, 0))
            return;

        QImage img = sourceImage;
        if (pwidget->getColorMode() == QPrinter::GrayScale) {
            img = graySourceImage;
        }

        img = img.scaledToWidth(qRound(img.width() * mScaleFactor));
        QSize size = img.size() / img.devicePixelRatio();
        int imgWidth = size.width();
        int imgHeight = size.height();
        int space = qMin(imgWidth, imgHeight);
        if (layout == Center) {
            QPointF leftTop(brect.center().x() - imgWidth / 2.0, brect.center().y() - imgHeight / 2.0);
            painter->drawImage(leftTop, img);
            break;
        }

        QRectF targetRectf = twoPolygon.boundingRect();
        QPointF leftTop = targetRectf.topLeft();
        qreal colStart = leftTop.x();
        for (int row = 0; targetRectf.contains(leftTop);) {
            leftTop += QPointF(row % 2 * space, 0);
            for (int col = 0; targetRectf.contains(leftTop); col++) {
                painter->drawImage(leftTop, img);
                leftTop += QPointF(imgWidth + space, 0);
            }
            row++;
            leftTop += QPointF(0, space + imgHeight);
            leftTop.setX(colStart);
        }
    }
        break;
    }
}

QPainterPath WaterMark::itemClipPath() const
{
    QPolygonF brectPol = mapFromScene(brectPolygon);
    QPolygonF twopol = mapFromScene(twoPolygon);
    QPainterPath path;
    path.addPolygon(twopol);
    path.addPolygon(brectPol);
    path.addPolygon(twopol);

    return path;
}

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    scaleRatio = 1.0;

    scaleResetButton = new DIconButton(this);
    scaleResetButton->setFixedSize(36, 36);
    scaleResetButton->setIcon(QIcon::fromTheme("print_previewscale"));
    scaleResetButton->setIconSize(QSize(18, 18));
    scaleResetButton->setVisible(false);

    onThemeTypeChanged(DGuiApplicationHelper::instance()->themeType());

    connect(scaleResetButton, &DIconButton::clicked, this, [this]() {
        resetScale(false);
    });
}

void GraphicsView::resetScale(bool autoReset)
{
    if (!autoReset)
        this->fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);

    scaleRatio = 1.0;
    scaleResetButton->setVisible(false);
}

void GraphicsView::mousePressEvent(QMouseEvent *e)
{
    if ((e->button() & Qt::LeftButton) && scaleRatio * 100 > 100)
        this->setDragMode(QGraphicsView::ScrollHandDrag);

    QGraphicsView::mousePressEvent(e);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() & Qt::LeftButton)
        this->setDragMode(QGraphicsView::NoDrag);

    QGraphicsView::mouseReleaseEvent(e);
}

void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (0 > e->angleDelta().y()) {
        if (scaleRatio * 100 > 10) {
            scale(PREVIEW_NARROW_RATIO, PREVIEW_NARROW_RATIO);
            scaleRatio *= PREVIEW_NARROW_RATIO;
            scaleResetButton->setVisible(true);
        }
    } else {
        if (scaleRatio * 100 < 200) {
            scale(PREVIEW_ENLARGE_RATIO, PREVIEW_ENLARGE_RATIO);
            scaleRatio *= PREVIEW_ENLARGE_RATIO;
            scaleResetButton->setVisible(true);
        }
    }

    if (qFuzzyCompare(scaleRatio, 1)) {
        scaleResetButton->setVisible(false);
        resetScale(false);
    }
}

void GraphicsView::resizeEvent(QResizeEvent *e)
{
    QGraphicsView::resizeEvent(e);
    scaleResetButton->move(this->width() - scaleResetButton->width() - PREVIEW_SCALEBUTTON_MARGIN,
                           PREVIEW_SCALEBUTTON_MARGIN);

    this->resetScale(false);
    Q_EMIT resized();
}

void GraphicsView::showEvent(QShowEvent *e)
{
    QGraphicsView::showEvent(e);
    scaleResetButton->move(this->width() - scaleResetButton->width() - PREVIEW_SCALEBUTTON_MARGIN,
                           PREVIEW_SCALEBUTTON_MARGIN);
    Q_EMIT resized();
}

void GraphicsView::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::PaletteChange:
        onThemeTypeChanged(DGuiApplicationHelper::instance()->themeType());
        break;

    default:
        QGraphicsView::changeEvent(e);
    }
}

void GraphicsView::onThemeTypeChanged(DGuiApplicationHelper::ColorType themeType)
{
    QPalette btnPalette = scaleResetButton->palette();
    if (themeType == DGuiApplicationHelper::LightType) {
        btnPalette.setColor(QPalette::Light, QColor(247, 247, 247, qRound(0.7 * 255)));
        btnPalette.setColor(QPalette::Dark, QColor(247, 247, 247, qRound(0.7 * 255)));
    } else if (themeType == DGuiApplicationHelper::DarkType) {
        btnPalette.setColor(QPalette::Light, QColor(32, 32, 32, qRound(0.5 * 255)));
        btnPalette.setColor(QPalette::Dark, QColor(32, 32, 32, qRound(0.5 * 255)));
    }

    scaleResetButton->setPalette(btnPalette);
}

DWIDGET_END_NAMESPACE
