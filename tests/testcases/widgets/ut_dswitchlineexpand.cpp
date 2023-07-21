// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>

#include "dswitchlineexpand.h"

DWIDGET_USE_NAMESPACE

class ut_DSwitchLineExpand : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DSwitchLineExpand *target = nullptr;
    QWidget *widget = nullptr;
};

void ut_DSwitchLineExpand::SetUp()
{
    widget = new QWidget;
    target = new DSwitchLineExpand(widget);
    widget->resize(300, 200);
}

void ut_DSwitchLineExpand::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DSwitchLineExpand, testSetTitle)
{
    // 测试设置标题
    target->setTitle("test");
    ASSERT_EQ(target->m_headerLine->title(), "test");
}

TEST_F(ut_DSwitchLineExpand, testSetExpand)
{
    // 测试设置展开
    target->setExpand(true);
    ASSERT_TRUE(target->m_headerLine->m_switchButton->isChecked());
    target->setExpand(false);
    ASSERT_FALSE(target->m_headerLine->m_switchButton->isChecked());
}

TEST_F(ut_DSwitchLineExpand, testHeader)
{
    // 测试获取头部
    ASSERT_EQ(target->header() , target->m_headerLine);
}

TEST_F(ut_DSwitchLineExpand, testResizeEvent)
{
    // 测试重置大小
    target->resize(QSize(100, 100));
    ASSERT_EQ(target->size(), QSize(100, 100));
}
