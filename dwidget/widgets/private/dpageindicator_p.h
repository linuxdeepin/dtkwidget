#ifndef DPAGEINDICATOR_P_H
#define DPAGEINDICATOR_P_H

#include "dpageindicator.h"
#include "private/dobject_p.h"

DWIDGET_BEGIN_NAMESPACE

class DPageIndicatorPrivate : public DObjectPrivate
{
    D_DECLARE_PUBLIC(DPageIndicator)

public:
    DPageIndicatorPrivate(DPageIndicator *q);

    void setPageCount(const int count);
    void nextPage();
    void previousPage();
    void setCurrentPage(const int index);

public:
    int m_pageCount;
    int m_currentPage;
    QColor m_color;
};

DWIDGET_END_NAMESPACE

#endif // DPAGEINDICATOR_P_H
