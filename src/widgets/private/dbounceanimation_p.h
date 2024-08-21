// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
#ifndef DBOUNCEANIMATION_P_H
#define DBOUNCEANIMATION_P_H

#include "dbounceanimation.h"
#include <DObjectPrivate>

class DBounceAnimationPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DBounceAnimationPrivate(DBounceAnimation *qq);

    QPropertyAnimation *m_animation;
    QAbstractScrollArea *m_animationTarget;
    int m_deltaSum;

private:
    D_DECLARE_PUBLIC(DBounceAnimation)
};

#endif // DBOUNCEANIMATION_P_H
