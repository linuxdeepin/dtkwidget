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
#include <DLineEdit>
#include <DApplicationHelper>

#include "dstyleoption.h"
DWIDGET_USE_NAMESPACE
class ut_DFontSizeManager : public testing::Test
{
protected:
    void SetUp() override
    {
        target = DFontSizeManager::instance();
    }
    void TearDown() override
    {
    }
    DFontSizeManager *target = nullptr;
};

TEST_F(ut_DFontSizeManager, bind)
{
    QWidget *widget = new QWidget();
    target->bind(widget, DFontSizeManager::T1);
    ASSERT_EQ(widget->font().pixelSize(), target->fontPixelSize(DFontSizeManager::T1));
    widget->deleteLater();
};

TEST_F(ut_DFontSizeManager, bind2)
{
    QWidget *widget = new QWidget();
    target->bind(widget, DFontSizeManager::T1, 1);
    ASSERT_EQ(widget->font().pixelSize(), target->fontPixelSize(DFontSizeManager::T1));
    ASSERT_EQ(widget->font().weight(), 1);
    widget->deleteLater();
};

TEST_F(ut_DFontSizeManager, setFontPixelSize)
{
    quint16 originSize = target->fontPixelSize(DFontSizeManager::T1);
    target->setFontPixelSize(DFontSizeManager::T1, 10);
    target->setFontPixelSize(DFontSizeManager::T1, originSize);
};

TEST_F(ut_DFontSizeManager, t1)
{
    ASSERT_EQ(target->t1().pixelSize(), target->fontPixelSize(DFontSizeManager::T1));
};

TEST_F(ut_DFontSizeManager, t10)
{
    ASSERT_EQ(target->t10().pixelSize(), target->fontPixelSize(DFontSizeManager::T10));
};

TEST_F(ut_DFontSizeManager, t2)
{
    ASSERT_EQ(target->t2().pixelSize(), target->fontPixelSize(DFontSizeManager::T2));
};

TEST_F(ut_DFontSizeManager, t3)
{
    ASSERT_EQ(target->t3().pixelSize(), target->fontPixelSize(DFontSizeManager::T3));
};

TEST_F(ut_DFontSizeManager, t4)
{
    ASSERT_EQ(target->t4().pixelSize(), target->fontPixelSize(DFontSizeManager::T4));
};

TEST_F(ut_DFontSizeManager, t5)
{
    ASSERT_EQ(target->t5().pixelSize(), target->fontPixelSize(DFontSizeManager::T5));
};

TEST_F(ut_DFontSizeManager, t6)
{
    ASSERT_EQ(target->t6().pixelSize(), target->fontPixelSize(DFontSizeManager::T6));
};

TEST_F(ut_DFontSizeManager, t7)
{
    ASSERT_EQ(target->t7().pixelSize(), target->fontPixelSize(DFontSizeManager::T7));
};

TEST_F(ut_DFontSizeManager, t8)
{
    ASSERT_EQ(target->t8().pixelSize(), target->fontPixelSize(DFontSizeManager::T8));
};

TEST_F(ut_DFontSizeManager, t9)
{
    ASSERT_EQ(target->t9().pixelSize(), target->fontPixelSize(DFontSizeManager::T9));
};

class ut_DStyleOptionBackgroundGroup : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DStyleOptionBackgroundGroup();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DStyleOptionBackgroundGroup *target = nullptr;
};

TEST_F(ut_DStyleOptionBackgroundGroup, init)
{
    QWidget *widget = new QWidget();
    target->init(widget);
    ASSERT_EQ(target->palette, DApplicationHelper::instance()->palette(widget));
    widget->deleteLater();
};


class ut_DStyleOptionLineEdit : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DStyleOptionLineEdit();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DStyleOptionLineEdit *target = nullptr;
};

TEST_F(ut_DStyleOptionLineEdit, Q_DECLARE_FLAGS)
{
    DLineEdit *widget = new DLineEdit();
    widget->setAlert(true);
    target->init(widget);
    ASSERT_TRUE(target->features & DStyleOptionLineEdit::Alert);
    widget->deleteLater();
};
