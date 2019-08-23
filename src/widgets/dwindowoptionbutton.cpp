/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dthememanager.h"

#include "dwindowoptionbutton.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~english \class DWindowOptionButton
 * \~english \brief The DWindowOptionButton class is used as the unified window option button.
 *
 * \~english It's actually a special DImageButton which has the appearance of option button.
 *
 * \~chinese \class DWindowOptionButton
 * \~chinese \brief DWindowOptionButton 类是 DTK 窗口统一的菜单按钮控件。
 * \~chinese 点击按钮后，默认会显示程序主菜单，包含“关于”、“帮助”等项。
 */

/*!
 * \~chinese \brief DWindowOptionButton::DWindowOptionButton 是 DWindowOptionButton 的构造
 * \~chinese 函数，返回 DWindowOptionButton 对象，普通程序一般无需使用。
 * \~chinese \param parent 为创建对象的父控件。
 */
DWindowOptionButton::DWindowOptionButton(QWidget * parent)
    : DIconButton(QStyle::SP_TitleBarMenuButton, parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFlat(true);
}

DWIDGET_END_NAMESPACE


