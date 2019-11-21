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

#include <DFontSizeManager>

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
    : DLabel(*new DTipLabelPrivate(this), parent)
{
    setText(text);

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
    if (isWindow()) {
        setWindowFlag(Qt::ToolTip);
    }

    move(pos);

    QLabel::show();
}

/*!
 * \brief DTipLabel::setForegroundRole显示的字体颜色
 * \param color字体颜色
 */
void DTipLabel::setForegroundRole(DPalette::ColorType color)
{
    DLabel::setForegroundRole(color);
}

void DTipLabel::initPainter(QPainter *painter) const
{
    DLabel::initPainter(painter);
}

void DTipLabel::paintEvent(QPaintEvent *event)
{
    DLabel::paintEvent(event);
}

DTipLabelPrivate::DTipLabelPrivate(DTipLabel *q)
    : DLabelPrivate(q)
{
    color = DPalette::TextTips;
}

void DTipLabelPrivate::init()
{
    Q_Q(DTipLabel);
    q->setAlignment(Qt::AlignCenter);

    DFontSizeManager::instance()->bind(q, DFontSizeManager::T7);
}

DWIDGET_END_NAMESPACE
