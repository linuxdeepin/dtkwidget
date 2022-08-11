// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dframe.h"
#include "dpalettehelper.h"
#include "private/dframe_p.h"

#include <DObjectPrivate>

#include <QPainter>
#include <QStyle>
#include <QStyleOptionFrame>

DWIDGET_BEGIN_NAMESPACE

DFramePrivate::DFramePrivate(DFrame *qq)
    : DCORE_NAMESPACE::DObjectPrivate(qq)
    , frameRounded(true)
    , backType(DPalette::NoType)
{

}

/*!
  \brief DFrame::DFrame 用于其他需要边框的widget的基类
  \a parent
 */
DFrame::DFrame(QWidget *parent)
    : DFrame(*new DFramePrivate(this), parent)
{

}

/*!
  \brief DFrame::setFrameRounded设置边框圆角是否开启
  \a on true开启　false关闭
 */
void DFrame::setFrameRounded(bool on)
{
    D_D(DFrame);

    if (d->frameRounded == on)
        return;

    d->frameRounded = on;
    update();
}

/*!
  \brief DFrame::setBackgroundRole　设置边框背景画刷的角色类型
  \a type 背景画刷的角色类型
 */
void DFrame::setBackgroundRole(DGUI_NAMESPACE::DPalette::ColorType type)
{
    D_D(DFrame);

    if (d->backType == type)
        return;

    d->backType = type;
    update();
}

DFrame::DFrame(DFramePrivate &dd, QWidget *parent)
    : QFrame(parent)
    , DObject(dd)
{
    setBackgroundRole(QPalette::Base);
    setFrameShape(QFrame::StyledPanel);
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

    const DPalette &dp = DPaletteHelper::instance()->palette(this);

    if (d->backType != DPalette::NoType) {
        p.setBackground(dp.brush(d->backType));
    }

    p.setPen(QPen(dp.frameBorder(), opt.lineWidth));
    style()->drawControl(QStyle::CE_ShapedFrame, &opt, &p, this);
}

DWIDGET_END_NAMESPACE
