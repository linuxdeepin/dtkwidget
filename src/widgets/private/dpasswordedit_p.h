// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPASSWORDEDIT_P_H
#define DPASSWORDEDIT_P_H

#include "dlineedit_p.h"
#include <DPasswordEdit>

DWIDGET_BEGIN_NAMESPACE

class DSuggestButton;
class DPasswordEditPrivate : DLineEditPrivate
{
public:
    DPasswordEditPrivate(DPasswordEdit *q);

    void init();

    void _q_toggleEchoMode();

protected:
    Q_DECLARE_PUBLIC(DPasswordEdit)

private:
    DSuggestButton * togglePasswordVisibleButton;
};

DWIDGET_END_NAMESPACE

#endif // DPASSWORDEDIT_P_H

