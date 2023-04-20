// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dswitchbutton.h"
#include <DStyle>
#include <DStyleOptionButton>
#include "private/dswitchbutton_p.h"

#include <QApplication>


DWIDGET_BEGIN_NAMESPACE

/*!
@~english
  @brief DSwitchButton::DSwitchButton implements a switch button
  \a parent
 */
DSwitchButton::DSwitchButton(QWidget *parent)
    : QAbstractButton(parent)
    , DObject(*new DSwitchButtonPrivate(this))
{
    D_D(DSwitchButton);

    d->init();
}

/*!
@~english
  @brief DSwitchButton::sizeHint initializes the control rectangle size (before drawing)
  @return Control rectangle size
 */
QSize DSwitchButton::sizeHint() const
{
    QSize size(0, 0);
    DStyleHelper dstyle(style());
    size = dstyle.sizeFromContents(DStyle::CT_SwitchButton, nullptr, QSize(0, 0), this);

    return size;
}

/*!
@~english
  @brief DSwitchButton::paintEvent Painting treatment
  \a e Painting event
  @sa QWidget::paintEvent()
 */
void DSwitchButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    DStylePainter painter(this);
    DStyleOptionButton opt;
    initStyleOption(&opt);
    painter.drawControl(DStyle::CE_SwitchButton, opt);
}

/*!
@~english
  @brief DSwitchButton::initStyleOption Initializes the abstract button object (for inheritance), which is later used for DStylePainter painting DStyle::CE_SwitchButton enumeration
  \a option The abstract style button object is initialized
 */
void DSwitchButton::initStyleOption(DStyleOptionButton *option) const
{
    if (!option)
        return;

    option->init(this);
    option->initFrom(this);

    if (isChecked()) {
        option->state |= QStyle::State_On;
    } else {
        option->state |= QStyle::State_Off;
    }

    // 针对switch button禁用hover状态
    option->state &= ~QStyle::State_MouseOver;

    if (hasFocus()) {
        option->state |= QStyle::State_HasFocus;
    }

    //按照设计要求: 这里需要预留绘制focusRect的区域
    option->rect.adjust(4, 4, -4, -4);
}

DSwitchButtonPrivate::DSwitchButtonPrivate(DSwitchButton *qq)
    : DObjectPrivate(qq)
{

}

DSwitchButtonPrivate::~DSwitchButtonPrivate()
{

}

void DSwitchButtonPrivate::init()
{
    checked = false;
    animationStartValue = 0;
    animationEndValue = 1;

    D_Q(DSwitchButton);

    q->setObjectName("DSwitchButton");
    q->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    q->setCheckable(true);
    q->connect(q, &DSwitchButton::toggled, q, &DSwitchButton::checkedChanged);
}

DWIDGET_END_NAMESPACE
