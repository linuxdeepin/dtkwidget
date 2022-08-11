// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dabstractdialog.h"
DWIDGET_USE_NAMESPACE
class ut_DAbstractDialog : public testing::Test
{
protected:
    void SetUp() override
    {
        widget = new DAbstractDialog();
    }
    void TearDown() override
    {
        if (widget) {
            delete widget;
            widget = nullptr;
        }
    }
    DAbstractDialog *widget = nullptr;
};

TEST_F(ut_DAbstractDialog, displayPosition)
{
    widget->displayPosition();
};

TEST_F(ut_DAbstractDialog, move)
{
    widget->move(QPoint(10, 10));

    ASSERT_EQ(widget->pos(), QPoint(10, 10));
};

TEST_F(ut_DAbstractDialog, moveToCenter)
{
    widget->moveToCenter();
};

TEST_F(ut_DAbstractDialog, moveToCenterByRect)
{
    widget->moveToCenterByRect(QRect(0, 0, 100, 100));
};

TEST_F(ut_DAbstractDialog, moveToTopRight)
{
    widget->moveToTopRight();
};

TEST_F(ut_DAbstractDialog, moveToTopRightByRect)
{
    widget->moveToTopRightByRect(QRect(0, 0, 100, 100));
};

TEST_F(ut_DAbstractDialog, setDisplayPosition)
{
    widget->setDisplayPosition(DAbstractDialog::Center);
    ASSERT_EQ(widget->displayPosition(), DAbstractDialog::Center);
};

TEST_F(ut_DAbstractDialog, setGeometry)
{
    widget->setGeometry(QRect(0, 0, 100, 100));
    ASSERT_EQ(widget->geometry(), QRect(0, 0, 100, 100));
};
