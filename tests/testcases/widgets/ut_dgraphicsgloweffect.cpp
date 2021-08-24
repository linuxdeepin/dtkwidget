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

#include "dgraphicsgloweffect.h"
DWIDGET_USE_NAMESPACE
class ut_DGraphicsGlowEffect : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DGraphicsGlowEffect();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DGraphicsGlowEffect *target = nullptr;
};

TEST_F(ut_DGraphicsGlowEffect, setBlurRadius)
{
    target->setBlurRadius(1);
    ASSERT_EQ(target->blurRadius(), 1);
};

TEST_F(ut_DGraphicsGlowEffect, setColor)
{
    target->setColor(Qt::red);
    ASSERT_EQ(target->color(), Qt::red);
};

TEST_F(ut_DGraphicsGlowEffect, setDistance)
{
    target->setDistance(0);
    ASSERT_EQ(target->distance(), 0);
};

TEST_F(ut_DGraphicsGlowEffect, setOffset)
{
    target->setOffset(1, 1);
    ASSERT_EQ(target->xOffset(), 1);
    ASSERT_EQ(target->yOffset(), 1);
};

TEST_F(ut_DGraphicsGlowEffect, setOpacity)
{
    target->setOpacity(1);
    ASSERT_EQ(target->opacity(), 1);
};

TEST_F(ut_DGraphicsGlowEffect, setXOffset)
{
    target->setXOffset(1);
    ASSERT_EQ(target->xOffset(), 1);
};

TEST_F(ut_DGraphicsGlowEffect, setYOffset)
{
    target->setYOffset(1);
    ASSERT_EQ(target->yOffset(), 1);
};
