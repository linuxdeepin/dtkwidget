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

#include "dlinkbutton.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \class DLinkButton
 * \brief The DLinkButton class provides a styled QPushButton to mini the style
 * and behavior of links displayed on the internet.
 */

/*!
 * \brief DLinkButton::DLinkButton constructs an instance of DLinkButton.
 * \param text is passed to QPushButton constructor.
 * \param parent is passed to QPushButton constructor.
 */
DLinkButton::DLinkButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent)
{
    DThemeManager::registerWidget(this);
}

DWIDGET_END_NAMESPACE
