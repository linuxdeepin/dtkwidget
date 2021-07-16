/*
 * Copyright (C) 2020 ~ 2021 Deepin Technology Co., Ltd.
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

#include "dtabletwindowoptionbutton.h"
#include "dstyleoption.h"

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DTabletWindowOptionButton
  \inmodule dtkwidget
  \brief The DTabletWindowOptionButton class is used as the unified tablet window option button.
  \brief DTabletWindowOptionButton 类是 DTK 平板窗口统一的菜单按钮控件。
  
  It's actually a special DImageButton which has the appearance of option button.
  点击按钮后，默认会显示程序主菜单，包含“关于”、“帮助”等项。
 */

/*!
  \brief DTabletWindowOptionButton::DTabletWindowOptionButton 是 DTabletWindowOptionButton 的构造
  函数，返回 DTabletWindowOptionButton 对象，普通程序一般无需使用。
  \a parent 为创建对象的父控件。
 */
DTabletWindowOptionButton::DTabletWindowOptionButton(QWidget *parent)
    : DIconButton(DStyle::SP_TitleMoreButton, parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setFlat(true);
}

QSize DTabletWindowOptionButton::sizeHint() const
{
    return iconSize();
}

void DTabletWindowOptionButton::initStyleOption(DStyleOptionButton *option) const
{
    DIconButton::initStyleOption(option);

    option->features |= QStyleOptionButton::ButtonFeature(DStyleOptionButton::TitleBarButton);
}

DWIDGET_END_NAMESPACE
