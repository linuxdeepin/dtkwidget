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

#include "dthememanager.h"

#include "dwindowminbutton.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \class DWindowMinButton
 * \brief The DWindowMinButton class is used as the unified window minimize button.
 *
 * It's actually a special DImageButton which has the appearance of minimize button.
 */
DWindowMinButton::DWindowMinButton(QWidget * parent) :
    DImageButton(parent)
{
    DThemeManager::registerWidget(this);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

DWIDGET_END_NAMESPACE
