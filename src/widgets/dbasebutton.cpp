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

#include "dbasebutton.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \class DBaseButton
 * \brief The DBaseButton class provides deepin style QPushButton.
 *
 * DBaseButton will be deprecated and removed in the future, please use
 * QPushButton, together with dstyle Qt style plugin, QPushButton should have
 * the same visual effect as DBaseButton.
 */

/*!
 * \brief DBaseButton::DBaseButton constructs an instance of DBaseButton.
 * \param parent is passed to QPushButton constructor.
 */
DBaseButton::DBaseButton(QWidget *parent) :
    QPushButton(parent)
{
    D_THEME_INIT_WIDGET(DBaseButton);

    initInsideFrame();
}

/*!
 * \brief DBaseButton::DBaseButton constructs an instance of DBaseButton.
 * \param text is passed to QPushButton constructor.
 * \param parent is passed to QPushButton constructor.
 */
DBaseButton::DBaseButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent)
{
    D_THEME_INIT_WIDGET(DBaseButton);

    initInsideFrame();
}

/*!
 * \brief DBaseButton::DBaseButton constructs an instance of DBaseButton.
 * \param icon is passed to QPushButton constructor.
 * \param text is passed to QPushButton constructor.
 * \param parent is passed to QPushButton constructor.
 */
DBaseButton::DBaseButton(const QIcon &icon, const QString &text, QWidget *parent) :
    QPushButton(icon, text, parent)
{
    D_THEME_INIT_WIDGET(DBaseButton);

    initInsideFrame();
}

//Bypassing the problem here
//qss can't draw box-shadow
void DBaseButton::initInsideFrame()
{
    QFrame *insideFrame = new QFrame;
    insideFrame->raise();
    insideFrame->setAttribute(Qt::WA_TransparentForMouseEvents);
    insideFrame->setObjectName("ButtonInsideFrame");
    QHBoxLayout *insideLayout = new QHBoxLayout(this);
    insideLayout->setContentsMargins(0, 1, 0, 0);
    insideLayout->addWidget(insideFrame);
}

DWIDGET_END_NAMESPACE
