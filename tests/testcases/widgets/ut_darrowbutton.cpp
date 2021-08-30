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

#include <QSignalSpy>
#include <QTest>

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

TEST_F(ut_DArrowButton, mouseEvent)
{
    target->setGeometry(QRect(0, 0, 100, 100));
    target->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(target, 100));

    QSignalSpy mousePressSpy(target, &DArrowButton::mousePress);
    QTest::mousePress(target, Qt::LeftButton);
    ASSERT_EQ(target->buttonState(), DArrowButton::ArrowStatePress);
    ASSERT_EQ(mousePressSpy.count(), 1);

    QSignalSpy mouseReleaseSpy(target, &DArrowButton::mouseRelease);
    QTest::mouseRelease(target, Qt::LeftButton);
    ASSERT_EQ(target->buttonState(), DArrowButton::ArrowStateNormal);
    ASSERT_EQ(mouseReleaseSpy.count(), 1);

    QSignalSpy mouseEnterSpy(target, &DArrowButton::mouseEnter);
    QTest::mouseMove(target);
    ASSERT_EQ(mouseEnterSpy.count(), 1);

    QSignalSpy mouseLeaveSpy(target, &DArrowButton::mouseLeave);
    QTest::mouseMove(target);
    QPoint outPoint(target->pos() + QPoint(target->x(), target->y() + target->height()));
    QTest::mouseMove(target, outPoint);
    ASSERT_EQ(mouseLeaveSpy.count(), 1);
};

