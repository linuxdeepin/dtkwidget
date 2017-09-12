/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

QColor DPageIndicator::secondaryPointColor() const
{
    D_DC(DPageIndicator);

    return d->m_secondaryColor;
}

void DPageIndicator::setSecondaryPointColor(QColor color)
{
    D_D(DPageIndicator);

    d->m_secondaryColor = color;
}

int DPageIndicator::activeSize() const
{
    D_DC(DPageIndicator);
    return  d->activeSize;
}

void DPageIndicator::setActiveSize(int size)
{
    D_D(DPageIndicator);
    d->activeSize = size;
    d->updateItemSize();
}

int DPageIndicator::inactiveSize() const
{
    D_DC(DPageIndicator);
    return  d->inactiveSize;
}

void DPageIndicator::setInactiveSize(int size)
{
    D_D(DPageIndicator);
    d->inactiveSize = size;
    d->updateItemSize();
}

void DPageIndicator::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    const int w = width();
    const int h = height();

    D_DC(DPageIndicator);

    const int total_w = d->m_pageCount * d->itemSize;
    const QPoint offset = QPoint((w - total_w) / 2, h / 2);

    QColor currentPtColor = d->m_color;
    QColor nonCurrentPtColor = d->m_secondaryColor;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::transparent);
    for (int i(0); i != d->m_pageCount; ++i) {
        if (d->m_currentPage == i) {
            painter.setBrush(currentPtColor);
            painter.drawEllipse(offset + QPoint(d->itemSize / 2 + d->itemSize * i, 0),
                                d->activeSize, d->activeSize);
        } else {
            painter.setBrush(nonCurrentPtColor);
            painter.drawEllipse(offset + QPoint(d->itemSize / 2 + d->itemSize * i, 0),
                                d->inactiveSize, d->inactiveSize);
        }
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
    if (index < -1 || index >= m_pageCount) {
        qWarning() << "index out of bounds: " << index << ", max is " << m_pageCount;
        return;
    }

    m_currentPage = index;

    D_Q(DPageIndicator);

    q->update();
}

void DPageIndicatorPrivate::updateItemSize()
{
    int space = (activeSize + inactiveSize) / 4;
    itemSize = activeSize + space + inactiveSize;
}
