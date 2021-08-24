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

#include "darrowrectangle.h"
DWIDGET_USE_NAMESPACE
class ut_DArrowRectangle : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DArrowRectangle(DArrowRectangle::ArrowLeft);
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DArrowRectangle *target = nullptr;
};

TEST_F(ut_DArrowRectangle, move)
{
    target->move(10, 10);
    ASSERT_EQ(target->pos().x(), 10);
};

TEST_F(ut_DArrowRectangle, resizeWithContent)
{
    target->resizeWithContent();
};

TEST_F(ut_DArrowRectangle, setArrowDirection)
{
    target->setArrowDirection(DArrowRectangle::ArrowRight);
    ASSERT_EQ(target->arrowDirection(), DArrowRectangle::ArrowRight);
};

TEST_F(ut_DArrowRectangle, setArrowHeight)
{
    target->setArrowHeight(1);
    ASSERT_EQ(target->arrowHeight(), 1);
};

TEST_F(ut_DArrowRectangle, setArrowWidth)
{
    target->setArrowWidth(1);
    ASSERT_EQ(target->arrowWidth(), 1);
};

TEST_F(ut_DArrowRectangle, setArrowX)
{
    target->setArrowX(1);
    ASSERT_EQ(target->arrowX(), 1);
};

TEST_F(ut_DArrowRectangle, setArrowY)
{
    target->setArrowY(1);
    ASSERT_EQ(target->arrowY(), 1);
};

TEST_F(ut_DArrowRectangle, setBackgroundColor)
{
    target->setBackgroundColor(Qt::red);
    ASSERT_EQ(target->backgroundColor(), Qt::red);
};

TEST_F(ut_DArrowRectangle, setBackgroundColor2)
{
    target->setBackgroundColor(DBlurEffectWidget::DarkColor);
};

TEST_F(ut_DArrowRectangle, setBorderColor)
{
    target->setBorderColor(Qt::red);
    ASSERT_EQ(target->borderColor(), Qt::red);
};

TEST_F(ut_DArrowRectangle, setBorderWidth)
{
    target->setBorderWidth(1);
    ASSERT_EQ(target->borderWidth(), 1);
};

TEST_F(ut_DArrowRectangle, setContent)
{
    auto con = new QWidget;
    target->setContent(con);
    ASSERT_EQ(target->getContent(), con);
};

TEST_F(ut_DArrowRectangle, setHeight)
{
    target->setHeight(1);
    ASSERT_EQ(target->height(), 1);
};

TEST_F(ut_DArrowRectangle, setLeftRightRadius)
{
    target->setLeftRightRadius(true);
};

TEST_F(ut_DArrowRectangle, setMargin)
{
    target->setMargin(1);
    ASSERT_EQ(target->margin(), 1);
};

TEST_F(ut_DArrowRectangle, setRadius)
{
    target->setRadius(0);
    ASSERT_EQ(target->radius(), 0);
};

TEST_F(ut_DArrowRectangle, setRadiusArrowStyleEnable)
{
    target->setRadiusArrowStyleEnable(true);
};

TEST_F(ut_DArrowRectangle, setShadowBlurRadius)
{
    target->setShadowBlurRadius(1);
    ASSERT_EQ(target->shadowBlurRadius(), 1);
};

TEST_F(ut_DArrowRectangle, setShadowXOffset)
{
    target->setShadowXOffset(1);
    ASSERT_EQ(target->shadowXOffset(), 1);
};

TEST_F(ut_DArrowRectangle, setShadowYOffset)
{
    target->setShadowYOffset(1);
    ASSERT_EQ(target->shadowYOffset(), 1);
};

TEST_F(ut_DArrowRectangle, setWidth)
{
    target->setWidth(1);
    ASSERT_EQ(target->width(), 1);
};
