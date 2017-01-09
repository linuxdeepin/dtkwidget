#include "dpageindicator.h"
#include "private/dpageindicator_p.h"

#include <QDebug>
#include <QPainter>

DWIDGET_USE_NAMESPACE

DPageIndicator::DPageIndicator(QWidget *parent)
    : QWidget(parent),
      DObject(*new DPageIndicatorPrivate(this), this)
{
}

int DPageIndicator::pageCount() const
{
    D_DC(DPageIndicator);

    return d->m_pageCount;
}

void DPageIndicator::setPageCount(const int count)
{
    D_D(DPageIndicator);

    d->setPageCount(count);
}

void DPageIndicator::nextPage()
{
    D_D(DPageIndicator);

    d->nextPage();
}

void DPageIndicator::previousPage()
{
    D_D(DPageIndicator);

    d->previousPage();
}

void DPageIndicator::setCurrentPage(const int index)
{
    D_D(DPageIndicator);

    d->setCurrentPage(index);
}

int DPageIndicator::currentPageIndex() const
{
    D_DC(DPageIndicator);

    return d->m_currentPage;
}

QColor DPageIndicator::pointColor() const
{
    D_DC(DPageIndicator);

    return d->m_color;
}

void DPageIndicator::setPointColor(QColor color)
{
    D_D(DPageIndicator);

    d->m_color = color;
}

void DPageIndicator::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    const int w = width();
    const int h = height();

    const int item_size = 12;
    const int inactive_size = 3;
    const int active_size = 5;

    D_DC(DPageIndicator);

    const int total_w = d->m_pageCount * item_size;
    const QPoint offset = QPoint((w - total_w) / 2, h / 2);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(d->m_color);
    painter.setPen(Qt::transparent);
    for (int i(0); i != d->m_pageCount; ++i)
    {
        if (d->m_currentPage == i)
            painter.drawEllipse(offset + QPoint(item_size / 2 + item_size * i, 0), active_size, active_size);
        else
            painter.drawEllipse(offset + QPoint(item_size / 2 + item_size * i, 0), inactive_size, inactive_size);
    }
}

DPageIndicatorPrivate::DPageIndicatorPrivate(DPageIndicator *q) :
    DObjectPrivate(q),
    m_pageCount(0),
    m_currentPage(-1),
    m_color(Qt::red)
{

}

void DPageIndicatorPrivate::setPageCount(const int count)
{
    m_pageCount = count;

    D_Q(DPageIndicator);

    q->update();
}

void DPageIndicatorPrivate::nextPage()
{
    m_currentPage = (m_currentPage + 1) % m_pageCount;

    D_Q(DPageIndicator);

    q->update();
}

void DPageIndicatorPrivate::previousPage()
{
    m_currentPage = (m_currentPage ? m_currentPage : m_pageCount) - 1;

    D_Q(DPageIndicator);

    q->update();
}

void DPageIndicatorPrivate::setCurrentPage(const int index)
{
    if (index < -1 || index >= m_pageCount)
    {
        qWarning() << "index out of bounds: " << index << ", max is " << m_pageCount;
        return;
    }

    m_currentPage = index;

    D_Q(DPageIndicator);

    q->update();
}
