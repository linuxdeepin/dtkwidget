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

#include "dbaseline.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

DBaseLine::DBaseLine(QWidget *parent) : QLabel(parent)
{
    D_THEME_INIT_WIDGET(DBaseLine);

    this->setFixedHeight(CONTENT_HEADER_HEIGHT);
    m_leftLayout = new QHBoxLayout();
    m_leftLayout->setMargin(0);
    m_leftLayout->setSpacing(0);
    m_rightLayout = new QHBoxLayout();
    m_rightLayout->setMargin(0);
    m_rightLayout->setSpacing(0);

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->setContentsMargins(m_leftMargin, 0, m_rightMargin, 0);
    m_mainLayout->addLayout(m_leftLayout);
    m_mainLayout->addStretch();
    m_mainLayout->addLayout(m_rightLayout);

    this->setLayout(m_mainLayout);
}

void DBaseLine::setLeftContent(QWidget *content)
{
    QLayoutItem *child;
    while ((child = m_leftLayout->takeAt(0)) != 0) {
        delete child;
    }

    m_leftLayout->addWidget(content);
}

void DBaseLine::setRightContent(QWidget *content)
{
    QLayoutItem *child;
    while ((child = m_rightLayout->takeAt(0)) != 0) {
        delete child;
    }

    m_rightLayout->addWidget(content);
}

QBoxLayout *DBaseLine::leftLayout()
{
    return m_leftLayout;
}

QBoxLayout *DBaseLine::rightLayout()
{
    return m_rightLayout;
}

void DBaseLine::setLeftMargin(int margin)
{
    m_leftMargin = margin;
    m_mainLayout->setContentsMargins(m_leftMargin, 0, m_rightMargin, 0);
}

void DBaseLine::setRightMargin(int margin)
{
    m_rightMargin = margin;
    m_mainLayout->setContentsMargins(m_leftMargin, 0, m_rightMargin, 0);
}

int DBaseLine::leftMargin() const
{
    return m_leftMargin;
}

int DBaseLine::rightMargin() const
{
    return m_rightMargin;
}

DWIDGET_END_NAMESPACE
