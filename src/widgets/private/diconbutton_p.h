// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DICONBUTTON_P_H
#define DICONBUTTON_P_H

#include <dobject_p.h>
#include <DIconButton>

DWIDGET_BEGIN_NAMESPACE

class DIconButtonPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DIconButtonPrivate(DIconButton *qq);

    bool flat = false;
    qint64 iconType = -1;
    bool circleStatus = false;
    DDciIcon dciIcon;

    D_DECLARE_PUBLIC(DIconButton)
};

DWIDGET_END_NAMESPACE

#endif // DICONBUTTON_P_H
