/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
