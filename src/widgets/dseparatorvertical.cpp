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

#include "dseparatorvertical.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \class DSeparatorVertical
 * \brief The DSeparatorVertical class provides an vertical separator.
 *
 * DSeparatorVertical has fixed two pixels in width.
 */

/*!
 * \brief DSeparatorVertical::DSeparatorVertical constructs an instance of DSeparatorVertical.
 * \param parent is passed to QWidget constructor.
 */
DSeparatorVertical::DSeparatorVertical(QWidget *parent) : QWidget(parent)
{
    D_THEME_INIT_WIDGET(DSeparatorVertical);

    this->setFixedWidth(2);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    m_leftRec = new QWidget(this);
    m_leftRec->setObjectName("DSeparatorLeftRec");
    m_rightRec = new QWidget(this);
    m_rightRec->setObjectName("DSeparatorRightRec");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->addStretch();
    mainLayout->addWidget(m_leftRec);
    mainLayout->addWidget(m_rightRec);
    mainLayout->addStretch();

    this->setLayout(mainLayout);
}

DWIDGET_END_NAMESPACE
