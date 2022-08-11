// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "darrowbutton.h"
DWIDGET_USE_NAMESPACE
class ut_ArrowButtonIcon : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new ArrowButtonIcon();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    ArrowButtonIcon *target = nullptr;
};

TEST_F(ut_ArrowButtonIcon, setArrowDirection)
{
    target->setArrowDirection(0);
    ASSERT_EQ(target->arrowDirection(), 0);
};

TEST_F(ut_ArrowButtonIcon, setButtonState)
{
    target->setButtonState(0);
    ASSERT_EQ(target->buttonState(), 0);
};

class ut_DArrowButton : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DArrowButton();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DArrowButton *target = nullptr;
};

TEST_F(ut_DArrowButton, buttonState)
{
    target->buttonState();
};

TEST_F(ut_DArrowButton, setArrowDirection)
{
    target->setArrowDirection(DArrowButton::ArrowUp);
    ASSERT_EQ(target->arrowDirection(), DArrowButton::ArrowUp);
};
