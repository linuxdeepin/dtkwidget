/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     wp <wangpeng_cm@deepin.com>
 *
 * Maintainer: wp <wangpeng_cm@deepin.com>
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

#include "dtiplabel.h"
#include "private/dtiplabel_p.h"
#include "private/qlabel_p.h"

#include <DFontSizeManager>
#include <DApplicationHelper>

DWIDGET_BEGIN_NAMESPACE

DTipLabel::DTipLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent, Qt::ToolTip | Qt::BypassGraphicsProxyWidget)
    , DObject(*new DTipLabelPrivate(this))
{
    D_D(DTipLabel);
    d->init();
}

DTipLabel::~DTipLabel()
{

}

void DTipLabel::show(const QPoint &pos)
{
    move(pos);

    QLabel::show();
}

void DTipLabel::setForegroundRole(DPalette::ColorType color)
{
    D_D(DTipLabel);
    d->color = color;
}

void DTipLabel::initPainter(QPainter *painter) const
{
    D_DC(DTipLabel);
    QLabel::initPainter(painter);
    if (d->color != DPalette::NoType) {
        QBrush color = DApplicationHelper::instance()->palette(this).brush(d->color);
        painter->setPen(QPen(color.color()));
    }
}

void DTipLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.drawText(contentsRect(), alignment(), text());
}

DTipLabelPrivate::DTipLabelPrivate(DTipLabel *q)
    : DObjectPrivate(q)
{
}

void DTipLabelPrivate::init()
{
    Q_Q(DTipLabel);
    q->setAlignment(Qt::AlignCenter);

    DFontSizeManager::instance()->bind(q, DFontSizeManager::T7);
}

DWIDGET_END_NAMESPACE
