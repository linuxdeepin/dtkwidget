// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QSignalSpy>
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

//TEST_F(ut_ColorLabel, pickColor)
//{
//    QSignalSpy spy(target, &ColorLabel::pickColor);
//    target->pickColor(QPoint(10, 10));
//    ASSERT_EQ(spy.count(), 1);
//};

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
        target = new DPrintPickColorWidget(nullptr);
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
