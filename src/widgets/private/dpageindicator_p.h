// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPAGEINDICATOR_P_H
#define DPAGEINDICATOR_P_H

#include "dpageindicator.h"
#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DPageIndicatorPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DPageIndicator)

public:
    DPageIndicatorPrivate(DPageIndicator *q);

    void setPageCount(const int count);
    void nextPage();
    void previousPage();
    void setCurrentPage(const int index);

public:
    int pointRadius = 5;
    int secondaryPointRadius = 3;
    int pointDistance = 12;
    int pageCount;
    int currentPage;
    QColor pointColor;
    QColor secondaryPointColor;
};

DWIDGET_END_NAMESPACE

#endif // DPAGEINDICATOR_P_H
