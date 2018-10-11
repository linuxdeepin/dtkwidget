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

#include "dheaderline.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

/*!
    \~chinese \class DHeaderLine
    \~chinese \brief 可以使用 DHeaderLine 类快速创建标题行控件。

    \~chinese DHeaderLine 提供一个固定高度的控件可供显示标题或相关内容，其行内提供左对齐的文字和可选的位于右侧的控件。

    \~chinese \sa DBaseLine
*/

/*!
    \~english \class DHeaderLine
    \~english \brief Use DHeaderLine to create a widget for display header title.

    \~english DHeaderLine provide a fixed height header line to display header title or related stuff.
    Title are aligned left and you can also set an optional right aligned QWidget.

    \~english \sa DBaseLine
*/

DHeaderLine::DHeaderLine(QWidget *parent) : DBaseLine(parent)
{
    DThemeManager::registerWidget(this);

    setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    setFixedHeight(EXPAND_HEADER_HEIGHT);
    m_titleLabel = new QLabel(this);
    m_titleLabel->setObjectName("DHeaderLineTitle");
    DBaseLine::setLeftContent(m_titleLabel);
}

/*!
 * \~english \brief Set the title of the header line.
 * \~chinese \brief 设置标题。
 *
 * \sa title()
 */
void DHeaderLine::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

/*!
 * \~english \brief Set the content widget of the header line.
 * \~chinese \brief 设置内容控件。
 *
 * \~english Content widget aligned right and the height is limited because of the fixed height.
 * \~chinese 内容控件将右对齐，且高度受限于 DHeaderLine 本身的固定高度。
 */
void DHeaderLine::setContent(QWidget *content)
{
    DBaseLine::setRightContent(content);
    setFixedHeight(CONTENT_HEADER_HEIGHT);
}

/*!
 * \~english \brief Title of the header line.
 * \~chinese \brief 获取标题。
 *
 * \sa title()
 */
QString DHeaderLine::title() const
{
    return m_titleLabel->text();
}

DWIDGET_END_NAMESPACE
