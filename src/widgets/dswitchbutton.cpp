/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dswitchbutton.h"
#include <DStyle>
#include <DStyleOptionButton>
#include <private/dswitchbutton_p.h>

#include <QApplication>


DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \brief DSwitchButton::DSwitchButton　实现一个开关按钮
 * \~chinese \param parent
 */
DSwitchButton::DSwitchButton(QWidget *parent)
    : QAbstractButton(parent)
    , DObject(*new DSwitchButtonPrivate(this))
{
    D_D(DSwitchButton);

    d->init();
}

/*!
 * \~chinese \brief DSwitchButton::sizeHint 初始化控件矩形大小（在绘画之前）
 * \~chinese \return 控件举行大小
 */
QSize DSwitchButton::sizeHint() const
{
    QSize size(0, 0);
    DStyleHelper dstyle(style());
    size = dstyle.sizeFromContents(DStyle::CT_SwitchButton, nullptr, QSize(0, 0), this);

    return size;
}

/*!
 * \~chinese \brief DSwitchButton::paintEvent 绘画处理
 * \~chinese \param e 绘画事件
 * \~chinese \see QWidget::paintEvent()
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
 * \~chinese \brief DSwitchButton::initStyleOption 初始化(用于继承的)抽象按钮对象,后面用于 DStylePainter 绘画 DStyle::CE_SwitchButton 枚举
 * \~chinese \param option 初始化了的的抽象风格按钮对象
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
