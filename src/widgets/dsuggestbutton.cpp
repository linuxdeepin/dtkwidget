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
@~english
  @class Dtk::Widget::DSuggestButton
  @brief This widget provides the default suggest button style in DTK.
 */

/*!
@~english
  @brief Constructor of DSuggestButton
  @param[in] parent
 */
DSuggestButton::DSuggestButton(QWidget *parent)
    : QPushButton(parent)
{

}

/*!
@~english
  @brief Overloaded constructor of DSuggestButton that accepts the default displayed text.
  @param[in] text The default displayed text
  @param[in] parent
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
