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

#include "dflowlayout.h"
DWIDGET_USE_NAMESPACE
class ut_DFlowLayout : public testing::Test
{
protected:
    void SetUp() override
    {
        holder = new QWidget;
        target = new DFlowLayout(holder);
    }
    void TearDown() override
    {
        if (holder) {
            delete holder;
            holder = nullptr;
        }
    }
    QWidget * holder = nullptr;
    DFlowLayout *target = nullptr;
};

TEST_F(ut_DFlowLayout, addItem)
{
    QSpacerItem *item = new QSpacerItem(1, 1);
    target->addItem(item);
    ASSERT_EQ(target->itemAt(0), item);
};

TEST_F(ut_DFlowLayout, addSpacerItem)
{
    QSpacerItem *item = new QSpacerItem(1, 1);
    target->addSpacerItem(item);
    ASSERT_EQ(target->itemAt(0), item);
};

TEST_F(ut_DFlowLayout, addStretch)
{
    target->addStretch(1);
    ASSERT_EQ(target->count(), 1);
};

TEST_F(ut_DFlowLayout, expandingDirections)
{
    ASSERT_EQ(target->expandingDirections(), Qt::Vertical);
};

TEST_F(ut_DFlowLayout, insertItem)
{
    QSpacerItem *item = new QSpacerItem(1, 1);
    target->insertItem(0, item);
    ASSERT_EQ(target->count(), 1);
};

TEST_F(ut_DFlowLayout, insertLayout)
{
    auto item = new QHBoxLayout();
    target->insertLayout(0, item);
    ASSERT_EQ(target->count(), 1);
};

TEST_F(ut_DFlowLayout, insertSpacerItem)
{
    QSpacerItem *item = new QSpacerItem(1, 1);
    target->insertSpacerItem(0, item);
    ASSERT_EQ(target->count(), 1);
};

TEST_F(ut_DFlowLayout, insertSpacing)
{
    target->insertSpacing(0, 1);
    ASSERT_EQ(target->count(), 1);
};

TEST_F(ut_DFlowLayout, insertStretch)
{
    target->insertStretch(0, 1);
    ASSERT_EQ(target->count(), 1);
};

TEST_F(ut_DFlowLayout, insertWidget)
{
    QWidget *widget = new QWidget();
    target->insertWidget(0, widget);
    ASSERT_EQ(target->count(), 1);
};

TEST_F(ut_DFlowLayout, minimumSize)
{
    {
        QSpacerItem *item = new QSpacerItem(1, 1);
        target->addSpacerItem(item);
    }
    {
        QSpacerItem *item = new QSpacerItem(2, 2);
        target->addSpacerItem(item);
    }
    ASSERT_GE(target->minimumSize().height(), QSize(2, 2).height());
};

TEST_F(ut_DFlowLayout, setFlow)
{
    target->setFlow(QListView::LeftToRight);
    ASSERT_EQ(target->flow(), QListView::LeftToRight);
};

TEST_F(ut_DFlowLayout, setGeometry)
{
    target->setGeometry(QRect(0, 0, 100, 100));
    ASSERT_EQ(target->geometry().topLeft(), QRect(0, 0, 100, 100).topLeft());
};

TEST_F(ut_DFlowLayout, setHorizontalSpacing)
{
    target->setHorizontalSpacing(1);
    ASSERT_EQ(target->horizontalSpacing(), 1);
};

TEST_F(ut_DFlowLayout, setSpacing)
{
    target->setSpacing(1);
    ASSERT_EQ(target->horizontalSpacing(), 1);
    ASSERT_EQ(target->verticalSpacing(), 1);
};

TEST_F(ut_DFlowLayout, setVerticalSpacing)
{
    target->setVerticalSpacing(1);
    ASSERT_EQ(target->verticalSpacing(), 1);
};

TEST_F(ut_DFlowLayout, takeAt)
{
    QSpacerItem *item = new QSpacerItem(1, 1);
    target->addSpacerItem(item);
    auto item1 = target->takeAt(0);
    ASSERT_EQ(item, item1);
    delete item1;
};
