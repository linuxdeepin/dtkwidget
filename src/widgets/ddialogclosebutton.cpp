// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "ddialogclosebutton.h"

DWIDGET_BEGIN_NAMESPACE

/*!
  \brief DDialogCloseButton::DDialogCloseButton 实现一个带有关闭按钮的button
  \a parent
 */
DDialogCloseButton::DDialogCloseButton(QWidget *parent)
    : DIconButton(DStyle::SP_CloseButton, parent)
{
    setFlat(true);
    setIconSize(QSize(20, 20));
}

DWIDGET_END_NAMESPACE
