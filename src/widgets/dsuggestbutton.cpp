// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dsuggestbutton.h"
#include "dstyleoption.h"

#include <QStylePainter>
#include <QStyleOptionButton>
#include <DGuiApplicationHelper>

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DSuggestButton
  \inmodule dtkwidget
  \brief 这个控件提供了DTK中默认的按钮样式.
 */

/*!
  \brief DSuggestButton的构造函数
  
  \a parent
 */
DSuggestButton::DSuggestButton(QWidget *parent)
    : QPushButton(parent)
{

}

/*!
  \brief DSuggestButton的重载构造函数，接收默认显示的文字
  
  \a text 默认显示的文字
  \a parent
 */
DSuggestButton::DSuggestButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{

}


void DSuggestButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStylePainter p(this);
    DStyleOptionButton option;
    initStyleOption(&option);
    option.init(this);
    option.features |= QStyleOptionButton::ButtonFeature(DStyleOptionButton::SuggestButton);

    QColor startColor = palette().color(QPalette::Highlight);
    QColor endColor = DGuiApplicationHelper::adjustColor(startColor, 0, 0, +10, 0, 0, 0, 0);

    option.palette.setBrush(QPalette::Light, QBrush(endColor));
    option.palette.setBrush(QPalette::Dark, QBrush(startColor));
    option.palette.setBrush(QPalette::ButtonText, option.palette.highlightedText());
    p.drawControl(QStyle::CE_PushButton, option);
}

DWIDGET_END_NAMESPACE
