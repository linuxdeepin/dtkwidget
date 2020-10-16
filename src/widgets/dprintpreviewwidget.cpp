#include "dprintpreviewwidget.h"
#include "private/dprintpreviewwidget_p.h"
#include <QVBoxLayout>
#include <private/qprinter_p.h>
#include <QPicture>

#define FIRST_PAGE 1
#define FIRST_INDEX 0

DWIDGET_BEGIN_NAMESPACE

DPrintPreviewWidgetPrivate::DPrintPreviewWidgetPrivate(DPrintPreviewWidget *qq)
    : DFramePrivate(qq)
    , imposition(DPrintPreviewWidget::One)
    , order(DPrintPreviewWidget::L2R_T2B)
    , refreshMode(DPrintPreviewWidgetPrivate::RefreshImmediately)
{
}

void DPrintPreviewWidgetPrivate::init()
{
    Q_Q(DPrintPreviewWidget);

    graphicsView = new GraphicsView;
    graphicsView->setInteractive(false);
    graphicsView->setDragMode(QGraphicsView::NoDrag);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setLineWidth(0);

    scene = new QGraphicsScene(graphicsView);
    q->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());
    graphicsView->setScene(scene);

    QVBoxLayout *layout = new QVBoxLayout(q);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(graphicsView);

    colorMode = previewPrinter->colorMode();
}

void DPrintPreviewWidgetPrivate::populateScene()
{
    QSize paperSize = previewPrinter->pageLayout().fullRectPixels(previewPrinter->resolution()).size();
    QRect pageRect = previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution());

    int page = 1;
    //todo 多页显示接口添加
    for (int i = 0; i < pictures.size(); i++) {
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

void DPrintPreviewWidgetPrivate::generatePreview()
{
    if (refreshMode == RefreshDelay)
        return;

    for (auto *page : qAsConst(pages))
        scene->removeItem(page);
    qDeleteAll(pages);
    pages.clear();

    Q_Q(DPrintPreviewWidget);
    previewPrinter->setPreviewMode(true);
    Q_EMIT q->paintRequested(previewPrinter);
    previewPrinter->setPreviewMode(false);
    pictures = previewPrinter->getPrinterPages();
    setPageRangeAll();
    populateScene();
    impositionPages();
    scene->setSceneRect(scene->itemsBoundingRect());
    fitView();
}

void DPrintPreviewWidgetPrivate::fitView()
{
    QRectF target = scene->sceneRect();
    graphicsView->fitInView(target, Qt::KeepAspectRatio);
}

void DPrintPreviewWidgetPrivate::print()
{
    QPainter painter(previewPrinter);
    QRect pageRect = previewPrinter->pageRect();
    painter.setClipRect(0, 0, pageRect.width(), pageRect.height());
    painter.scale(scale, scale);
    QPointF leftTopPoint;
    if (scale >= 1.0) {
        leftTopPoint.setX(0.0);
        leftTopPoint.setY(0.0);
    } else {
        leftTopPoint.setX((pageRect.width() * (1.0 - scale) / (2.0 * scale)));
        leftTopPoint.setY((pageRect.height() * (1.0 - scale) / (2.0 * scale)));
    }
    for (int i = 0; i < pageRange.size(); i++) {
        if (0 != i)
            previewPrinter->newPage();
        painter.save();
        //todo scale,black and white,watermarking,……
        painter.drawPicture(leftTopPoint, *(pictures[pageRange.at(i) - 1]));
        painter.restore();
    }
}

void DPrintPreviewWidgetPrivate::setPageRangeAll()
{
    int size = pictures.count();
    size = targetPage(size);
    pageRange.clear();
    for (int i = FIRST_PAGE; i <= size; i++) {
        pageRange.append(i);
    }
    Q_Q(DPrintPreviewWidget);
    Q_EMIT q->totalPages(size);

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
    pages.at(pageNumber - 1)->setVisible(true);

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

DPrintPreviewWidget::DPrintPreviewWidget(DPrinter *printer, QWidget *parent)
    : DFrame(*new DPrintPreviewWidgetPrivate(this))
{
    Q_D(DPrintPreviewWidget);
    d->previewPrinter = printer;
    d->init();
}

void DPrintPreviewWidget::setVisible(bool visible)
{
    QWidget::setVisible(visible);
    if (visible) {
        updatePreview();
    }
}

void DPrintPreviewWidget::setPageRangeALL()
{
    Q_D(DPrintPreviewWidget);
    d->setPageRangeAll();
    if (!d->pageRange.isEmpty())
        d->setCurrentPage(FIRST_PAGE);
}

void DPrintPreviewWidget::setReGenerate(bool generate)
{
    reviewChange(generate);
}

void DPrintPreviewWidget::setPageRangeMode(PageRange mode)
{
    Q_D(DPrintPreviewWidget);
    d->pageRangeMode = mode;
}

DPrintPreviewWidget::PageRange DPrintPreviewWidget::pageRangeMode()
{
    Q_D(DPrintPreviewWidget);
    return d->pageRangeMode;
}

void DPrintPreviewWidget::reviewChange(bool generate)
{
    Q_D(DPrintPreviewWidget);
    d->reviewChanged = generate;
    d->impositionPages();
}

void DPrintPreviewWidget::setPageRange(const QVector<int> &rangePages)
{
    Q_D(DPrintPreviewWidget);
    int currentPage = d->index2page(d->currentPageNumber - 1);
    if (currentPage > 0) {
        d->pages.at(currentPage - 1)->setVisible(false);
    }
    d->pageRange = rangePages;
    Q_EMIT pagesCountChanged(d->pagesCount());
    d->setCurrentPage(d->currentPageNumber);
}

void DPrintPreviewWidget::setPageRange(int from, int to)
{
    Q_D(DPrintPreviewWidget);
    if (from > to)
        return;
    int currentPage = d->index2page(d->currentPageNumber - 1);
    if (currentPage > 0) {
        d->pages.at(currentPage - 1)->setVisible(false);
    }
    d->pageRange.clear();
    for (int i = from; i <= to; i++)
        d->pageRange.append(i);
    Q_EMIT pagesCountChanged(d->pagesCount());
    d->setCurrentPage(d->currentPageNumber);
}

int DPrintPreviewWidget::pagesCount()
{
    Q_D(DPrintPreviewWidget);
    return d->pagesCount();
}

int DPrintPreviewWidget::currentPage()
{
    Q_D(DPrintPreviewWidget);
    return d->index2page(d->currentPageNumber - 1);
}

bool DPrintPreviewWidget::turnPageAble()
{
    Q_D(DPrintPreviewWidget);
    return pagesCount() > 1;
}

void DPrintPreviewWidget::setColorMode(const QPrinter::ColorMode &colorMode)
{
    Q_D(DPrintPreviewWidget);

    d->colorMode = colorMode;
    d->previewPrinter->setColorMode(colorMode);
    int page = d->index2page(d->currentPageNumber - 1);
    if (page > 0)
        d->pages.at(page - 1)->update();
}

void DPrintPreviewWidget::setOrientation(const QPrinter::Orientation &pageOrientation)
{
    Q_D(DPrintPreviewWidget);

    d->previewPrinter->setOrientation(pageOrientation);
    reviewChange(true);
    d->generatePreview();
}

DPrinter::ColorMode DPrintPreviewWidget::getColorMode()
{
    Q_D(DPrintPreviewWidget);
    return d->colorMode;
}

void DPrintPreviewWidget::setScale(qreal scale)
{
    Q_D(DPrintPreviewWidget);
    d->scale = scale;
}

qreal DPrintPreviewWidget::getScale() const
{
    D_DC(DPrintPreviewWidget);
    return d->scale;
}

void DPrintPreviewWidget::updateView()
{
    Q_D(DPrintPreviewWidget);
    if (d->currentPageNumber < 0 || d->currentPageNumber > d->pages.count() || d->pages.empty())
        return;
    d->pages.at(d->currentPageNumber - 1)->update();
}

void DPrintPreviewWidget::refreshBegin()
{
    Q_D(DPrintPreviewWidget);

    d->refreshMode = DPrintPreviewWidgetPrivate::RefreshDelay;
}

void DPrintPreviewWidget::refreshEnd()
{
    Q_D(DPrintPreviewWidget);

    d->refreshMode = DPrintPreviewWidgetPrivate::RefreshImmediately;
    updatePreview();
}

void DPrintPreviewWidget::setImposition(Imposition im)
{
    Q_D(DPrintPreviewWidget);
    d->imposition = im;
    d->impositionPages();
}

void DPrintPreviewWidget::setOrder(Order order)
{
    Q_D(DPrintPreviewWidget);
    d->order = order;
    d->impositionPages();
}

void DPrintPreviewWidget::updatePreview()
{
    Q_D(DPrintPreviewWidget);
    reviewChange(true);
    d->generatePreview();
    d->graphicsView->updateGeometry();
}

void DPrintPreviewWidget::turnFront()
{
    Q_D(DPrintPreviewWidget);
    if (d->currentPageNumber < 2)
        return;
    setCurrentPage(d->currentPageNumber - 1);
}

void DPrintPreviewWidget::turnBack()
{
    Q_D(DPrintPreviewWidget);
    if (d->currentPageNumber >= d->pagesCount())
        return;
    setCurrentPage(d->currentPageNumber + 1);
}

void DPrintPreviewWidget::turnBegin()
{
    Q_D(DPrintPreviewWidget);
    if (d->pageRange.isEmpty())
        return;
    setCurrentPage(FIRST_PAGE);
}

void DPrintPreviewWidget::turnEnd()
{
    Q_D(DPrintPreviewWidget);
    if (d->pageRange.isEmpty())
        return;
    setCurrentPage(d->pageRange.size());
}

void DPrintPreviewWidget::setCurrentPage(int page)
{
    Q_D(DPrintPreviewWidget);
    d->setCurrentPage(page);
}

void DPrintPreviewWidget::print()
{
    Q_D(DPrintPreviewWidget);
    d->print();
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
    painter->fillRect(paperRect, Qt::white);

    if (!pagePicture)
        return;

    content->setRect(QRectF(pageRect.topLeft(), pageRect.size()));
    content->update();
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
        painter->drawPicture(leftTopPoint, grayscalePaint(*pagePicture));
    } else if (pwidget && (pwidget->getColorMode() == QPrinter::Color)) {
        painter->drawPicture(leftTopPoint, *pagePicture);
    }
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

DWIDGET_END_NAMESPACE
