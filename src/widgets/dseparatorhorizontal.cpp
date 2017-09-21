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

#include "dseparatorhorizontal.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

/*!
 * \class DSeparatorHorizontal
 * \brief The DSeparatorHorizontal class provides an horizontal separator.
 *
 * DSeparatorHorizontal has fixed two pixels in height.
 */

/*!
 * \brief DSeparatorHorizontal::DSeparatorHorizontal constructs an instance of DSeparatorHorizontal.
 * \param parent is passed to QWidget constructor.
 */
DSeparatorHorizontal::DSeparatorHorizontal(QWidget *parent) : QWidget(parent)
{
    D_THEME_INIT_WIDGET(DSeparatorHorizontal);

    this->setFixedHeight(2);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_topRec = new QWidget(this);
    m_topRec->setObjectName("DSeparatorTopRec");
    m_bottomRec = new QWidget(this);
    m_bottomRec->setObjectName("DSeparatorBottomRec");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->addStretch();
    mainLayout->addWidget(m_topRec);
    mainLayout->addWidget(m_bottomRec);
    mainLayout->addStretch();

    this->setLayout(mainLayout);
}

DWIDGET_END_NAMESPACE
