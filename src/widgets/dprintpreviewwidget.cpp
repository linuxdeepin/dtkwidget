// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "private/dprintpreviewwidget_p.h"
#include <QVBoxLayout>
#include <private/qprinter_p.h>
#include <QPicture>
#include <QFileInfo>
#include <QtConcurrent>
#include <QtAlgorithms>
#include <QPaintEngine>
#include <DWidgetUtil>
#include <DIconTheme>

#include <cups/cups.h>
#include <cups/ppd.h>


#define FIRST_PAGE 1
#define FIRST_INDEX 0

#define ONE_PAGE 1
#define ONE_ROW_TWO_COL_PAGES 2
#define TWO_ROW_TWO_COL_PAGES 4
#define TWO_ROW_THREE_COL_PAGES 6
#define THREE_ROW_THREE_COL_PAGES 9
#define FOUR_ROW_FOUR_COL_PAGES 16

#define WATER_DEFAULTFONTSIZE 65
#define WATER_TEXTSPACE WATER_DEFAULTFONTSIZE

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

static QString truncateFileName(const QString &str, int maxLength = 255) {
    if (str.length() <= maxLength)
        return str;

    int keepFront = (maxLength - 3) / 2; // 例如 maxLength=255 → keepFront=126
    int keepBack = (maxLength - 3) - keepFront; // 126

    // 截取前半部分 + "..." + 后半部分
    return str.left(keepFront) + QString("...") + str.right(keepBack);
}

static void saveImageToFile(int index, const QString &outPutFileName, const QString &suffix, bool isJpegImage, const QImage &srcImage)
{
    // write image
    QString stres = outPutFileName.right(suffix.length() + 1);
    QString tmpString = outPutFileName.left(outPutFileName.length() - suffix.length() - 1) + QString("(%1)").arg(QString::number(index + 1)) + stres;

    // 多线程保存文件修复大文件卡顿问题
    (void)QtConcurrent::run(QThreadPool::globalInstance(), [srcImage, tmpString, isJpegImage] {
        const QFileInfo file(tmpString);
        const auto fileName = file.absolutePath() + "/" + truncateFileName(file.fileName());
        if (!srcImage.save(fileName, isJpegImage ? "JPEG" : "PNG")) {
            qWarning() << "Failed to save image to file, filePath:" << fileName;
        }
    });
}

DPrintPreviewWidgetPrivate::DPrintPreviewWidgetPrivate(DPrintPreviewWidget *qq)
    : DFramePrivate(qq)
    , imposition(DPrintPreviewWidget::One)
    , order(DPrintPreviewWidget::L2R_T2B)
    , refreshMode(DPrintPreviewWidgetPrivate::RefreshImmediately)
    , printMode(DPrintPreviewWidget::PrintToPrinter)
    , isAsynPreview(false)
    , asynPreviewNeedUpdate(false)
    , numberUpPrintData(nullptr)
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
    waterMark->setZValue(1);

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
    background->setRect(QRect(QPoint(0, 0), paperSize));
    background->setBrush(Qt::white);
    background->setPen(Qt::NoPen);

    for (auto *page : std::as_const(pages))
        scene->removeItem(page);
    qDeleteAll(pages);
    pages.clear();

    int page = 1;
    //todo 多页显示接口添加
    for (int i = 0; i < pictures.size(); i++) {
        PageItem *item = new PageItem(page++, pictures[i], paperSize, pageRect);
        item->setVisible(false);
        scene->addItem(item);
        pages.append(item);
    }

    if (isAsynPreview) {
        if (imposition == DPrintPreviewWidget::One) {
            if (PageItem *pi = dynamic_cast<PageItem *>(pages.first()))
                pi->setVisible(true);
        }
    } else {
        if (currentPageNumber == 0 && pages.count() > 0) {
            if (PageItem *pi = dynamic_cast<PageItem *>(pages.first())) {
                pi->setVisible(true);
                setCurrentPageNumber(FIRST_PAGE);
            }
        }
    }

    waterMark->setBoundingRect(pageRect);

    scene->setSceneRect(QRect(QPoint(0, 0), paperSize));
}

void DPrintPreviewWidgetPrivate::updatePreview()
{
    generatePreview();
    graphicsView->updateGeometry();
}

void DPrintPreviewWidgetPrivate::generatePreview()
{
    int totalPages = 0;
    if (isAsynPreview) {
        if (currentPageNumber == 0) {
            pageRange.clear();
            for (int i = 1; i <= asynPreviewTotalPage; i++) {
                pageRange.append(i);
            }
            setCurrentPageNumber(FIRST_PAGE);
        }

        previewPages = requestPages(currentPageNumber);
    }
    generatePreviewPicture();
    populateScene();

    // 同步或者异步（全部，当前）页码时 更新总页码
    if (!isAsynPreview || (isAsynPreview && pageRangeMode != DPrintPreviewWidget::SelectPage))
        setPageRangeAll();

    totalPages = pageRange.count();

    // 触发重绘水印必须重新生成！
    if (numberUpPrintData)
        numberUpPrintData->needRecreateWater = true;

    if (imposition != DPrintPreviewWidget::One) {
        calculateNumberUpPage();
        updateNumberUpContent();
    }
    if (!isAsynPreview)
        setCurrentPage(currentPageNumber);

    fitView();

    Q_Q(DPrintPreviewWidget);
    if (!isAsynPreview) {
        Q_EMIT q->totalPages(totalPages);
    } else {
        if (pageRangeMode == DPrintPreviewWidget::SelectPage) {
            Q_EMIT q->pagesCountChanged(totalPages);
        } else {
            Q_EMIT q->totalPages(totalPages);
        }
    }
}

void DPrintPreviewWidgetPrivate::fitView()
{
    QRectF target = scene->sceneRect();
    graphicsView->fitInView(target, Qt::KeepAspectRatio);
    graphicsView->resetScale();
}

void DPrintPreviewWidgetPrivate::asynPrint(const QPointF &leftTop, const QRect &pageRect, const QVector<int> &pageVector)
{
    QPainter painter(previewPrinter);

    painter.setClipRect(0, 0, pageRect.width(), pageRect.height());
    painter.scale(scale, scale);

    if (imposition == DPrintPreviewWidget::One) {
        const QImage &waterMarkImage = generateWaterMarkImage();
        for (int i = 0; i < pageVector.size(); ++i) {
            if (0 != i)
                previewPrinter->newPage();

            printSinglePageDrawUtil(&painter, pageRect.size(), leftTop, waterMarkImage, pictures.at(i));
        }
    } else {
        QImage waterMarkImage;
        int curPageCount = numberUpPrintData->rowCount * numberUpPrintData->columnCount;
        for (int i = 0; i < q_func()->targetPageCount(pageVector.size()); ++i) {
            if (0 != i)
                previewPrinter->newPage();

            // 异步下pictures只有需要打印的数据 需要按照pageVector当前的值进行迭代
            numberUpPrintData->previewPictures.clear();
            if (order != DPrintPreviewWidget::Copy) {
                for (int c = 0; c < curPageCount; ++c) {
                    int index = i * curPageCount + c;
                    if (index + 1 > pictures.length())
                        break;

                    numberUpPrintData->previewPictures.append(qMakePair(index, pictures.at(index)));
                }
            } else {
                numberUpPrintData->previewPictures = {curPageCount, qMakePair(i, pictures.at(i))};
            }

            // 并打时 水印需要在第一次或者当前页数与总页面数量不一致时重新生成
            if ((0 == i) || (numberUpPrintData->previewPictures.count() != numberUpPrintData->paintPoints.count()))
                waterMarkImage = generateWaterMarkImage();

            printMultiPageDrawUtil(&painter, leftTop, waterMarkImage);
        }
    }
}

void DPrintPreviewWidgetPrivate::syncPrint(const QPointF &leftTop, const QRect &pageRect, const QVector<int> &pageVector)
{
    QPainter painter(previewPrinter);

    painter.setClipRect(0, 0, pageRect.width(), pageRect.height());
    painter.scale(scale, scale);

    if (imposition == DPrintPreviewWidget::One) {
        const QImage &waterMarkImage = generateWaterMarkImage();
        for (int i = 0; i < pageVector.size(); ++i) {
            if (0 != i)
                previewPrinter->newPage();

            printSinglePageDrawUtil(&painter, pageRect.size(), leftTop, waterMarkImage, pictures[pageVector.at(i) - 1]);
        }
    } else {
        QImage waterMarkImage;
        for (int i = 0; i < q_func()->targetPageCount(pageVector.size()); ++i) {
            if (0 != i)
                previewPrinter->newPage();

            // 调整当前页码 更新当前页数据
            if (pageRangeMode == DPrintPreviewWidget::CurrentPage) {
                currentPageNumber = pageVector.at(i);
            } else {
                currentPageNumber = i + 1;
            }
            // 同步模式下pictures有所有数据，因此可以直接计算
            calculateCurrentNumberPage();

            // 并打时 水印需要在第一次或者当前页数与总页面数量不一致时重新生成
            if ((0 == i) || (numberUpPrintData->previewPictures.count() != numberUpPrintData->paintPoints.count()))
                waterMarkImage = generateWaterMarkImage();

            printMultiPageDrawUtil(&painter, leftTop, waterMarkImage);
        }
    }
}

void DPrintPreviewWidgetPrivate::printAsImage(const QSize &paperSize, QVector<int> &pageVector)
{
    QMargins pageMargins = previewPrinter->pageLayout().marginsPixels(previewPrinter->resolution());
    QImage savedImages(paperSize, QImage::Format_ARGB32);
    QString outPutFileName = previewPrinter->outputFileName();
    QString suffix = QFileInfo(outPutFileName).suffix();
    bool isJpegImage = !suffix.compare(QLatin1String("jpeg"), Qt::CaseInsensitive);
    QImage waterMarkImage = (imposition == DPrintPreviewWidget::One) ? generateWaterMarkImage() : QImage();

    savedImages.fill(Qt::white);

    QPainter painter(&savedImages);
    painter.setClipRect(previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution()));
    painter.scale(scale, scale);

    QPointF leftTopPoint;
    if (scale >= 1.0) {
        leftTopPoint = QPointF(pageMargins.left() / scale, pageMargins.top() / scale);
    } else {
        leftTopPoint = {paperSize.width() * (1.0 - scale) / (2.0 * scale) + pageMargins.left(), paperSize.height() * (1.0 - scale) / (2.0 * scale) + pageMargins.top()};
    }

    // 水印需要调整的位置大小  跟随页面内容位置变化
    QSize translateSize = paperSize + QSize(pageMargins.left() - pageMargins.right(), pageMargins.top() - pageMargins.bottom());

    if (isAsynPreview) {
        // 异步先获取需要打印的数据
        if (pageRangeMode == DPrintPreviewWidget::CurrentPage) {
            previewPages = requestPages(pageVector.first());
        } else {
            previewPages = pageVector;
        }
        generatePreviewPicture();
        // 更新逐页打印页码和页面数据
        updatePageByPagePrintVector(pageVector, pictures);
        if (imposition == DPrintPreviewWidget::One) {
            // 异步+非并打
            // 异步模式下pictures可以直接按顺序拿取
            for (int i = 0; i < pageVector.size(); ++i) {
                printSinglePageDrawUtil(&painter, translateSize, leftTopPoint, waterMarkImage, pictures.at(i));
                saveImageToFile(i, outPutFileName, suffix, isJpegImage, savedImages);
                savedImages.fill(Qt::white);
            }
        } else {
            // 异步+并打
            int curPageCount = numberUpPrintData->rowCount * numberUpPrintData->columnCount;
            for (int i = 0; i < q_func()->targetPageCount(pageVector.size()); ++i) {
                // 异步下pictures只有需要打印的数据 需要按照pageVector当前的值进行迭代
                numberUpPrintData->previewPictures.clear();
                if (order != DPrintPreviewWidget::Copy) {
                    for (int c = 0; c < curPageCount; ++c) {
                        int index = i * curPageCount + c;
                        if (index + 1 > pictures.length())
                            break;

                        numberUpPrintData->previewPictures.append(qMakePair(index, pictures.at(index)));
                    }
                } else {
                    numberUpPrintData->previewPictures = {curPageCount, qMakePair(i, pictures.at(i))};
                }

                if ((0 == i) || (numberUpPrintData->previewPictures.count() != numberUpPrintData->paintPoints.count()))
                    waterMarkImage = generateWaterMarkImage();

                printMultiPageDrawUtil(&painter, leftTopPoint, waterMarkImage);
                saveImageToFile(i, outPutFileName, suffix, isJpegImage, savedImages);
                savedImages.fill(Qt::white);
            }
        }
    } else {
        if (imposition == DPrintPreviewWidget::One) {
            // 更新逐页打印页码和页面数据
            updatePageByPagePrintVector(pageVector, pictures);
            // 同步+非并打
            // 同步模式下需要按照位置拿取
            for (int i = 0; i < pageVector.size(); ++i) {
                printSinglePageDrawUtil(&painter, translateSize, leftTopPoint, waterMarkImage, pictures[pageVector.at(i) - 1]);
                saveImageToFile(i, outPutFileName, suffix, isJpegImage, savedImages);
                savedImages.fill(Qt::white);
            }
        } else {
            // 同步+并打
            for (int i = 0; i < q_func()->targetPageCount(pageVector.size()); ++i) {
                // 调整当前页码 更新当前页数据
                if (pageRangeMode == DPrintPreviewWidget::CurrentPage) {
                    currentPageNumber = pageVector.at(i);
                } else {
                    currentPageNumber = i + 1;
                }

                // 同步模式下pictures有所有数据，因此可以直接计算
                calculateCurrentNumberPage();
                // 如果当前页面水印数量和内容数量不一致 需要更新水印使其保持一致
                if ((0 == i) || (numberUpPrintData->previewPictures.count() != numberUpPrintData->paintPoints.count()))
                    waterMarkImage = generateWaterMarkImage();

                printMultiPageDrawUtil(&painter, leftTopPoint, waterMarkImage);
                saveImageToFile(i, outPutFileName, suffix, isJpegImage, savedImages);
                savedImages.fill(Qt::white);
            }
        }
    }
}

void DPrintPreviewWidgetPrivate::printSinglePageDrawUtil(QPainter *painter, const QSize &translateSize, const QPointF &leftTop, const QImage &waterImage, const QPicture *picture)
{
    // 绘制原始数据
    painter->save();
    if (scale > 1) {
        // Bug-61709: Qt原因右下页边距在缩放大于100后出现失效问题，这里先用一个临时的解决办法处理
        QImage tmpImage(previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution()).size() * scale, QImage::Format_ARGB32);
        tmpImage.fill(Qt::white);
        QPainter tmpPainter(&tmpImage);
        tmpPainter.scale(scale, scale);
        tmpPainter.drawPicture(0, 0, *picture);

        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        // 将缩放系数设置为1
        painter->resetTransform();
        // 由小到大缩放的时候  图片数据容易失真  这里直接将原始数据绘制到放大后的图片中 然后再进行绘图 数据失真程度较低
        painter->drawImage(leftTop, tmpImage);
    } else {
        painter->drawPicture(leftTop, *picture);
    }
    // 绘制水印
    if (!waterImage.isNull()) {
        painter->resetTransform();
        painter->translate(translateSize.width() / 2, translateSize.height() / 2);
        painter->rotate(waterMark->rotation());

        painter->drawImage(-waterImage.width() / 2, -waterImage.height() / 2, waterImage);
    }

    painter->restore();
}

void DPrintPreviewWidgetPrivate::printMultiPageDrawUtil(QPainter *painter, const QPointF &leftTop, const QImage &waterImage)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    painter->save();
    painter->scale(numberUpPrintData->scaleRatio, numberUpPrintData->scaleRatio);
    if (scale > 1) {
        // Bug-61709: Qt原因右下页边距在缩放大于100后出现失效问题，这里先用一个临时的解决办法处理
        QImage tmpImage(previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution()).size() / numberUpPrintData->scaleRatio, QImage::Format_ARGB32);
        tmpImage.fill(Qt::white);
        QPainter tmpPainter(&tmpImage);

        // 为了保证并打缩放的清晰度 防止先缩放小再缩放大导致图像不清晰的问题 这里直接将并打内容放大 然后在统一缩小到并打大小
        for (int c = 0; c < numberUpPrintData->previewPictures.count(); ++c) {
            QPointF paintPoint = numberUpPrintData->paintPoints.at(c) / numberUpPrintData->scaleRatio;
            const QPicture *pic = numberUpPrintData->previewPictures.at(c).second;
            tmpPainter.drawPicture(paintPoint, *pic);
        }

        painter->drawImage(leftTop / numberUpPrintData->scaleRatio, tmpImage);
    } else {
        for (int c = 0; c < numberUpPrintData->previewPictures.count(); ++c) {
            QPointF paintPoint = numberUpPrintData->paintPoints.at(c) / numberUpPrintData->scaleRatio;
            const QPicture *pic = numberUpPrintData->previewPictures.at(c).second;
            painter->drawPicture(leftTop / numberUpPrintData->scaleRatio + paintPoint, *pic);
        }
    }
    painter->restore();

    // 绘制并打水印 此时不能再设置缩放比
    if (!waterImage.isNull())
        painter->drawImage(leftTop, waterImage);
}

void DPrintPreviewWidgetPrivate::print(bool printAsPicture)
{
    QVector<int> pageVector;
    if (pageRangeMode == DPrintPreviewWidget::CurrentPage)
        pageVector.append(pageRange.at(currentPageNumber - 1));
    else {
        pageVector = pageRange;
    }

    QSize paperSize = previewPrinter->pageLayout().fullRectPixels(previewPrinter->resolution()).size();
    if (printAsPicture) {
        printAsImage(paperSize, pageVector);
    } else {
        QRect pageRect = previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution());
        QPointF leftTopPoint;
        if (scale >= 1.0) {
            leftTopPoint = {0, 0};
        } else {
            leftTopPoint = {pageRect.width() * (1.0 - scale) / (2.0 * scale), pageRect.height() * (1.0 - scale) / (2.0 * scale)};
        }

        if (isAsynPreview) {
            // 异步先获取需要打印的数据
            if (pageRangeMode == DPrintPreviewWidget::CurrentPage) {
                previewPages = requestPages(pageVector.first());
            } else {
                previewPages = pageVector;
            }

            generatePreviewPicture();
            // 更新逐页打印页码和页面数据
            updatePageByPagePrintVector(pageVector, pictures);
            asynPrint(leftTopPoint, pageRect, pageVector);
        } else {
            // 更新逐页打印页码和页面数据
            updatePageByPagePrintVector(pageVector, pictures);
            syncPrint(leftTopPoint, pageRect, pageVector);
        }
    }
}

void DPrintPreviewWidgetPrivate::updatePageByPagePrintVector(QVector<int> &pageVector, QList<const QPicture *> &pictures) const
{
    //逐页打印情况下，手动设置页码和图片处理
    //当拷贝份数不为1时，需要手动插入图片页码
    if (pageCopyCount != 0) {
        if (pageCopyCount != 1) {
            QVector<int> vector = pageVector;
            //异步传输图片，插入拷贝份数的图片使其与页码对齐
            QList<const QPicture *> pic = pictures;
            if (isAsynPreview) {
                for (int i = 0; i <= pic.count() - 1; i++) {
                    for (int j = 1; j < pageCopyCount; j++) {
                        // cppcheck-suppress arrayIndexOutOfBounds
                        pictures.insert(pictures.indexOf(pic.at(i)), pic.at(i));
                    }
                }
            }
            //插入拷贝份数需要打印的页码
            for (int i = 0; i <= vector.count() - 1; i++) {
                for (int j = 1; j < pageCopyCount; j++) {
                    // cppcheck-suppress arrayIndexOutOfBounds
                    pageVector.insert(pageVector.indexOf(vector.at(i)), vector.at(i));
                }
            }
        }
        //逐页打印时，判断当前是否是由后向前打印
        if (!isFirstPage) {
            //异步传输时，使图片的顺序改为逆序
            if (isAsynPreview) {
                QList<const QPicture *> reservepic;
                QList<const QPicture *>::iterator i;
                for (i = pictures.end(); i != pictures.begin(); --i) {
                    reservepic.append(*(i - 1));
                }
                pictures = reservepic;
            }
            //将页码数值按从大到小排序
            std::sort(pageVector.begin(), pageVector.end(), std::greater<int>());
        }
    }
}

void DPrintPreviewWidgetPrivate::setPageRangeAll()
{
    int size = pictures.size();
    if (isAsynPreview)
        size = asynPreviewTotalPage;
    pageRange.clear();
    for (int i = FIRST_PAGE; i <= size; i++) {
        pageRange.append(i);
    }
}

int DPrintPreviewWidgetPrivate::pagesCount()
{
    int pages = pageRange.count();
    if (order == DPrintPreviewWidget::Copy)
        return pages;
    return targetPage(pages);
}

void DPrintPreviewWidgetPrivate::setCurrentPage(int page)
{
    Q_Q(DPrintPreviewWidget);

    int pageCount = pagesCount();
    if (page > pageCount) {
        page = pageCount;
    }
    int preCurrentPage = currentPageNumber;
    setCurrentPageNumber(page);
    Q_EMIT q->currentPageChanged(currentPageNumber);

    if (isAsynPreview) {
        if (PageItem *pi = dynamic_cast<PageItem *>(pages.first()))
            pi->setVisible(true);
        return;
    }

    int currentPage = index2page(currentPageNumber - 1);
    int lastPage = index2page(preCurrentPage - 1);
    if (currentPage < 0)
        return;

    if (lastPage > 0)
        pages.at(lastPage - 1)->setVisible(false);

    if (PageItem *pi = dynamic_cast<PageItem *>(pages.at(currentPage - 1)))
        pi->setVisible(true);

    graphicsView->resetScale(false);
}

int DPrintPreviewWidgetPrivate::targetPage(int page)
{
    int mod = 0;
    switch (imposition) {
    case DPrintPreviewWidget::Imposition::One:
        break;
    case DPrintPreviewWidget::Imposition::OneRowTwoCol:
        mod = page % ONE_ROW_TWO_COL_PAGES;
        page = page / ONE_ROW_TWO_COL_PAGES;
        break;
    case DPrintPreviewWidget::Imposition::TwoRowTwoCol:
        mod = page % TWO_ROW_TWO_COL_PAGES;
        page = page / TWO_ROW_TWO_COL_PAGES;
        break;
    case DPrintPreviewWidget::Imposition::TwoRowThreeCol:
        mod = page % TWO_ROW_THREE_COL_PAGES;
        page = page / TWO_ROW_THREE_COL_PAGES;
        break;
    case DPrintPreviewWidget::Imposition::ThreeRowThreeCol:
        mod = page % THREE_ROW_THREE_COL_PAGES;
        page = page / THREE_ROW_THREE_COL_PAGES;
        break;
    case DPrintPreviewWidget::Imposition::FourRowFourCol:
        mod = page % FOUR_ROW_FOUR_COL_PAGES;
        page = page / FOUR_ROW_FOUR_COL_PAGES;
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

#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)

void DPrintPreviewWidgetPrivate::impositionPages()
{
    setCurrentPage(FIRST_PAGE);
    return;
}
#endif

int DPrintPreviewWidgetPrivate::impositionPages(DPrintPreviewWidget::Imposition im)
{
    Q_UNUSED(im)
    switch (imposition) {
    case DPrintPreviewWidget::One:
        return ONE_PAGE;
    case DPrintPreviewWidget::OneRowTwoCol:
        return ONE_ROW_TWO_COL_PAGES;
    case DPrintPreviewWidget::TwoRowTwoCol:
        return TWO_ROW_TWO_COL_PAGES;
    case DPrintPreviewWidget::TwoRowThreeCol:
        return TWO_ROW_THREE_COL_PAGES;
    case DPrintPreviewWidget::ThreeRowThreeCol:
        return THREE_ROW_THREE_COL_PAGES;
    case DPrintPreviewWidget::FourRowFourCol:
        return FOUR_ROW_FOUR_COL_PAGES;
    }

    qWarning() << "DPrintPreviewWidget::Imposition enumeration value is out of range";
    return -1;
}

QImage DPrintPreviewWidgetPrivate::generateWaterMarkImage() const
{
    auto drawSingleWaterMarkImage = [ = ]() -> QImage {
        QRectF itemMaxRect = waterMark->itemMaxPolygon().boundingRect();
        QImage originImage(itemMaxRect.size().toSize(), QImage::Format_ARGB32);
        originImage.fill(Qt::transparent);

        QPainter picPainter;
        picPainter.begin(&originImage);
        // 由于painter绘制此image中的位置是从0,0点开始的 但预览图的位置左移（上移）过 因此此处的绘图原点和预览图保持一致
        picPainter.translate(-itemMaxRect.topLeft());
        if (imposition == DPrintPreviewWidget::One)
        {
            picPainter.setOpacity(waterMark->opacity());
            waterMark->updatePicture(&picPainter, false);
        } else
        {
            if (!numberUpPrintData->waterList.isEmpty()) {
                WaterMark *wm = numberUpPrintData->waterList.first();
                wm->setBoundingRect(previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution()));
                wm->setNumberUpScale(1);
                picPainter.setOpacity(wm->opacity());
                wm->updatePicture(&picPainter, false);
            }
        }
        picPainter.end();

        return originImage;
    };

    QImage waterMarkImage = drawSingleWaterMarkImage();
    if (imposition == DPrintPreviewWidget::One) {
        return waterMarkImage;
    } else {
        const QRectF &pageRect = previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution());
        qreal rotation = numberUpPrintData->waterList.isEmpty() ? 0 : numberUpPrintData->waterList.first()->rotation();

        // 绘制水印
        QImage singleWaterImage(pageRect.size().toSize(), QImage::Format_ARGB32);
        singleWaterImage.fill(Qt::transparent);
        QPainter sp;
        sp.begin(&singleWaterImage);
        sp.setRenderHint(QPainter::SmoothPixmapTransform);
        sp.translate(pageRect.width() / 2, pageRect.height() / 2);
        sp.rotate(rotation);
        sp.drawImage(-waterMarkImage.width() / 2, -waterMarkImage.height() / 2, waterMarkImage);
        sp.end();

        QImage totalWaterImage(pageRect.size().toSize(), QImage::Format_ARGB32);
        totalWaterImage.fill(Qt::transparent);

        QPainter tp;
        tp.begin(&totalWaterImage);
        tp.setRenderHint(QPainter::SmoothPixmapTransform);
        tp.scale(numberUpPrintData->scaleRatio, numberUpPrintData->scaleRatio);

        for (int c = 0; c < numberUpPrintData->previewPictures.count(); ++c) {
            QPointF paintPoint = numberUpPrintData->paintPoints.at(c) / numberUpPrintData->scaleRatio;
            tp.drawImage(paintPoint, singleWaterImage);
        }
        tp.end();
        return totalWaterImage;
    }
}

PrintOptions DPrintPreviewWidgetPrivate::printerOptions()
{
    PrintOptions options;

    options.append(QPair<QByteArray, QByteArray>(QStringLiteral("media").toLocal8Bit(), QPageSize(QPageSize::PageSizeId(previewPrinter->pageLayout().pageSize().id())).key().toLocal8Bit()));
    options.append(QPair<QByteArray, QByteArray>(QStringLiteral("copies").toLocal8Bit(), QString::number(previewPrinter->copyCount()).toLocal8Bit()));
    options.append(QPair<QByteArray, QByteArray>(QStringLiteral("fit-to-page").toLocal8Bit(), QStringLiteral("true").toLocal8Bit()));

    if (pageRangeMode != DPrintPreviewWidget::AllPage) {
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
    }

    switch (previewPrinter->duplex()) {
    case QPrinter::DuplexNone:
        options.append(QPair<QByteArray, QByteArray>(QStringLiteral("sides").toLocal8Bit(), QStringLiteral("one-sided").toLocal8Bit()));
        break;
    case QPrinter::DuplexAuto:
        if (previewPrinter->pageLayout().orientation() == QPageLayout::Portrait) {
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
        Q_Q(DPrintPreviewWidget);
        QByteArray colorModel = q->printerColorModel();
        options.append(QPair<QByteArray, QByteArray>(QStringLiteral("ColorModel").toLocal8Bit(), colorModel.isEmpty() ? QByteArrayLiteral("RGB") : colorModel));
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

void DPrintPreviewWidgetPrivate::calculateNumberPageScale()
{
    numberUpPrintData->resetData();

    QRectF pageRect = previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution());
    switch (imposition) {
    case DPrintPreviewWidget::OneRowTwoCol:
        numberUpPrintData->rowCount = 1;
        numberUpPrintData->columnCount = 2;
        numberUpPrintData->scaleRatio = 1 / (NUMBERUP_SCALE_RATIO * numberUpPrintData->columnCount - NUMBERUP_SPACE_SCALE_RATIO);
        numberUpPrintData->pageStartPoint = QPointF(0, (1 - (NUMBERUP_SCALE_RATIO * numberUpPrintData->rowCount - NUMBERUP_SPACE_SCALE_RATIO) * numberUpPrintData->scaleRatio) * pageRect.height() / 2);
        break;
    case DPrintPreviewWidget::TwoRowTwoCol:
        numberUpPrintData->rowCount = 2;
        numberUpPrintData->columnCount = 2;
        numberUpPrintData->scaleRatio = 1 / (NUMBERUP_SCALE_RATIO * numberUpPrintData->columnCount - NUMBERUP_SPACE_SCALE_RATIO);
        break;
    case DPrintPreviewWidget::TwoRowThreeCol:
        numberUpPrintData->rowCount = 2;
        numberUpPrintData->columnCount = 3;
        numberUpPrintData->scaleRatio = 1 / (NUMBERUP_SCALE_RATIO * numberUpPrintData->columnCount - NUMBERUP_SPACE_SCALE_RATIO);
        numberUpPrintData->pageStartPoint = QPointF(0, (1 - (NUMBERUP_SCALE_RATIO * numberUpPrintData->rowCount - NUMBERUP_SPACE_SCALE_RATIO) * numberUpPrintData->scaleRatio) * pageRect.height() / 2);
        break;
    case DPrintPreviewWidget::ThreeRowThreeCol:
        numberUpPrintData->rowCount = 3;
        numberUpPrintData->columnCount = 3;
        numberUpPrintData->scaleRatio = 1 / (NUMBERUP_SCALE_RATIO * numberUpPrintData->columnCount - NUMBERUP_SPACE_SCALE_RATIO);
        break;
    case DPrintPreviewWidget::FourRowFourCol:
        numberUpPrintData->rowCount = 4;
        numberUpPrintData->columnCount = 4;
        numberUpPrintData->scaleRatio = 1 / (NUMBERUP_SCALE_RATIO * numberUpPrintData->columnCount - NUMBERUP_SPACE_SCALE_RATIO);
        break;
    default:
        break;
    }
}

void DPrintPreviewWidgetPrivate::calculateNumberPagePosition()
{
    numberUpPrintData->paintPoints.clear();
    // 由于页面内容的缩放走的自己的逻辑 这里更新坐标的时候仅使用缩放比为1进行更新
    numberUpPrintData->paintPoints = numberUpPrintData->updatePositions(1);
}

void DPrintPreviewWidgetPrivate::calculateNumberUpPage()
{
    if (!numberUpPrintData)
        numberUpPrintData = new NumberUpData(this);

    calculateNumberPageScale();
    calculateNumberPagePosition();
}

void DPrintPreviewWidgetPrivate::calculateCurrentNumberPage()
{
    numberUpPrintData->previewPictures.clear();
    int count = numberUpPrintData->rowCount * numberUpPrintData->columnCount;

    // 页码和索引值
    QVector<QPair<int, int>> pageIndexVector;
    if (isAsynPreview) {
        // 异步模式下 由于pictures里仅有需要预览的数据 只是遍历索引就够了
        if (order == DPrintPreviewWidget::Copy) {
            // 获取到pageRange中当前页面对应索引的页码
            int page = index2page(currentPageNumber - 1);
            // 由于拷贝模式下，仅拿取第一页数据这里索引的pictures值就是0
            int index = 0;
            auto pair = qMakePair(page, index);
            pageIndexVector = QVector<QPair<int, int>>(count, pair);
        } else {
            // 其他情况下 传入当前页码和pictures中存在的数据索引
            for (int c = 0; c < count; ++c) {
                if (c + 1 > pictures.length())
                    break;

                int page = index2page((currentPageNumber - 1) * count + c);
                if (page == -1)
                    break;

                // 这里直接传入当前索引值，因为异步模式下pictures里仅有<=count的数据
                auto pair = qMakePair(page, c);
                pageIndexVector.append(pair);
            }
        }
    } else {
        // 非异步模式下 从pageRange里拿取pictures中的数据
        if (order == DPrintPreviewWidget::Copy) {
            // copy模式下
            int page = index2page(currentPageNumber - 1);
            if (page < 0)
                return;
            auto pair = qMakePair(page, page - 1);
            pageIndexVector = QVector<QPair<int, int>>(count, pair);
        } else {
            // 非拷贝模式下 传入对应页面
            for (int c = 0; c < count; ++c) {
                int page = index2page((currentPageNumber - 1) * count + c);
                if (page == -1)
                    break;

                auto pair = qMakePair(page, page - 1);
                pageIndexVector.append(pair);
            }
        }
    }

    Q_FOREACH (auto pageIndex, pageIndexVector) {
        QPair<int, const QPicture *> picPair(pageIndex.first, pictures.at(pageIndex.second));
        numberUpPrintData->previewPictures.append(picPair);
    }
}

QByteArray DPrintPreviewWidgetPrivate::foundColorModelByCups() const
{
    const auto parts = previewPrinter->printerName().split(QLatin1Char('/'));
    const auto printerOriginalName = parts.at(0);

    QByteArray m_cupsInstance;
    if (parts.size() > 1)
        m_cupsInstance = parts.at(1).toUtf8();

    QLibrary cupsLibrary("cups", "2");
    if (!cupsLibrary.isLoaded()) {
        if (!cupsLibrary.load()) {
            qWarning() << "Cups not found";
            return {};
        }
    }

    cups_dest_t *(*cupsGetNamedDest)(http_t * http, const char *name, const char *instance) = nullptr;
    cupsGetNamedDest = reinterpret_cast<decltype(cupsGetNamedDest)>(cupsLibrary.resolve("cupsGetNamedDest"));

    if (!cupsGetNamedDest) {
        qWarning() << "cupsGetNamedDest Function load failed.";
        return {};
    }

    void (*cupsFreeDests)(int num_dests, cups_dest_t *dests) = nullptr;
    cupsFreeDests = reinterpret_cast<decltype(cupsFreeDests)>(cupsLibrary.resolve("cupsFreeDests"));

    if (!cupsFreeDests) {
        qWarning() << "cupsFreeDests Function load failed.";
        return {};
    }

    // 根据打印机名称获取cup实例 用于读取ppd文件
    cups_dest_t *m_cupsDest = cupsGetNamedDest(CUPS_HTTP_DEFAULT, printerOriginalName.toLocal8Bit(), m_cupsInstance.isNull() ? nullptr : m_cupsInstance.constData());

    if (m_cupsDest) {
        ppd_file_t *m_ppd = nullptr;
        const char *(*cupsGetPPD)(const char *name) = nullptr;
        cupsGetPPD = reinterpret_cast<decltype(cupsGetPPD)>(cupsLibrary.resolve("cupsGetPPD"));

        if (!cupsGetPPD) {
            qWarning() << "cupsGetPPD Function load failed.";
            cupsFreeDests(1, m_cupsDest);
            return {};
        }

        // 获取对应打印机的ppd文件指针
        const char *ppdFile = cupsGetPPD(printerOriginalName.toLocal8Bit());

        if (ppdFile) {
            ppd_file_t *(*ppdOpenFile)(const char *filename) = nullptr;
            ppdOpenFile = reinterpret_cast<decltype(ppdOpenFile)>(cupsLibrary.resolve("ppdOpenFile"));

            if (!ppdOpenFile) {
                qWarning() << "ppdOpenFile Function load failed.";
                cupsFreeDests(1, m_cupsDest);
                return {};
            }

            // 打开ppd文件
            m_ppd = ppdOpenFile(ppdFile);
            unlink(ppdFile);
        }

        if (m_ppd) {
            void (*ppdMarkDefaults)(ppd_file_t * ppd) = nullptr;
            int (*cupsMarkOptions)(ppd_file_t * ppd, int num_options, cups_option_t *options) = nullptr;
            int (*ppdLocalize)(ppd_file_t * ppd) = nullptr;
            ppd_option_t *(*ppdFindOption)(ppd_file_t * ppd, const char *keyword) = nullptr;
            ppdMarkDefaults = reinterpret_cast<decltype(ppdMarkDefaults)>(cupsLibrary.resolve("ppdMarkDefaults"));
            cupsMarkOptions = reinterpret_cast<decltype(cupsMarkOptions)>(cupsLibrary.resolve("cupsMarkOptions"));
            ppdLocalize = reinterpret_cast<decltype(ppdLocalize)>(cupsLibrary.resolve("ppdLocalize"));
            ppdFindOption = reinterpret_cast<decltype(ppdFindOption)>(cupsLibrary.resolve("ppdFindOption"));

            if (!ppdMarkDefaults || !cupsMarkOptions || !ppdLocalize || !ppdFindOption) {
                qWarning() << "ppdMarkDefaults, cupsMarkOptions, ppdLocalize, ppdFindOption function load failed.";
                cupsFreeDests(1, m_cupsDest);
                return {};
            }

            ppdMarkDefaults(m_ppd);
            cupsMarkOptions(m_ppd, m_cupsDest->num_options, m_cupsDest->options);
            ppdLocalize(m_ppd);

            // 从ppd文件中找到对应属性
            ppd_option_t *colorModel = ppdFindOption(m_ppd, "ColorModel");

            if (colorModel) {
                for (int i = 0; i < colorModel->num_choices; ++i) {
                    ppd_choice_t *choice = colorModel->choices + i;

                    if (QString(choice->choice).startsWith("gray", Qt::CaseInsensitive)) {
                        continue;
                    } else {
                        // 寻找ColorModel属性 获取到时返回支持的颜色
                        QByteArray colorModel(choice->choice);
                        cupsFreeDests(1, m_cupsDest);
                        return colorModel;
                    }
                }
            }
        } else {
            cupsFreeDests(1, m_cupsDest);
            m_cupsDest = nullptr;
            m_ppd = nullptr;
        }
    }

    return {};
}

void DPrintPreviewWidgetPrivate::displayWaterMarkItem()
{
    // 如果当前预览页面和水印页面不一致（切换页码时）或者重绘制时（页面发生变化） 重新生成水印效果
    if ((numberUpPrintData->previewPictures.count() == numberUpPrintData->waterList.count())
            && !numberUpPrintData->needRecreateWater) {
        // 不重新生成水印但需要更新当前水印的位置和缩放等属性
        numberUpPrintData->setWaterMarksScale(scale);
        return;
    }

    numberUpPrintData->needRecreateWater = false;
    QRectF pageRect = previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution());

    // 拷贝旧的水印属性，防止水印重新添加后原有水印无法设置到现有内容中
    numberUpPrintData->copyWaterMarkProperties();

    // 释放旧的序号item
    if (numberUpPrintData->numberItem) {
        scene->removeItem(numberUpPrintData->numberItem);
        delete numberUpPrintData->numberItem;
        numberUpPrintData->numberItem = nullptr;
    }

    // 释放水印内容 释放父类时会将其子类一起释放
    if (numberUpPrintData->waterParentItem) {
        scene->removeItem(numberUpPrintData->waterParentItem);
        delete numberUpPrintData->waterParentItem;
        numberUpPrintData->waterParentItem = nullptr;
    }

    numberUpPrintData->waterList.clear();

    // 水印父类 主要用于限定大小和位置，防止缩放后水印超出界面显示
    numberUpPrintData->waterParentItem = new QGraphicsRectItem(pageRect);
    numberUpPrintData->waterParentItem->setZValue(1);
    numberUpPrintData->waterParentItem->setPen(Qt::NoPen);
    numberUpPrintData->waterParentItem->setBrush(Qt::NoBrush);
    scene->addItem(numberUpPrintData->waterParentItem);

    QVector<int> nVector;
    QVector<QPointF> pVector;
    // 按照位置添加水印 这里没有设置水印的boundingrect 会在下方的设置缩放时一起设置
    for (int c = 0; c < numberUpPrintData->previewPictures.count(); ++c) {
        WaterMark *wm = new WaterMark(numberUpPrintData->waterParentItem);
        wm->setZValue(1);
        wm->setNumberUpScale(numberUpPrintData->scaleRatio);
        wm->setParentItem(numberUpPrintData->waterParentItem);
        numberUpPrintData->setWaterMarkOriginProperties(wm);
        numberUpPrintData->waterList.append(wm);
        pVector.append((numberUpPrintData->paintPoints.at(c) + QPointF(pageRect.width() * numberUpPrintData->scaleRatio, 0)));
        nVector.append(numberUpPrintData->previewPictures.at(c).first);
    }

    // 更新缩放后的水印坐标 和 序号item坐标
    numberUpPrintData->setWaterMarksScale(scale);
    // 新建序号item主要用于序号的显示
    numberUpPrintData->numberItem = new NumberUpData::NumberItem(nVector, pVector, pageRect.toRect());
    numberUpPrintData->numberItem->setZValue(2);
    scene->addItem(numberUpPrintData->numberItem);
}

void DPrintPreviewWidgetPrivate::updateNumberUpContent()
{
    // 重新计算当前页面需要显示的pictures
    calculateCurrentNumberPage();
    //重新计算水印各个item
    displayWaterMarkItem();
    // 更新当前PageItem的内容 如果是灰色的页面 需要先更新其灰色内容
    int currentPage = index2page(currentPageNumber - 1);

    if (currentPage != -1) {
        int index = isAsynPreview ? 0 : currentPage - 1;
        PageItem *pi = dynamic_cast<PageItem *>(pages.at(index));
        if (colorMode == DPrinter::GrayScale || !pi->isVisible())
            pi->setVisible(true);

        pi->update();
    }

    // 调整序号角标
    QVector<int> nVector;
    for (auto &i : std::as_const(numberUpPrintData->previewPictures)) {
        nVector.append(i.first);
    }

    // 调整序号坐标显示位置（纸张大小发生改变时）
    QVector<QPointF> paintPoints;
    for (auto &p : std::as_const(numberUpPrintData->paintPoints)) {
        paintPoints.append(p + QPointF(previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution()).width() * numberUpPrintData->scaleRatio, 0));
    }

    numberUpPrintData->numberItem->setRect(previewPrinter->pageLayout().paintRectPixels(previewPrinter->resolution()));
    numberUpPrintData->numberItem->setNumberPositon(paintPoints);
    numberUpPrintData->numberItem->setPageNumbers(nVector);
    numberUpPrintData->numberItem->update();
}

void DPrintPreviewWidgetPrivate::releaseImpositionData()
{
    waterMark->setVisible(true);
    // 重新拷贝并打水印数据并设置到非并打模式下
    numberUpPrintData->copyWaterMarkProperties();
    numberUpPrintData->setWaterMarkOriginProperties(waterMark);
    // 释放并打内容并置空指针
    delete numberUpPrintData;
    numberUpPrintData = nullptr;
}

QVector<int> DPrintPreviewWidgetPrivate::requestPages(int page)
{
    QVector<int> pagesVector;
    int pageCount = impositionPages(imposition); //小页面个数
    int rangeCount = pageRange.count();
    if (page > pagesCount()) // 超出页面范围
        return pagesVector;
    if (order == DPrintPreviewWidget::Copy || imposition == DPrintPreviewWidget::One) {
        pagesVector.append(pageRange.at(page - 1));
        return pagesVector;
    }
    if (page == targetPage(rangeCount) && rangeCount % pageCount) { // 并打非重复，最后一页存在空白
        int count = rangeCount % pageCount;
        for (int i = 0; i < count; i++)
            pagesVector.append(pageRange.at((page - 1) * pageCount + i));
        return pagesVector;
    }

    for (int i = 0; i < pageCount; i++) {
        pagesVector.append(pageRange.at((page - 1) * pageCount + i));
    }
    return pagesVector;
}

/*!
    \class Dtk::Widget::DPrintPreviewWidget
    \inmodule dtkwidget

    打印预览对话框界面中的视图预览窗口，用于预览来自第三方应用中的
    数据。通常情况下应用无需调用该类中的接口，只需要在 DPrintPreviewDialog
    中处理即可。除此之外，该类还管理了将预览数据输出到打印机的功能，例如，另存为
    PDF、另存为图片等功能。

    \sa Dtk::Widget::DPrintPreviewDialog
*/

/*!
  \brief 构造一个 DPrintPreviewWidget。
  
  \a printer 打印机
  \a parent 父控件
 */
DPrintPreviewWidget::DPrintPreviewWidget(DPrinter *printer, QWidget *parent)
    : DFrame(*new DPrintPreviewWidgetPrivate(this))
{
    Q_D(DPrintPreviewWidget);
    d->previewPrinter = printer;
    d->init();
}

DPrintPreviewWidget::~DPrintPreviewWidget()
{
    Q_D(DPrintPreviewWidget);

    d->updateTimer.stop();
    delete d->numberUpPrintData;
}

/*!
  \brief 设置打印预览widget是否可见.
  \reimp
  
  \a visible 是否可见
 */
void DPrintPreviewWidget::setVisible(bool visible)
{
    QWidget::setVisible(visible);
    // updatePreview() function move into DPrintPreviewDialog show event.
}

/*!
  \brief 设置打印预览页面范围为所有页.
 */
void DPrintPreviewWidget::setPageRangeALL()
{
    Q_D(DPrintPreviewWidget);
    d->setPageRangeAll();
    if (!d->pageRange.isEmpty())
        d->setCurrentPage(FIRST_PAGE);
}

/*!
  \brief 设置是否需要重新生成页面内容.

  \a generate 是否需要重新生成页面内容
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
void DPrintPreviewWidget::setReGenerate(bool generate)
{
    reviewChange(generate);
}
#endif

/*!
  \brief 设置页面选择范围模式。
  
  \a mode 页面选择范围模式，AllPage所有页，CurrentPage当前页，SelectPage选择页
 */
void DPrintPreviewWidget::setPageRangeMode(PageRange mode)
{
    Q_D(DPrintPreviewWidget);
    d->pageRangeMode = mode;
}

/*!
  \brief 获取页面选择范围模式.

  \return 返回页码返回的模式.
 */
DPrintPreviewWidget::PageRange DPrintPreviewWidget::pageRangeMode()
{
    Q_D(DPrintPreviewWidget);
    return d->pageRangeMode;
}

/*!
  \brief 预览是否改变，当预览改变时需要调用.

  \a generate 预览是否发生改变.
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
void DPrintPreviewWidget::reviewChange(bool generate)
{
    Q_D(DPrintPreviewWidget);
    d->reviewChanged = generate;
}
#endif

/*!
  \brief 设置预览页面范围。
  
  \a rangePages 页码Vector
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
    Q_EMIT pagesCountChanged(rangePages.count());

    setCurrentPage(d->currentPageNumber);
}

/*!
  \brief 设置预览页面范围。
  
  \a from 起始页码
  \a to 终止页码
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
  \brief 获取预览总页数.

  \return 返回预览总页数.
 */
int DPrintPreviewWidget::pagesCount()
{
    Q_D(DPrintPreviewWidget);
    return d->pagesCount();
}

/*!
  \brief 获取当前页的页码.

  \return 返回当前页的页码.
 */
int DPrintPreviewWidget::currentPage()
{
    Q_D(DPrintPreviewWidget);
    return d->index2page(d->currentPageNumber - 1);
}

/*!
  \brief 获取是否可翻页.

  \return 可翻页返回true,不可翻页返回false.
 */
bool DPrintPreviewWidget::turnPageAble()
{
    Q_D(DPrintPreviewWidget);
    return pagesCount() > 1;
}

/*!
  \brief 设置色彩模式。
  
  \a colorMode 色彩模式
 */
void DPrintPreviewWidget::setColorMode(const DPrinter::ColorMode &colorMode)
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
  \brief 设置页面方向。
  
  \a pageOrientation 页面方向
 */
void DPrintPreviewWidget::setOrientation(const DPrinter::Orientation &pageOrientation)
{
    Q_D(DPrintPreviewWidget);
    d->previewPrinter->setPageOrientation(static_cast<QPageLayout::Orientation>(pageOrientation));
    updatePreview();
}

/*!
  \brief 获取色彩模式。
 */
DPrinter::ColorMode DPrintPreviewWidget::getColorMode()
{
    Q_D(DPrintPreviewWidget);
    return d->colorMode;
}

/*!
  \brief 设置页面缩放。
  
  \a scale 缩放大小
 */
void DPrintPreviewWidget::setScale(qreal scale)
{
    Q_D(DPrintPreviewWidget);
    d->scale = scale;

    if (d->numberUpPrintData)
        d->numberUpPrintData->setWaterMarksScale(scale);
}

/*!
  \brief 获取缩放大小。
 */
qreal DPrintPreviewWidget::getScale() const
{
    D_DC(DPrintPreviewWidget);
    return d->scale;
}

/*!
  \brief 刷新预览页面。
 */
void DPrintPreviewWidget::updateView()
{
    Q_D(DPrintPreviewWidget);
    if (d->currentPageNumber < 0 || d->currentPageNumber > targetPageCount(d->pageRange.count()) || d->pages.empty())
        return;

    if (d->isAsynPreview) {
        d->pages.first()->update();
    } else {
        int lastPage = d->index2page(d->currentPageNumber - 1);
        if (lastPage > 0)
            d->pages.at(lastPage - 1)->update();
    }

    d->graphicsView->resetScale(false);
}

/*!
  \brief 强制刷新水印效果
 */
void DPrintPreviewWidget::updateWaterMark()
{
    Q_D(DPrintPreviewWidget);

    if (d->refreshMode == DPrintPreviewWidgetPrivate::RefreshImmediately) {
        if (imposition() == DPrintPreviewWidget::One) {
            d->waterMark->update();
        } else {
            d->numberUpPrintData->updateWaterMarks();
        }
    }
}

/*!
  \brief 水印刷新开始，主要用于同时多次调用水印属性改变的接口时，只刷新一次
 */
void DPrintPreviewWidget::refreshBegin()
{
    Q_D(DPrintPreviewWidget);

    d->refreshMode = DPrintPreviewWidgetPrivate::RefreshDelay;
}

/*!
  \brief 水印刷新结束，刷新水印效果
 */
void DPrintPreviewWidget::refreshEnd()
{
    Q_D(DPrintPreviewWidget);

    d->refreshMode = DPrintPreviewWidgetPrivate::RefreshImmediately;
    updateWaterMark();
}

/*!
  \brief 设置水印类型。
  
  \a type 水印类型
 */
void DPrintPreviewWidget::setWaterMarkType(int type)
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setType(static_cast<WaterMark::Type>(type));
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setType(static_cast<WaterMark::Type>(type));
        });
    }

    updateWaterMark();
}

/*!
  \brief 设置水印图片。
  
  \a image 水印图片
 */
void DPrintPreviewWidget::setWaterMargImage(const QImage &image)
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setImage(image);
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setImage(image);
        });
    }

    updateWaterMark();
}

/*!
  \brief 设置水印旋转角度。
  
  \a rotate 水印旋转角度
 */
void DPrintPreviewWidget::setWaterMarkRotate(qreal rotate)
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setRotation(rotate);
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setRotation(rotate);
        });
    }
}

/*!
  \brief 设置水印缩放大小。
  
  \a scale 水印缩放大小
 */
void DPrintPreviewWidget::setWaterMarkScale(qreal scale)
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setScaleFactor(scale);
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setScaleFactor(scale);
        });
    }

    updateWaterMark();
}

/*!
  \brief 设置水印透明度。
  
  \a opacity 水印透明度
 */
void DPrintPreviewWidget::setWaterMarkOpacity(qreal opacity)
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setOpacity(opacity);
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setOpacity(opacity);
        });
    }
}

/*!
  \brief 设置“绝密”文字水印。
 */
void DPrintPreviewWidget::setConfidentialWaterMark()
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setText(qApp->translate("DPrintPreviewWidget", "Confidential"));
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setText(qApp->translate("DPrintPreviewWidget", "Confidential"));
        });
    }

    updateWaterMark();
}

/*!
  \brief 设置“草稿”文字水印。
 */
void DPrintPreviewWidget::setDraftWaterMark()
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setText(qApp->translate("DPrintPreviewWidget", "Draft"));
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setText(qApp->translate("DPrintPreviewWidget", "Draft"));
        });
    }

    updateWaterMark();
}

/*!
  \brief 设置“样本”文字水印。
 */
void DPrintPreviewWidget::setSampleWaterMark()
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setText(qApp->translate("DPrintPreviewWidget", "Sample"));
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setText(qApp->translate("DPrintPreviewWidget", "Sample"));
        });
    }

    updateWaterMark();
}

/*!
  \brief 设置自定义文字水印。
  
  \a text 自定义文字
 */
void DPrintPreviewWidget::setCustomWaterMark(const QString &text)
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setText(text);
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setText(text);
        });
    }

    updateWaterMark();
}

/*!
  \brief 设置文字水印的文字内容。
  
  \a text 文字水印的文字内容
 */
void DPrintPreviewWidget::setTextWaterMark(const QString &text)
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setText(text);
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setText(text);
        });
    }

    updateWaterMark();
}

/*!
  \brief 设置文字水印的字体。
  
  \a font 文字水印的字体
 */
void DPrintPreviewWidget::setWaterMarkFont(const QFont &font)
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setFont(font);
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setFont(font);
        });
    }

    updateWaterMark();
}

/*!
  \brief 获取文字水印的颜色。

  \return 文字水印的颜色
 */
QColor DPrintPreviewWidget::waterMarkColor() const
{
    Q_D(const DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        return d->waterMark->getColor();
    } else {
        return d->numberUpPrintData->waterProperty->color;
    }
}

/*!
  \brief 设置文字水印的颜色。
  
  \a color 文字水印的颜色
 */
void DPrintPreviewWidget::setWaterMarkColor(const QColor &color)
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setColor(color);
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setColor(color);
        });
    }

    updateWaterMark();
}

/*!
  \brief 设置水印的布局。
  
  \a layout 水印的布局，Center居中，Tiled平铺
 */
void DPrintPreviewWidget::setWaterMarkLayout(int layout)
{
    Q_D(DPrintPreviewWidget);

    if (imposition() == DPrintPreviewWidget::One) {
        d->waterMark->setLayoutType(static_cast<WaterMark::Layout>(layout));
    } else {
        d->numberUpPrintData->setWaterMarkProperty([ = ](WaterMark * item) {
            item->setLayoutType(static_cast<WaterMark::Layout>(layout));
        });
    }

    updateWaterMark();
}

/*!
  \brief 设置并打的模式。
  
  \a im 并打的模式
 */
void DPrintPreviewWidget::setImposition(Imposition im)
{
    Q_D(DPrintPreviewWidget);
    if (im == d->imposition)
        return;

    d->imposition = im;
    if (d->isAsynPreview) {
        d->setCurrentPageNumber(FIRST_PAGE);
        d->generatePreview();
        if (im != DPrintPreviewWidget::One) {
            d->updateNumberUpContent();
            d->waterMark->setVisible(false);
        }
        d->setCurrentPage(FIRST_PAGE); // 显示页面
    } else {
        int lastPage = d->index2page(d->currentPageNumber - 1);
        if (im == DPrintPreviewWidget::One) {
            if (lastPage > 0)
                d->pages.at(lastPage - 1)->setVisible(false);
            d->currentPageNumber = FIRST_PAGE;
            d->setCurrentPage(d->currentPageNumber);
        } else {
            // 将上一个页面内容隐藏
            if (lastPage > 0) {
                d->pages.at(lastPage - 1)->setVisible(false);
            }
            d->setCurrentPageNumber(FIRST_PAGE);
            d->calculateNumberUpPage();
            d->updateNumberUpContent();
            d->setCurrentPage(FIRST_PAGE);
        }
        Q_EMIT pagesCountChanged(d->pageRange.count());
    }

    if (im == DPrintPreviewWidget::One) {
        d->releaseImpositionData();
    }
}

DPrintPreviewWidget::Imposition DPrintPreviewWidget::imposition() const
{
    D_DC(DPrintPreviewWidget);
    return d->imposition;
}

/*!
  \brief 设置页面内并打的顺序。
  
  \a order 页面内并打的顺序
 */
void DPrintPreviewWidget::setOrder(Order order)
{
    Q_D(DPrintPreviewWidget);
    if (d->order == order)
        return;
    int totalPage;
    int min;
    if (d->isAsynPreview) {
        if (d->order == Order::Copy && order != Order::Copy) {
            d->order = order;
            totalPage = d->pagesCount();
            min = qMin(d->currentPageNumber, totalPage);
            d->setCurrentPageNumber(min);
            Q_EMIT currentPageChanged(min);
        } else {
            d->order = order;
            totalPage = d->pagesCount();
        }

        d->generatePreview();
        Q_EMIT pagesCountChanged(d->pageRange.count());
    } else {
        d->order = order;
        totalPage = pagesCount();
        Q_EMIT pagesCountChanged(d->pageRange.count());

        min = qMin(d->currentPageNumber, totalPage);
        int lastPage = d->index2page(d->currentPageNumber - 1);
        if (lastPage > -1)
            d->pages.at(lastPage - 1)->setVisible(false);
        d->setCurrentPageNumber(min);
        Q_EMIT currentPageChanged(min);
        d->calculateNumberPagePosition();
        d->updateNumberUpContent();
    }
}

DPrintPreviewWidget::Order DPrintPreviewWidget::order() const
{
    D_DC(DPrintPreviewWidget);
    return d->order;
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

void DPrintPreviewWidget::isPageByPage(int pageCopy, bool isFirst)
{
    Q_D(DPrintPreviewWidget);
    d->pageCopyCount = pageCopy;
    d->isFirstPage = isFirst;
}

int DPrintPreviewWidget::targetPageCount(int pageCount)
{
    Q_D(DPrintPreviewWidget);

    if (d->order == DPrintPreviewWidget::Copy)
        return pageCount;
    return d->targetPage(pageCount);
}

int DPrintPreviewWidget::originPageCount()
{
    Q_D(DPrintPreviewWidget);
    if (d->isAsynPreview)
        return d->asynPreviewTotalPage;

    return d->pictures.count();
}

QByteArray DPrintPreviewWidget::printerColorModel() const
{
    D_DC(DPrintPreviewWidget);

    return d->foundColorModelByCups();
}

/*!
  \brief 刷新预览。
 */
void DPrintPreviewWidget::updatePreview()
{
    Q_D(DPrintPreviewWidget);

    if (!d->updateTimer.isActive())
        d->updateTimer.start(0, this);
}

/*!
  \brief 前翻一页。
 */
void DPrintPreviewWidget::turnFront()
{
    Q_D(DPrintPreviewWidget);
    if (d->currentPageNumber < 2)
        return;
    setCurrentPage(d->currentPageNumber - 1);
}

/*!
  \brief 后翻一页。
 */
void DPrintPreviewWidget::turnBack()
{
    Q_D(DPrintPreviewWidget);
    if (d->currentPageNumber >= d->pagesCount())
        return;
    setCurrentPage(d->currentPageNumber + 1);
}

/*!
  \brief 第一页。
 */
void DPrintPreviewWidget::turnBegin()
{
    Q_D(DPrintPreviewWidget);
    if (d->pageRange.isEmpty())
        return;
    setCurrentPage(FIRST_PAGE);
}

/*!
  \brief 最后一页。
 */
void DPrintPreviewWidget::turnEnd()
{
    Q_D(DPrintPreviewWidget);
    if (d->pageRange.isEmpty())
        return;
    setCurrentPage(pagesCount());
}

/*!
  \brief 设置当前页。
  
  \a page 当前页
 */
void DPrintPreviewWidget::setCurrentPage(int page)
{
    Q_D(DPrintPreviewWidget);

    if (page > d->pagesCount())
        page = d->pagesCount();
    if (!d->isAsynPreview) {
        int lastPage = d->index2page(d->currentPageNumber - 1);
        if (lastPage > 0)
            d->pages.at(lastPage - 1)->setVisible(false);
    } else if (!d->pages.isEmpty()) {
        d->pages.first()->setVisible(false);
    }
    d->setCurrentPageNumber(page);
    if (d->isAsynPreview) {
        d->previewPages = d->requestPages(page);
        d->generatePreviewPicture();
    }

    if (d->imposition != Imposition::One) {
        d->updateNumberUpContent();
    }

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
        d->graphicsView->setBackgroundBrush(QColor(0, 0, 0, 42));
    else
        d->graphicsView->setBackgroundBrush(QColor(255, 255, 255, 120));
}

void DPrintPreviewWidget::timerEvent(QTimerEvent *event)
{
    Q_D(DPrintPreviewWidget);
    if (event->timerId() == d->updateTimer.timerId()) {
        if (d->updateTimer.isActive()) {
            d->updateTimer.stop();
            d->updatePreview();
        }
    }

    return DFrame::timerEvent(event);
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

    painter->translate(leftTopPoint);

    if (pwidget && (pwidget->getColorMode() == QPrinter::GrayScale)) {
        // 图像灰度处理
        painter->drawPicture(0, 0, grayPicture);
    } else if (pwidget && (pwidget->getColorMode() == QPrinter::Color)) {
        drawNumberUpPictures(painter);
    }
}

void ContentItem::updateGrayContent()
{
    grayPicture = grayscalePaint(*pagePicture);
}

void ContentItem::drawNumberUpPictures(QPainter *painter)
{
    DPrintPreviewWidget *pwidget = qobject_cast<DPrintPreviewWidget *>(scene()->parent()->parent());
    DPrintPreviewWidget::Imposition imposition = pwidget->imposition();

    if (imposition == DPrintPreviewWidget::One) {
        if (pwidget->d_func()->isAsynPreview) {
            painter->drawPicture(0, 0, *pwidget->d_func()->pictures.first());
        } else {
            painter->drawPicture(0, 0, *pagePicture);
        }

        return;
    }

    qreal scaleRatio = pwidget->d_func()->numberUpPrintData->scaleRatio;
    const QVector<QPair<int, const QPicture *>> &numberUpPictures = pwidget->d_func()->numberUpPrintData->previewPictures;
    const QVector<QPointF> paintPoints = pwidget->d_func()->numberUpPrintData->paintPoints;

    painter->save();
    painter->scale(scaleRatio, scaleRatio);
    for (int c = 0; c < numberUpPictures.count(); ++c) {
        QPointF paintPoint = paintPoints.at(c) / scaleRatio;
        const QPicture *pic = numberUpPictures.at(c).second;
        painter->drawPicture(paintPoint, *pic);
    }

    painter->restore();
}

QPicture ContentItem::grayscalePaint(const QPicture &picture)
{
    Q_UNUSED(picture);

    QImage image(pageRect.size(), QImage::Format_ARGB32);
    QPainter imageP;

    image.fill(Qt::transparent);
    imageP.begin(&image);
    drawNumberUpPictures(&imageP);
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
    DWIDGET_NAMESPACE::grayScale(img, graySourceImage, img.rect());
}

void WaterMark::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(item);
    Q_UNUSED(widget);

    QPainterPath path = itemClipPath();
    painter->setClipPath(path, Qt::IntersectClip);
    updatePicture(painter, true);
}

void WaterMark::updatePicture(QPainter *painter, bool isPreview)
{
    DPrintPreviewWidget *pwidget = qobject_cast<DPrintPreviewWidget *>(scene()->parent()->parent());
    qreal wScale = 1;
    if (isPreview && (pwidget->imposition() != DPrintPreviewWidget::One))
        wScale = pwidget->getScale();

    switch (type) {
    case Type::None:
        break;
    case Type::Text: {
        if (!(font.styleStrategy() & QFont::PreferAntialias))
            font.setStyleStrategy(QFont::PreferAntialias);

        font.setPointSize(qRound(WATER_DEFAULTFONTSIZE * mScaleFactor * numberUpScale * wScale));

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

        // TODO: Remove it.
        QVariant spacingProperty = pwidget->property("_d_print_waterMarkRowSpacing");
        int rowSpace;
        if (spacingProperty.isValid()) {
            rowSpace = qRound(textSize.height() * spacingProperty.toDouble());
        } else {
            rowSpace = WATER_TEXTSPACE;
        }

        int columnSpace;
        spacingProperty = pwidget->property("_d_print_waterMarkColumnSpacing");
        if (spacingProperty.isValid()) {
            columnSpace = qRound(textSize.width() * spacingProperty.toDouble());
        } else {
            columnSpace = qMin(textSize.width(), textSize.height());
        }

        QSize spaceSize = QSize(columnSpace, rowSpace) * numberUpScale * wScale;
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
    } break;
    case Type::Image: {
        if (sourceImage.isNull() || graySourceImage.isNull() || qFuzzyCompare(mScaleFactor, 0))
            return;

        QImage img = sourceImage;
        if (pwidget->getColorMode() == QPrinter::GrayScale) {
            img = graySourceImage;
        }

        img = img.scaledToWidth(qRound(img.width() * mScaleFactor * numberUpScale * wScale));
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
                leftTop += QPointF(imgWidth + space * numberUpScale * wScale, 0);
            }
            row++;
            leftTop += QPointF(0, space + imgHeight);
            leftTop.setX(colStart);
        }
    } break;
    }
}

QPainterPath WaterMark::itemClipPath() const
{
    QPainterPath path;
    DPrintPreviewWidget *pwidget = qobject_cast<DPrintPreviewWidget *>(scene()->parent()->parent());

    if (pwidget->imposition() == DPrintPreviewWidget::One) {
        QPolygonF brectPol = mapFromScene(brectPolygon);
        QPolygonF twopol = mapFromScene(twoPolygon);
        path.addPolygon(twopol);
        path.addPolygon(brectPol);
        path.addPolygon(twopol);
    } else {
        if (!parentItem())
            return path;

        if (QGraphicsRectItem *parentRectItem = dynamic_cast<QGraphicsRectItem *>(parentItem())) {
            QRectF pRect = parentRectItem->rect();
            QRectF clipRect = brectPolygon.boundingRect();

            if (!pRect.contains(clipRect.topLeft()))
                return path;

            if (clipRect.right() > pRect.right())
                clipRect.setRight(pRect.right());

            if (clipRect.bottom() > pRect.bottom())
                clipRect.setBottom(pRect.bottom());

            path.addPolygon(mapFromScene(clipRect));
        }
    }

    return path;
}

void WaterMark::setNumberUpScale(const qreal &value)
{
    numberUpScale = value;
}

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    scaleRatio = 1.0;

    scaleResetButton = new DIconButton(this);
    scaleResetButton->setFixedSize(36, 36);
    scaleResetButton->setIcon(DIconTheme::findQIcon("print_previewscale"));
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

void DPrintPreviewWidgetPrivate::NumberUpData::NumberItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
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
        leftTopPoint.setX(((brect.width() * (1.0 - scale) / 2.0)) / scale);
        leftTopPoint.setY(((brect.height() * (1.0 - scale) / 2.0)) / scale);
    }

    painter->translate(leftTopPoint);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    for (int c = 0; c < numberVector.count(); ++c) {
        QSizeF rectSize = QSizeF(PREVIEW_WATER_COUNT_WIDTH, PREVIEW_WATER_COUNT_HEIGHT);
        QPointF rectPoint = QPointF(numberPointVector.at(c).x() - PREVIEW_WATER_COUNT_WIDTH - PREVIEW_WATER_COUNT_SPACE,
                                    numberPointVector.at(c).y() + PREVIEW_WATER_COUNT_SPACE);
        QRectF textRect(rectPoint, rectSize);
        DDrawUtils::drawShadow(painter, textRect.adjusted(-6, -8, 6, 8).toRect(), 10, 10, QColor(0, 0, 0, qRound(0.25 * 255)), 4, QPoint(0, 2));

        painter->setPen(QColor(0, 0, 0, qRound(NUMBERUP_SPACE_SCALE_RATIO * 255)));
        painter->setBrush(QColor(247, 247, 247, qRound(0.6 * 255)));
        painter->drawRoundedRect(textRect, 8, 8);

        QFont textFont;
        textFont.setFamily("SourceHanSansSC");
        textFont.setPixelSize(12);
        painter->setFont(textFont);
        painter->setPen(QPen(Qt::black));
        painter->setBrush(Qt::NoBrush);
        painter->drawText(textRect, Qt::AlignCenter, QString::number(numberVector.at(c)));
    }

    painter->restore();
}

DWIDGET_END_NAMESPACE
