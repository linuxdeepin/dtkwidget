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

#include "danchors.h"
DWIDGET_USE_NAMESPACE

class ut_DAnchorsBase : public testing::Test
{
protected:
    void SetUp() override
    {
        parentWidget = new QWidget();
        target = new DAnchorsBase(parentWidget);
    }
    void TearDown() override
    {
        if (parentWidget) {
            delete parentWidget;
            parentWidget = nullptr;
        }
    }
    DAnchorsBase *target = nullptr;
    QWidget *parentWidget = nullptr;
};

TEST_F(ut_DAnchorsBase, setBottomMargin)
{
    target->setBottomMargin(1);
    ASSERT_EQ(target->bottomMargin(), 1);
};

TEST_F(ut_DAnchorsBase, setEnabled)
{
    target->setEnabled(true);
    ASSERT_EQ(target->enabled(), true);
};

TEST_F(ut_DAnchorsBase, setHorizontalCenterOffset)
{
    target->setHorizontalCenterOffset(1);
    ASSERT_EQ(target->horizontalCenterOffset(), 1);
};

TEST_F(ut_DAnchorsBase, setLeftMargin)
{
    target->setLeftMargin(1);
    ASSERT_EQ(target->leftMargin(), 1);
};

TEST_F(ut_DAnchorsBase, setMargins)
{
    target->setMargins(1);
    ASSERT_EQ(target->margins(), 1);
};

TEST_F(ut_DAnchorsBase, setRightMargin)
{
    target->setRightMargin(1);
    ASSERT_EQ(target->rightMargin(), 1);
};

TEST_F(ut_DAnchorsBase, setTopMargin)
{
    target->setTopMargin(1);
    ASSERT_EQ(target->topMargin(), 1);
};

TEST_F(ut_DAnchorsBase, setVerticalCenterOffset)
{
    target->setVerticalCenterOffset(1);
    ASSERT_EQ(target->verticalCenterOffset(), 1);
};
