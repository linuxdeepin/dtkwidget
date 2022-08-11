// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
