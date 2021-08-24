/*
* Copyright (C) 2021 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     Ye ShanShan <yeshanshan@uniontech.com>
*
* Maintainer: Ye ShanShan <yeshanshan@uniontech.com>>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
