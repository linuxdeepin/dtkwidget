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

#include <QIcon>
#include <QSignalSpy>

#include "dtabbar.h"
DWIDGET_USE_NAMESPACE
class ut_DTabBar : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DTabBar();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DTabBar *target = nullptr;
};

TEST_F(ut_DTabBar, addTab)
{
    auto index = target->addTab(QIcon::fromTheme("preferences-system"), "addTab");
    ASSERT_EQ(target->tabText(index), "addTab");
};

TEST_F(ut_DTabBar, addTab2)
{
    auto index = target->addTab("addTab");
    ASSERT_EQ(target->tabText(index), "addTab");
};

TEST_F(ut_DTabBar, insertTab)
{
    QSignalSpy spy(target, &DTabBar::tabIsInserted);
    target->insertTab(0, "insertTab");
    ASSERT_EQ(target->count(), 1);
    ASSERT_EQ(spy.count(), 1);
};

TEST_F(ut_DTabBar, insertTab2)
{
    target->insertTab(1, QIcon::fromTheme("preferences-system"), "insertTab");
    ASSERT_EQ(target->count(), 1);
};

TEST_F(ut_DTabBar, moveTab)
{
    target->insertTab(0, "insertTab1");
    target->insertTab(1, "insertTab2");
    QSignalSpy spy(target, &DTabBar::tabMoved);
    target->moveTab(1, 0);
    ASSERT_EQ(spy.count(), 1);
};

TEST_F(ut_DTabBar, removeTab)
{
    target->insertTab(0, "insertTab1");
    QSignalSpy spy(target, &DTabBar::tabIsRemoved);
    target->removeTab(0);
    ASSERT_EQ(spy.count(), 1);
};

TEST_F(ut_DTabBar, setAutoHide)
{
    target->setAutoHide(true);
    ASSERT_EQ(target->autoHide(), true);
};

TEST_F(ut_DTabBar, setChangeCurrentOnDrag)
{
    target->setChangeCurrentOnDrag(true);
    ASSERT_EQ(target->changeCurrentOnDrag(), true);
};

TEST_F(ut_DTabBar, setCurrentIndex)
{
    target->insertTab(0, "insertTab1");
    target->insertTab(1, "insertTab2");
    target->setCurrentIndex(1);
    ASSERT_EQ(target->currentIndex(), 1);
};

TEST_F(ut_DTabBar, setDocumentMode)
{
    target->setDocumentMode(true);
    ASSERT_EQ(target->documentMode(), true);
};

TEST_F(ut_DTabBar, setDragable)
{
    target->setDragable(true);
    ASSERT_EQ(target->isDragable(), true);
};

TEST_F(ut_DTabBar, setDrawBase)
{
    target->setDrawBase(true);
    ASSERT_EQ(target->drawBase(), true);
};

TEST_F(ut_DTabBar, setElideMode)
{
    target->setElideMode(Qt::ElideLeft);
    ASSERT_EQ(target->elideMode(), Qt::ElideLeft);
};

TEST_F(ut_DTabBar, setEnabledEmbedStyle)
{
    target->setEnabledEmbedStyle(true);
    ASSERT_EQ(target->property("_d_dtk_tabbartab_type").toBool(), true);
};

TEST_F(ut_DTabBar, setExpanding)
{
    target->setExpanding(true);
    ASSERT_EQ(target->expanding(), true);
};

TEST_F(ut_DTabBar, setFlashColor)
{
    target->setFlashColor(Qt::red);
    ASSERT_EQ(target->flashColor(), Qt::red);
};

TEST_F(ut_DTabBar, setIconSize)
{
    target->setIconSize(QSize(10, 10));
    ASSERT_EQ(target->iconSize(), QSize(10, 10));
};

TEST_F(ut_DTabBar, setMaskColor)
{
    target->setMaskColor(Qt::red);
    ASSERT_EQ(target->maskColor(), Qt::red);
};

TEST_F(ut_DTabBar, setMovable)
{
    target->setMovable(true);
    ASSERT_EQ(target->isMovable(), true);
};

TEST_F(ut_DTabBar, setSelectionBehaviorOnRemove)
{
    target->setSelectionBehaviorOnRemove(QTabBar::SelectLeftTab);
    ASSERT_EQ(target->selectionBehaviorOnRemove(), QTabBar::SelectLeftTab);
};

TEST_F(ut_DTabBar, setShape)
{
    target->setShape(QTabBar::RoundedNorth);
    ASSERT_EQ(target->shape(), QTabBar::RoundedNorth);
};

TEST_F(ut_DTabBar, setStartDragDistance)
{
    target->setStartDragDistance(1);
    ASSERT_EQ(target->startDragDistance(), 1);
};

TEST_F(ut_DTabBar, setTabButton)
{
    target->insertTab(0, "insertTab1");
    QWidget *widget = new QWidget();
    target->setTabButton(0, QTabBar::LeftSide, widget);
    ASSERT_EQ(target->tabButton(0, QTabBar::LeftSide), widget);
};

TEST_F(ut_DTabBar, setTabData)
{
    target->insertTab(0, "insertTab1");
    target->setTabData(0, 1);
    ASSERT_EQ(target->tabData(0).toInt(), 1);
};

TEST_F(ut_DTabBar, setTabEnabled)
{
    target->insertTab(0, "insertTab1");
    target->setTabEnabled(0, true);
    ASSERT_EQ(target->isTabEnabled(0), true);
};

TEST_F(ut_DTabBar, setTabIcon)
{
    target->insertTab(0, "insertTab1");
    target->setTabIcon(0, QIcon::fromTheme("preferences-system"));
    ASSERT_EQ(target->tabIcon(0).name(), QIcon::fromTheme("preferences-system").name());
};

TEST_F(ut_DTabBar, setTabLabelAlignment)
{
    target->setTabLabelAlignment(Qt::AlignLeft);
    ASSERT_EQ(target->property("_d_dtk_tabbar_alignment").toInt(), Qt::AlignLeft);
};

TEST_F(ut_DTabBar, setTabMaximumSize)
{
    target->insertTab(0, "insertTab1");
    target->setTabMaximumSize(0, QSize(10, 10));
};

TEST_F(ut_DTabBar, setTabMinimumSize)
{
    target->insertTab(0, "insertTab1");
    target->setTabMinimumSize(0, QSize(10, 10));
};

TEST_F(ut_DTabBar, setTabText)
{
    target->insertTab(0, "insertTab1");
    target->setTabText(0, "setTabText");
    ASSERT_EQ(target->tabText(0), "setTabText");
};

TEST_F(ut_DTabBar, setTabToolTip)
{
    target->insertTab(0, "insertTab1");
    target->setTabToolTip(0, "setTabToolTip");
    ASSERT_EQ(target->tabToolTip(0), "setTabToolTip");
};

TEST_F(ut_DTabBar, setTabWhatsThis)
{
    target->insertTab(0, "insertTab1");
    target->setTabWhatsThis(0, "setTabWhatsThis");
    ASSERT_EQ(target->tabWhatsThis(0), "setTabWhatsThis");
};

TEST_F(ut_DTabBar, setTabsClosable)
{
    target->setTabsClosable(true);
    ASSERT_EQ(target->tabsClosable(), true);
};

TEST_F(ut_DTabBar, setUsesScrollButtons)
{
    target->setUsesScrollButtons(true);
    ASSERT_EQ(target->usesScrollButtons(), true);
};

TEST_F(ut_DTabBar, setVisibleAddButton)
{
    target->setVisibleAddButton(true);
    ASSERT_EQ(target->visibleAddButton(), true);
};
