// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "dshadowline.h"

DWIDGET_USE_NAMESPACE

class ut_DShadowLine : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DShadowLine *shadowline = nullptr;
};

void ut_DShadowLine::SetUp()
{
    widget = new QWidget;
    shadowline = new DShadowLine(widget);
    widget->resize(300, 100);
}

void ut_DShadowLine::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DShadowLine, testDShadowLine)
{
    QSize size(20, 20);
    ASSERT_EQ(shadowline->sizeHint(), size);
}
