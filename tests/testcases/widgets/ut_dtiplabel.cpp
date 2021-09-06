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

#include "dtiplabel.h"
#include "DPalette"

DWIDGET_USE_NAMESPACE

class ut_DTipLabel : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DTipLabel *tips = nullptr;
};

void ut_DTipLabel::SetUp()
{
    widget = new QWidget;
    tips = new DTipLabel("DTipLabel", widget);
    widget->resize(300, 100);
}

void ut_DTipLabel::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DTipLabel, testDTipLabelSetText)
{
    QString text("testDTipLabelPositions");
    tips->setText(text);
    ASSERT_EQ(tips->text(), text);
}

TEST_F(ut_DTipLabel, testDTipLabelSetForegroundRole)
{
    DPalette::ColorRole roles[] = {
       DPalette::WindowText,
        DPalette::Button,
        DPalette::Light,
        DPalette::Midlight,
        DPalette::Dark,
        DPalette::Mid,
        DPalette::Text,
        DPalette::BrightText,
        DPalette::ButtonText,
        DPalette::Base,
        DPalette::Window,
        DPalette::Shadow,
        DPalette::Highlight,
        DPalette::HighlightedText,
        DPalette::Link,
        DPalette::LinkVisited,
        DPalette::AlternateBase,
        DPalette::NoRole,
        DPalette::ToolTipBase,
        DPalette::ToolTipText,
        DPalette::NColorRoles
    };

    Q_UNUSED(roles)
//    for (DPalette::ColorRole role : roles) {
//        tips->setForegroundRole(QColor(role));
//        ASSERT_EQ(tips->foregroundRole(),QColor(role));
//    }

    DPalette::ColorType types[] ={
        DPalette::NoType,
        DPalette::ItemBackground,
        DPalette::TextTitle,
        DPalette::TextTips,
        DPalette::TextWarning,
        DPalette::TextLively,
        DPalette::LightLively,
        DPalette::DarkLively,
        DPalette::FrameBorder,
        DPalette::PlaceholderText,
        DPalette::FrameShadowBorder,
        DPalette::ObviousBackground,
        DPalette::NColorTypes
    };
    Q_UNUSED(types)
//    DLabel *label = static_cast<DLabel*>(tips);
//    for (DPalette::ColorType type : types) {
//        tips->setForegroundRole(type);
//        ASSERT_EQ(label->foregroundRole(), (QPalette::ColorRole)type);
//    }


    // TODO
}
