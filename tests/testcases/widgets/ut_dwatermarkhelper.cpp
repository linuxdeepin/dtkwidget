// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "DWaterMarkHelper"

DWIDGET_USE_NAMESPACE

class ut_DWaterMarkHelper : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DWaterMarkHelper *helper = nullptr;
};

void ut_DWaterMarkHelper::SetUp()
{
    widget = new QWidget;
    helper = DWaterMarkHelper::instance();
}

void ut_DWaterMarkHelper::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DWaterMarkHelper, testSetData)
{
    WaterMarkData data;
    data.setText("test");
    helper->setData(data);
    EXPECT_EQ(helper->data().text(), "test");
}

TEST_F(ut_DWaterMarkHelper, testSetType)
{
    WaterMarkData data;
    data.setType(WaterMarkData::WaterMarkType::Text);
    helper->setData(data);
    EXPECT_EQ(helper->data().type(), WaterMarkData::WaterMarkType::Text);
}

TEST_F(ut_DWaterMarkHelper, testSetLayout)
{
    WaterMarkData data;
    data.setLayout(WaterMarkData::WaterMarkLayout::Center);
    helper->setData(data);
    EXPECT_EQ(helper->data().layout(), WaterMarkData::WaterMarkLayout::Center);
}

TEST_F(ut_DWaterMarkHelper, testSetScaleFactor)
{
    WaterMarkData data;
    data.setScaleFactor(0.5);
    helper->setData(data);
    EXPECT_EQ(helper->data().scaleFactor(), 0.5);
}

TEST_F(ut_DWaterMarkHelper, testSetSpacing)
{
    WaterMarkData data;
    data.setSpacing(10);
    helper->setData(data);
    EXPECT_EQ(helper->data().spacing(), 10);
}

TEST_F(ut_DWaterMarkHelper, testSetLineSpacing)
{
    WaterMarkData data;
    data.setLineSpacing(10);
    helper->setData(data);
    EXPECT_EQ(helper->data().lineSpacing(), 10);
}

TEST_F(ut_DWaterMarkHelper, testSetText)
{
    WaterMarkData data;
    data.setText("test");
    helper->setData(data);
    EXPECT_EQ(helper->data().text(), "test");
}

TEST_F(ut_DWaterMarkHelper, testSetFont)
{
    WaterMarkData data;
    QFont font;
    font.setFamily("test");
    data.setFont(font);
    helper->setData(data);
    EXPECT_EQ(helper->data().font().family(), "test");
}

TEST_F(ut_DWaterMarkHelper, testSetColor)
{
    WaterMarkData data;
    data.setColor(Qt::red);
    helper->setData(data);
    EXPECT_EQ(helper->data().color(), Qt::red);
}

TEST_F(ut_DWaterMarkHelper, testSetRotation)
{
    WaterMarkData data;
    data.setRotation(10);
    helper->setData(data);
    EXPECT_EQ(helper->data().rotation(), 10);
}

TEST_F(ut_DWaterMarkHelper, testSetOpacity)
{
    WaterMarkData data;
    data.setOpacity(0.5);
    helper->setData(data);
    EXPECT_EQ(helper->data().opacity(), 0.5);
}

TEST_F(ut_DWaterMarkHelper, testSetImage)
{
    WaterMarkData data;
    QImage image;
    image.load(":/images/test.png");
    data.setImage(image);
    helper->setData(data);
    EXPECT_EQ(helper->data().image().width(), 100);
}

TEST_F(ut_DWaterMarkHelper, testSetGrayScale)
{
    WaterMarkData data;
    data.setGrayScale(true);
    helper->setData(data);
    EXPECT_TRUE(helper->data().grayScale());
}
