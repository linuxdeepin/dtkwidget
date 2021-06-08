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

#include "danchors.h"
#include <QLabel>

DWIDGET_USE_NAMESPACE

class ut_DAnchors : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
};

void ut_DAnchors::SetUp()
{
    widget = new QWidget;
    widget->resize(300, 200);
}

void ut_DAnchors::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DAnchors, testDAnchors)
{
    // 锚定布局也是布局管理的一种
    QLabel *lb1 = new QLabel("anchor1", widget);
    QLabel *lb2 = new QLabel("anchor2", widget);

    DAnchors<QLabel> anchor1(lb1);
    DAnchors<QLabel> anchor2(lb2);

    int margin = 15;
    anchor2.setLeftMargin(margin);
    ASSERT_TRUE(anchor2.leftMargin() == margin);
    anchor2.setTop(anchor1.bottom());
    anchor2.setLeft(anchor1.right());

    anchor1.moveCenter(QPoint(widget->width() / 2, widget->height() / 2));
    ASSERT_TRUE(anchor1->pos().x() < anchor2->pos().x());
    ASSERT_TRUE(anchor1->pos().y() > anchor2->pos().y());

    anchor1.moveHorizontalCenter(widget->width()/2);
    anchor1.moveVerticalCenter(widget->height()/2);
    ASSERT_TRUE(anchor1->pos().x() < anchor2->pos().x());
    ASSERT_TRUE(anchor1->pos().y() > anchor2->pos().y());

    // 测试offset改变的信号槽的有效性
    bool result = anchor2.setAnchor(Qt::AnchorRight, widget, Qt::AnchorRight);
    ASSERT_TRUE(result);

    int offset = 10;
    int signalTest = 0;
    DAnchorsBase *anchorBase = anchor2.getAnchorBaseByWidget(widget);
    ASSERT_TRUE(anchorBase != nullptr);

    anchorBase && QObject::connect(anchorBase, &DAnchorsBase::horizontalCenterOffsetChanged, widget, [&](int offsetIn){
        ASSERT_TRUE(offsetIn == offset);
        signalTest++;
    });

    anchorBase && QObject::connect(anchorBase, &DAnchorsBase::verticalCenterOffsetChanged, widget, [&](int offsetIn){
        ASSERT_TRUE(offsetIn == offset);
        signalTest++;
    });
    result = anchor2.setAnchor(Qt::AnchorBottom, widget, Qt::AnchorBottom);
    ASSERT_TRUE(result);

    emit anchorBase->horizontalCenterOffsetChanged(offset);
    emit anchorBase->verticalCenterOffsetChanged(offset);
    ASSERT_TRUE(signalTest == 2);

    anchor2.setHorizontalCenterOffset(offset);
    anchor2.setVerticalCenterOffset(offset);

    // 测试设置的偏移量是否正确：
    ASSERT_EQ(anchor2.horizontalCenterOffset(), offset);
    ASSERT_EQ(anchor2.verticalCenterOffset(), offset);
}
