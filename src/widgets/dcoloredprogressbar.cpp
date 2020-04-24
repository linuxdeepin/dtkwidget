/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     Chris Xiong <chirs241097@gmail.com>
 *
 * Maintainer: Chris Xiong <chirs241097@gmail.com>
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

#include "dcoloredprogressbar.h"

#include <DObjectPrivate>

#include <QMap>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionProgressBar>

DWIDGET_BEGIN_NAMESPACE

class DColoredProgressBarPrivate : DTK_CORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DColoredProgressBar)
    DColoredProgressBarPrivate(DColoredProgressBar *q);
private:
    QMap<int, QBrush> threshmap;
};

DWIDGET_END_NAMESPACE

DWIDGET_USE_NAMESPACE

DColoredProgressBarPrivate::DColoredProgressBarPrivate(DColoredProgressBar *q)
    : DObjectPrivate(q)
{
}

/*!
 * \class DColoredProgressBar
 * \brief DColoredProgressBar is the same as QProgressBar, except it can change its appearance depending on the value displayed.
 */

/*!
 * \~chinese \class DColoredProgressBar::DColoredProgressBar
 * \~chinese \brief DColoredProgressBar和QProgressBar功能差不多一样,只是它可以根据显示的值更改其外观
 */
DColoredProgressBar::DColoredProgressBar(QWidget *parent)
    : QProgressBar(parent)
    , DObject(*new DColoredProgressBarPrivate(this))
{
}

/*!
 * \brief DColoredProgressBar::addThreshold adds a new threshold value and specifies the brush to use once that value is reached.
 * If a threshold of the same value already exists, it will be overwritten.
 * \param threshold Minimum value for this brush to be used.
 * \param brush The brush to use when the currently displayed value is no less than \threshold and less than the next threshold value.
 */

/*!
 * \~chinese \brief DColoredProgressBar::addThreshold 添加一个新的阈值，并指定达到该值后要使用的画笔。如果一个相同值的阈值已经存在，它将被覆盖。
 * \~chinese \param threshold 使用此画笔的最小值。
 * \~chinese \param brush 当前显示的值不小于 threshold且小于下一个阈值时使用的画笔。
 */
void DColoredProgressBar::addThreshold(int threshold, QBrush brush)
{
    D_D(DColoredProgressBar);
    d->threshmap[threshold] = brush;
}

/*!
 * \brief DColoredProgressBar::removeThreshold removes a threshold.
 * \param threshold The threshold value to remove.
 */

/*!
 * \~chinese \brief DColoredProgressBar::removeThreshold 移除一个threshold
 * \~chinese \param threshold 被移除的threshold值
 */
void DColoredProgressBar::removeThreshold(int threshold)
{
    D_D(DColoredProgressBar);
    if (d->threshmap.contains(threshold)) {
        d->threshmap.remove(threshold);
    }
}

/*!
 * \brief DColoredProgressBar::threadsholds gets all threshold values.
 * \return A list of threshold values.
 */

/*!
 * \~chinese \brief DColoredProgressBar::thresholds 获取所有的thresholds值
 * \~chinese \return  返回一个　threshold值的列表
 */
QList<int> DColoredProgressBar::thresholds() const
{
    D_D(const DColoredProgressBar);
    return d->threshmap.keys();
}

void DColoredProgressBar::paintEvent(QPaintEvent *)
{
    D_D(DColoredProgressBar);

    QStylePainter painter(this);
    QStyleOptionProgressBar styopt;
    initStyleOption(&styopt);
    if (d->threshmap.upperBound(value()) != d->threshmap.begin()) {
        styopt.palette.setBrush(QPalette::ColorRole::Highlight, (--d->threshmap.upperBound(value())).value());
    }

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.drawRect(styopt.rect);

    painter.drawControl(QStyle::ControlElement::CE_ProgressBarGroove, styopt);
    painter.drawControl(QStyle::ControlElement::CE_ProgressBarContents, styopt);

    if (styopt.textVisible && styopt.orientation == Qt::Horizontal) {
        painter.drawControl(QStyle::ControlElement::CE_ProgressBarLabel, styopt);
    }
}
