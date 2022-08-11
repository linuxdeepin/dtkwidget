// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
