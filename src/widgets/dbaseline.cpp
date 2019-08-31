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

/*!
    \~chinese \class DBaseLine
    \~chinese \brief 可以使用 DBaseLine 类快速创建具有位于左右两侧的两个控件的行控件。

    \~chinese DBaseLine 提供一个固定高度的控件，包含分别位于左侧和右侧的两个可供显示的控件。

    \~chinese DBaseLine 可用于构建固定行高并需要自定义左右控件的控件，比如上方导航栏等。如 DHeaderLine 即使用 DBaseLine
    构建了一个带有文字标题和可选内容控件的控件。

    \~chinese \sa DHeaderLine
*/

/*!
    \~english \class DBaseLine
    \~english \brief Use DBaseLine to create a widget for display inline widget.

    \~english DBaseLine provide a fixed height inline widget with two widgets for displaying related stuff.

    \~english \sa DHeaderLine
*/

DBaseLine::DBaseLine(QWidget *parent) : QLabel(parent)
{
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

/*!
 * \~english \brief set left content widget
 * \~chinese \brief 设置左侧内容控件
 */
void DBaseLine::setLeftContent(QWidget *content)
{
    QLayoutItem *child;
    while ((child = m_leftLayout->takeAt(0)) != 0) {
        delete child;
    }

    m_leftLayout->addWidget(content);
}

/*!
 * \~english \brief set right content widget
 * \~chinese \brief 设置右侧内容控件
 */
void DBaseLine::setRightContent(QWidget *content)
{
    QLayoutItem *child;
    while ((child = m_rightLayout->takeAt(0)) != 0) {
        delete child;
    }

    m_rightLayout->addWidget(content);
}

/*!
 * \~english \brief get left layout
 * \~chinese \brief 获取左侧 QBoxLayout
 */
QBoxLayout *DBaseLine::leftLayout()
{
    return m_leftLayout;
}

/*!
 * \~english \brief get right layout
 * \~chinese \brief 获取右侧 QBoxLayout
 */
QBoxLayout *DBaseLine::rightLayout()
{
    return m_rightLayout;
}

/*!
 * \~english \brief set left margin
 * \~chinese \brief 设置左侧外补(margin)边距
 */
void DBaseLine::setLeftMargin(int margin)
{
    m_leftMargin = margin;
    m_mainLayout->setContentsMargins(m_leftMargin, 0, m_rightMargin, 0);
}

/*!
 * \~english \brief set right margin
 * \~chinese \brief 设置右侧外补(margin)边距
 */
void DBaseLine::setRightMargin(int margin)
{
    m_rightMargin = margin;
    m_mainLayout->setContentsMargins(m_leftMargin, 0, m_rightMargin, 0);
}

/*!
 * \~english \brief get left margin
 * \~chinese \brief 左侧外补(margin)边距
 */
int DBaseLine::leftMargin() const
{
    return m_leftMargin;
}

/*!
 * \~english \brief get right margin
 * \~chinese \brief 右侧外补(margin)边距
 */
int DBaseLine::rightMargin() const
{
    return m_rightMargin;
}

DWIDGET_END_NAMESPACE
