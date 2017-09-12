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

#include <DObjectPrivate>

#include "dthememanager.h"
#include "dwindowmaxbutton.h"

#include <QStyle>

DWIDGET_BEGIN_NAMESPACE

class DWindowMaxButtonPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate{
public:
    DWindowMaxButtonPrivate(DWindowMaxButton* qq):DObjectPrivate(qq) {
        m_isMaximized = false;
    }

private:
    bool m_isMaximized;
    Q_DECLARE_PUBLIC(DWindowMaxButton)
};

DWindowMaxButton::DWindowMaxButton(QWidget * parent) :
    DImageButton(parent),
    DObject(*new DWindowMaxButtonPrivate(this))
{
    D_THEME_INIT_WIDGET(DWindowMaxButton, isMaximized);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

bool DWindowMaxButton::isMaximized() const
{
    D_DC(DWindowMaxButton);

    return d->m_isMaximized;
}

void DWindowMaxButton::setMaximized(bool isMaximized)
{
    D_D(DWindowMaxButton);

    if (d->m_isMaximized == isMaximized)
        return;

    d->m_isMaximized = isMaximized;
    Q_EMIT maximizedChanged(isMaximized);
}

DWIDGET_END_NAMESPACE


