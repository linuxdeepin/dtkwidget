#include "dprintpreviewwidget.h"
#include "private/dprintpreviewwidget_p.h"
#include <QVBoxLayout>
#include <private/qprinter_p.h>
#include <QPicture>

#define FIRST_PAGE 1
#define FIRST_INDEX 0

DWIDGET_BEGIN_NAMESPACE

DPrintPreviewWidgetPrivate::DPrintPreviewWidgetPrivate(DPrintPreviewWidget *qq)
    : DObjectPrivate(qq)
    , imposition(DPrintPreviewWidget::Imposition::None)
{
}

void DPrintPreviewWidgetPrivate::init()
{
    Q_Q(DPrintPreviewWidget);

    graphicsView = new GraphicsView;
    graphicsView->setInteractive(false);
    graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    scene = new QGraphicsScene(graphicsView);
    scene->setBackgroundBrush(Qt::gray);
    graphicsView->setScene(scene);

    QVBoxLayout *layout = new QVBoxLayout(q);
    layout->setContentsMargins(0, 0, 0, 0);
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
    if (pageRange.isEmpty()) {
        setPageRangeAll();
    }
    if (!pages.isEmpty()) {
        currentPageNumber = FIRST_PAGE;
        isGenerate = true;
        pages.at(index2page(FIRST_INDEX) - 1)->setVisible(true);
    }
}

void DPrintPreviewWidgetPrivate::generatePreview()
{
    for (auto *page : qAsConst(pages))
        scene->removeItem(page);
    qDeleteAll(pages);
    pages.clear();

    Q_Q(DPrintPreviewWidget);
    previewPrinter->setPreviewMode(true);
    Q_EMIT q->paintRequested(previewPrinter);
    previewPrinter->setPreviewMode(false);
    pictures = previewPrinter->getPrinterPages();
    populateScene();
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
    previewPrinter->setFromTo(1, pictures.count());
    for (int i = 0; i < pictures.count(); i++) {
        if (0 != i)
            previewPrinter->newPage();
        painter.save();
        //todo scale,black and white,watermarking,……
        painter.drawPicture(0, 0, *(pictures[i]));
        painter.restore();
    }
}

void DPrintPreviewWidgetPrivate::setPageRangeAll()
{
    int size = pages.size();
    pageRange.clear();
    for (int i = FIRST_PAGE; i <= size; i++) {
        pageRange.append(i);
    }
    isGenerate = false;
    Q_Q(DPrintPreviewWidget);
    Q_EMIT q->totalPages(size);
}

int DPrintPreviewWidgetPrivate::pagesCount()
{
    return targetPage(pageRange.size());
}

void DPrintPreviewWidgetPrivate::setCurrentPage(int page)
{
    int pageCount = pagesCount();
    if (page < FIRST_PAGE || page > pageCount || (page == FIRST_PAGE && page == currentPageNumber && isGenerate))
        return;
    if (page == FIRST_PAGE)
        isGenerate = true;
    int pageNumber = index2page(page - 1);
    int lastPage = index2page(currentPageNumber - 1);
    currentPageNumber = page;

    pages.at(lastPage - 1)->setVisible(false);
    pages.at(pageNumber - 1)->setVisible(true);

    Q_Q(DPrintPreviewWidget);
    Q_EMIT q->currentPageChanged(page);
}

int DPrintPreviewWidgetPrivate::targetPage(int page)
{
    int mod = 0;
    switch (imposition) {
    case DPrintPreviewWidget::Imposition::None:
        break;
    case DPrintPreviewWidget::Imposition::OneTwo:
        mod = page % 2;
        break;
    case DPrintPreviewWidget::Imposition::TwoTwo:
        mod = page % 4;
        break;
    case DPrintPreviewWidget::Imposition::TwoThree:
        mod = page % 6;
        break;
    case DPrintPreviewWidget::Imposition::ThreeThree:
        mod = page % 9;
        break;
    case DPrintPreviewWidget::Imposition::FourFour:
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
    if (index >= pageRange.size())
        return -1;
    return pageRange.at(index);
}

int DPrintPreviewWidgetPrivate::page2index(int page)
{
    return pageRange.indexOf(page);
}

QPicture PageItem::grayscalePaint(const QPicture &picture)
{
    QImage image(paperSize, QImage::Format_ARGB32);
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

QImage PageItem::imageGrayscale(const QImage *origin)
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

DPrintPreviewWidget::DPrintPreviewWidget(DPrinter *printer, QWidget *parent)
    : QWidget(parent)
    , DObject(*new DPrintPreviewWidgetPrivate(this))
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

void DPrintPreviewWidget::setPageRange(const QVector<int> &rangePages)
{
    Q_D(DPrintPreviewWidget);
    d->pageRange = rangePages;
    d->generatePreview();
}

void DPrintPreviewWidget::setPageRange(int from, int to)
{
    Q_D(DPrintPreviewWidget);
    if (from > to)
        return;
    d->pageRange.clear();
    for (int i = from; i <= to; i++)
        d->pageRange.append(i);
    d->generatePreview();
}

int DPrintPreviewWidget::pagesCount()
{
    Q_D(DPrintPreviewWidget);
    return d->pagesCount();
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
    d->pages.at(d->currentPageNumber - 1)->update();
}

void DPrintPreviewWidget::setOrientation(const QPrinter::Orientation &pageOrientation)
{
    Q_D(DPrintPreviewWidget);

    d->previewPrinter->setOrientation(pageOrientation);
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

void DPrintPreviewWidget::updatePreview()
{
    Q_D(DPrintPreviewWidget);
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

    DPrintPreviewWidget *pwidget = qobject_cast<DPrintPreviewWidget *>(scene()->parent()->parent());
    qreal scale = pwidget->getScale();
    painter->scale(scale, scale);
    if (pwidget && (pwidget->getColorMode() == QPrinter::GrayScale)) {
        // 图像灰度处理
        painter->drawPicture(QPointF(pageRect.topLeft().x() / scale, pageRect.topLeft().y() / scale), grayscalePaint(*pagePicture));
    } else if (pwidget && (pwidget->getColorMode() == QPrinter::Color)) {
        painter->drawPicture(QPointF(pageRect.topLeft().x() / scale, pageRect.topLeft().y() / scale), *pagePicture);
    }

    painter->resetTransform();
    painter->setPen(QPen(Qt::black, 0));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(paperRect);
}
DWIDGET_END_NAMESPACE
