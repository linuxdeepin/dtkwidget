// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dcommandlinkbutton.h"
#include "dpalettehelper.h"

#include <DStyleOption>
#include <DStylePainter>

DWIDGET_BEGIN_NAMESPACE

enum Margins{
    LeftMargins = 3,
    RightMargins = LeftMargins,
    TopLeftMargins = 3,
    BottomMargins = LeftMargins,
    TextMargins = 4
};

/*!
  @~english
  \class Dtk::Widget::DCommandLinkButton
  \inmodule dtkwidget

  \brief `DCommandLinkButton`  A button inherited in `QABSTRACTBUTTON`, the shape and link are very similar;
  It can also be a link with arrows.Commonly used after clicking, jump to another window or page, such as the forward and back button of the browser
 */

/*!
  @~english
  \brief Constructor
  \a text The text displayed by the control
  \a parent The father of the control
 */
DCommandLinkButton::DCommandLinkButton(const QString text, QWidget *parent)
    : QAbstractButton(parent)
{
    this->setText(text);
}

/*!
  @~english
  \brief Get the rectangle size of the control
  \return Return the control rectangle size
 */
QSize DCommandLinkButton::sizeHint() const
{
    QString text = this->text();
    QSize size = fontMetrics().size(0, text);
    size += QSize(LeftMargins + RightMargins + TextMargins *2, TopLeftMargins + BottomMargins);
    return size;
}

/*!
  @~english
  \brief The style of an Option, and some basic attributes
  \a option Real parameters are a (button control) style attribute used to initialize
 */
void DCommandLinkButton::initStyleOption(DStyleOptionButton *option) const
{
    option->initFrom(this);

    if (isDown())
        option->state |= QStyle::State_Sunken;

    DPalette pa = DPaletteHelper::instance()->palette(this);
    option->palette.setBrush(DPalette::ButtonText, pa.highlight());
    option->text = this->text();
}

/*!
  @~english
  \brief Painting incident
  \a e Not used here
 */
void DCommandLinkButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    DStyleOptionButton opt;
    initStyleOption(&opt);

    DStylePainter painter(this);
    painter.drawControl(DStyle::CE_TextButton, opt);
}

DWIDGET_END_NAMESPACE
