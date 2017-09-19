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

#include "dlabel.h"
#include "dthememanager.h"

DWIDGET_USE_NAMESPACE

/*!
 * \class DLabel
 * \brief The DLabel class is a direct subclass of QLabel, styled by deepin to
 * provide better style unification.
 */

/*!
 * \brief DLabel::DLabel constructs an instance of DLabel.
 * \param parent is passed to QLabel constructor.
 * \param f is passed to QLabel constructor.
 */
DLabel::DLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    D_THEME_INIT_WIDGET(DLabel);
}

/*!
 * \brief DLabel::DLabel constructs an instance of DLabel.
 * \param text is used to initialize the content, passed to QLabel constructor.
 * \param parent is passed to QLabel constructor.
 * \param f is passed to QLabel constructor.
 */
DLabel::DLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{
    D_THEME_INIT_WIDGET(DLabel);
}

