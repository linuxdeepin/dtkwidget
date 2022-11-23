// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include <QFile>
#include <QObject>

#include "dimageviewer.h"
#include "private/dimagevieweritems_p.h"

DWIDGET_USE_NAMESPACE

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
    viewer->resize(300, 300);
}

void ut_DImageViewer::TearDown()
{
    if (viewer) {
        delete viewer;
        viewer = nullptr;
    }
}

TEST_F(ut_DImageViewer, testSetImage)
{
    QImage tmpImage(300, 300, QImage::Format_ARGB32);
    tmpImage.fill(Qt::red);

    viewer->setImage(tmpImage);
    ASSERT_EQ(tmpImage, viewer->image());

    auto items = viewer->scene()->items();
    ASSERT_FALSE(items.isEmpty());
}

TEST_F(ut_DImageViewer, testSetFileNameWithStaticImage)
{
    QString tmpFilePath("/tmp/ut_DImageViewer_tmp.png");
    QImage tmpImage(300, 300, QImage::Format_ARGB32);
    tmpImage.fill(Qt::red);
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

TEST_F(ut_DImageViewer, testClear)
{
    QImage tmpImage(300, 300, QImage::Format_ARGB32);
    tmpImage.fill(Qt::red);

    viewer->setImage(tmpImage);
    viewer->clear();

    ASSERT_TRUE(viewer->image().isNull());
    auto items = viewer->scene()->items();
    ASSERT_TRUE(items.isEmpty());
}

TEST_F(ut_DImageViewer, testImageChanged)
{
    QImage tmpImage(300, 300, QImage::Format_ARGB32);
    tmpImage.fill(Qt::red);
    auto conn = QObject::connect(viewer, &DImageViewer::imageChanged, [&](const QImage &image) {
        ASSERT_EQ(tmpImage, image);
    });
    ASSERT_TRUE(conn);
    viewer->setImage(tmpImage);
    QObject::disconnect(conn);
    
    conn = QObject::connect(viewer, &DImageViewer::imageChanged, [](const QImage &image) {
        ASSERT_TRUE(image.isNull());
    });
    ASSERT_TRUE(conn);
    viewer->clear();
    QObject::disconnect(conn);
}

TEST_F(ut_DImageViewer, testFileNameChanged) {
    QString tmpFilePath("/tmp/ut_DImageViewer_tmp.png");
    QImage tmpImage(300, 300, QImage::Format_ARGB32);
    tmpImage.fill(Qt::red);
    ASSERT_TRUE(tmpImage.save(tmpFilePath));

    auto conn = QObject::connect(viewer, &DImageViewer::fileNameChanged, [&](const QString &fileName) {
        EXPECT_EQ(tmpFilePath, fileName);
    });
    EXPECT_TRUE(conn);
    viewer->setFileName(tmpFilePath);
    QObject::disconnect(conn);

    conn = QObject::connect(viewer, &DImageViewer::fileNameChanged, [&](const QString &fileName) {
        EXPECT_TRUE(fileName.isEmpty());
    });
    EXPECT_TRUE(conn);
    viewer->clear();
    QObject::disconnect(conn);

    EXPECT_TRUE(QFile::remove(tmpFilePath));
}