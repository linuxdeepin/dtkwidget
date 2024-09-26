// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dtoolbutton.h"

#include <QStyleOptionButton>
#include <QStylePainter>

DWIDGET_BEGIN_NAMESPACE

/*!
@~english
    @class Dtk::Widget::DToolButton
    @brief Inherited from the QToolButton class, used to customize tool buttons.
 */

DToolButton::DToolButton(QWidget *parent)
    : QToolButton(parent)
{

}

/*!
@~english
    @fn void DToolButton::paintEvent(QPaintEvent *event)
    @brief This method overrides the paintEvent method of QToolButton to draw a custom appearance for the tool button.
    @param event The paint event, which is not used.
 */

void DToolButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStylePainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    p.drawComplexControl(QStyle::CC_ToolButton, opt);
}

/*!
@~english
    @fn void DToolButton::initStyleOption(QStyleOptionToolButton *option) const
    @brief Initializes the style options for the tool button.
    @param option A pointer to a QStyleOptionToolButton object, used to specify style options.
 */
void DToolButton::initStyleOption(QStyleOptionToolButton *option) const
{
    QToolButton::initStyleOption(option);
}

/*!
@~english
    @fn QSize DToolButton::sizeHint() const
    @brief Gets the size hint for the tool button.
    @return The size hint for the tool button, of type QSize.
 */

QSize DToolButton::sizeHint() const
{
    return QToolButton::sizeHint();
}

/*!
@~english
    @fn void DToolButton::setAlignment(Qt::Alignment flag)
    @brief DToolButton:: setAlignment set the alignment of DToolButton
    @a flag alignment mode
 */
void DToolButton::setAlignment(Qt::Alignment flag)
{
    this->setProperty("_d_dtk_toolButtonAlign", QVariant(flag));
}

/*!
@~english
    @fn Qt::Alignment DToolButton::alignment() const
    @brief DToolButton:: alignment returns the current alignment of DToolButton
    @return If setAlignment is successfully set, the current alignment is returned; otherwise, Qt:: AlignLeft is returned
 */
Qt::Alignment DToolButton::alignment() const
{
    if(this->property("_d_dtk_toolButtonAlign").isValid())
        return static_cast<Qt::Alignment>(this->property("_d_dtk_toolButtonAlign").toInt());
    else
        return Qt::AlignLeft;
}

DWIDGET_END_NAMESPACE
