/*
 * Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#include "diconbutton.h"
#include "dstyleoption.h"
#include "dobject_p.h"
#include "dstyle.h"
#include "private/diconbutton_p.h"

#include <private/qabstractbutton_p.h>

DWIDGET_BEGIN_NAMESPACE

DIconButtonPrivate::DIconButtonPrivate(DIconButton *qq)
    : DObjectPrivate(qq)
{

}

/*!
 * \~chinese \brief DIconButton::DIconButton 实现一个方便定义图标的button
 * \~chinese \param parent
 */
DIconButton::DIconButton(QWidget *parent)
    : DIconButton(*new DIconButtonPrivate(this), parent)
{

}

DIconButton::DIconButton(QStyle::StandardPixmap iconType, QWidget *parent)
    : DIconButton(static_cast<DStyle::StandardPixmap>(iconType), parent)
{

}

DIconButton::DIconButton(DStyle::StandardPixmap iconType, QWidget *parent)
    : DIconButton(parent)
{
    d_func()->iconType = static_cast<qint64>(iconType);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

DIconButton::~DIconButton()
{

}

/*!
 * \~chinese \brief DIconButton::setIcon　设置button的图标
 * \~chinese \param icon　button的图标
 */
void DIconButton::setIcon(const QIcon &icon)
{
    D_D(DIconButton);

    d->iconType = -1;
    QAbstractButton::setIcon(icon);
}

/*!
 * \~chinese \brief DIconButton::setIcon 设置button图标的类型,传入QT预定义枚举值
 * \~chinese \param iconType icon类型
 */
void DIconButton::setIcon(QStyle::StandardPixmap iconType)
{
    D_D(DIconButton);

    d->iconType = iconType;
    QAbstractButton::setIcon(style()->standardIcon(iconType, nullptr, this));
}

/*!
 * \~chinese \brief DIconButton::setIcon 设置button图标的类型,传入DTK预定义枚举值
 * \~chinese \param iconType icon类型
 */
void DIconButton::setIcon(DStyle::StandardPixmap iconType)
{
    D_D(DIconButton);

    d->iconType = iconType;
    QAbstractButton::setIcon(DStyleHelper(style()).standardIcon(iconType, nullptr, this));
}

QSize DIconButton::sizeHint() const
{
    QAbstractButtonPrivate *bp = static_cast<QAbstractButtonPrivate*>(QAbstractButton::d_ptr.data());

    if (bp->sizeHint.isValid()) {
        return bp->sizeHint;
    }

    DStyleOptionButton opt;
    initStyleOption(&opt);

    opt.rect.setSize(opt.iconSize);
    bp->sizeHint = DStyleHelper(style()).sizeFromContents(DStyle::CT_IconButton, &opt, opt.iconSize, this).expandedTo(QApplication::globalStrut());
    int size = qMax(bp->sizeHint.width(), bp->sizeHint.height());
    bp->sizeHint.setHeight(size);
    bp->sizeHint.setWidth(size);

    return bp->sizeHint;
}

QSize DIconButton::minimumSizeHint() const
{
    return sizeHint();
}

/*!
 * \~chinese \brief DIconButton::iconSize
 * \~chinese \return 返回ICON的尺寸
 */
QSize DIconButton::iconSize() const
{
    QAbstractButtonPrivate *bp = static_cast<QAbstractButtonPrivate*>(QAbstractButton::d_ptr.data());

    if (bp->iconSize.isValid()) {
        return bp->iconSize;
    }

    DStyleHelper dstyle(style());
    DStyleOptionButton opt = baseStyleOption();
    int size = dstyle.pixelMetric(DStyle::PM_IconButtonIconSize, &opt, this);

    if (Q_LIKELY(size > 0)) {
        return QSize(size, size);
    }

    return QAbstractButton::iconSize();
}

/*!
 * \~chinese \brief DIconButton::isFlat
 * \~chinese \return true无边框　false有边框
 */
bool DIconButton::isFlat() const
{
    D_DC(DIconButton);

    return d->flat;
}

/*!
 * \~chinese \brief DIconButton::setFlat 设置是否保留边框
 * \~chinese \param flat true无边框　false有边框
 */
void DIconButton::setFlat(bool flat)
{
    D_D(DIconButton);

    if (d->flat == flat)
        return;

    d->flat = flat;

    QAbstractButtonPrivate *bp = static_cast<QAbstractButtonPrivate*>(QAbstractButton::d_ptr.data());
    bp->sizeHint = QSize();

    update();
    updateGeometry();
}

DIconButton::DIconButton(DIconButtonPrivate &dd, QWidget *parent)
    : QAbstractButton(parent)
    , DObject(dd)
{

}

DStyleOptionButton DIconButton::baseStyleOption() const
{
    return DStyleOptionButton();
}

void DIconButton::initStyleOption(DStyleOptionButton *option) const
{
    D_DC(DIconButton);

    option->initFrom(this);
    option->init(this);

    if (d->flat)
        option->features |= QStyleOptionButton::Flat;

    if (isChecked())
        option->state |= QStyle::State_On;

    if (isDown())
        option->state |= QStyle::State_Sunken;

    if (!d->flat && !isDown())
        option->state |= QStyle::State_Raised;

    option->text = text();
    option->icon = icon();
    option->iconSize = iconSize();

    if (backgroundRole() == QPalette::Highlight) {
        option->palette.setBrush(QPalette::Light, option->dpalette.lightLively());
        option->palette.setBrush(QPalette::Button, option->palette.highlight());
        option->palette.setBrush(QPalette::Dark, option->dpalette.darkLively());
        option->palette.setBrush(QPalette::ButtonText, option->palette.highlightedText());
    }
}

void DIconButton::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if (hasFocus()) {
            click();
            break;
        }
        Q_FALLTHROUGH();
    default:
        QAbstractButton::keyPressEvent(event);
    }
}

void DIconButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    DStylePainter p(this);
    DStyleOptionButton opt;
    initStyleOption(&opt);
    p.drawControl(DStyle::CE_IconButton, opt);
}

bool DIconButton::event(QEvent *e)
{
    if (e->type() == QEvent::Polish) {
        D_DC(DIconButton);

        if (d->iconType >= 0) {
            if (d->iconType > static_cast<qint64>(QStyle::SP_CustomBase)) {
                DStyleHelper dstyle(style());
                setIcon(dstyle.standardIcon(static_cast<DStyle::StandardPixmap>(d->iconType), nullptr, this));
            } else {
                setIcon(style()->standardIcon(static_cast<QStyle::StandardPixmap>(d->iconType), nullptr, this));
            }
        }
    } else if (e->type() == QEvent::ShowToParent) {
        // 默认调整大小，否则可能会导致按钮显示后为 QWidget 的默认大小
        resize(sizeHint());
    }

    return QAbstractButton::event(e);
}

DWIDGET_END_NAMESPACE
