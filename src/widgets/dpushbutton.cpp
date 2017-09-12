/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dpushbutton.h"
#include "private/dpushbutton_p.h"

DWIDGET_BEGIN_NAMESPACE

DPushButton::DPushButton(QWidget *parent)
    : QPushButton(parent)
    , DObject(*new DPushButtonPrivate(this))
{

}

DPushButton::DPushButton(const QString &normalIcon, const QString &hoverIcon, const QString &pressIcon, QWidget *parent)
    : DPushButton(parent)
{
    setNormalIcon(normalIcon);
    setHoverIcon(hoverIcon);
    setPressIcon(pressIcon);
}

const QString DPushButton::normalIcon() const
{
    D_DC(DPushButton);

    return d->normalIconPath;
}

const QString DPushButton::hoverIcon() const
{
    D_DC(DPushButton);

    return d->hoverIconPath;
}

const QString DPushButton::pressIcon() const
{
    D_DC(DPushButton);

    return d->pressIconPath;
}

void DPushButton::setNormalIcon(const QString &normalIcon)
{
    D_D(DPushButton);

    setNormalIcon(QIcon(normalIcon));

    d->normalIconPath = normalIcon;
}

void DPushButton::setHoverIcon(const QString &hoverIcon)
{
    D_D(DPushButton);

    setHoverIcon(QIcon(hoverIcon));

    d->hoverIconPath = hoverIcon;
}

void DPushButton::setPressIcon(const QString &pressIcon)
{
    D_D(DPushButton);

    setPressIcon(QIcon(pressIcon));

    d->pressIconPath = pressIcon;
}

void DPushButton::setNormalIcon(const QIcon &normalIcon)
{
    D_D(DPushButton);

    d->normalIconPath.clear();
    d->normalIcon = QIcon(normalIcon);

    setIcon(d->normalIcon);
}

void DPushButton::setHoverIcon(const QIcon &hoverIcon)
{
    D_D(DPushButton);

    d->hoverIconPath.clear();
    d->hoverIcon = QIcon(hoverIcon);
}

void DPushButton::setPressIcon(const QIcon &pressIcon)
{
    D_D(DPushButton);

    d->pressIconPath.clear();
    d->pressIcon = QIcon(pressIcon);
}

void DPushButton::enterEvent(QEvent *e)
{
    D_D(DPushButton);

    setIcon(d->hoverIcon);

    QPushButton::enterEvent(e);
}

void DPushButton::leaveEvent(QEvent *e)
{
    D_D(DPushButton);

    setIcon(d->normalIcon);

    QPushButton::leaveEvent(e);
}

void DPushButton::mousePressEvent(QMouseEvent *e)
{
    D_D(DPushButton);

    setIcon(d->pressIcon);

    QPushButton::mousePressEvent(e);
}

DPushButtonPrivate::DPushButtonPrivate(DPushButton *qq)
    : DObjectPrivate(qq)
{

}

DWIDGET_END_NAMESPACE
