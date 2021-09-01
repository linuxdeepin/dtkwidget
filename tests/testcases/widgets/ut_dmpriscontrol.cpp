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

#include "private/dmpriscontrol_p.h"
#include "dmpriscontrol.h"
DWIDGET_USE_NAMESPACE
class ut_DMPRISControl : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DMPRISControl();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DMPRISControl *target = nullptr;
};

TEST_F(ut_DMPRISControl, isWorking)
{
    DMPRISControlPrivate* d = target->d_func();
    ASSERT_EQ(target->isWorking(), d->m_mprisInter != nullptr);
};

TEST_F(ut_DMPRISControl, setPictureVisible)
{
    target->setPictureVisible(true);
    ASSERT_EQ(target->d_func()->m_pictureVisible, true);
};

TEST_F(ut_DMPRISControl, setPictureSize)
{
    target->setPictureSize(QSize(10, 10));
    ASSERT_EQ(target->d_func()->m_picture->size(), QSize(10, 10));
};

TEST_F(ut_DMPRISControl, event)
{
    target->setGeometry(0, 0, 100, 100);
    target->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(target, 100));

    {
        DFloatingButton *btn = target->d_func()->m_prevBtn;
        QSignalSpy spy(btn, &DFloatingButton::clicked);
        QTest::mouseClick(btn, Qt::LeftButton);
        ASSERT_EQ(spy.count(), 1);
    }
    {
        DFloatingButton *btn = target->d_func()->m_playBtn;
        QSignalSpy spy(btn, &DFloatingButton::clicked);
        QTest::mouseClick(btn, Qt::LeftButton);
        ASSERT_EQ(spy.count(), 1);
    }
};
