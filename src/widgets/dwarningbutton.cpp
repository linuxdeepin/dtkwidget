// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dwarningbutton.h"
#include "dpalettehelper.h"
#include "dstyleoption.h"

#include <QStyleOptionButton>
#include <QStylePainter>

DWIDGET_BEGIN_NAMESPACE

/*!
@~english
  @class Dtk::Widget::DWarningButton
  \inmodule dtkwidget
  @brief warning button widget
  @details inherits from DPushButton widget, mainly replacing the color of text and background.
  @note Usually gives a warning sign when executing some dangerous commands but not forbid it; For example: some programs require higher permission to run; files connot be restored after delete
  @sa QPushButton
 */

/*!
@~english
  @brief constructor
  @param[in] parent is the parent object of this widget
 */
DWarningButton::DWarningButton(QWidget *parent): DPushButton(parent)
{
}

/*!
@~english
  @brief initialize the style option
  @param[in] option is the style option used to initialize this widget
 */
void DWarningButton::initStyleOption(QStyleOptionButton *option) const
{
    DPushButton::initStyleOption(option);
    DPalette pa = DPaletteHelper::instance()->palette(this);
    option->palette.setBrush(QPalette::ButtonText, pa.textWarning());
    option->features |= QStyleOptionButton::ButtonFeature(DStyleOptionButton::WarningButton);
}

void DWarningButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QStyleOptionButton opt;
    initStyleOption(&opt);
    QStylePainter painter(this);
    painter.drawControl(QStyle::CE_PushButton, opt);
}

DWIDGET_END_NAMESPACE
