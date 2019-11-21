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
#include "dlabel.h"
#include "private/dlabel_p.h"

#include <DApplicationHelper>

#include <private/qlabel_p.h>

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \class DLabel
 * \~chinese \brief DLabel一个重新实现的 QLabel
 * \~chinese \li DLabel提供了将 DLabel 显示在指定位置的函数
 * \~chinese \li DLabel提供了改变字体颜色的函数
 */

/*!
 * \~chinese \brief DLabel的构造函数
 * \~chinese \param text文本信息
 * \~chinese \param parent参数被发送到 QLabel 构造函数。
 */
DLabel::DLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
    , DObject(*new DLabelPrivate(this))
{
    D_D(DLabel);
    d->init();
}

DLabel::DLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
    , DObject(*new DLabelPrivate(this))
{
    D_D(DLabel);
    d->init();
}

DLabel::~DLabel()
{

}

/*!
 * \brief DLabel::setForegroundRole显示的字体颜色
 * \param color字体颜色
 */
void DLabel::setForegroundRole(DPalette::ColorType color)
{
    D_D(DLabel);
    d->color = color;
}

DLabel::DLabel(DLabelPrivate &dd, QWidget *parent)
    : DObject(dd)
    , QLabel(parent)
{
    dd.init();
}

void DLabel::initPainter(QPainter *painter) const
{
    D_DC(DLabel);
    QLabel::initPainter(painter);
    if (d->color != DPalette::NoType) {
        QBrush color = DApplicationHelper::instance()->palette(this).brush(d->color);
        painter->setPen(QPen(color.color()));
    }
}

void DLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);

    QTextOption opt(alignment());

    if (wordWrap()) {
        opt.setWrapMode(QTextOption::WordWrap);
    }

    p.drawText(contentsRect(), text(), opt);
}

DLabelPrivate::DLabelPrivate(DLabel *q)
    : DObjectPrivate(q)
{
}

void DLabelPrivate::init()
{

}

DWIDGET_END_NAMESPACE
