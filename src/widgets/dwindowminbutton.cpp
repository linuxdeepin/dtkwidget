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

#include "dwindowminbutton.h"

#include <QEvent>
#include <QStyle>

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~english \class DWindowMinButton
 * \~english \brief The DWindowMinButton class is used as the unified window minimize button.
 *
 * \~english It's actually a special DImageButton which has the appearance of minimize button.
 *
 * \~chinese \class DWindowMinButton
 * \~chinese \brief DWindowMinButton 类是 DTK 窗口统一的最小化按钮控件。
 */

/*!
 * \~chinese \brief DWindowMinButton::DWindowMinButton 是 DWindowMinButton 的构造
 * \~chinese 函数，返回 DWindowMinButton 对象，普通程序一般无需使用。
 * \~chinese \param parent 为创建对象的父控件。
 */
DWindowMinButton::DWindowMinButton(QWidget * parent)
    : DIconButton(QStyle::SP_TitleBarMinButton, parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setFlat(true);
    setIconSize(QSize(10, 10));
}

QSize DWindowMinButton::sizeHint() const
{
    return iconSize() * 4;
}

DWIDGET_END_NAMESPACE
