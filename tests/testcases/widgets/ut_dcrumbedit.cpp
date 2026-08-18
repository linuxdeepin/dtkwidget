// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QClipboard>
#include <QMimeData>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QImage>

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

// 验证：容器（文档）可用宽度小于 crumb 固有宽度时，带标记颜色的 crumb 文字会被省略且不溢出框外。
// 对应 issue：放大缩小右侧边栏时预览区标记自适应不好（缩小溢出、放大留白）。
TEST_F(ut_DCrumbedit, tagCrumbElidedAndNoOverflowWhenContainerNarrow)
{
    // 创建带标记颜色 + 超长文字的 crumb
    DCrumbTextFormat format = edit->makeTextFormat(DCrumbEdit::red);
    ASSERT_TRUE(format.tagColor().isValid());

    const QString longText = QString::fromUtf8("标记文字").repeated(20); // 远超容器宽度的超长文字
    format.setText(longText);
    // 用纯色背景/文字，便于按像素检测绘制范围
    format.setBackground(QBrush(Qt::yellow));
    format.setTextColor(Qt::black);

    ASSERT_TRUE(edit->appendCrumb(format));
    ASSERT_EQ(edit->crumbList().size(), 1);
    ASSERT_EQ(edit->crumbList().first(), longText);

    QTextDocument *doc = edit->document();
    const QFontMetricsF fm(format.font());
    // crumb 固有宽度（与 intrinsicSize 口径一致）
    const qreal intrinsicWidth =
        fm.horizontalAdvance(longText) + fm.height() + format.backgroundRadius() + 2;
    // 将文档可用宽度设为远小于固有宽度，模拟缩小侧边栏
    const int narrowWidth = int(intrinsicWidth / 3);
    ASSERT_GT(narrowWidth, 0);
    ASSERT_LT(qreal(narrowWidth), intrinsicWidth);
    doc->setTextWidth(narrowWidth);

    // 修复前 intrinsicSize 未限宽，单个超宽 crumb 会撑开文档宽度导致水平滚动条；
    // 修复后 idealWidth 应不超出文档可用宽度。
    EXPECT_LE(doc->idealWidth(), qreal(narrowWidth))
        << "单个超宽 crumb 撑开了文档宽度(" << doc->idealWidth()
        << " > " << narrowWidth << ")，会产生水平滚动条";

    // 渲染文档到一张比 narrowWidth 更宽的图片，检测 crumb 是否溢出文档可用宽度
    QImage img(int(intrinsicWidth) + 40, 100, QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    QAbstractTextDocumentLayout::PaintContext ctx;
    doc->documentLayout()->draw(&p, ctx);
    p.end();

    // 找出最右侧的非背景像素，crumb 不应溢出到 narrowWidth 右侧（留少量抗锯齿容差）
    int rightMostContentX = -1;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = img.width() - 1; x > rightMostContentX; --x) {
            if (img.pixelColor(x, y) != Qt::white) {
                rightMostContentX = x;
                break;
            }
        }
    }
    // 容差 8 像素用于抗锯齿/圆角；无修复时 crumb 会溢出到约 intrinsicWidth 处
    EXPECT_LT(rightMostContentX, narrowWidth + 8)
        << "crumb 内容溢出文档可用宽度(" << narrowWidth
        << ")，rightMostContentX=" << rightMostContentX;

    // 绘制层省略不改变实际 crumb 文本
    EXPECT_EQ(edit->crumbList().first(), longText);

    // 省略逻辑：对窄宽度做 elidedText 应得到含省略号的较短文本
    const QString elided = fm.elidedText(longText, Qt::ElideRight, narrowWidth);
    EXPECT_LT(fm.horizontalAdvance(elided), fm.horizontalAdvance(longText));
    EXPECT_TRUE(elided.contains(QChar(0x2026))); // 包含省略号 “…”
}
