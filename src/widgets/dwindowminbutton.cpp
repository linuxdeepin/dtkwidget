// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dwindowminbutton.h"
#include "dstyleoption.h"

#include <QEvent>
#include <QStyle>

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DWindowMinButton
  \inmodule dtkwidget
  \brief The DWindowMinButton class is used as the unified window minimize button.
  \brief DWindowMinButton 类是 DTK 窗口统一的最小化按钮控件.
  
  It's actually a special DImageButton which has the appearance of minimize button.
 */

/*!
  \brief DWindowMinButton::DWindowMinButton 是 DWindowMinButton 的构造
  函数，返回 DWindowMinButton 对象，普通程序一般无需使用。
  \a parent 为创建对象的父控件。
 */
DWindowMinButton::DWindowMinButton(QWidget * parent)
    : DIconButton(QStyle::SP_TitleBarMinButton, parent)
{
    //QStyle::SP_TitleBarMinButton
    auto iconEngine = new DStyledIconEngine(DDrawUtils::drawTitleBarMinButton, QStringLiteral("TitleBarMinButton"));
    setIcon(QIcon(iconEngine));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setFlat(true);
}

QSize DWindowMinButton::sizeHint() const
{
    return iconSize();
}

void DWindowMinButton::initStyleOption(DStyleOptionButton *option) const
{
    DIconButton::initStyleOption(option);

    option->features |= QStyleOptionButton::ButtonFeature(DStyleOptionButton::TitleBarButton);
}

DWIDGET_END_NAMESPACE
