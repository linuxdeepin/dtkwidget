// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DINPUTDIALOG_P_H
#define DINPUTDIALOG_P_H

#include "dinputdialog.h"
#include "ddialog_p.h"

class QComboBox;

DWIDGET_BEGIN_NAMESPACE

class DLabel;
class DLineEdit;
class DSpinBox;
class DDoubleSpinBox;
class DInputDialogPrivate : public DDialogPrivate
{
    DInputDialogPrivate(DInputDialog *qq);

    QAbstractButton *cancelButton = nullptr;
    QAbstractButton *okButton = nullptr;
    DLineEdit *lineEdit = nullptr;
    DSpinBox* spinBox = nullptr;
    DDoubleSpinBox *doubleSpinBox = nullptr;
    QComboBox *comboBox = nullptr;

    DInputDialog::InputMode inputMode = DInputDialog::InputMode::TextInput;

    void init();

    D_DECLARE_PUBLIC(DInputDialog)
};

DWIDGET_END_NAMESPACE

#endif // DINPUTDIALOG_P_H

