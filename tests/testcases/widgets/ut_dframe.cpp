// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include <DPalette>

#include "dframe.h"
DWIDGET_USE_NAMESPACE
class ut_DFrame : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DFrame();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DFrame *target = nullptr;
};

TEST_F(ut_DFrame, setBackgroundRole)
{
    target->setBackgroundRole(DTK_GUI_NAMESPACE::DPalette::ItemBackground);
};

TEST_F(ut_DFrame, setFrameRounded)
{
    target->setFrameRounded(true);
};

class ut_DHorizontalLine : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DHorizontalLine();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DHorizontalLine *target = nullptr;
};

TEST_F(ut_DHorizontalLine, DHorizontalLine)
{
    ASSERT_EQ(target->frameShape(), QFrame::HLine);
};


class ut_DVerticalLine : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DVerticalLine();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DVerticalLine *target = nullptr;
};

TEST_F(ut_DVerticalLine, DVerticalLine)
{
    ASSERT_EQ(target->frameShape(), QFrame::VLine);
};
