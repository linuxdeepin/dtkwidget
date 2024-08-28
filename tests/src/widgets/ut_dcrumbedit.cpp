// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QClipboard>
#include <QMimeData>

#include "dcrumbedit.h"
#include <QDebug>

DWIDGET_USE_NAMESPACE

class ut_DCrumbedit : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    DCrumbEdit *edit;
};

void ut_DCrumbedit::SetUp()
{
    edit = new DCrumbEdit;
}

void ut_DCrumbedit::TearDown()
{
    delete edit;
}

TEST_F(ut_DCrumbedit, createMimeDataFromSelection)
{
    edit->appendCrumb("人物");
    edit->appendCrumb("儿童");
    edit->appendCrumb("照片");

    QTextCursor cursor = edit->textCursor();
    cursor.setPosition(0);
    cursor.insertText("测试1");
    cursor.movePosition(QTextCursor::Right);
    cursor.insertText("测试2");
    cursor.movePosition(QTextCursor::Right);
    cursor.insertText("测试3");
    cursor.movePosition(QTextCursor::End);
    cursor.insertText("测试代码调试添加GTest");

    // 重置光标到最前面
    cursor.setPosition(0);
    // 此时 测试1 人物 测试2 儿童 测试3 照片 测试代码调试添加GTest
    edit->setTextCursor(cursor);

    ASSERT_EQ(edit->textCursor().position(), 0);

    // 模拟鼠标中选中操作 选中字符为： 测试1
    QTest::keyClick(edit, Qt::Key_Right, Qt::ShiftModifier);
    QTest::keyClick(edit, Qt::Key_Right, Qt::ShiftModifier);
    QTest::keyClick(edit, Qt::Key_Right, Qt::ShiftModifier);
    // 此时位置： 测试1 | 人物 测试2 儿童 测试3 照片 测试代码调试添加GTest
    ASSERT_TRUE(edit->textCursor().hasSelection());
    ASSERT_FALSE(edit->textCursor().selectedText().isEmpty());
    edit->copy();
    ASSERT_EQ(edit->createMimeDataFromSelection()->text(), "测试1");
    ASSERT_EQ(qApp->clipboard()->text(), "测试1");

    // 模拟鼠标中选中操作 选中字符为： 测试1 人物
    QTest::keyClick(edit, Qt::Key_Right, Qt::ShiftModifier);
    // 此时位置 测试1 人物  | 测试2 儿童 测试3 照片 测试代码调试添加GTest
    ASSERT_TRUE(edit->textCursor().hasSelection());
    ASSERT_FALSE(edit->textCursor().selectedText().isEmpty());
    edit->copy();
    ASSERT_EQ(edit->createMimeDataFromSelection()->text(), "测试1 人物");
    ASSERT_EQ(qApp->clipboard()->text(), "测试1 人物");

    edit->moveCursor(QTextCursor::Start);
    edit->moveCursor(QTextCursor::Right);
    edit->moveCursor(QTextCursor::Right);
    edit->moveCursor(QTextCursor::Right);

    // 选中字符为： 人物 测试
    QTest::keyClick(edit, Qt::Key_Right, Qt::ShiftModifier);
    QTest::keyClick(edit, Qt::Key_Right, Qt::ShiftModifier);
    QTest::keyClick(edit, Qt::Key_Right, Qt::ShiftModifier);
    // 此时位置 测试1 人物 测试|2 儿童 测试3 照片 测试代码调试添加GTest
    edit->copy();
    ASSERT_EQ(edit->createMimeDataFromSelection()->text(), "人物 测试");
    ASSERT_EQ(qApp->clipboard()->text(), "人物 测试");

    edit->moveCursor(QTextCursor::Start);

    // 选中所有字符
    QTest::keyClick(edit, Qt::Key_End, Qt::ShiftModifier);
    edit->copy();
    ASSERT_EQ(edit->createMimeDataFromSelection()->text(), "测试1 人物 测试2 儿童 测试3 照片 测试代码调试添加GTest");
    ASSERT_EQ(qApp->clipboard()->text(), "测试1 人物 测试2 儿童 测试3 照片 测试代码调试添加GTest");
}
