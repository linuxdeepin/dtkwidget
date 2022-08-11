// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dloadingindicator.h"
DWIDGET_USE_NAMESPACE
class ut_DLoadingIndicator : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DLoadingIndicator();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DLoadingIndicator *target = nullptr;
};

TEST_F(ut_DLoadingIndicator, setAniDuration)
{
    target->setAniDuration(1);
    ASSERT_EQ(target->aniDuration(), 1);
};

TEST_F(ut_DLoadingIndicator, setAniEasingCurve)
{
    QEasingCurve curve;
    curve.setType(QEasingCurve::Linear);
    target->setAniEasingCurve(curve);
    ASSERT_EQ(target->aniEasingType(), curve.type());
};

TEST_F(ut_DLoadingIndicator, setAniEasingType)
{
    target->setAniEasingType(QEasingCurve::Linear);
    ASSERT_EQ(target->aniEasingType(), 0);
};

TEST_F(ut_DLoadingIndicator, setBackgroundColor)
{
    target->setBackgroundColor(Qt::red);
    ASSERT_EQ(target->backgroundColor(), Qt::red);
};

TEST_F(ut_DLoadingIndicator, setDirection)
{
    target->setDirection(DLoadingIndicator::Clockwise);
    ASSERT_EQ(target->direction(), DLoadingIndicator::Clockwise);
};

TEST_F(ut_DLoadingIndicator, setImageSource)
{
    target->setImageSource(QPixmap(50, 50));
    ASSERT_EQ(target->imageSource().width(), QPixmap(50, 50).width());
};

TEST_F(ut_DLoadingIndicator, setLoading)
{
    target->setLoading(true);
    ASSERT_EQ(target->loading(), true);
};

TEST_F(ut_DLoadingIndicator, setRotate)
{
    target->setRotate(0);
    ASSERT_EQ(target->rotate(), 0);
};

TEST_F(ut_DLoadingIndicator, setSmooth)
{
    target->setSmooth(true);
    ASSERT_EQ(target->smooth(), true);
};

TEST_F(ut_DLoadingIndicator, setWidgetSource)
{
    QWidget *widget = new QWidget();
    target->setWidgetSource(widget);
    ASSERT_EQ(target->widgetSource(), widget);
};
