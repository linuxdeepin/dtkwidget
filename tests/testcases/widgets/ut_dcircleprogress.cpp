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

#include "dcircleprogress.h"
DWIDGET_USE_NAMESPACE
class ut_DCircleProgress : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DCircleProgress();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DCircleProgress *target = nullptr;
};

TEST_F(ut_DCircleProgress, bottomLabel)
{
    ASSERT_EQ(target->bottomLabel()->accessibleName(), "DCircleProgressBottomLabel");
};

TEST_F(ut_DCircleProgress, setBackgroundColor)
{
    target->setBackgroundColor(Qt::red);
    ASSERT_EQ(target->backgroundColor(), Qt::red);
};

TEST_F(ut_DCircleProgress, setChunkColor)
{
    target->setChunkColor(Qt::red);
    ASSERT_EQ(target->chunkColor(), Qt::red);
};

TEST_F(ut_DCircleProgress, setLineWidth)
{
    target->setLineWidth(1);
    ASSERT_EQ(target->lineWidth(), 1);
};

TEST_F(ut_DCircleProgress, setText)
{
    target->setText("setText");
    ASSERT_EQ(target->text(), "setText");
};

TEST_F(ut_DCircleProgress, setValue)
{
    target->setValue(1);
    ASSERT_EQ(target->value(), 1);
};

TEST_F(ut_DCircleProgress, topLabel)
{
    ASSERT_EQ(target->topLabel()->text(), target->text());
};
