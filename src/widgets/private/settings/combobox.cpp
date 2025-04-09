// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "combobox.h"

ComboBox::ComboBox(QWidget *parent) :
    QComboBox(parent)
{

}

void ComboBox::wheelEvent(QWheelEvent *e)
{
    if (hasFocus()) {
        QComboBox::wheelEvent(e);
    } else {
        return QWidget::wheelEvent(e);
    }
}
