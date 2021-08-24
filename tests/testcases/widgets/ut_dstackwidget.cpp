/*
* Copyright (C) 2021 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     Ye ShanShan <yeshanshan@uniontech.com>
*
* Maintainer: Ye ShanShan <yeshanshan@uniontech.com>>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
