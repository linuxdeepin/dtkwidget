/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dswitchbutton.h"
#include <DStyle>
#include <DStyleOptionButton>
#include <private/dswitchbutton_p.h>

#include <QApplication>


DWIDGET_BEGIN_NAMESPACE

DSwitchButton::DSwitchButton(QWidget *parent)
    : QAbstractButton(parent)
    , DObject(*new DSwitchButtonPrivate(this))
{
    setObjectName("DSwitchButton");
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setCheckable(true);

    D_D(DSwitchButton);

    d->init();

    connect(d->animation, &QVariantAnimation::valueChanged, [&]() {
        this->update();
    });
}

QSize DSwitchButton::sizeHint() const
{
    QSize size(0, 0);
    DStyleHelper dstyle(style());
    size = dstyle.sizeFromContents(DStyle::CT_SwitchButton, nullptr, QSize(0, 0), this);

    return size;
}

void DSwitchButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    DStylePainter painter(this);
    DStyleOptionButton opt;
    initStyleOption(&opt);
    painter.drawControl(DStyle::CE_SwitchButton, opt);
}

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
}

DSwitchButtonPrivate::DSwitchButtonPrivate(DSwitchButton *qq)
    : DObjectPrivate(qq)
{
    init();
}

DSwitchButtonPrivate::~DSwitchButtonPrivate()
{
    animation->deleteLater();
}

void DSwitchButtonPrivate::init()
{
    checked = false;
    animation = new QVariantAnimation;
    animationStartValue = 0;
    animationEndValue = 1;
}

DWIDGET_END_NAMESPACE
