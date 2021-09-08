/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dheaderline.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

/*!
    \class Dtk::Widget::DHeaderLine
    \inmodule dtkwidget

    \brief 可以使用 DHeaderLine 类快速创建标题行控件.
    \brief Use DHeaderLine to create a widget for display header title.

    DHeaderLine 提供一个固定高度的控件可供显示标题或相关内容，其行内提供左对齐的文字和可选的位于右侧的控件。


    我们可以利用可选的内容控件为用户提供交互方式的提示，状态提示或其他类似的目的。上图中的例子，我们使用 DHeaderLine
    并结合 DArrowButton 作为内容控件，构成了一个外观近似可折叠标题的标题行控件。
    DHeaderLine provide a fixed height header line to display header title or related stuff.
    Title are aligned left and you can also set an optional right aligned QWidget.

    \sa DBaseLine
*/

DHeaderLine::DHeaderLine(QWidget *parent) : DBaseLine(parent)
{
    setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    setFixedHeight(EXPAND_HEADER_HEIGHT);
    m_titleLabel = new QLabel(this);
    m_titleLabel->setObjectName("DHeaderLineTitle");
    DBaseLine::setLeftContent(m_titleLabel);
}

/*!
  \brief Set the \a title of the header line.
  \brief 设置标题。
  
  \sa title()
 */
void DHeaderLine::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

/*!
  \brief Set the \a content widget of the header line.
  \brief 设置内容控件。
  
  Content widget aligned right and the height is limited because of the fixed height.
  内容控件将右对齐，且高度受限于 DHeaderLine 本身的固定高度。
 */
void DHeaderLine::setContent(QWidget *content)
{
    DBaseLine::setRightContent(content);
    setFixedHeight(CONTENT_HEADER_HEIGHT);
}

/*!
  \brief Title of the header line.
  \brief 获取标题。
  
  \sa title()
 */
QString DHeaderLine::title() const
{
    return m_titleLabel->text();
}

DWIDGET_END_NAMESPACE
