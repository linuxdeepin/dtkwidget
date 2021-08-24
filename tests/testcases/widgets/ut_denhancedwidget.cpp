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

#include "denhancedwidget.h"
DWIDGET_USE_NAMESPACE
class ut_DEnhancedWidget : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DEnhancedWidget(nullptr);
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DEnhancedWidget *target = nullptr;
};

TEST_F(ut_DEnhancedWidget, setEnabled)
{
    target->setEnabled(true);
    ASSERT_EQ(target->enabled(), true);
};

TEST_F(ut_DEnhancedWidget, setTarget)
{
    QWidget *targetWidget = new QWidget();
    target->setTarget(targetWidget);
    ASSERT_EQ(target->target(), targetWidget);
    targetWidget->deleteLater();
};
