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
#include <QLayout>

#include "dbackgroundgroup.h"
DWIDGET_USE_NAMESPACE
class ut_DBackgroundGroup : public testing::Test
{
protected:
    void SetUp() override
    {
        widget = new QWidget();
        layout = new QHBoxLayout(widget);
        target = new DBackgroundGroup(layout, widget);
    }
    void TearDown() override
    {
        if (widget) {
            delete widget;
            widget = nullptr;
        }
    }
    DBackgroundGroup *target = nullptr;
    QWidget* widget = nullptr;
    QLayout* layout = nullptr;
};

TEST_F(ut_DBackgroundGroup, setItemMargins)
{
    QMargins margin;
    margin.setLeft(10);
    target->setItemMargins(margin);
    ASSERT_EQ(target->itemMargins(), margin);
};

TEST_F(ut_DBackgroundGroup, setLayout)
{
    ASSERT_EQ(target->layout(), layout);
};

TEST_F(ut_DBackgroundGroup, setUseWidgetBackground)
{
    target->setUseWidgetBackground(true);
    ASSERT_EQ(target->useWidgetBackground(), true);
};
