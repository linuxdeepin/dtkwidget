// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DABOUTDIALOG_P_H
#define DABOUTDIALOG_P_H

#include "dmessagemanager.h"

#include <DObjectPrivate>

#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

DWIDGET_BEGIN_NAMESPACE

class DMessageManagerPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DMessageManagerPrivate(DMessageManager *qq);

    QPropertyAnimation *m_aniGeometry;
    QPropertyAnimation *m_aniOpacity;;
    QParallelAnimationGroup *m_aniGroup;
    QLabel *m_label;

    D_DECLARE_PUBLIC(DMessageManager);
};

DWIDGET_END_NAMESPACE

#endif // DABOUTDIALOG_P_H
