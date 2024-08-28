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
  \class Dtk::Widget::DColoredProgressBar
  \inmodule dtkwidget

  \brief DColoredProgressBar is the same as QProgressBar, except it can change its appearance depending on the value displayed.
  \brief DColoredProgressBar和QProgressBar功能差不多一样,只是它可以根据显示的值更改其外观
 */
DColoredProgressBar::DColoredProgressBar(QWidget *parent)
    : QProgressBar(parent)
    , DObject(*new DColoredProgressBarPrivate(this))
{
}

/*!
  \brief DColoredProgressBar::addThreshold 添加一个新的阈值，并指定达到该值后要使用的画笔。如果一个相同值的阈值已经存在，它将被覆盖。
  \brief DColoredProgressBar::addThreshold adds a new threshold value and specifies the brush to use once that value is reached.

  If a threshold of the same value already exists, it will be overwritten.
  \a brush 当前显示的值不小于 threshold且小于下一个阈值时使用的画笔。
  \a brush The brush to use when the currently displayed value is no less than and less than the next threshold value.
  \a threshold 使用此画笔的最小值。
  \a threshold Minimum value for this brush to be used.
 */
void DColoredProgressBar::addThreshold(int threshold, QBrush brush)
{
    D_D(DColoredProgressBar);
    d->threshmap[threshold] = brush;
}

/*!
  \brief DColoredProgressBar::removeThreshold removes a threshold.
  \brief DColoredProgressBar::removeThreshold 移除一个threshold

  \a threshold The threshold value to remove.
  \a threshold 被移除的threshold值
 */
void DColoredProgressBar::removeThreshold(int threshold)
{
    D_D(DColoredProgressBar);
    if (d->threshmap.contains(threshold)) {
        d->threshmap.remove(threshold);
    }
}

/*!
  \brief DColoredProgressBar::threadsholds gets all threshold values.
  \brief DColoredProgressBar::thresholds 获取所有的thresholds值

  \return A list of threshold values.
  \return  返回一个　threshold值的列表
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
