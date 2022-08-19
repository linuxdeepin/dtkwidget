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
  \brief DSwitchButton::DSwitchButton　实现一个开关按钮
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
  \brief DSwitchButton::sizeHint 初始化控件矩形大小（在绘画之前）
  \return 控件举行大小
 */
QSize DSwitchButton::sizeHint() const
{
    QSize size(0, 0);
    DStyleHelper dstyle(style());
    size = dstyle.sizeFromContents(DStyle::CT_SwitchButton, nullptr, QSize(0, 0), this);

    return size;
}

/*!
  \brief DSwitchButton::paintEvent 绘画处理
  \a e 绘画事件
  \sa QWidget::paintEvent()
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
  \brief DSwitchButton::initStyleOption 初始化(用于继承的)抽象按钮对象,后面用于 DStylePainter 绘画 DStyle::CE_SwitchButton 枚举
  \a option 初始化了的的抽象风格按钮对象
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
