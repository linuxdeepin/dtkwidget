// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include <QFile>
#include <QObject>
#include <QSignalSpy>
#include <QTouchEvent>
#include <QGraphicsSceneMouseEvent>
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
#include <QWindow>
#include <private/qeventpoint_p.h>
#endif

#include "dimageviewer.h"
#include "private/dimagevieweritems_p.h"

DWIDGET_USE_NAMESPACE

#define NORMAL_WIDTH 300
#define NORMAL_HEIGHT 300

class ut_DImageViewer : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    DImageViewer *viewer = nullptr;
};

void ut_DImageViewer::SetUp()
{
    viewer = new DImageViewer();
    viewer->resize(NORMAL_WIDTH, NORMAL_HEIGHT);
}

void ut_DImageViewer::TearDown()
{
    if (viewer) {
        delete viewer;
        viewer = nullptr;
    }
}

QImage createNormalImage()
{
    // 300 x 300
    QImage tmpImage(NORMAL_WIDTH, NORMAL_HEIGHT, QImage::Format_ARGB32);
    tmpImage.fill(Qt::red);
    return tmpImage;
}

QImage createDoubleSizeImage()
{
    // 600 x 600
    QImage tmpImage(NORMAL_WIDTH * 2, NORMAL_HEIGHT * 2, QImage::Format_ARGB32);
    tmpImage.fill(Qt::red);
    return tmpImage;
}

QImage createHalfSizeImage()
{
    // 150 x 150
    QImage tmpImage(NORMAL_WIDTH / 2, NORMAL_HEIGHT / 2, QImage::Format_ARGB32);
    tmpImage.fill(Qt::red);
    return tmpImage;
}

TEST_F(ut_DImageViewer, testSetImage)
{
    QImage tmpImage = createNormalImage();

    viewer->setImage(tmpImage);
    ASSERT_EQ(tmpImage, viewer->image());

    auto items = viewer->scene()->items();
    ASSERT_FALSE(items.isEmpty());
}

TEST_F(ut_DImageViewer, testSetImageWithNull)
{
    viewer->setImage(QImage());
    ASSERT_TRUE(viewer->image().isNull());
}

TEST_F(ut_DImageViewer, testSetFileNameWithStaticImage)
{
    QString tmpFilePath("/tmp/ut_DImageViewer_tmp.png");
    QImage tmpImage = createNormalImage();
    ASSERT_TRUE(tmpImage.save(tmpFilePath));

    viewer->setFileName(tmpFilePath);
    EXPECT_EQ(viewer->fileName(), tmpFilePath);

    auto items = viewer->scene()->items();
    EXPECT_FALSE(items.isEmpty());

    QImage readImage = viewer->image();
    EXPECT_EQ(readImage.size(), tmpImage.size());

    EXPECT_TRUE(QFile::remove(tmpFilePath));
}

TEST_F(ut_DImageViewer, testSetFileNameWithDynamicImage)
{
    QString tmpFilePath("/tmp/ut_DImageViewer_tmp.gif");
    QFile tmpFile(tmpFilePath);
    ASSERT_TRUE(tmpFile.open(QFile::WriteOnly));
    tmpFile.write("");
    tmpFile.close();

    viewer->setFileName(tmpFilePath);
    EXPECT_EQ(viewer->fileName(), tmpFilePath);

    auto items = viewer->scene()->items();
    EXPECT_FALSE(items.isEmpty());

    QImage readImage = viewer->image();
    EXPECT_TRUE(readImage.isNull());

    EXPECT_TRUE(QFile::remove(tmpFilePath));
}

TEST_F(ut_DImageViewer, testSetFileNameWithSvgImage)
{
    QByteArray svgCode("<svg version=\"1.1\"> <rect width=\"300\" height=\"300\" /> </svg>");
    QString tmpFilePath("/tmp/ut_DImageViewer_tmp.svg");
    QFile tmpFile(tmpFilePath);
    ASSERT_TRUE(tmpFile.open(QFile::WriteOnly));
    tmpFile.write(svgCode);
    tmpFile.close();

    viewer->setFileName(tmpFilePath);
    EXPECT_EQ(viewer->fileName(), tmpFilePath);

    auto items = viewer->scene()->items();
    EXPECT_FALSE(items.isEmpty());

    QImage readImage = viewer->image();
    EXPECT_EQ(readImage.size(), QSize(300, 300));

    EXPECT_TRUE(QFile::remove(tmpFilePath));
}

TEST_F(ut_DImageViewer, testAutoFitImage)
{
    QImage tmpImage = createDoubleSizeImage();

    ASSERT_TRUE(qFuzzyCompare(1.0, viewer->scaleFactor()));
    // Internal called autoFitImage.
    viewer->setImage(tmpImage);

    ASSERT_TRUE(qFuzzyCompare(0.5, viewer->scaleFactor()));
}

TEST_F(ut_DImageViewer, testFitToWidget)
{
    QImage tmpImage = createDoubleSizeImage();

    viewer->setImage(tmpImage);
    viewer->fitToWidget();
    ASSERT_TRUE(qFuzzyCompare(0.5, viewer->scaleFactor()));

    QImage tmpImage2 = createHalfSizeImage();

    viewer->setImage(tmpImage2);
    viewer->fitToWidget();
    ASSERT_TRUE(qFuzzyCompare(2.0, viewer->scaleFactor()));
}

TEST_F(ut_DImageViewer, testFitNormalSize)
{
    QImage tmpImage = createDoubleSizeImage();

    viewer->setImage(tmpImage);
    viewer->fitNormalSize();
    ASSERT_TRUE(qFuzzyCompare(1.0, viewer->scaleFactor()));

    QImage tmpImage2 = createHalfSizeImage();

    viewer->setImage(tmpImage2);
    viewer->fitNormalSize();
    ASSERT_TRUE(qFuzzyCompare(1.0, viewer->scaleFactor()));
}

TEST_F(ut_DImageViewer, testRotateClockwise)
{
    QImage tmpImage = createNormalImage();

    viewer->setImage(tmpImage);
    ASSERT_EQ(0, viewer->rotateAngle());
    viewer->rotateClockwise();
    ASSERT_EQ(90, viewer->rotateAngle());

    // Rotate 360 degree.
    for (int i = 0; i < 4; ++i) {
        viewer->rotateClockwise();
    }
    ASSERT_EQ(90, viewer->rotateAngle());
}

TEST_F(ut_DImageViewer, testRotateCounterclockwise)
{
    QImage tmpImage = createNormalImage();

    viewer->setImage(tmpImage);
    ASSERT_EQ(0, viewer->rotateAngle());
    viewer->rotateCounterclockwise();
    ASSERT_EQ(-90, viewer->rotateAngle());

    // Rotate 360 degree.
    for (int i = 0; i < 4; ++i) {
        viewer->rotateCounterclockwise();
    }
    ASSERT_EQ(-90, viewer->rotateAngle());
}

TEST_F(ut_DImageViewer, testResetRotateAngle)
{
    QImage tmpImage = createNormalImage();

    viewer->setImage(tmpImage);
    viewer->rotateClockwise();
    ASSERT_NE(0, viewer->rotateAngle());
    viewer->resetRotateAngle();
    ASSERT_EQ(0, viewer->rotateAngle());
}

TEST_F(ut_DImageViewer, testClear)
{
    QImage tmpImage = createNormalImage();

    viewer->setImage(tmpImage);
    viewer->clear();

    ASSERT_TRUE(viewer->image().isNull());
    auto items = viewer->scene()->items();
    // Has proxy item.
    ASSERT_EQ(items.size(), 1);

    ASSERT_EQ(0, viewer->rotateAngle());
    ASSERT_TRUE(qFuzzyCompare(1.0, viewer->scaleFactor()));
}

TEST_F(ut_DImageViewer, testCenterOn)
{
    QImage tmpImage = createDoubleSizeImage();
    viewer->setImage(tmpImage);
    viewer->fitNormalSize();

    viewer->centerOn(0, 0);
    QRect visibleRect = viewer->visibleImageRect();
    ASSERT_EQ(QPoint(0, 0), visibleRect.topLeft());

    QSize imageSize = tmpImage.size();
    viewer->centerOn(imageSize.width() * 2, imageSize.height() * 2);
    visibleRect = viewer->visibleImageRect();
    ASSERT_EQ(QPoint(imageSize.width() - 1, imageSize.height() - 1), visibleRect.bottomRight());
}

TEST_F(ut_DImageViewer, testVisibleImageRect)
{
    QImage tmpImage = createDoubleSizeImage();
    viewer->setImage(tmpImage);

    QRect visibleRect = viewer->visibleImageRect();
    ASSERT_EQ(visibleRect.size(), tmpImage.size());

    viewer->fitNormalSize();
    visibleRect = viewer->visibleImageRect();
    ASSERT_EQ(visibleRect.size(), viewer->size());
}

TEST_F(ut_DImageViewer, testScaleAtPoint)
{
    QImage tmpImage = createNormalImage();
    viewer->setImage(tmpImage);
    ASSERT_TRUE(qFuzzyCompare(1.0, viewer->scaleFactor()));

    viewer->scaleAtPoint(QPoint(0, 0), 2);
    ASSERT_TRUE(qFuzzyCompare(2.0, viewer->scaleFactor()));
}

TEST_F(ut_DImageViewer, testCropImageRect)
{
    viewer->setImage(createNormalImage());
    viewer->beginCropImage();

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setButton(Qt::LeftButton);
    pressEvent.setPos(QPointF(0, 0));
    viewer->scene()->mousePressEvent(&pressEvent);

    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setButton(Qt::LeftButton);
    moveEvent.setLastScenePos(QPointF(0, 0));
    moveEvent.setScenePos(QPointF(150, 150));
    viewer->scene()->mouseMoveEvent(&moveEvent);

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setButton(Qt::LeftButton);
    viewer->scene()->mouseReleaseEvent(&releaseEvent);

    viewer->endCropImage();

    ASSERT_EQ(QRect(150, 150, 150, 150), viewer->cropImageRect());
}

TEST_F(ut_DImageViewer, testSetCropAspectRatio)
{
    viewer->setImage(createNormalImage());
    viewer->beginCropImage();
    viewer->setCropAspectRatio(16.0, 9.0);
    viewer->endCropImage();
    QRect cropRect = viewer->cropImageRect();
    qreal ratio = (1.0 * cropRect.width()) / cropRect.height();
    ASSERT_TRUE(ratio - (16.0 / 9.0) < 0.01);

    viewer->beginCropImage();
    viewer->setCropAspectRatio(3.0, 4.0);
    viewer->endCropImage();
    cropRect = viewer->cropImageRect();
    ratio = (1.0 * cropRect.width()) / cropRect.height();
    ASSERT_TRUE(ratio - (3.0 / 4.0) < 0.01);
}

TEST_F(ut_DImageViewer, testImageChanged)
{
    QImage tmpImage = createNormalImage();
    auto conn = QObject::connect(viewer, &DImageViewer::imageChanged, [&](const QImage &image) { ASSERT_EQ(tmpImage, image); });
    ASSERT_TRUE(conn);
    viewer->setImage(tmpImage);
    QObject::disconnect(conn);

    conn = QObject::connect(viewer, &DImageViewer::imageChanged, [](const QImage &image) { ASSERT_TRUE(image.isNull()); });
    ASSERT_TRUE(conn);
    viewer->clear();
    QObject::disconnect(conn);
}

TEST_F(ut_DImageViewer, testFileNameChanged)
{
    QString tmpFilePath("/tmp/ut_DImageViewer_tmp.png");
    QImage tmpImage = createNormalImage();
    ASSERT_TRUE(tmpImage.save(tmpFilePath));

    auto conn = QObject::connect(
        viewer, &DImageViewer::fileNameChanged, [&](const QString &fileName) { EXPECT_EQ(tmpFilePath, fileName); });
    EXPECT_TRUE(conn);
    viewer->setFileName(tmpFilePath);
    QObject::disconnect(conn);

    conn = QObject::connect(
        viewer, &DImageViewer::fileNameChanged, [&](const QString &fileName) { EXPECT_TRUE(fileName.isEmpty()); });
    EXPECT_TRUE(conn);
    viewer->clear();
    QObject::disconnect(conn);

    EXPECT_TRUE(QFile::remove(tmpFilePath));
}

TEST_F(ut_DImageViewer, testScaleFactorChanged)
{
    // Ensure image size not equal viewport size.
    QImage tmpImage = createDoubleSizeImage();
    // Internal called autoFitImage().
    viewer->setImage(tmpImage);

    qreal scaleFactor = 0;
    auto conn = QObject::connect(viewer, &DImageViewer::scaleFactorChanged, [&](qreal facotr) { scaleFactor = facotr; });
    QSignalSpy changeSignal(viewer, &DImageViewer::scaleFactorChanged);

    viewer->scaleImage(5);
    ASSERT_EQ(1, changeSignal.count());
    ASSERT_TRUE(qFuzzyCompare(scaleFactor, viewer->scaleFactor()));

    viewer->fitToWidget();
    ASSERT_EQ(2, changeSignal.count());
    ASSERT_TRUE(qFuzzyCompare(scaleFactor, viewer->scaleFactor()));

    viewer->fitNormalSize();
    ASSERT_EQ(3, changeSignal.count());
    ASSERT_TRUE(qFuzzyCompare(scaleFactor, viewer->scaleFactor()));

    viewer->setScaleFactor(10);
    ASSERT_EQ(4, changeSignal.count());
    ASSERT_TRUE(qFuzzyCompare(scaleFactor, viewer->scaleFactor()));

    viewer->autoFitImage();
    ASSERT_EQ(5, changeSignal.count());
    ASSERT_TRUE(qFuzzyCompare(scaleFactor, viewer->scaleFactor()));

    QObject::disconnect(conn);
}

TEST_F(ut_DImageViewer, testRotateAngleChanged)
{
    QImage tmpImage = createNormalImage();
    viewer->setImage(tmpImage);

    int rotateAngle = 0;
    auto conn = QObject::connect(viewer, &DImageViewer::rotateAngleChanged, [&](int angle) { rotateAngle = angle; });
    QSignalSpy changeSignal(viewer, &DImageViewer::rotateAngleChanged);

    viewer->rotateClockwise();
    ASSERT_EQ(1, changeSignal.count());
    ASSERT_EQ(rotateAngle, viewer->rotateAngle());

    viewer->resetRotateAngle();
    ASSERT_EQ(2, changeSignal.count());
    ASSERT_EQ(rotateAngle, viewer->rotateAngle());

    viewer->rotateCounterclockwise();
    ASSERT_EQ(3, changeSignal.count());
    ASSERT_EQ(rotateAngle, viewer->rotateAngle());

    QObject::disconnect(conn);
}

TEST_F(ut_DImageViewer, testRequestPreviousImage)
{
    viewer->setImage(createNormalImage());
    QSignalSpy changeSignal(viewer, &DImageViewer::requestPreviousImage);

    // Simulate event trigger.
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QTouchEvent::TouchPoint point;
    point.setStartPos(QPointF(0, 0));
    point.setLastPos(QPointF(300, 0));
    QTouchEvent touchEvent(QEvent::TouchEnd, nullptr, Qt::NoModifier, Qt::TouchPointReleased, {point});

    viewer->event(&touchEvent);
    ASSERT_EQ(changeSignal.count(), 1);

    point.setLastPos(QPointF(100, 0));
    QTouchEvent touchEvent2(QEvent::TouchEnd, nullptr, Qt::NoModifier, Qt::TouchPointReleased, {point});
    viewer->event(&touchEvent2);
    ASSERT_EQ(changeSignal.count(), 1);
#else
    QMutableEventPoint point;
    QEventPoint ep;
    point.setGlobalPosition(ep, {0, 0});
    point.setGlobalLastPosition(ep, {300, 0});
    QTouchEvent touchEvent(QEvent::TouchEnd, nullptr, Qt::NoModifier, {ep});

    viewer->event(&touchEvent);
    ASSERT_EQ(changeSignal.count(), 1);

    point.setGlobalLastPosition(ep, {100, 0});
    QTouchEvent touchEvent2(QEvent::TouchEnd, nullptr, Qt::NoModifier, {ep});
    viewer->event(&touchEvent2);
    ASSERT_EQ(changeSignal.count(), 1);
#endif
}

TEST_F(ut_DImageViewer, testRequestNextImage)
{
    viewer->setImage(createNormalImage());
    QSignalSpy changeSignal(viewer, &DImageViewer::requestNextImage);

    // Simulate event trigger.
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QTouchEvent::TouchPoint point;
    point.setStartPos(QPointF(0, 0));
    point.setLastPos(QPointF(-300, 0));
    QTouchEvent touchEvent(QEvent::TouchEnd, nullptr, Qt::NoModifier, Qt::TouchPointReleased, {point});

    viewer->event(&touchEvent);
    ASSERT_EQ(changeSignal.count(), 1);

    // Test multi point touch.
    QTouchEvent touchEvent2(QEvent::TouchEnd, nullptr, Qt::NoModifier, Qt::TouchPointReleased, {point, point});
    viewer->event(&touchEvent2);
    ASSERT_EQ(changeSignal.count(), 1);
#else
    QMutableEventPoint point;
    QEventPoint ep;
    point.setGlobalPosition(ep, {0, 0});
    point.setGlobalLastPosition(ep, {-300, 0});
    QTouchEvent touchEvent(QEvent::TouchEnd, nullptr, Qt::NoModifier, {ep});

    viewer->event(&touchEvent);
    ASSERT_EQ(changeSignal.count(), 1);

    // Test multi point touch.
    QTouchEvent touchEvent2(QEvent::TouchEnd, nullptr, Qt::NoModifier, {ep, ep});
    viewer->event(&touchEvent2);
    ASSERT_EQ(changeSignal.count(), 1);
#endif
}

TEST_F(ut_DImageViewer, testCropImageFinished)
{
    viewer->setImage(createNormalImage());
    QSignalSpy changeSignal(viewer, &DImageViewer::cropImageChanged);

    viewer->beginCropImage();
    viewer->setCropAspectRatio(16.0, 9.0);
    viewer->endCropImage();

    ASSERT_EQ(changeSignal.count(), 1);
}
