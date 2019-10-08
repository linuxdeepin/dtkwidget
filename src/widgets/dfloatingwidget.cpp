/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zhangwengeng <zhangwengeng_cm@deepin.com>
 *
 * Maintainer: zhangwengeng <zhangwengeng_cm@deepin.com>
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

#include "dfloatingwidget.h"
#include "private/dfloatingwidget_p.h"
#include "dstyleoption.h"
#include "dstyle.h"
#include "dblureffectwidget.h"

#include <QHBoxLayout>
#include <QEvent>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

//DFloatingWidgetPrivate 为 DFloatingWidget 的私有数据类
DFloatingWidgetPrivate::DFloatingWidgetPrivate(DFloatingWidget *qq)
    : DObjectPrivate(qq)
{
}

DFloatingWidgetPrivate::~DFloatingWidgetPrivate()
{
}

void DFloatingWidgetPrivate::init()
{
}

void DFloatingWidgetPrivate::adjustPalette()
{
    if (!layout || layout->count() == 0)
        return;

    QWidget *content = layout->itemAt(0)->widget();

    if (background) {
        D_Q(DFloatingWidget);
        // 开启模糊背景后应到调整调色板
        QPalette pa = q->palette();
        pa.setBrush(QPalette::Button, pa.brush(QPalette::Base));
        pa.setBrush(QPalette::Light, pa.brush(QPalette::Base));
        pa.setBrush(QPalette::Dark, pa.brush(QPalette::Base));

        content->setPalette(pa);
    } else {
        content->setPalette(QPalette());
    }
}

void DFloatingWidget::setWidget(QWidget *widget)
{
    D_D(DFloatingWidget);

    if (!d->layout) {
        d->layout = new QHBoxLayout(this);
        d->layout->setMargin(0);
    }

    for (int i = 0; i < d->layout->count(); ++i) {
        delete d->layout->takeAt(i);
    }

    d->layout->addWidget(widget);
    d->adjustPalette();
}

DFloatingWidget::DFloatingWidget(DFloatingWidgetPrivate &dd, QWidget *parent)
    : QWidget(parent)
    , DObject(dd)
{
    D_D(DFloatingWidget);
    d->init();
    setAutoFillBackground(false);
}

DFloatingWidget::DFloatingWidget(QWidget *parent)
    : DFloatingWidget(*new DFloatingWidgetPrivate(this), parent)
{

}

QSize DFloatingWidget::sizeHint() const
{
    D_DC(DFloatingWidget);

    QSize size = d->layout ? d->layout->sizeHint() : QSize(0, 0);
    DStyleHelper dstyle(style());
    size = dstyle.sizeFromContents(DStyle::CT_FloatingWidget, nullptr, size, this);

    return size;
}

void DFloatingWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    DStylePainter painter(this);
    DStyleOptionFloatingWidget opt;
    initStyleOption(&opt);
    painter.drawControl(DStyle::CE_FloatingWidget, opt);
}

bool DFloatingWidget::event(QEvent *event)
{
    D_D(DFloatingWidget);

    if (event->type() == QEvent::Polish) {
        DStyleHelper dstyle(style());
        int margins = dstyle.pixelMetric(DStyle::PM_FloatingWidgetShadowMargins, nullptr, this);
        setContentsMargins(margins, margins, margins, margins);

        if (d->background) {
            int radius = DStyleHelper(style()).pixelMetric(DStyle::PM_TopLevelWindowRadius);

            d->background->setBlurRectXRadius(radius);
            d->background->setBlurRectYRadius(radius);
            d->background->setGeometry(contentsRect());

            d->adjustPalette();
        }
    } else if (event->type() == QEvent::PaletteChange) {
        d->adjustPalette();
    }

    return QWidget::event(event);
}

void DFloatingWidget::initStyleOption(DStyleOptionFloatingWidget *option) const
{
    option->init(this);
    option->initFrom(this);
    option->noBackground = blurBackgroundIsEnabled();
}

bool DFloatingWidget::blurBackgroundIsEnabled() const
{
    D_DC(DFloatingWidget);

    return d->background;
}

DBlurEffectWidget *DFloatingWidget::blurBackground() const
{
    D_DC(DFloatingWidget);

    return d->background;
}

void DFloatingWidget::setBlurBackgroundEnabled(bool blurBackgroundEnabled)
{
    D_D(DFloatingWidget);

    if (bool(d->background) == blurBackgroundEnabled)
        return;

    if (blurBackgroundEnabled) {
        d->background = new DBlurEffectWidget(this);
        d->background->setGeometry(contentsRect());
        d->background->lower();
        d->background->setFocusPolicy(Qt::NoFocus);
        d->background->setAttribute(Qt::WA_TransparentForMouseEvents);
        d->background->show();

        int radius = DStyleHelper(style()).pixelMetric(DStyle::PM_TopLevelWindowRadius);

        d->background->setBlurRectXRadius(radius);
        d->background->setBlurRectYRadius(radius);
    } else {
        d->background->hide();
        d->background->deleteLater();
    }

    d->adjustPalette();
}

DWIDGET_END_NAMESPACE
