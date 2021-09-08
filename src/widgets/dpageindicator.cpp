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

#include "dpageindicator.h"
#include "private/dpageindicator_p.h"
#include "dapplicationhelper.h"

#include <DPalette>

#include <QDebug>
#include <QPainter>

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DPageIndicator
  \inmodule dtkwidget
  \brief The DPageIndicator class provides indicator of which page is currently showing.
  \brief DPageIndicator 类提供了一种用来指示一组元素中当前可见元素索引的控件.
  
  DPageIndicator is control that displays a horizontal series of dots,
  each of which corresponds to a page/widget may appear on top of it.
  
  It's inspired by UIPageControl of Apple.
  See \l {https://developer.apple.com/documentation/uikit/uipagecontrol}.
  
  控件会显示了一系列的点，代表一组元素。其中每一个点固定对应于元素组中
  的一个元素，元素组中的元素在一个时间点只能显示其中的一个（ 如 QStackedLayout ），
  当前显示的元素所对应的点会被高亮以告知用户元素左右（或者上下）还有多少元素可以切换。
  
  例如下图中控制中心首页插件区域底部的指示器：
  \image pageindicator.png
  
  \note 此控件只用来显示当前元素位置以及层叠关系，并不能用来实际操纵元素的显示行为。
  \note The design of this control is that it's just used to display, not
  to control which page is currently visible.
 */

/*!
  \brief DPageIndicator::DPageIndicator constructs an instance of DPageIndicator.
  \a parent is passed to QWidget constructor.
  
  
  \brief DPageIndicator::DPageIndicator 是 DPageIndicator 类的构造函数。
  \a parent 指定了控件的父控件。
 */
DPageIndicator::DPageIndicator(QWidget *parent)
    : QWidget(parent),
      DObject(*new DPageIndicatorPrivate(this), this)
{
    setMinimumHeight(d_func()->pointRadius * 2);
}

/*!
  \property DPageIndicator::pageCount
  \brief This property holds the number of pages that are represented by this widget.
  
  
  \property DPageIndicator::pageCount
  \brief DPageIndicator::pageCount 属性代表了控件显示的点的数量。
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
  \brief DPageIndicator::nextPage highlights the next dot.
  
  
  \brief DPageIndicator::nextPage 将高亮位置移至下一个点。
  
  \sa currentPage.
 */
void DPageIndicator::nextPage()
{
    D_D(DPageIndicator);

    d->nextPage();
}

/*!
  \brief DPageIndicator::previousPage hightlights the previous dot.
  
  
  \brief DPageIndicator::previousPage 将高亮位置移至前一个点。
  
  \sa currentPage.
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
  \property DPageIndicator::currentPage
  \brief This property holds the index of currently highlighted dot.
  
  
  \property DPageIndicator::currentPage
  \brief DPageIndicator::currentPage 属性代表了当前高亮的点在所有点中的索引。
 */
int DPageIndicator::currentPageIndex() const
{
    D_DC(DPageIndicator);

    return d->currentPage;
}

/*!
  \property DPageIndicator::pointColor
  \brief This property holds the color used to draw the highlighted dot.
  
  
  \property DPageIndicator::pointColor
  \brief DPageIndicator::pointColor 属性用来控制高亮点的颜色。
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
  \property DPageIndicator::secondaryPointColor
  \brief This property holds the color used to draw the noraml dots.
  
  
  \property DPageIndicator::secondaryPointColor
  \brief DPageIndicator::secondaryPointColor 属性用来控制默认点的颜色。
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
  \property DPageIndicator::pointRadius
  \brief This property holds the radius of the highlighted dot.
  
  
  \property DPageIndicator::pointRadius
  \brief DPageIndicator::pointRadius 属性用来控制高亮点的大小（半径）。
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
  \property DPageIndicator::secondaryPointRadius
  \brief This property holds the radius of the normal dot.
  
  
  \property DPageIndicator::secondaryPointRadius
  \brief DPageIndicator::secondaryPointRadius 属性用来控制普通点的大小（半径）。
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
  \property DPageIndicator::pointDistance
  \brief This property holds the distance between two dots.
  
  
  \property DPageIndicator::pointDistance
  \brief DPageIndicator::pointDistance 属性用来控制两个点之间的距离。
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

    if (!d->pointColor.isValid())
        currentPtColor = this->palette().highlight().color();

    if (!d->secondaryPointColor.isValid()) {
        nonCurrentPtColor = this->palette().button().color();
    }

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

DPageIndicatorPrivate::DPageIndicatorPrivate(DPageIndicator *q)
    : DObjectPrivate(q)
    , pageCount(3)
    , currentPage(1)
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
    currentPage = (pageCount == 0) ? 0 : (currentPage + 1) % pageCount;

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
