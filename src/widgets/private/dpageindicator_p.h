/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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

    void updateItemSize();
public:
    int activeSize = 5;
    int inactiveSize = 3;
    int itemSize = 12;
    int m_pageCount;
    int m_currentPage;
    QColor m_color;
    QColor m_secondaryColor;
};

DWIDGET_END_NAMESPACE

#endif // DPAGEINDICATOR_P_H
