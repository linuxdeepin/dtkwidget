/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dwindowclosebutton.h"
#include "dstyleoption.h"

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DWindowCloseButton
  \inmodule dtkwidget
  \brief The DWindowCloseButton class is used as the unified window close button.
  \brief DWindowCloseButton 类是 DTK 窗口统一的关闭按钮控件.
  
  It's actually a special DImageButton which has the appearance of close button.
 */

/*!
  \brief DWindowCloseButton::DWindowCloseButton 是 DWindowCloseButton 的构造
  函数，返回 DWindowCloseButton 对象，普通程序一般无需使用。
  \a parent 为创建对象的父控件。
 */
DWindowCloseButton::DWindowCloseButton(QWidget * parent)
    : DIconButton(parent)
{
    //QStyle::SP_TitleBarCloseButton
    auto iconEngine = new DStyledIconEngine(DDrawUtils::drawTitleBarCloseButton, QStringLiteral("TitleBarCloseButton"));
    setIcon(QIcon(iconEngine));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setFlat(true);
}

QSize DWindowCloseButton::sizeHint() const
{
    return iconSize();
}

void DWindowCloseButton::initStyleOption(DStyleOptionButton *option) const
{
    DIconButton::initStyleOption(option);

    option->features |= QStyleOptionButton::ButtonFeature(DStyleOptionButton::TitleBarButton);
}

DWIDGET_END_NAMESPACE
