#include "dprintpreviewwidget.h"
#include "private/dprintpreviewwidget_p.h"
#include <QVBoxLayout>
#include <private/qprinter_p.h>
#include <QPicture>

DWIDGET_BEGIN_NAMESPACE

DPrintPreviewWidgetPrivate::DPrintPreviewWidgetPrivate(DPrintPreviewWidget *qq)
    : DObjectPrivate(qq)
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
}

void DPrintPreviewWidgetPrivate::populateScene()
{
    for (auto *page : qAsConst(pages))
        scene->removeItem(page);
    qDeleteAll(pages);
    pages.clear();

    QSize paperSize = previewPrinter->pageLayout().fullRectPixels(previewPrinter->resolution()).size();
    QRect pageRect = previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution());

    int page = 1;
    for (int i = 0; i < targetPictures.size(); i++) {
        PageItem *item = new PageItem(page++, &targetPictures[i], paperSize, pageRect);
        scene->addItem(item);
        pages.append(item);
    }
    currentPageNumber = 1;
    setPageRangeAll();
}

void DPrintPreviewWidgetPrivate::generatePreview()
{
    Q_Q(DPrintPreviewWidget);
    previewPrinter->setPreviewMode(true);
    Q_EMIT q->paintRequested(previewPrinter);
    previewPrinter->setPreviewMode(false);
    pictures = previewPrinter->getPrinterPages();
    generateTargetPictures();
    populateScene();
    scene->setSceneRect(scene->itemsBoundingRect());
    fitView();
}

void DPrintPreviewWidgetPrivate::generateTargetPictures()
{
    targetPictures.clear();

    for (auto *picture : qAsConst(pictures)) {
        QPicture target;
        QPainter painter;
        painter.begin(&target);
        painter.drawPicture(0, 0, *picture);
        //todo scale,black and white,watermarking,……
        painter.end();
        targetPictures.append(target);
    }
}

void DPrintPreviewWidgetPrivate::fitView()
{
    QRectF target = scene->sceneRect();
    graphicsView->fitInView(target, Qt::KeepAspectRatio);
}

void DPrintPreviewWidgetPrivate::setPageRangeAll()
{
    int size = pages.size();
    for (int i = 1; i <= size; i++) {
        pageRange.append(i);
    }
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

void DPrintPreviewWidget::setPageRange(const QVector<int> &rangePages)
{
    Q_D(DPrintPreviewWidget);
    d->pageRange = rangePages;
}

int DPrintPreviewWidget::pagesCount()
{
    Q_D(DPrintPreviewWidget);
    return d->pages.size();
}

bool DPrintPreviewWidget::turnPageAble()
{
    Q_D(DPrintPreviewWidget);
    return d->pages.size() > 1;
}

void DPrintPreviewWidget::showPage(int pageNumber)
{
    Q_D(DPrintPreviewWidget);
    if (pageNumber < 1 || pageNumber > d->pages.size())
        return;
    d->currentPageNumber = pageNumber;
    d->pages[pageNumber - 1]->setVisible(true);
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
    showPage(d->currentPageNumber - 1);
}

void DPrintPreviewWidget::turnBack()
{
    Q_D(DPrintPreviewWidget);
    if (d->currentPageNumber >= d->pages.size())
        return;
    showPage(d->currentPageNumber + 1);
}

void DPrintPreviewWidget::turnBegin()
{
    Q_D(DPrintPreviewWidget);
    if (d->pageRange.isEmpty())
        return;
    showPage(1);
}

void DPrintPreviewWidget::turnEnd()
{
    Q_D(DPrintPreviewWidget);
    if (d->pageRange.isEmpty())
        return;
    showPage(d->pages.size());
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

    painter->drawPicture(pageRect.topLeft(), *pagePicture);

    painter->setPen(QPen(Qt::black, 0));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(paperRect);
}
DWIDGET_END_NAMESPACE
