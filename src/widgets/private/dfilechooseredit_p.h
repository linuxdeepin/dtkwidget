// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILECHOOSEREDIT_P_H
#define DFILECHOOSEREDIT_P_H

#include "dlineedit_p.h"
#include <DFileChooserEdit>
#include <DImageButton>

DWIDGET_BEGIN_NAMESPACE

class DFileChooserEditPrivate : DLineEditPrivate
{
    Q_DECLARE_PUBLIC(DFileChooserEdit)

public:
    DFileChooserEditPrivate(DFileChooserEdit *q);

    void init();

public:
    void _q_showFileChooserDialog();

    DFileChooserEdit::DialogDisplayPosition dialogDisplayPosition;

    QFileDialog *dialog = nullptr;
};

DWIDGET_END_NAMESPACE

#endif // DFILECHOOSEREDIT_P_H

