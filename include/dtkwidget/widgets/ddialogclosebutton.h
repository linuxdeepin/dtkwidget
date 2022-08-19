// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DDIALOGCLOSEBUTTON_H
#define DDIALOGCLOSEBUTTON_H

#include <DIconButton>

DWIDGET_BEGIN_NAMESPACE

class DDialogCloseButton : public DIconButton
{
    Q_OBJECT
public:
    explicit DDialogCloseButton(QWidget *parent = nullptr);
};

DWIDGET_END_NAMESPACE

#endif // DDIALOGCLOSEBUTTON_H
