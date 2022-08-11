// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "dfloatingmessage.h"
#include "dlabel.h"
#include "qlabel.h"
#include "qtimer.h"
#include "private/dfloatingmessage_p.h"

DWIDGET_USE_NAMESPACE

// for test setMessage
class MessageLabel : public QLabel
{
public:
    QSize sizeHint() const override
    {
        return fontMetrics().size(Qt::TextSingleLine, text());
    }
};

class ut_DFloatingMessage : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DFloatingMessage *message = nullptr;
    QWidget *widget = nullptr;
};

void ut_DFloatingMessage::SetUp()
{
    widget = new QWidget;
    widget->resize(300, 200);
    message = new DFloatingMessage(DFloatingMessage::TransientType, widget);
}

void ut_DFloatingMessage::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DFloatingMessage, testDFloatingMessageFrontEnd)
{
    // 测试消息是否设置成功
    QString msg("Hello DTK~");
    message->setMessage(msg);
    ASSERT_TRUE(message->d_func()->labMessage->text() == msg);

    // 测试 Icon 是否正确设置
    QIcon icon(QIcon::fromTheme("preferences-system"));
    message->setIcon(icon);
    QIcon tmp(message->d_func()->iconButton->icon());
    ASSERT_TRUE(message->d_func()->iconButton->icon().data_ptr() == tmp.data_ptr());

    // 测试 widget 是否设置成功
    QWidget w;
    message->setWidget(&w);
    ASSERT_TRUE(message->d_func()->content == &w);
}

TEST_F(ut_DFloatingMessage, testDFloatingMessageBackEnd)
{
    // 测试设置的持续时间
    int duration = 3 * 1000;
    message->setDuration(duration);
    ASSERT_TRUE(message->d_func()->timer->interval() == duration);

    // 测试边距设置
    QMargins contentsMargins(1, 2, 3, 4);
    message->setContentsMargins(contentsMargins);
    ASSERT_TRUE(message->contentsMargins() == contentsMargins);

    // 测试自动填充背景
    message->setAutoFillBackground(true);
    ASSERT_TRUE(message->autoFillBackground());
}
