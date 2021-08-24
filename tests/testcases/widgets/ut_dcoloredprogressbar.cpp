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

#include "dcoloredprogressbar.h"
DWIDGET_USE_NAMESPACE
class ut_DColoredProgressBar : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DColoredProgressBar();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DColoredProgressBar *target = nullptr;
};

TEST_F(ut_DColoredProgressBar, addThreshold)
{
    auto thredsholds = target->thresholds();
    target->addThreshold(1, QBrush(Qt::red));
    ASSERT_EQ(thredsholds.size() + 1, target->thresholds().size());
};

TEST_F(ut_DColoredProgressBar, removeThreshold)
{
    auto thredsholds = target->thresholds();
    target->addThreshold(1, QBrush(Qt::red));
    ASSERT_EQ(thredsholds.size() + 1, target->thresholds().size());
    target->removeThreshold(1);
    ASSERT_EQ(thredsholds.size(), target->thresholds().size());
};
