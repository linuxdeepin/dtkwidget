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

DWIDGET_BEGIN_NAMESPACE

DFloatingButton::DFloatingButton(QWidget *parent)
    : DIconButton(parent)
{
    resize(48, 48);
    setBackgroundRole(QPalette::Highlight);
}

DFloatingButton::DFloatingButton(QStyle::StandardPixmap iconType, QWidget *parent)
    : DIconButton(iconType, parent)
{
    resize(48, 48);
    setBackgroundRole(QPalette::Highlight);
}

DFloatingButton::DFloatingButton(DStyle::StandardPixmap iconType, QWidget *parent)
    : DIconButton(iconType, parent)
{
    resize(48, 48);
    setBackgroundRole(QPalette::Highlight);
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

void DFloatingButton::initStyleOption(DStyleOptionButton *option) const
{
    DIconButton::initStyleOption(option);
    option->features = QStyleOptionButton::ButtonFeature(DStyleOptionButton::FloatingButton);
}

DWIDGET_END_NAMESPACE
