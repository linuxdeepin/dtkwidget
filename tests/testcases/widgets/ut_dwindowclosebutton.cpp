// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "dwindowclosebutton.h"

DWIDGET_USE_NAMESPACE

class ut_DWindowCloseButton : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DWindowCloseButton *closeButton = nullptr;
};

void ut_DWindowCloseButton::SetUp()
{
    widget = new QWidget;
    closeButton = new DWindowCloseButton(widget);
    widget->resize(300, 100);
}

void ut_DWindowCloseButton::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DWindowCloseButton, testDWindowCloseButton)
{
    QSize size(12, 12);
    ASSERT_EQ(closeButton->sizeHint(), size);
}
