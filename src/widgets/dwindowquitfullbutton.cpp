// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dstyleoption.h"
#include "dwindowquitfullbutton.h"

#include <DStyledIconEngine>

DWIDGET_BEGIN_NAMESPACE

DWindowQuitFullButton::DWindowQuitFullButton(QWidget * parent)
    : DIconButton(parent)
{
    //DStyle::SP_TitleQuitFullButton
    auto iconEngine = new DStyledIconEngine(DDrawUtils::drawTitleQuitFullButton, QStringLiteral("TitleQuitFullButton"));
    setIcon(QIcon(iconEngine));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setFlat(true);
}

QSize DWindowQuitFullButton::sizeHint() const
{
    return iconSize();
}

void DWindowQuitFullButton::initStyleOption(DStyleOptionButton *option) const
{
    DIconButton::initStyleOption(option);

    option->features |= QStyleOptionButton::ButtonFeature(DStyleOptionButton::TitleBarButton);
}

DWIDGET_END_NAMESPACE
