// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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

class LIBDTKWIDGETSHARED_EXPORT TestListItem : public DSimpleListItem {
public:
    bool isSelectTest = false;
    selectType type = selectSingle;
    int idx = 0;

    TestListItem() {}
    virtual ~TestListItem() override {}

    bool sameAs(DSimpleListItem *item) override {
        return type == static_cast<TestListItem *>(item)->type;
    }

    void drawBackground(QRect rect, QPainter *painter, int index, bool isSelect, bool isHover) override {
        Q_UNUSED(index)
        Q_UNUSED(isHover)
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
        Q_UNUSED(column)
        Q_UNUSED(index)
        Q_UNUSED(isHover)
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
    widget->deleteLater();
}

TEST_F(ut_DSimpleListView, testDSimpleListViewSelectFirst)
{
    if (!qgetenv("QT_QPA_PLATFORM").isEmpty()) {
        return;
    }
    int idx = 0;
    TestListItem item[4];
    QList<DSimpleListItem *> itemList;
    for (TestListItem &i : item) {
        itemList << &i;
        i.isSelectTest = true;
        i.type = selectFirst;
        i.idx = idx++;
    }

    listView->addItems(itemList);
    listView->selectFirstItem();
    widget->show();
}

TEST_F(ut_DSimpleListView, testDSimpleListViewSelectLast)
{
    if (!qgetenv("QT_QPA_PLATFORM").isEmpty()) {
        return;
    }
    int idx = 0;
    TestListItem item[4];
    QList<DSimpleListItem *> itemList;
    for (TestListItem &i : item) {
        itemList << &i;
        i.isSelectTest = true;
        i.type = selectLast;
        i.idx = idx++;
    }

    listView->addItems(itemList);
    listView->selectLastItem();
    widget->show();
}

TEST_F(ut_DSimpleListView, testDSimpleListViewSelectAll)
{
    if (!qgetenv("QT_QPA_PLATFORM").isEmpty()) {
        return;
    }
    int idx = 0;
    TestListItem item[4];
    QList<DSimpleListItem *> itemList;

    for (TestListItem &i : item) {
        itemList << &i;
        i.isSelectTest = true;
        i.type = selectAll;
        i.idx = idx++;
    }

    listView->addItems(itemList);
    listView->selectAllItems();
    widget->show();
}

TEST_F(ut_DSimpleListView, testDSimpleListViewSelectPrev)
{
    if (!qgetenv("QT_QPA_PLATFORM").isEmpty()) {
        return;
    }
    int idx = 0;
    TestListItem item[4];
    QList<DSimpleListItem *> itemList;

    for (TestListItem &i : item) {
        itemList << &i;
        i.isSelectTest = true;
        i.type = selectPrev;
        i.idx = idx++;
    }

    listView->addItems(itemList);
    listView->selectLastItem();
    listView->selectPrevItem();
    widget->show();
}
