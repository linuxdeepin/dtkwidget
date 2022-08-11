// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSPINBOX_P_H
#define DSPINBOX_P_H

#include <DSpinBox>

#include <DObjectPrivate>

class QFrame;
class QLabel;

DWIDGET_BEGIN_NAMESPACE

class DLineEdit;
class DSpinBoxPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    DSpinBoxPrivate(DSpinBox *parent);

    void init();

    DLineEdit *lineEdit = nullptr;
    D_DECLARE_PUBLIC(DSpinBox)
};

class DDoubleSpinBoxPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    DDoubleSpinBoxPrivate(DDoubleSpinBox *parent);

    void init();

    DLineEdit *lineEdit = nullptr;
    D_DECLARE_PUBLIC(DDoubleSpinBox)
};

DWIDGET_END_NAMESPACE

#endif // DSPINBOX_P_H

