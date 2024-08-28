// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
  \brief DIconButton::DIconButton 实现一个方便定义图标的button
  \a parent
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

DIconButton::DIconButton(const DDciIcon &dciIcon, QWidget *parent)
    : DIconButton(parent)
{
    setIcon(dciIcon);
}

DIconButton::~DIconButton()
{

}

/*!
  \brief DIconButton::setIcon　设置button的图标
  \a icon　button的图标
 */
void DIconButton::setIcon(const QIcon &icon)
{
    D_D(DIconButton);

    d->iconType = -1;
    QAbstractButton::setIcon(icon);
}

/*!
  \brief DIconButton::setIcon 设置button图标的类型,传入QT预定义枚举值
  \a iconType icon类型
 */
void DIconButton::setIcon(QStyle::StandardPixmap iconType)
{
    D_D(DIconButton);

    d->iconType = iconType;
    QAbstractButton::setIcon(style()->standardIcon(iconType, nullptr, this));
}

/*!
  \brief DIconButton::setIcon 设置button图标的类型,传入DTK预定义枚举值
  \a iconType icon类型
 */
void DIconButton::setIcon(DStyle::StandardPixmap iconType)
{
    D_D(DIconButton);

    d->iconType = iconType;
    QAbstractButton::setIcon(DStyleHelper(style()).standardIcon(iconType, nullptr, this));
}

void DIconButton::setIcon(const DDciIcon &icon)
{
    D_D(DIconButton);

    d->iconType = -1;
    d->dciIcon = icon;
    this->update();
    this->updateGeometry();
}

DDciIcon DIconButton::dciIcon() const
{
    D_DC(DIconButton);

    return d->dciIcon;
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
  \brief DIconButton::iconSize
  \return 返回ICON的尺寸
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
  \brief DIconButton::isFlat
  \return true无边框　false有边框
 */
bool DIconButton::isFlat() const
{
    D_DC(DIconButton);

    return d->flat;
}

void DIconButton::setEnabledCircle(bool status)
{
    D_D(DIconButton);

    if (status == d->circleStatus)
        return;

    d->circleStatus = status;
    update();
}

bool DIconButton::enabledCircle() const
{
    D_D(const DIconButton);

    return d->circleStatus;
}

/*!
  \brief DIconButton::setFlat 设置是否保留边框
  \a flat true无边框　false有边框
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

    *option = baseStyleOption();
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

    if (enabledCircle()) {
        option->features |= QStyleOptionButton::ButtonFeature(DStyleOptionButton::CircleButton);
    }

    if (!d->dciIcon.isNull()) {
        option->dciIcon = d->dciIcon;
        option->features |= QStyleOptionButton::ButtonFeature(DStyleOptionButton::HasDciIcon);
    }

    option->text = text();
    option->icon = icon();
    option->iconSize = iconSize();

    if (backgroundRole() == QPalette::Highlight) {
        option->palette.setBrush(QPalette::Light, option->dpalette.highlight());
        option->palette.setBrush(QPalette::Button, option->palette.highlight());
        option->palette.setBrush(QPalette::Dark, option->dpalette.highlight());
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

/*!
  \brief DIconButton::setNewNotification 设置是否显示小红点
  \a set_new true有红点　false无红点
 */
void DIconButton::setNewNotification(const bool set_new)
{
    DStyle::setRedPointVisible(this, set_new);
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
