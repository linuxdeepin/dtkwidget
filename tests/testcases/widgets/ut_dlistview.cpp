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

#include "dlistview.h"
DWIDGET_USE_NAMESPACE
class ut_DListView : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DListView();
        auto model = new DVariantListModel(target);
        target->setModel(model);
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DListView *target = nullptr;
};

TEST_F(ut_DListView, addFooterWidget)
{
    QWidget *widget = new QWidget();
    target->addFooterWidget(widget);
    ASSERT_EQ(target->getFooterWidget(0), widget);
};

TEST_F(ut_DListView, addHeaderWidget)
{
    QWidget *widget = new QWidget();
    target->addHeaderWidget(widget);
    ASSERT_EQ(target->getHeaderWidget(0), widget);
};

TEST_F(ut_DListView, addItem)
{
    target->addItem(1);
    ASSERT_EQ(target->count(), 1);
};

TEST_F(ut_DListView, addItems)
{
    target->addItems({1});
    ASSERT_EQ(target->count(), 1);
};

TEST_F(ut_DListView, insertItem)
{
    target->insertItem(0, 0);
    ASSERT_EQ(target->count(), 1);
};

TEST_F(ut_DListView, insertItems)
{
    target->insertItems(0, {0});
    ASSERT_EQ(target->count(), 1);
};

TEST_F(ut_DListView, isActiveRect)
{
    target->isActiveRect(QRect(0, 0, 100, 100));
};

TEST_F(ut_DListView, isVisualRect)
{
    target->isVisualRect(QRect(0, 0, 100, 100));
};

TEST_F(ut_DListView, itemMargins)
{
    target->itemMargins();
};

TEST_F(ut_DListView, removeFooterWidget)
{
    QWidget *widget = new QWidget();
    target->addFooterWidget(widget);
    target->removeFooterWidget(0);
    ASSERT_EQ(target->getFooterWidget(0), nullptr);
};

TEST_F(ut_DListView, removeHeaderWidget)
{
    QWidget *widget = new QWidget();
    target->addHeaderWidget(widget);
    target->removeHeaderWidget(0);
    ASSERT_EQ(target->getHeaderWidget(0), nullptr);
};

TEST_F(ut_DListView, removeItem)
{
    target->insertItem(0, 0);
    target->removeItem(0);
    ASSERT_EQ(target->count(), 0);
};

TEST_F(ut_DListView, removeItems)
{
    target->insertItem(0, 0);
    target->removeItems(0, 1);
    ASSERT_EQ(target->count(), 0);
};

TEST_F(ut_DListView, setBackgroundType)
{
    target->setBackgroundType(DStyledItemDelegate::NoBackground);
    ASSERT_EQ(target->backgroundType(), DStyledItemDelegate::NoBackground);
};

TEST_F(ut_DListView, setItemMargins)
{
    QMargins margin(1, 1, 1, 1);
    target->setItemMargins(margin);
    ASSERT_EQ(target->itemMargins(), margin);
};

TEST_F(ut_DListView, setItemRadius)
{
    target->setItemRadius(1);
    ASSERT_EQ(target->property("_d_dtk_frameRadius").toInt(), 1);
};

TEST_F(ut_DListView, setItemSize)
{
    target->setItemSize(QSize(1, 1));
    ASSERT_EQ(target->itemSize(), QSize(1, 1));
};

TEST_F(ut_DListView, setModel)
{
    auto model = new DVariantListModel(target);
    target->setModel(model);
    ASSERT_EQ(target->model(), model);
};

TEST_F(ut_DListView, setOrientation)
{
    target->setOrientation(QListView::LeftToRight, true);
    ASSERT_EQ(target->orientation(), Qt::Vertical);
};

TEST_F(ut_DListView, setViewportMargins)
{
    QMargins margin(1, 1, 1, 1);
    target->setViewportMargins(margin);
};

TEST_F(ut_DListView, takeFooterWidget)
{
    QWidget *widget = new QWidget();
    target->addFooterWidget(widget);
    auto widget2 = target->takeFooterWidget(0);
    ASSERT_EQ(widget2, widget);
    widget2->deleteLater();
};

TEST_F(ut_DListView, takeHeaderWidget)
{
    QWidget *widget = new QWidget();
    target->addHeaderWidget(widget);
    auto widget2 = target->takeHeaderWidget(0);
    ASSERT_EQ(widget2, widget);
    widget2->deleteLater();
};

class ut_DVariantListModel : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DVariantListModel();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DVariantListModel *target = nullptr;
};

TEST_F(ut_DVariantListModel, insertRows)
{
    target->insertRows(0, 1);
    ASSERT_EQ(target->rowCount(), 1);
};

TEST_F(ut_DVariantListModel, removeRows)
{
    target->insertRows(0, 1);
    target->removeRows(0, 1);
    ASSERT_EQ(target->rowCount(), 0);
};

TEST_F(ut_DVariantListModel, setData)
{
    target->insertRows(0, 1);
    target->setData(target->index(0, 0), 1, Qt::DisplayRole);
    ASSERT_EQ(target->data(target->index(0, 0)).toInt(), 1);
};
