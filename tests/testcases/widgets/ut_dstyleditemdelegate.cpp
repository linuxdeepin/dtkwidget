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
#include <QListView>

#include "dstyleditemdelegate.h"
DWIDGET_USE_NAMESPACE
class ut_DStandardItem : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DStandardItem();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DStandardItem *target = nullptr;
};

TEST_F(ut_DStandardItem, setActionList)
{
    DViewItemActionList list;
    list.push_back(new DViewItemAction());
    target->setActionList(Qt::LeftEdge, list);
    ASSERT_EQ(target->actionList(Qt::LeftEdge).size(), list.size());
};

TEST_F(ut_DStandardItem, setBackgroundRole)
{
    QStandardItemModel* model = new QStandardItemModel();
    model->appendRow(target);
    target->setBackgroundRole(DPalette::ItemBackground);
    ASSERT_EQ(target->backgroundType(), DPalette::ItemBackground);
    model->deleteLater();
};

TEST_F(ut_DStandardItem, setBackgroundRole2)
{
    QStandardItemModel* model = new QStandardItemModel();
    model->appendRow(target);
    target->setBackgroundRole(QPalette::WindowText);
    ASSERT_EQ(target->backgroundRole(), QPalette::WindowText);
    model->deleteLater();
};

TEST_F(ut_DStandardItem, setFontSize)
{
    QStandardItemModel* model = new QStandardItemModel();
    model->appendRow(target);
    target->setFontSize(DFontSizeManager::T1);
    ASSERT_EQ(target->font(), DFontSizeManager::instance()->get(DFontSizeManager::T1));
    model->deleteLater();
};

TEST_F(ut_DStandardItem, setTextActionList)
{
    DViewItemActionList list;
    list.push_back(new DViewItemAction());
    target->setTextActionList(list);
    ASSERT_EQ(target->textActionList().size(), list.size());
};

TEST_F(ut_DStandardItem, setTextColorRole)
{
    QStandardItemModel* model = new QStandardItemModel();
    model->appendRow(target);
    target->setTextColorRole(DPalette::ItemBackground);
    ASSERT_EQ(target->textColorType(), DPalette::ItemBackground);
    model->deleteLater();
};

TEST_F(ut_DStandardItem, setTextColorRole2)
{
    QStandardItemModel* model = new QStandardItemModel();
    model->appendRow(target);
    target->setTextColorRole(QPalette::WindowText);
    ASSERT_EQ(target->textColorRole(), QPalette::WindowText);
    model->deleteLater();
};

class ut_DStyledItemDelegate : public testing::Test
{
protected:
    void SetUp() override
    {
        parent = new QListView();
        target = new DStyledItemDelegate(parent);
    }
    void TearDown() override
    {
        if (parent) {
            delete parent;
            parent = nullptr;
        }
    }
    DStyledItemDelegate *target = nullptr;
    QListView *parent = nullptr;
};

TEST_F(ut_DStyledItemDelegate, setBackgroundType)
{
    target->setBackgroundType(DStyledItemDelegate::ClipCornerBackground);
    ASSERT_EQ(target->backgroundType(), DStyledItemDelegate::ClipCornerBackground);
};

TEST_F(ut_DStyledItemDelegate, setItemSize)
{
    target->setItemSize(QSize(10, 10));
    ASSERT_EQ(target->itemSize(), QSize(10, 10));
};

TEST_F(ut_DStyledItemDelegate, setItemSpacing)
{
    target->setItemSpacing(1);
    ASSERT_EQ(target->spacing(), 1);
};

TEST_F(ut_DStyledItemDelegate, setMargins)
{
    QMargins margin(1, 1, 1, 1);
    target->setMargins(margin);
    ASSERT_EQ(target->margins(), margin);
};

TEST_F(ut_DStyledItemDelegate, paint)
{
    parent->setItemDelegate(target);
    QStandardItemModel* model = new QStandardItemModel();
    model->appendRow(new QStandardItem);
    parent->setModel(model);
    parent->repaint();
    parent->show();
    ASSERT_TRUE(parent->isVisible());
    model->deleteLater();
};

class ut_DViewItemAction : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DViewItemAction();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DViewItemAction *target = nullptr;
};

TEST_F(ut_DViewItemAction, setClickAreaMargins)
{
    QMargins margin(1, 1, 1, 1);
    target->setClickAreaMargins(margin);
    ASSERT_EQ(target->clickAreaMargins(), margin);
};

TEST_F(ut_DViewItemAction, setFontSize)
{
    target->setFontSize(DFontSizeManager::T1);
    ASSERT_EQ(target->font(), DFontSizeManager::instance()->get(DFontSizeManager::T1, target->QAction::font()));
};

TEST_F(ut_DViewItemAction, setTextColorRole)
{
    target->setTextColorRole(QPalette::WindowText);
    ASSERT_EQ(target->textColorRole(), QPalette::WindowText);
};

TEST_F(ut_DViewItemAction, setTextColorRole2)
{
    target->setTextColorRole(DPalette::WindowText);
    ASSERT_EQ(target->textColorRole(), DPalette::WindowText);
};

TEST_F(ut_DViewItemAction, setWidget)
{
    QWidget *widget = new QWidget();
    target->setWidget(widget);
    ASSERT_EQ(target->widget(), widget);
    widget->deleteLater();
};
