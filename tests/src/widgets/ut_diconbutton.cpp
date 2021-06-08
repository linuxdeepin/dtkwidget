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
    ASSERT_TRUE(button->property("_d_dtk_newNotification").toBool());
}
