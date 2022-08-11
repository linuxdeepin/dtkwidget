// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dpicturesequenceview.h"
DWIDGET_USE_NAMESPACE
class ut_DPictureSequenceView : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DPictureSequenceView();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DPictureSequenceView *target = nullptr;
};

TEST_F(ut_DPictureSequenceView, setSingleShot)
{
    target->setSingleShot(true);
    ASSERT_EQ(target->singleShot(), true);
};

TEST_F(ut_DPictureSequenceView, setSpeed)
{
    target->setSpeed(1);
    ASSERT_EQ(target->speed(), 1);
};

TEST_F(ut_DPictureSequenceView, stop)
{
    target->stop();
};
