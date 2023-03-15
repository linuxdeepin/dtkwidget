// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFEATUREDISPLAYDIALOG_H
#define DFEATUREDISPLAYDIALOG_H

#include <DObjectPrivate>

#include "dsidebarhelper.h"

DWIDGET_BEGIN_NAMESPACE

class DSidebarHelperPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DSidebarHelperPrivate(DSidebarHelper * qq) : DObjectPrivate(qq) {}


    bool m_visible = true;
    bool m_expanded = true;
    int m_width = -1;
    QColor m_backgroundColor;
};

DSidebarHelper::DSidebarHelper(QObject *parent)
    : QObject(parent)
    , DObject(*new DSidebarHelperPrivate(this))
{

}

DSidebarHelper::~DSidebarHelper()
{
}

QColor DSidebarHelper::backgroundColor() const
{
    D_D(const DSidebarHelper);
    return d->m_backgroundColor;
}

void DSidebarHelper::setBackgroundColor(QColor backgroundColor)
{
    D_D(DSidebarHelper);

    if (d->m_backgroundColor == backgroundColor)
        return;

    d->m_backgroundColor = backgroundColor;
    Q_EMIT backgroundColorChanged(d->m_backgroundColor);
}

bool DSidebarHelper::visible() const
{
    D_D(const DSidebarHelper);
    return d->m_visible;
}

void DSidebarHelper::setVisible(bool visible)
{
    D_D(DSidebarHelper);

    if (d->m_visible == visible)
        return;

    d->m_visible = visible;
    Q_EMIT visibleChanged(d->m_visible);
}

bool DSidebarHelper::expanded() const
{
    D_D(const DSidebarHelper);
    return d->m_expanded;
}

void DSidebarHelper::setExpanded(bool expanded)
{
    D_D(DSidebarHelper);

    if (d->m_expanded == expanded)
        return;

    d->m_expanded = expanded;
    Q_EMIT expandChanged(d->m_expanded);
}

int DSidebarHelper::width() const
{
    D_D(const DSidebarHelper);
    return d->m_width;
}

void DSidebarHelper::setWidth(int width)
{
    D_D(DSidebarHelper);

    if (d->m_width == width)
        return;

    d->m_width = width;
    Q_EMIT widthChanged(d->m_width);
}

DWIDGET_END_NAMESPACE

#endif // DFEATUREDISPLAYDIALOG_H
