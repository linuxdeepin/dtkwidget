// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "DTextEdit"

DWIDGET_USE_NAMESPACE

class ut_DTextEdit : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DTextEdit *edit = nullptr;
};

void ut_DTextEdit::SetUp()
{
    widget = new QWidget;
    edit = new DTextEdit(widget);
    widget->resize(300, 100);
}

void ut_DTextEdit::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DTextEdit, testDTextEditSpeechToTextEnabled)
{
    bool enableState = false;
    edit->setSpeechToTextEnabled(enableState);
    ASSERT_FALSE(edit->speechToTextIsEnabled());

    enableState = true;
    edit->setSpeechToTextEnabled(enableState);
    ASSERT_TRUE(edit->speechToTextIsEnabled());
}

TEST_F(ut_DTextEdit, testDTextEditTextToSpeechEnabled)
{
    bool enableState = false;
    edit->setTextToSpeechEnabled(enableState);
    ASSERT_FALSE(edit->textToSpeechIsEnabled());

    enableState = true;
    edit->setTextToSpeechEnabled(enableState);
    ASSERT_TRUE(edit->textToSpeechIsEnabled());
}

TEST_F(ut_DTextEdit, testDTextEditTextToTranslateEnabled)
{
    bool enableState = false;
    edit->setTextToTranslateEnabled(enableState);
    ASSERT_FALSE(edit->textToTranslateIsEnabled());

    enableState = true;
    edit->setTextToTranslateEnabled(enableState);
    ASSERT_TRUE(edit->textToTranslateIsEnabled());
}

TEST_F(ut_DTextEdit, testDTextEditKeyPressEvent)
{
    QKeyEvent keyEvent(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier);
    widget->keyPressEvent(&keyEvent);
}

TEST_F(ut_DTextEdit, testDTextEditContextMenuEvent)
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    QContextMenuEvent menuEvent(QContextMenuEvent::Mouse, QPoint(0,0), QPoint(0,0));
#else
    QContextMenuEvent menuEvent(QContextMenuEvent::Mouse, QPoint(0,0));
#endif
    widget->contextMenuEvent(&menuEvent);
}
