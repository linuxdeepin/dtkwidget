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
