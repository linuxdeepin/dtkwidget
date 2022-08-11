// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "ddrawer.h"
DWIDGET_USE_NAMESPACE
class ut_DDrawer : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DDrawer();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DDrawer *target = nullptr;
};

TEST_F(ut_DDrawer, setContent)
{
    QWidget *content = new QWidget();
    target->setContent(content);
    ASSERT_EQ(target->getContent(), content);
};

TEST_F(ut_DDrawer, setExpand)
{
    target->setExpand(true);
    ASSERT_EQ(target->expand(), true);
};
