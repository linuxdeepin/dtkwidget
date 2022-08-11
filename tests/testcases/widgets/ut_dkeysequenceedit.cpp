// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include <QLabel>
#include <QKeySequence>
#include "dkeysequenceedit.h"

DWIDGET_USE_NAMESPACE

class ut_DKeySequenceEdit : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DKeySequenceEdit *sequence = nullptr;
    QWidget *widget = nullptr;
};

void ut_DKeySequenceEdit::SetUp()
{
    widget = new QWidget;
    sequence = new DKeySequenceEdit(widget);
    widget->resize(300, 200);
}

void ut_DKeySequenceEdit::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DKeySequenceEdit, testDKeySequenceEdit)
{
    // 测试默认快捷键
    const QKeySequence seq(Qt::CTRL, Qt::SHIFT);
    sequence->setKeySequence(seq);
    ASSERT_TRUE(seq == sequence->keySequence());

    // 测试清空功能
    sequence->clear();
    ASSERT_TRUE(sequence->keySequence() == QKeySequence());

    // 测试 resize 功能
    QSize newSize(sequence->size() * 2);
    sequence->resize(newSize);
    ASSERT_TRUE(newSize == sequence->size());

    sequence->setKeySequence(seq);

    // 测试对齐方式，只支持左对齐和右对齐
    sequence->ShortcutDirection(Qt::AlignRight);
    ASSERT_TRUE(sequence->alignment().testFlag(Qt::AlignLeft));

    sequence->ShortcutDirection(Qt::AlignLeft);
    ASSERT_TRUE(sequence->alignment().testFlag(Qt::AlignRight));
}
