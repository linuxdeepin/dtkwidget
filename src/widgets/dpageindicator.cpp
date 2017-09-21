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

DWIDGET_BEGIN_NAMESPACE

/*!
 * \class DPageIndicator
 * \brief The DPageIndicator class provides indicator of which page is currently showing.
 *
 * DPageIndicator is control that displays a horizontal series of dots,
 * each of which corresponds to a page/widget may appear on top of it.
 *
 * It's inspired by UIPageControl of Apple.
 * See https://developer.apple.com/documentation/uikit/uipagecontrol.
 *
 * \note The design of this control is that it's just used to display, not
 * to control which page is currently visible.
 */

/*!
 * \brief DPageIndicator::DPageIndicator constructs an instance of DPageIndicator.
 * \param parent is passed to QWidget constructor.
 */
DPageIndicator::DPageIndicator(QWidget *parent)
    : QWidget(parent),
      DObject(*new DPageIndicatorPrivate(this), this)
{
}

/*!
 * \property DPageIndicator::pageCount
 * \brief This property holds the number of pages that are represented by this widget.
 */
int DPageIndicator::pageCount() const
{
    D_DC(DPageIndicator);

    return d->pageCount;
}

void DPageIndicator::setPageCount(const int count)
{
    D_D(DPageIndicator);

    d->setPageCount(count);
}

/*!
 * \brief DPageIndicator::nextPage highlights the next dot.
 *
 * \see DPageIndicator::setCurrentPage.
 */
void DPageIndicator::nextPage()
{
    D_D(DPageIndicator);

    d->nextPage();
}

/*!
 * \brief DPageIndicator::previousPage hightlights the previous dot.
 *
 * \see DPageIndicator::setCurrentPage.
 */
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

/*!
 * \property DPageIndicator::currentPage
 * \brief This property holds the index of currently highlighted dot.
 */
int DPageIndicator::currentPageIndex() const
{
    D_DC(DPageIndicator);

    return d->currentPage;
}

/*!
 * \property DPageIndicator::pointColor
 * \brief This property holds the color used to draw the highlighted dot.
 */
QColor DPageIndicator::pointColor() const
{
    D_DC(DPageIndicator);

    return d->pointColor;
}

void DPageIndicator::setPointColor(QColor color)
{
    D_D(DPageIndicator);

    d->pointColor = color;
}

/*!
 * \property DPageIndicator::secondaryPointColor
 * \brief This property holds the color used to draw the noraml dots.
 */
QColor DPageIndicator::secondaryPointColor() const
{
    D_DC(DPageIndicator);

    return d->secondaryPointColor;
}

void DPageIndicator::setSecondaryPointColor(QColor color)
{
    D_D(DPageIndicator);

    d->secondaryPointColor = color;
}

/*!
 * \property DPageIndicator::pointRadius
 * \brief This property holds the radius of the highlighted dot.
 */
int DPageIndicator::pointRadius() const
{
    D_DC(DPageIndicator);
    return  d->pointRadius;
}

void DPageIndicator::setPointRadius(int size)
{
    D_D(DPageIndicator);
    d->pointRadius = size;
}

/*!
 * \property DPageIndicator::secondaryPointRadius
 * \brief This property holds the radius of the normal dot.
 */
int DPageIndicator::secondaryPointRadius() const
{
    D_DC(DPageIndicator);
    return  d->secondaryPointRadius;
}

void DPageIndicator::setSecondaryPointRadius(int size)
{
    D_D(DPageIndicator);
    d->secondaryPointRadius = size;
}

/*!
 * \property DPageIndicator::pointDistance
 * \brief This property holds the distance between two dots.
 */
int DPageIndicator::pointDistance() const
{
    D_DC(DPageIndicator);
    return  d->pointDistance;
}

void DPageIndicator::setPointDistance(int distance)
{
    D_D(DPageIndicator);
    d->pointDistance = distance;
}

void DPageIndicator::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    const int w = width();
    const int h = height();

    D_DC(DPageIndicator);

    const int total_w = d->pageCount * d->pointDistance;
    const QPoint offset = QPoint((w - total_w) / 2, h / 2);

    QColor currentPtColor = d->pointColor;
    QColor nonCurrentPtColor = d->secondaryPointColor;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::transparent);
    for (int i(0); i != d->pageCount; ++i) {
        if (d->currentPage == i) {
            painter.setBrush(currentPtColor);
            painter.drawEllipse(offset + QPoint(d->pointDistance / 2 + d->pointDistance * i, 0),
                                d->pointRadius, d->pointRadius);
        } else {
            painter.setBrush(nonCurrentPtColor);
            painter.drawEllipse(offset + QPoint(d->pointDistance / 2 + d->pointDistance * i, 0),
                                d->secondaryPointRadius, d->secondaryPointRadius);
        }
    }
}

DPageIndicatorPrivate::DPageIndicatorPrivate(DPageIndicator *q) :
    DObjectPrivate(q),
    pageCount(0),
    currentPage(-1),
    pointColor(Qt::red)
{

}

void DPageIndicatorPrivate::setPageCount(const int count)
{
    pageCount = count;

    D_Q(DPageIndicator);

    q->update();
}

void DPageIndicatorPrivate::nextPage()
{
    currentPage = (currentPage + 1) % pageCount;

    D_Q(DPageIndicator);

    q->update();
}

void DPageIndicatorPrivate::previousPage()
{
    currentPage = (currentPage ? currentPage : pageCount) - 1;

    D_Q(DPageIndicator);

    q->update();
}

void DPageIndicatorPrivate::setCurrentPage(const int index)
{
    if (index < -1 || index >= pageCount) {
        qWarning() << "index out of bounds: " << index << ", max is " << pageCount;
        return;
    }

    currentPage = index;

    D_Q(DPageIndicator);

    q->update();
}

DWIDGET_END_NAMESPACE
