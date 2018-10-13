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

/*!
 * \~chinese \class DPushButton
 * \~chinese \brief 封装了三种状态的QPushButton，可以自动在不同状态更换图标，不需要自己控制。
 */

/*!
 * \~chinese \brief DPushButton的构造函数
 *
 * @param parent
 */
DPushButton::DPushButton(QWidget *parent)
    : QPushButton(parent)
    , DObject(*new DPushButtonPrivate(this))
{

}

/*!
 * \~chinese \brief DPushButton的重载构造函数，初始化时接收不同状态的图标
 *
 * @param normalIcon 正常状态下的图标
 * @param hoverIcon 鼠标在按钮上时的图标
 * @param pressIcon 鼠标按下时的图标
 * @param parent 初始化QPushButton的构造函数
 */
DPushButton::DPushButton(const QString &normalIcon, const QString &hoverIcon, const QString &pressIcon, QWidget *parent)
    : DPushButton(parent)
{
    setNormalIcon(normalIcon);
    setHoverIcon(hoverIcon);
    setPressIcon(pressIcon);
}

/*!
 * \~chinese \property DPushButton::normalIcon
 * \~chinese \brief 该属性控制正常状态下的图标
 */
const QString DPushButton::normalIcon() const
{
    D_DC(DPushButton);

    return d->normalIconPath;
}

/*!
 * \~chinese \property DPushButton::hoverIcon
 * \~chinese \brief 该属性控制鼠标在按钮上时的图标
 */
const QString DPushButton::hoverIcon() const
{
    D_DC(DPushButton);

    return d->hoverIconPath;
}

/*!
 * \~chinese \property DPushButton::pressIcon
 * \~chinese \brief 该属性控制鼠标按下时的图标
 */
const QString DPushButton::pressIcon() const
{
    D_DC(DPushButton);

    return d->pressIconPath;
}

/*!
 * \~chinese \brief 设置正常状态下的图标
 *
 * @param normalIcon
 */
void DPushButton::setNormalIcon(const QString &normalIcon)
{
    D_D(DPushButton);

    setNormalIcon(QIcon(normalIcon));

    d->normalIconPath = normalIcon;
}

/*!
 * \~chinese \brief 设置鼠标在按钮上时的图标
 *
 * @param hoverIcon
 */
void DPushButton::setHoverIcon(const QString &hoverIcon)
{
    D_D(DPushButton);

    setHoverIcon(QIcon(hoverIcon));

    d->hoverIconPath = hoverIcon;
}

/*!
 * \~chinese \brief 设置鼠标按下时的图标
 *
 * @param pressIcon
 */
void DPushButton::setPressIcon(const QString &pressIcon)
{
    D_D(DPushButton);

    setPressIcon(QIcon(pressIcon));

    d->pressIconPath = pressIcon;
}

/*!
 * \~chinese \brief 重载的方法
 * \see setNormalIcon
 * @param normalIcon
 */
void DPushButton::setNormalIcon(const QIcon &normalIcon)
{
    D_D(DPushButton);

    d->normalIconPath.clear();
    d->normalIcon = QIcon(normalIcon);

    setIcon(d->normalIcon);
}

/*!
 * \~chinese \brief 重载的方法
 * \see setHoverIcon
 * @param hoverIcon
 */
void DPushButton::setHoverIcon(const QIcon &hoverIcon)
{
    D_D(DPushButton);

    d->hoverIconPath.clear();
    d->hoverIcon = QIcon(hoverIcon);
}

/*!
 * \~chinese \brief 重载的方法
 * \see setPressIcon
 * @param pressIcon
 */
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
