/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     Chris Xiong <chirs241097@gmail.com>
 *
 * Maintainer: Chris Xiong <chirs241097@gmail.com>
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
void DColoredProgressBar::addThreshold(int threshold, QBrush brush)
{
    D_D(DColoredProgressBar);
    d->threshmap[threshold] = brush;
}

/*!
 * \brief DColoredProgressBar::removeThreshold removes a threshold.
 * \param threshold The threshold value to remove.
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
