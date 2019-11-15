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


/*!
 * \~chinese \class DTipLabel
 * \~chinese \brief DTipLabel一个重新实现的 QLabel
 * \~chinese \li DTipLabel提供了将 DTipLabel 显示在指定位置的函数
 * \~chinese \li DTipLabel提供了改变字体颜色的函数
 */

/*!
 * \~chinese \brief DTipLabel的构造函数
 * \~chinese \param text文本信息
 * \~chinese \param parent参数被发送到 QLabel 构造函数。
 */
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

/*!
 * \brief DTipLabel::show显示在指定的位置上
 * \param pos 显示位置
 */
void DTipLabel::show(const QPoint &pos)
{
    move(pos);

    QLabel::show();
}

/*!
 * \brief DTipLabel::setForegroundRole显示的字体颜色
 * \param color字体颜色
 */
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

    QTextOption opt(alignment());

    if (wordWrap()) {
        opt.setWrapMode(QTextOption::WordWrap);
    }

    p.drawText(contentsRect(), text(), opt);
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
