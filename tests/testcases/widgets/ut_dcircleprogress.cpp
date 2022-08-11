// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dcircleprogress.h"
DWIDGET_USE_NAMESPACE
class ut_DCircleProgress : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DCircleProgress();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DCircleProgress *target = nullptr;
};

TEST_F(ut_DCircleProgress, bottomLabel)
{
    ASSERT_EQ(target->bottomLabel()->accessibleName(), "DCircleProgressBottomLabel");
};

TEST_F(ut_DCircleProgress, setBackgroundColor)
{
    target->setBackgroundColor(Qt::red);
    ASSERT_EQ(target->backgroundColor(), Qt::red);
};

TEST_F(ut_DCircleProgress, setChunkColor)
{
    target->setChunkColor(Qt::red);
    ASSERT_EQ(target->chunkColor(), Qt::red);
};

TEST_F(ut_DCircleProgress, setLineWidth)
{
    target->setLineWidth(1);
    ASSERT_EQ(target->lineWidth(), 1);
};

TEST_F(ut_DCircleProgress, setText)
{
    target->setText("setText");
    ASSERT_EQ(target->text(), "setText");
};

TEST_F(ut_DCircleProgress, setValue)
{
    target->setValue(1);
    ASSERT_EQ(target->value(), 1);
};

TEST_F(ut_DCircleProgress, topLabel)
{
    ASSERT_EQ(target->topLabel()->text(), target->text());
};
