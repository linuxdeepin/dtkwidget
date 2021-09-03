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
    QContextMenuEvent menuEvent(QContextMenuEvent::Mouse, QPoint(0,0));
    widget->contextMenuEvent(&menuEvent);
}
