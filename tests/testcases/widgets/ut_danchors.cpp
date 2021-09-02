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

#include <QLabel>
#include <QSignalSpy>

#include "danchors.h"
DWIDGET_USE_NAMESPACE

class ut_DAnchorsBase : public testing::Test
{
protected:
    void SetUp() override
    {
        parentWidget = new QWidget();
        target = new DAnchorsBase(parentWidget);
    }
    void TearDown() override
    {
        if (parentWidget) {
            delete parentWidget;
            parentWidget = nullptr;
        }
    }
    DAnchorsBase *target = nullptr;
    QWidget *parentWidget = nullptr;
};

TEST_F(ut_DAnchorsBase, setBottomMargin)
{
    target->setBottomMargin(1);
    ASSERT_EQ(target->bottomMargin(), 1);
};

TEST_F(ut_DAnchorsBase, setEnabled)
{
    target->setEnabled(true);
    ASSERT_EQ(target->enabled(), true);
};

TEST_F(ut_DAnchorsBase, setHorizontalCenterOffset)
{
    target->setHorizontalCenterOffset(1);
    ASSERT_EQ(target->horizontalCenterOffset(), 1);
};

TEST_F(ut_DAnchorsBase, setLeftMargin)
{
    target->setLeftMargin(1);
    ASSERT_EQ(target->leftMargin(), 1);
};

TEST_F(ut_DAnchorsBase, setMargins)
{
    target->setMargins(1);
    ASSERT_EQ(target->margins(), 1);
};

TEST_F(ut_DAnchorsBase, setRightMargin)
{
    target->setRightMargin(1);
    ASSERT_EQ(target->rightMargin(), 1);
};

TEST_F(ut_DAnchorsBase, setTopMargin)
{
    target->setTopMargin(1);
    ASSERT_EQ(target->topMargin(), 1);
};

TEST_F(ut_DAnchorsBase, setVerticalCenterOffset)
{
    target->setVerticalCenterOffset(1);
    ASSERT_EQ(target->verticalCenterOffset(), 1);
};

TEST_F(ut_DAnchorsBase, setAnchorInfo)
{
    QWidget * pw = new QWidget();
    {
        DAnchorsBase rect1(new QLabel("rect1", pw));
        DAnchorsBase rect2(new QLabel("rect2", pw));
        QSignalSpy spy(&rect1, &DAnchorsBase::topChanged);
        ASSERT_TRUE(rect1.setTop(rect2.top()));
        ASSERT_EQ(spy.count(), 1);
    }
    {
        DAnchorsBase rect1(new QLabel("rect1", pw));
        DAnchorsBase rect2(new QLabel("rect2", pw));
        QSignalSpy spy(&rect1, &DAnchorsBase::leftChanged);
        ASSERT_TRUE(rect1.setLeft(rect2.left()));
        ASSERT_EQ(spy.count(), 1);
    }
    {
        DAnchorsBase rect1(new QLabel("rect1", pw));
        DAnchorsBase rect2(new QLabel("rect2", pw));
        QSignalSpy spy(&rect1, &DAnchorsBase::rightChanged);
        ASSERT_TRUE(rect1.setRight(rect2.right()));
        ASSERT_EQ(spy.count(), 1);
    }
    {
        DAnchorsBase rect1(new QLabel("rect1", pw));
        DAnchorsBase rect2(new QLabel("rect2", pw));
        QSignalSpy spy(&rect1, &DAnchorsBase::bottomChanged);
        ASSERT_TRUE(rect1.setBottom(rect2.bottom()));
        ASSERT_EQ(spy.count(), 1);
    }
    {
        DAnchorsBase rect1(new QLabel("rect1", pw));
        DAnchorsBase rect2(new QLabel("rect2", pw));
        QSignalSpy spy(&rect1, &DAnchorsBase::horizontalCenterChanged);
        ASSERT_TRUE(rect1.setHorizontalCenter(rect2.horizontalCenter()));
        ASSERT_EQ(spy.count(), 1);
    }
    {
        DAnchorsBase rect1(new QLabel("rect1", pw));
        DAnchorsBase rect2(new QLabel("rect2", pw));
        QSignalSpy spy(&rect1, &DAnchorsBase::verticalCenterChanged);
        ASSERT_TRUE(rect1.setVerticalCenter(rect2.verticalCenter()));
        ASSERT_EQ(spy.count(), 1);
        ASSERT_TRUE(rect1.errorString().isEmpty());
        ASSERT_EQ(rect1.errorCode(), DAnchorsBase::NoError);
    }
    pw->deleteLater();
};

TEST_F(ut_DAnchorsBase, setFill)
{
    QWidget * pw = new QWidget();
    DAnchorsBase rect1(new QLabel("rect1", pw));
    DAnchorsBase rect2(new QLabel("rect2", pw));
    rect1.setFill(&rect2);
    ASSERT_EQ(rect1.fill(), rect2.target());
    pw->deleteLater();
}

TEST_F(ut_DAnchorsBase, setCenterIn)
{
    QWidget * pw = new QWidget();
    DAnchorsBase rect1(new QLabel("rect1", pw));
    DAnchorsBase rect2(new QLabel("rect2", pw));
    rect1.setCenterIn(&rect2);
    ASSERT_EQ(rect1.centerIn(), rect2.target());
    pw->deleteLater();
}

TEST_F(ut_DAnchorsBase, supplement)
{
    QWidget * pw = new QWidget();
    DAnchorsBase rect1(new QLabel("rect1", pw));
    rect1.getAnchorBaseByWidget(pw);

    rect1.setEnabled(false);
    rect1.setEnabled(true);

    rect1.setTop(10, Qt::AnchorPoint::AnchorTop);
    rect1.setBottom(10, Qt::AnchorPoint::AnchorBottom);
    rect1.setLeft(10, Qt::AnchorPoint::AnchorLeft);

    pw->deleteLater();
}
