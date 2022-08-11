// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "dprogressbar.h"

DWIDGET_USE_NAMESPACE

class ut_DProgressBar: public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DProgressBar *bar = nullptr;
    QWidget *widget = nullptr;
};

void ut_DProgressBar::SetUp()
{
    widget = new QWidget;
    bar = new DProgressBar(widget);
    widget->resize(300, 100);
}

void ut_DProgressBar::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DProgressBar, testDProgressBar)
{
    // 测试数值的设置是否正确
    for (int i = 0; i < 101; ++i) {
        bar->setValue(i);
        ASSERT_TRUE(bar->value() == i);
    }

    // 测试字体的设置是否正确
    QFont arial("Arial");
    bar->setFont(arial);
    ASSERT_TRUE(bar->font().family() == arial.family());
}
