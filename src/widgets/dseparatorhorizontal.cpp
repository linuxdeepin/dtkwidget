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
 * \~english \class DSeparatorHorizontal
 * \~english \brief The DSeparatorHorizontal class provides an horizontal separator.
 *
 * \~english DSeparatorHorizontal has fixed two pixels in height.
 *
 *
 * \~chinese \class DSeparatorHorizontal
 * \~chinese \brief DSeparatorHorizontal 类提供了通用的横向分割线，可以应用在任何基于
 * \~chinese QWidget 的控件上。
 *
 * \~chinese DSeparatorHorizontal 固定高度为2px，如果是放置在 QVBoxLayout 中进行使用，
 * \~chinese 使用者无需手动指定 DSeparatorHorizontal 控件的宽度，控件会默认使用 QVBoxLayout
 * \~chinese 提供的最大宽度。否则，使用者需要手动指定控件宽度。
 *
 *
 * \sa DSeparatorVertical
 */

/*!
 * \~english \brief DSeparatorHorizontal::DSeparatorHorizontal constructs an instance of DSeparatorHorizontal.
 * \~english \param parent is passed to QWidget constructor.
 */

/*!
 * \~chinese \brief DSeparatorHorizontal::DSeparatorHorizontal 函数是
 * \~chinese DSeparatorHorizontal 类的构造函数。
 * \~chinese \param parent 为控件的父控件。
 */
DSeparatorHorizontal::DSeparatorHorizontal(QWidget *parent) : QWidget(parent)
{
    DThemeManager::registerWidget(this);

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
