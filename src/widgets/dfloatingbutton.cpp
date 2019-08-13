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
#include "dfloatingbutton.h"
#include "dstyleoption.h"
#include "dobject_p.h"
#include "dstyle.h"

#include <private/qabstractbutton_p.h>

#include <QScopedPointer>
#include <QStylePainter>

DWIDGET_BEGIN_NAMESPACE

class DFloatingButtonPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DFloatingButtonPrivate(DFloatingButton *qq)
        : DObjectPrivate(qq)
    {

    }

    bool flat = false;

    D_DECLARE_PUBLIC(DFloatingButton)
};

DFloatingButton::DFloatingButton(QWidget *parent)
    : QAbstractButton(parent)
    , DObject(*new DFloatingButtonPrivate(this))
{

}

DFloatingButton::DFloatingButton(const QString &text, QWidget *parent)
    : DFloatingButton(parent)
{
    setText(text);
}

DFloatingButton::DFloatingButton(const QIcon &icon, const QString &text, QWidget *parent)
    : DFloatingButton(parent)
{
    setIcon(icon);
    setText(text);
}

QSize DFloatingButton::sizeHint() const
{
    QAbstractButtonPrivate *bp = static_cast<QAbstractButtonPrivate*>(QAbstractButton::d_ptr.data());

    if (bp->sizeHint.isValid()) {
        return bp->sizeHint;
    }

    DStyleOptionButton opt;
    initStyleOption(&opt);

    QString s(text());
    bool empty = s.isEmpty();

    if (empty)
        s = QStringLiteral("X");

    QFontMetrics fm = fontMetrics();
    QSize sz = fm.size(Qt::TextShowMnemonic, s);
    int w = 0, h = 0;

    if (!empty || !w)
        w += sz.width();

    if (!empty || !h)
        h = qMax(h, sz.height());

    int size = qMax(w, h);
    opt.rect.setSize(QSize(size, size));
    bp->sizeHint = (style()->sizeFromContents(DStyle::CT_PushButton, &opt, QSize(w, h), this).expandedTo(QApplication::globalStrut()));
    size = qMax(bp->sizeHint.width(), bp->sizeHint.height());
    bp->sizeHint.setHeight(size);
    bp->sizeHint.setWidth(size);

    return bp->sizeHint;
}

QSize DFloatingButton::minimumSizeHint() const
{
    return sizeHint();
}

bool DFloatingButton::isFlat() const
{
    D_DC(DFloatingButton);

    return d->flat;
}

void DFloatingButton::setFlat(bool flat)
{
    D_D(DFloatingButton);

    d->flat = flat;

    if (d->flat == flat)
        return;

    QAbstractButtonPrivate *bp = static_cast<QAbstractButtonPrivate*>(QAbstractButton::d_ptr.data());
    bp->sizeHint = QSize();

    update();
    updateGeometry();
}

void DFloatingButton::initStyleOption(DStyleOptionButton *option) const
{
    if (!option)
        return;

    D_DC(DFloatingButton);

    option->initFrom(this);
    option->init(this);
    option->features = QStyleOptionButton::ButtonFeature(DStyleOptionButton::FloatingButton);

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
}

void DFloatingButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QStylePainter p(this);
    DStyleOptionButton opt;
    initStyleOption(&opt);
    p.drawControl(QStyle::CE_PushButton, opt);
}

DWIDGET_END_NAMESPACE
