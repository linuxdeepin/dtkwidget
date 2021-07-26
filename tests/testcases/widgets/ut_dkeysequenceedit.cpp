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
