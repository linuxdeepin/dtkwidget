// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "dtoolbutton.h"

DWIDGET_USE_NAMESPACE

class ut_DToolButton : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DToolButton *button = nullptr;
    QWidget *widget = nullptr;
};

void ut_DToolButton::SetUp()
{
    widget = new QWidget;
    button = new DToolButton(widget);
    widget->resize(300, 200);
}

void ut_DToolButton::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DToolButton, testDToolButton)
{
    QIcon icon(QIcon::fromTheme("preferences-system"));
    button->setIcon(icon);

    QString btStr("aaaaaaaa");
    button->setText(btStr);
    ASSERT_TRUE(button->text() == btStr);

    Qt::Alignment align[] = {
        Qt::AlignLeft,
        Qt::AlignRight,
        Qt::AlignHCenter,
        Qt::AlignJustify,
        Qt::AlignAbsolute,
        Qt::AlignTop,
        Qt::AlignBottom,
        Qt::AlignVCenter,
        Qt::AlignBaseline,
        Qt::AlignCenter,
    };

    for (Qt::Alignment al : align) {
        button->setAlignment(al);
        ASSERT_TRUE(button->alignment() == al);
    }
}
