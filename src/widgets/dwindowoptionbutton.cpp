// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dwindowoptionbutton.h"
#include "dstyleoption.h"

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DWindowOptionButton
  \inmodule dtkwidget
  \brief The DWindowOptionButton class is used as the unified window option button.
  \brief DWindowOptionButton 类是 DTK 窗口统一的菜单按钮控件.
  
  It's actually a special DImageButton which has the appearance of option button.
  点击按钮后，默认会显示程序主菜单，包含“关于”、“帮助”等项。
 */

/*!
  \brief DWindowOptionButton::DWindowOptionButton 是 DWindowOptionButton 的构造
  函数，返回 DWindowOptionButton 对象，普通程序一般无需使用。
  \a parent 为创建对象的父控件。
 */
DWindowOptionButton::DWindowOptionButton(QWidget * parent)
    : DIconButton(parent)
{
    //QStyle::SP_TitleBarMenuButton
    auto iconEngine = new DStyledIconEngine(DDrawUtils::drawTitleBarMenuButton, QStringLiteral("TitleBarMenuButton"));
    setIcon(QIcon(iconEngine));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setFlat(true);
}

QSize DWindowOptionButton::sizeHint() const
{
    return iconSize();
}

void DWindowOptionButton::initStyleOption(DStyleOptionButton *option) const
{
    DIconButton::initStyleOption(option);

    option->features |= QStyleOptionButton::ButtonFeature(DStyleOptionButton::TitleBarButton);
}

DWIDGET_END_NAMESPACE


