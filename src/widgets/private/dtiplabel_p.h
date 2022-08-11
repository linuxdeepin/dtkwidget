// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DTIPLABEL_P_H
#define DTIPLABEL_P_H

#include <DTipLabel>
#include <DStyle>
#include "dlabel_p.h"

DWIDGET_BEGIN_NAMESPACE

class DTipLabelPrivate : public DLabelPrivate
{
    Q_DECLARE_PUBLIC(DTipLabel)

public:
    DTipLabelPrivate(DTipLabel *q);

    void init();
};

DWIDGET_END_NAMESPACE

#endif // DTIPLABEL_P_H
