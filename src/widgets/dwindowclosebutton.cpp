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

#include "dwindowclosebutton.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~english \class DWindowCloseButton
 * \~english \brief The DWindowCloseButton class is used as the unified window close button.
 *
 * \~english It's actually a special DImageButton which has the appearance of close button.
 *
 * \~chinese \class DWindowCloseButton
 * \~chinese \brief DWindowCloseButton 类是 DTK 窗口统一的关闭按钮控件。
 */

/*!
 * \~chinese \brief DWindowCloseButton::DWindowCloseButton 是 DWindowCloseButton 的构造
 * \~chinese 函数，返回 DWindowCloseButton 对象，普通程序一般无需使用。
 * \~chinese \param parent 为创建对象的父控件。
 */
DWindowCloseButton::DWindowCloseButton(QWidget * parent)
    : DIconButton(QStyle::SP_TitleBarCloseButton, parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setFlat(true);
    setIconSize(QSize(10, 10));
}

QSize DWindowCloseButton::sizeHint() const
{
    return iconSize() * 4;
}

DWIDGET_END_NAMESPACE
