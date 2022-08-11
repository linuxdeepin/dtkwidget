// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dbaseexpand.h"
DWIDGET_USE_NAMESPACE
class ut_DBaseExpand : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DBaseExpand();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DBaseExpand *target = nullptr;
};

TEST_F(ut_DBaseExpand, setContent)
{
    auto widget = new QWidget();
    target->setContent(widget);
    ASSERT_EQ(target->getContent(), widget);
};

TEST_F(ut_DBaseExpand, setExpand)
{
    target->setExpand(true);
    ASSERT_EQ(target->expand(), true);
};
