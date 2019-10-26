/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#include "dframe.h"
#include "dapplicationhelper.h"

#include <DObjectPrivate>

#include <QPainter>
#include <QStyle>
#include <QStyleOptionFrame>

DWIDGET_BEGIN_NAMESPACE

class DFramePrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DFramePrivate(DFrame *qq)
        : DObjectPrivate(qq)
    {

    }

    bool frameRounded = true;
    DPalette::ColorType backType = DPalette::NoType;
};

DFrame::DFrame(QWidget *parent)
    : QFrame(parent)
    , DObject(*new DFramePrivate(this))
{
    setBackgroundRole(QPalette::Base);
    setFrameShape(QFrame::StyledPanel);
}

void DFrame::setFrameRounded(bool on)
{
    D_D(DFrame);

    if (d->frameRounded == on)
        return;

    d->frameRounded = on;
    update();
}

void DFrame::setBackgroundRole(DPalette::ColorType type)
{
    D_D(DFrame);

    if (d->backType == type)
        return;

    d->backType = type;
    update();
}

void DFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOptionFrame opt;
    initStyleOption(&opt);
    QPainter p(this);
    D_DC(DFrame);

    if (d->frameRounded) {
        opt.features |= QStyleOptionFrame::Rounded;
    }

    const DPalette &dp = DApplicationHelper::instance()->palette(this);

    if (d->backType != DPalette::NoType) {
        p.setBackground(dp.brush(d->backType));
    }

    p.setPen(QPen(dp.frameBorder(), opt.lineWidth));
    style()->drawControl(QStyle::CE_ShapedFrame, &opt, &p, this);
}

DWIDGET_END_NAMESPACE
