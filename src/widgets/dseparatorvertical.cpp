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
 * \~english \class DSeparatorVertical
 * \~english \brief The DSeparatorVertical class provides an vertical separator.
 *
 * \~english DSeparatorVertical has fixed two pixels in width.
 *
 *
 * \~chinese \class DSeparatorVertical
 * \~chinese \brief DSeparatorVertical 类提供了通用的纵向分割线，可以应用在任何基于
 * \~chinese QWidget 的控件上。
 *
 * \~chinese DSeparatorVertical 固定宽度为2px，如果是放置在 QHBoxLayout 中进行使用，
 * \~chinese 使用者无需手动指定 DSeparatorHorizontal 控件的高度，控件会默认使用 QHBoxLayout
 * \~chinese 提供的最大高度。否则，使用者需要手动指定控件高度。
 *
 *
 * \sa DSeparatorHorizontal
 */

/*!
 * \~english \brief DSeparatorVertical::DSeparatorVertical constructs an instance of DSeparatorVertical.
 * \~english \param parent is passed to QWidget constructor.
 *
 * \~chinese \brief DSeparatorVertical::DSeparatorVertical 函数是
 * \~chinese DSeparatorVertical 类的构造函数。
 * \~chinese \param parent 为控件的父控件。
 */
DSeparatorVertical::DSeparatorVertical(QWidget *parent) : QWidget(parent)
{
    DThemeManager::registerWidget(this);

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
