/*
 * Copyright (C) 2021 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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
    QMimeData *data = edit->createMimeDataFromSelection();
    ASSERT_EQ(data->text(), "测试1");
    ASSERT_EQ(qApp->clipboard()->text(), "测试1");
    delete data;

    // 模拟鼠标中选中操作 选中字符为： 测试1 人物
    QTest::keyClick(edit, Qt::Key_Right, Qt::ShiftModifier);
    // 此时位置 测试1 人物  | 测试2 儿童 测试3 照片 测试代码调试添加GTest
    ASSERT_TRUE(edit->textCursor().hasSelection());
    ASSERT_FALSE(edit->textCursor().selectedText().isEmpty());
    edit->copy();
    data = edit->createMimeDataFromSelection();
    ASSERT_EQ(data->text(), "测试1 人物");
    ASSERT_EQ(qApp->clipboard()->text(), "测试1 人物");
    delete data;

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
    data = edit->createMimeDataFromSelection();
    ASSERT_EQ(data->text(), "人物 测试");
    ASSERT_EQ(qApp->clipboard()->text(), "人物 测试");
    delete data;

    edit->moveCursor(QTextCursor::Start);

    // 选中所有字符
    QTest::keyClick(edit, Qt::Key_End, Qt::ShiftModifier);
    edit->copy();
    data = edit->createMimeDataFromSelection();
    ASSERT_EQ(data->text(), "测试1 人物 测试2 儿童 测试3 照片 测试代码调试添加GTest");
    ASSERT_EQ(qApp->clipboard()->text(), "测试1 人物 测试2 儿童 测试3 照片 测试代码调试添加GTest");
    delete data;
}

TEST_F(ut_DCrumbedit, setCrumbReadOnly)
{
    edit->setCrumbReadOnly(true);
    ASSERT_EQ(edit->crumbReadOnly(), true);
}

TEST_F(ut_DCrumbedit, setCrumbRadius)
{
    edit->setCrumbRadius(1);
    ASSERT_EQ(edit->crumbRadius(), 1);
}

TEST_F(ut_DCrumbedit, setSplitter)
{
    edit->setSplitter("|");
    ASSERT_EQ(edit->splitter(), "|");
}

TEST_F(ut_DCrumbedit, setDualClickMakeCrumb)
{
    edit->setDualClickMakeCrumb(true);
    ASSERT_EQ(edit->dualClickMakeCrumb(), true);
}

TEST_F(ut_DCrumbedit, containCrumb)
{
    edit->insertCrumb("123");
    ASSERT_TRUE(edit->containCrumb("123"));
    ASSERT_EQ(edit->crumbList().size(), 1);

    auto format = edit->crumbTextFormat("123");
    ASSERT_EQ(format.objectType(), QTextFormat::UserObject + 1);
}

TEST_F(ut_DCrumbedit, event)
{
    edit->show();
    ASSERT_TRUE(QTest::qWaitForWindowExposed(edit, 100));

    edit->setFocus();

    QTest::keyPress(edit, Qt::Key_Return);

    QTest::mouseDClick(edit, Qt::LeftButton);
}

class ut_DCrumbTextFormat : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    DCrumbTextFormat *format;
};

void ut_DCrumbTextFormat::SetUp()
{
    format = new DCrumbTextFormat;
}

void ut_DCrumbTextFormat::TearDown()
{
    delete format;
}

TEST_F(ut_DCrumbTextFormat, setTagColor)
{
    format->setTagColor(Qt::red);
    ASSERT_EQ(format->tagColor(), Qt::red);
}

TEST_F(ut_DCrumbTextFormat, setText)
{
    format->setText("text");
    ASSERT_EQ(format->text(), "text");
}

TEST_F(ut_DCrumbTextFormat, setTextColor)
{
    format->setTextColor(Qt::red);
    ASSERT_EQ(format->textColor(), Qt::red);
}

TEST_F(ut_DCrumbTextFormat, setBackground)
{
    format->setBackground(Qt::red);
    ASSERT_EQ(format->background(), Qt::red);
}

TEST_F(ut_DCrumbTextFormat, setBackgroundRadius)
{
    format->setBackgroundRadius(1);
    ASSERT_EQ(format->backgroundRadius(), 1);
}
