/*
 * Copyright (C) 2021 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     Wang Peng <wangpenga@uniontech.com>
 *
 * Maintainer: Wang Peng <wangpenga@uniontech.com>
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
#include <QTest>
#include <QDebug>

#include "dsimplelistview.h"
#include "dsimplelistitem.h"
#include "dlabel.h"

DWIDGET_USE_NAMESPACE

// std::atomic<bool> g_isSelectTest(false);
enum selectType {
    selectAll, selectPrev, selectLast, selectFirst, selectSingle
};

class TestListItem : public DSimpleListItem {
public:
    bool isSelectTest = false;
    selectType type = selectSingle;
    int idx = 0;

    TestListItem() {}
    virtual ~TestListItem() {}

    bool sameAs(DSimpleListItem *item) override {
        return type == static_cast<TestListItem *>(item)->type;
    }

    void drawBackground(QRect rect, QPainter *painter, int index, bool isSelect, bool isHover) override {
        QPen pen;
        pen.setColor(QColor(Qt::green));
        pen.setWidth(50);
        painter->setPen(pen);
        painter->setBrush(QBrush(Qt::green));
        painter->drawRoundRect(rect);
        if (isSelectTest) {
            SelectedTest(isSelect);
        }
    }

    void drawForeground(QRect rect, QPainter *painter, int column, int index, bool isSelect, bool isHover) override {
        QPen pen;
        pen.setColor(QColor(Qt::red));
        pen.setWidth(5);
        painter->setPen(pen);
        painter->setBrush(QBrush(Qt::red));
        painter->drawRoundedRect(rect, 0, 0);
        if (isSelectTest) {
            SelectedTest(isSelect);
        }
    }

    void SelectedTest(bool isSelect) {
        switch (type) {
        case selectAll: {
            ASSERT_TRUE(isSelect);
        }
            break;
        case selectPrev: {
            if (idx == 2) {
                ASSERT_TRUE(isSelect);
            } else {
                ASSERT_FALSE(isSelect);
            }
        }
            break;
        case selectLast: {
            if (idx == 3) {
                ASSERT_TRUE(isSelect);
            } else {
                ASSERT_FALSE(isSelect);
            }
        }
            break;
        case selectFirst: {
            if (idx == 0) {
                ASSERT_TRUE(isSelect);
            } else {
                ASSERT_FALSE(isSelect);
            }
        }
            break;
        case selectSingle: {
            static int count = 0;
            if (isSelect) {
                count++;
            }
            ASSERT_TRUE(count < 2);
        }
            break;
        default:
            ASSERT_TRUE(false);
        }
    }
};

class ut_DSimpleListView : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DSimpleListView *listView = nullptr;
    QWidget *widget = nullptr;

    void addItems(const selectType& type, const int count = 4)
    {
        QList<DSimpleListItem *> itemList;
        for (int i = 0; i < count; i++) {
            TestListItem *item = new TestListItem();
            item->isSelectTest = true;
            item->type = type;
            item->idx = i;
            itemList << item;
        }

        listView->addItems(itemList);
    }
};

void ut_DSimpleListView::SetUp()
{
    widget = new QWidget;
    listView = new DSimpleListView(widget);
    QSize szie(300, 200);
    widget->resize(szie);
    listView->resize(szie);
}

void ut_DSimpleListView::TearDown()
{
    listView->clearItems();
    widget->deleteLater();
}

TEST_F(ut_DSimpleListView, testDSimpleListViewSelectFirst)
{
    addItems(selectFirst);
    listView->selectFirstItem();
    widget->show();
}

TEST_F(ut_DSimpleListView, testDSimpleListViewSelectLast)
{
    addItems(selectLast);
    listView->selectLastItem();
    widget->show();
}

TEST_F(ut_DSimpleListView, testDSimpleListViewSelectAll)
{
    addItems(selectAll);
    listView->selectAllItems();
    widget->show();
}

TEST_F(ut_DSimpleListView, testDSimpleListViewSelectPrev)
{
    addItems(selectPrev);
    listView->selectLastItem();
    listView->selectPrevItem();
    widget->show();
}

TEST_F(ut_DSimpleListView, mouseEvent)
{
    addItems(selectFirst);
    listView->selectFirstItem();
    widget->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(widget, 100));

    QTest::mousePress(widget, Qt::LeftButton);

    QTest::mouseRelease(widget, Qt::LeftButton);

    QTest::mousePress(widget, Qt::LeftButton);

    QTest::mouseMove(listView);
    QPoint outPoint(listView->pos() + QPoint(listView->x(), listView->y() + listView->height()));
    QTest::mouseMove(listView, outPoint);
}

TEST_F(ut_DSimpleListView, keyEvent)
{
    addItems(selectFirst);
    listView->selectFirstItem();
    widget->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(widget, 100));

    QTest::keyClick(widget, Qt::Key_Home);
    QTest::keyClick(widget, Qt::Key_Home, Qt::ControlModifier);
    QTest::keyClick(widget, Qt::Key_Home, Qt::ShiftModifier);

    QTest::keyClick(widget, Qt::Key_End);
    QTest::keyClick(widget, Qt::Key_End, Qt::ControlModifier);
    QTest::keyClick(widget, Qt::Key_End, Qt::ShiftModifier);

    QTest::keyClick(widget, Qt::Key_Up);
    QTest::keyClick(widget, Qt::Key_Up, Qt::ShiftModifier);

    QTest::keyClick(widget, Qt::Key_Down);
    QTest::keyClick(widget, Qt::Key_Down, Qt::ShiftModifier);


    QTest::keyClick(widget, Qt::Key_PageUp);
    QTest::keyClick(widget, Qt::Key_PageUp, Qt::ControlModifier);
    QTest::keyClick(widget, Qt::Key_PageUp, Qt::ShiftModifier);

    QTest::keyClick(widget, Qt::Key_PageDown);
    QTest::keyClick(widget, Qt::Key_PageUp, Qt::ControlModifier);
    QTest::keyClick(widget, Qt::Key_PageUp, Qt::ShiftModifier);
}
