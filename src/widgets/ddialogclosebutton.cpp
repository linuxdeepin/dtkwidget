/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#include "ddialogclosebutton.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \brief DDialogCloseButton::DDialogCloseButton 实现一个带有关闭按钮的button
 * \~chinese \param parent
 */
DDialogCloseButton::DDialogCloseButton(QWidget *parent)
    : DIconButton(DStyle::SP_CloseButton, parent)
{
    setFlat(true);
    setIconSize(QSize(20, 20));
}

DWIDGET_END_NAMESPACE
