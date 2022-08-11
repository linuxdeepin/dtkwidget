// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
