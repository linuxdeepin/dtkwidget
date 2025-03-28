// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dtiplabel.h"
#include "private/dtiplabel_p.h"

#include <DFontSizeManager>

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DTipLabel
  \inmodule dtkwidget
  \brief DTipLabel一个重新实现的 QLabel.

  \list
  \li DTipLabel提供了将 DTipLabel 显示在指定位置的函数
  \li DTipLabel提供了改变字体颜色的函数
  \endlist
 */

/*!
  \brief DTipLabel的构造函数
  \a text 文本信息
  \a parent 参数被发送到 QLabel 构造函数。
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
  \brief DTipLabel::show显示在指定的位置上
  \a pos 显示位置
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
  \brief DTipLabel::setForegroundRole显示的字体颜色
  \a color 字体颜色
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

    DFontSizeManager::instance()->bind(q, DFontSizeManager::T7);
}

DWIDGET_END_NAMESPACE
