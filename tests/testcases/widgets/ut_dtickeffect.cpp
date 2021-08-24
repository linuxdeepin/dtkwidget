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
#include <QWidget>

#include "dtickeffect.h"
DWIDGET_USE_NAMESPACE
class ut_DTickEffect : public testing::Test
{
protected:
    void SetUp() override
    {
        effectWidget = new QWidget();
        target = new DTickEffect(effectWidget);
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
        if (effectWidget) {
            effectWidget->deleteLater();
        }
    }
    DTickEffect *target = nullptr;
    QWidget *effectWidget = nullptr;
};

TEST_F(ut_DTickEffect, pause)
{
    target->play();
    QSignalSpy spy(target, &DTickEffect::stateChanged);
    target->pause();
    ASSERT_EQ(spy.count(), 1);
};

TEST_F(ut_DTickEffect, play)
{
    QSignalSpy spy(target, &DTickEffect::stateChanged);
    target->play();
    ASSERT_EQ(spy.count(), 1);
};

TEST_F(ut_DTickEffect, resume)
{
    target->play();
    target->pause();
    QSignalSpy spy(target, &DTickEffect::stateChanged);
    target->resume();
    ASSERT_EQ(spy.count(), 1);
};

TEST_F(ut_DTickEffect, stop)
{
    target->play();
    QSignalSpy spy(target, &DTickEffect::stateChanged);
    target->stop();
    ASSERT_EQ(spy.count(), 1);
};
