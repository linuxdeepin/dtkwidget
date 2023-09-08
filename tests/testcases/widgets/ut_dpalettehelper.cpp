// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "DPaletteHelper"

DWIDGET_USE_NAMESPACE

class ut_DPaletteHelper : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DPaletteHelper *helper = nullptr;
};

void ut_DPaletteHelper::SetUp()
{
    widget = new QWidget;
    helper = DPaletteHelper::instance();
    widget->resize(300, 100);
}

void ut_DPaletteHelper::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DPaletteHelper, testDefaultPalette)
{
    DPalette palette = helper->palette(widget);
    EXPECT_FALSE(widget->testAttribute(Qt::WA_SetPalette));
}

TEST_F(ut_DPaletteHelper, testSetPalette)
{
    DPalette palette;
    palette.setColor(QPalette::Window, Qt::red);
    helper->setPalette(widget, palette);
    palette = helper->palette(widget);
    EXPECT_EQ(palette.color(QPalette::Window), Qt::red);
}

TEST_F(ut_DPaletteHelper, testResetPalette)
{
    DPalette palette;
    palette.setColor(QPalette::Window, Qt::red);
    helper->setPalette(widget, palette);
    palette = helper->palette(widget);
    EXPECT_EQ(palette.color(QPalette::Window), Qt::red);
    helper->resetPalette(widget);
    EXPECT_FALSE(widget->testAttribute(Qt::WA_SetPalette));
}
