// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dheaderline.h"
DWIDGET_USE_NAMESPACE
class ut_DHeaderLine : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DHeaderLine();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DHeaderLine *target = nullptr;
};

TEST_F(ut_DHeaderLine, setContent)
{
    QWidget *content = new QWidget();
    target->setContent(content);
    ASSERT_EQ(target->rightLayout()->count(), 1);
};

TEST_F(ut_DHeaderLine, setTitle)
{
    target->setTitle("setTitle");
    ASSERT_EQ(target->title(), "setTitle");
};
