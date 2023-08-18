// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QLabel>
#include <QTest>

#include "dwatermarkwidget.h"
DWIDGET_USE_NAMESPACE
class ut_DWaterMarkWidget : public testing::Test
{
protected:
    void SetUp() override
    {
        root = new QLabel();
        target = new DWaterMarkWidget(root);
        root->resize(600, 600);

        QImage img(":/data/test.png");
        QPixmap pixmap = QPixmap::fromImage(img.scaled(root->size()));
        root->setPixmap(pixmap);
    }
    void TearDown() override
    {
        if (root)
            delete root;
    }
    inline bool equalImage(const QImage &source)
    {
        QPixmap result = root->grab();
        return result.toImage() == source;
    }

    DWaterMarkWidget *target = nullptr;
    QLabel *root = nullptr;
};

TEST_F(ut_DWaterMarkWidget, paintText)
{
    WaterMarkData data = target->data();
    data.setType(WaterMarkData::Text);
    data.setText("deepin water mark");
    data.setLineSpacing(200);
    data.setScaleFactor(1.5);
    auto font = data.font();
    font.setBold(true);
    font.setPointSize(20);
    data.setFont(font);
    data.setColor(Qt::red);
    data.setOpacity(0.5);
    target->setData(data);

    EXPECT_TRUE(equalImage(QImage(":/data/watermarks/text.png")));
}

TEST_F(ut_DWaterMarkWidget, paintImage)
{
    WaterMarkData data = target->data();
    data.setType(WaterMarkData::Image);
    QImage img(":/assets/images/uos.svg");
    data.setImage(img);
    data.setLineSpacing(200);
    data.setGrayScale(true);
    target->setData(data);

    EXPECT_TRUE(equalImage(QImage(":/data/watermarks/image.png")));
}
