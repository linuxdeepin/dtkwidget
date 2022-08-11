// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
