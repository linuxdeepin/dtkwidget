// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
@~english
  @class Dtk::Widget::DColoredProgressBar
  @ingroup dtkwidget
  @brief DColoredProgressBar is the same as QProgressBar, except it can change its appearance depending on the value displayed.
 */
DColoredProgressBar::DColoredProgressBar(QWidget *parent)
    : QProgressBar(parent)
    , DObject(*new DColoredProgressBarPrivate(this))
{
}

/*!
@~english
  @brief DColoredProgressBar::addThreshold adds a new threshold value and specifies the brush to use once that value is reached.
  If a threshold of the same value already exists, it will be overwritten.
  @param[in] brush The brush to use when the currently displayed value is no less than and less than the next threshold value.
  @param[in] threshold Minimum value for this brush to be used.
 */
void DColoredProgressBar::addThreshold(int threshold, QBrush brush)
{
    D_D(DColoredProgressBar);
    d->threshmap[threshold] = brush;
}

/*!
@~english
  @brief DColoredProgressBar::removeThreshold removes a threshold.
  @param[in] threshold The threshold value to remove.
 */
void DColoredProgressBar::removeThreshold(int threshold)
{
    D_D(DColoredProgressBar);
    if (d->threshmap.contains(threshold)) {
        d->threshmap.remove(threshold);
    }
}

/*!
@~english
  @brief DColoredProgressBar::threadsholds gets all threshold values.
  @return A list of threshold values.
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

    painter.drawControl(QStyle::ControlElement::CE_ProgressBar, styopt);

}
