// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "dlineedit.h"
DWIDGET_USE_NAMESPACE
class ut_DLineEdit : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DLineEdit();
    }
    void TearDown() override
    {
        if (target) {
            delete target;
            target = nullptr;
        }
    }
    DLineEdit *target = nullptr;
};

TEST_F(ut_DLineEdit, clear)
{
    target->setText("setText");
    target->clear();
    ASSERT_EQ(target->text(), "");
};

TEST_F(ut_DLineEdit, hideAlertMessage)
{
    target->hideAlertMessage();
};

TEST_F(ut_DLineEdit, setAlert)
{
    target->setAlert(true);
    ASSERT_EQ(target->isAlert(), true);
};

TEST_F(ut_DLineEdit, setAlertMessageAlignment)
{
    target->setAlertMessageAlignment(Qt::AlignLeft);
    ASSERT_EQ(target->alertMessageAlignment(), Qt::AlignLeft);
};

TEST_F(ut_DLineEdit, setClearButtonEnabled)
{
    target->setClearButtonEnabled(true);
    ASSERT_EQ(target->isClearButtonEnabled(), true);
};

TEST_F(ut_DLineEdit, setContextMenuPolicy)
{
    target->setContextMenuPolicy(Qt::NoContextMenu);
    ASSERT_EQ(target->lineEdit()->contextMenuPolicy(), Qt::NoContextMenu);
};

TEST_F(ut_DLineEdit, setEchoMode)
{
    target->setEchoMode(QLineEdit::Normal);
    ASSERT_EQ(target->echoMode(), QLineEdit::Normal);
};

TEST_F(ut_DLineEdit, setPlaceholderText)
{
    target->setPlaceholderText("setPlaceholderText");
    ASSERT_EQ(target->lineEdit()->placeholderText(), "setPlaceholderText");
};

TEST_F(ut_DLineEdit, setSpeechToTextEnabled)
{
    target->setSpeechToTextEnabled(true);
    ASSERT_EQ(target->speechToTextIsEnabled(), true);
};

TEST_F(ut_DLineEdit, setText)
{
    target->setText("setText");
    ASSERT_EQ(target->text(), "setText");
};

TEST_F(ut_DLineEdit, setTextToSpeechEnabled)
{
    target->setTextToSpeechEnabled(true);
    ASSERT_EQ(target->textToSpeechIsEnabled(), true);
};

TEST_F(ut_DLineEdit, setTextToTranslateEnabled)
{
    target->setTextToTranslateEnabled(true);
    ASSERT_EQ(target->textToTranslateIsEnabled(), true);
};
