// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later


#include <gtest/gtest.h>
#include <QListView>
#include <QPointer>

#include "dstyleditemdelegate.h"
DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE
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

TEST_F(ut_DViewItemAction, actionDestoryByDStandItem)
{
    QStandardItemModel* model = new QStandardItemModel();
    QPointer<DViewItemAction> actionPointer(new DViewItemAction());
    ASSERT_TRUE(actionPointer);

    DStandardItem *item = new DStandardItem();
    item->setActionList(Qt::RightEdge, {actionPointer});
    model->appendRow(item);

    QPointer<DViewItemAction> actionPointer2(new DViewItemAction());
    item->setActionList(Qt::RightEdge, {actionPointer2});
    ASSERT_FALSE(actionPointer);

    // release now avoid DStandardItem is clear in next event loop.
    delete model;

    ASSERT_FALSE(actionPointer2);
}

TEST_F(ut_DViewItemAction, actionDestoryByDStandItemWithClone)
{
    DStandardItem *item = new DStandardItem();
    QPointer<DViewItemAction> actionPointer(new DViewItemAction());
    item->setActionList(Qt::RightEdge, {actionPointer});

    QStandardItem *item2 = item->clone();
    delete item;
    ASSERT_TRUE(actionPointer);
    delete item2;
    ASSERT_FALSE(actionPointer);
}

TEST_F(ut_DViewItemAction, accessActionByActionList)
{
    QStandardItemModel* model = new QStandardItemModel();
    DViewItemAction *action = new DViewItemAction();

    DStandardItem *item = new DStandardItem();
    item->setActionList(Qt::RightEdge, {action});
    model->appendRow(item);

    auto itemModel = dynamic_cast<DStandardItem *>(model->item(0));
    ASSERT_TRUE(itemModel);

    ASSERT_TRUE(itemModel->actionList(Qt::RightEdge).contains(action));

    model->deleteLater();
}
