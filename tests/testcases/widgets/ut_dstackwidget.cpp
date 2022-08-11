// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dstackwidget.h"
DWIDGET_USE_NAMESPACE

class ut_DStackWidget : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DStackWidget();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DStackWidget *target = nullptr;
};

TEST_F(ut_DStackWidget, clear)
{
    QWidget *widget = new QWidget();
    target->insertWidget(0, widget);
    target->clear();
    ASSERT_EQ(target->depth(), 0);
};

TEST_F(ut_DStackWidget, currentIndex)
{
    QWidget *widget = new QWidget();
    target->pushWidget(widget, true);
    ASSERT_EQ(target->currentIndex(), 0);
};

TEST_F(ut_DStackWidget, getWidgetByIndex)
{
    QWidget *widget = new QWidget();
    target->pushWidget(widget, true);
    ASSERT_EQ(target->getWidgetByIndex(0), widget);
};

TEST_F(ut_DStackWidget, indexOf)
{
    QWidget *widget = new QWidget();
    target->pushWidget(widget, true);
    ASSERT_EQ(target->indexOf(widget), 0);
};

TEST_F(ut_DStackWidget, insertWidget)
{
    QWidget *widget = new QWidget();
    target->insertWidget(0, widget);
    ASSERT_EQ(target->depth(), 1);
};

TEST_F(ut_DStackWidget, popWidget)
{
    QWidget *widget = new QWidget();
    target->pushWidget(widget, true);
    target->popWidget(nullptr);
    ASSERT_EQ(target->depth(), 0);
};

TEST_F(ut_DStackWidget, pushWidget)
{
    QWidget *widget = new QWidget();
    target->pushWidget(widget, true);
    ASSERT_EQ(target->currentWidget(), widget);
};

TEST_F(ut_DStackWidget, setAnimationDuration)
{
    target->setAnimationDuration(1);
    ASSERT_EQ(target->animationDuration(), 1);
};

TEST_F(ut_DStackWidget, setAnimationType)
{
    target->setAnimationType(QEasingCurve::Linear);
    ASSERT_EQ(target->animationType(), QEasingCurve::Linear);
};

TEST_F(ut_DStackWidget, setTransition)
{
    auto transition = new DSlideStackWidgetTransition();
    target->setTransition(transition);
    ASSERT_EQ(target->transition(), transition);
};
