// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <DStyleOptionButton>

#include "dfloatingwidget.h"
DWIDGET_USE_NAMESPACE
class ut_DFloatingWidget : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DFloatingWidget();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DFloatingWidget *target = nullptr;
};

TEST_F(ut_DFloatingWidget, initStyleOption)
{
    DStyleOptionFloatingWidget option;
    target->initStyleOption(&option);
    ASSERT_EQ(option.noBackground, target->blurBackgroundIsEnabled());
};

TEST_F(ut_DFloatingWidget, setBlurBackgroundEnabled)
{
    target->setBlurBackgroundEnabled(true);
    ASSERT_EQ(target->blurBackgroundIsEnabled(), true);
};

TEST_F(ut_DFloatingWidget, setFramRadius)
{
    target->setFramRadius(1);
    DStyleOptionFloatingWidget option;
    target->initStyleOption(&option);
    ASSERT_EQ(option.frameRadius, 1);
};
