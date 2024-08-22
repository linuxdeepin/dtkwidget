// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DABOUTDIALOG_P_H
#define DABOUTDIALOG_P_H

#include <dtoolbutton.h>
#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DToolButtonPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DToolButtonPrivate(DToolButton *qq);

    DDciIcon m_dciIcon;
    DDciIconPlayer m_dciPlayer;

    Q_DECLARE_PUBLIC(DToolButton)
};

DWIDGET_END_NAMESPACE

#endif // DABOUTDIALOG_P_H
