// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSIZEMODE_H
#define DSIZEMODE_H

#include <dtkwidget_global.h>

#include <DGuiApplicationHelper>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DSizeModeHelper
{
public:
    template<class T>
    static inline T element(const T &t1, const T &t2)
    {
        return DGUI_NAMESPACE::DGuiApplicationHelper::isCompactMode() ? t1 : t2;
    }
};

DWIDGET_END_NAMESPACE
#endif // DSIZEMODE_H
