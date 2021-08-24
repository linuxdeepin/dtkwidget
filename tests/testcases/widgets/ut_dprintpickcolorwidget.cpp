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

#include "dprintpickcolorwidget.h"
DWIDGET_USE_NAMESPACE
class ut_ColorButton : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new ColorButton(Qt::red);
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    ColorButton *target = nullptr;
};

class ut_ColorLabel : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new ColorLabel();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    ColorLabel *target = nullptr;
};

TEST_F(ut_ColorLabel, pickColor)
{
    QSignalSpy spy(target, &ColorLabel::pickColor);
    target->pickColor(QPoint(10, 10));
    ASSERT_EQ(spy.count(), 1);
};

class ut_ColorSlider : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new ColorSlider();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    ColorSlider *target = nullptr;
};

TEST_F(ut_ColorSlider, getColor)
{
  
};

class ut_DPrintPickColorWidget : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DPrintPickColorWidget();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DPrintPickColorWidget *target = nullptr;
};

TEST_F(ut_DPrintPickColorWidget, convertColor)
{
    target->convertColor(Qt::red, true);
};

TEST_F(ut_DPrintPickColorWidget, initConnection)
{
    target->initConnection();
};

TEST_F(ut_DPrintPickColorWidget, initUI)
{
    target->initUI();
};

TEST_F(ut_DPrintPickColorWidget, setRgbEdit)
{
    target->setRgbEdit(Qt::red, true);
};

TEST_F(ut_DPrintPickColorWidget, slotColorPick)
{
    target->slotColorPick("slotColorPick", "slotColorPick");
};

TEST_F(ut_DPrintPickColorWidget, slotEditColor)
{
    target->slotEditColor("slotEditColor");
};
