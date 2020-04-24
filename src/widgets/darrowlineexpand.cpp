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

#include "darrowlineexpand.h"
#include "dthememanager.h"

#include <QResizeEvent>

DWIDGET_BEGIN_NAMESPACE

ArrowHeaderLine::ArrowHeaderLine(QWidget *parent) :
    DHeaderLine(parent)
{
    m_arrowButton = new DIconButton(DStyle::SP_ReduceElement, this);
    m_arrowButton->setFlat(true);
    setExpand(false);
    connect(m_arrowButton, &DIconButton::clicked, this, &ArrowHeaderLine::mousePress);
    setContent(m_arrowButton);
    setFixedHeight(EXPAND_HEADER_HEIGHT);
}

void ArrowHeaderLine::setExpand(bool value)
{
    if (value) {
        m_arrowButton->setIcon(DStyle::SP_ExpandElement);
    } else {
        m_arrowButton->setIcon(DStyle::SP_ReduceElement);
    }
    m_isExpanded = value;
}

void ArrowHeaderLine::mousePressEvent(QMouseEvent *)
{
    Q_EMIT mousePress();
}

void ArrowHeaderLine::mouseMoveEvent(QMouseEvent *event)
{
    //屏蔽掉鼠标移动事件
    event->accept();
}

void ArrowHeaderLine::reverseArrowDirection()
{
    setExpand(!m_isExpanded);
}

/**
 * \~chinese \class DArrowLineExpand
 * \~chinese \brief 一个美观的可展开的控件
 * \~chinese
 * \~chinese DArrowLineExpand 继承自 DBaseExpand 并提供了了 ArrowHeaderLine (一个带有箭头标示的按钮)作为其固定的标题控件，也就是说跟 DBaseExpand 相比省去了提供标题控件的步骤，只需要提供内容控件即可，如果需要自定义标题控件应该使用 DBaseExpand 类。
 * \~chinese \sa  DBaseExpand
 */

/**
 * \~chinese \brief 构造一个 DArrowLineExpand 实例
 * \~chinese
 * \~chinese \param parent 为实例的父控件
 */
DArrowLineExpand::DArrowLineExpand(QWidget *parent) : DBaseExpand(parent)
{
    m_headerLine = new ArrowHeaderLine(this);
    m_headerLine->setExpand(expand());
    connect(m_headerLine, &ArrowHeaderLine::mousePress, [=]{
        setExpand(!expand());
    });
    setHeader(m_headerLine);
}

/**
 * \~chinese \brief 设置标题要显示的文字
 * \~chinese
 * \~chinese \param title 标题内容
 */
void DArrowLineExpand::setTitle(const QString &title)
{
    m_headerLine->setTitle(title);
}

/**
 * \~chinese \brief 设置是否展开以显示内容控件
 * \~chinese
 * \~chinese \param value 为 true 即为显示，反之则反
 */
void DArrowLineExpand::setExpand(bool value)
{
    //Header's arrow direction change here
    m_headerLine->setExpand(value);
    DBaseExpand::setExpand(value);
}

/**
 * \~chinese \brief 获取标题控件
 * \~chinese \return 标题控件
 * \~chinese \sa ArrowHeaderLine DHeaderLine DBaseLine
 */
DBaseLine *DArrowLineExpand::headerLine()
{
    return m_headerLine;
}

void DArrowLineExpand::setHeader(QWidget *header)
{
    DBaseExpand::setHeader(header);
}

void DArrowLineExpand::resizeEvent(QResizeEvent *e)
{
    m_headerLine->setFixedWidth(e->size().width());

    DBaseExpand::resizeEvent(e);
}

DWIDGET_END_NAMESPACE
