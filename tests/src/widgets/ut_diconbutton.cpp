// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "diconbutton.h"
#include "private/diconbutton_p.h"

DWIDGET_USE_NAMESPACE

class ut_IConButton : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    DIconButton *button = nullptr;
    QWidget *widget = nullptr;
};

void ut_IConButton::SetUp()
{
    widget = new QWidget;
    button = new DIconButton(widget);
    widget->resize(300, 200);
}

void ut_IConButton::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_IConButton, testIConButton)
{
    QIcon icon(QIcon::fromTheme("preferences-system"));
    button->setIcon(icon);

    button->setFlat(true);
    ASSERT_TRUE(button->isFlat());

    QString btStr("aaaaaaaa");
    button->setText(btStr);
    ASSERT_TRUE(button->text() == btStr);

    button->setEnabledCircle(true);
    ASSERT_TRUE(button->enabledCircle());

    button->setNewNotification(true);
    ASSERT_TRUE(button->property("_d_menu_item_redpoint").toBool());
}
