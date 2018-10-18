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

#include "dswitchlineexpand.h"
#include "dthememanager.h"

#include <QResizeEvent>

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \class DSwitchHeaderLine
 * \~chinese \brief DSwitchHeaderLine 类用于提供 DSwitchLineExpand 的标题栏。
 */

/*!
 * \~chinese \fn checkedChanged(bool arg)
 * \~chinese \brief 信号在控件开关状态发生改变时触发。
 */

/*!
 * \~chinese \brief DSwitchHeaderLine::DSwitchHeaderLine 为 DSwitchHeaderLine 的构造函数。
 * \~chinese \param parent 指定了控件的父控件。
 * \~chinese
 * \~chinese 一般情况下，无需手动创建 DSwitchHeaderLine 对象。
 */
DSwitchHeaderLine::DSwitchHeaderLine(QWidget *parent) :
    DHeaderLine(parent)
{
    m_switchButton = new DSwitchButton(this);
    connect(m_switchButton, &DSwitchButton::checkedChanged, this, &DSwitchHeaderLine::checkedChanged);
    setContent(m_switchButton);
}

/*!
 * \~chinese \brief DSwitchHeaderLine::setExpand 用于设置控件的开关状态。
 * \~chinese \param value 为指定的开关状态，true 为开， false 为关。
 */
void DSwitchHeaderLine::setExpand(bool value)
{
    m_switchButton->setChecked(value);
}

void DSwitchHeaderLine::mousePressEvent(QMouseEvent *)
{
    m_switchButton->setChecked(!m_switchButton->checked());
//    Q_EMIT mousePress();
}


/*!
 * \~chinese \class DSwitchLineExpand
 * \~chinese \brief DSwitchLineExpand 类提供了一种通过开关按钮来展开/合上显示内容的控件。
 * \~chinese 控件由标题栏和内容两部分组成，标题栏左侧显示控件标题，右侧显示一个开关控件，
 * \~chinese 用户通过点击右侧的开关控件，来控制标题栏下面的内容部分展开还是合上。
 * \~chinese
 * \~chinese \image html switchlineexpand.png
 */

/*!
 * \~chinese \brief DSwitchLineExpand::DSwitchLineExpand 为 DSwitchLineExpand 类
 * \~chinese 的构造函数。
 * \~chinese \param parent 指定了控件的父控件。
 */
DSwitchLineExpand::DSwitchLineExpand(QWidget *parent) :
    DBaseExpand(parent)
{
    m_headerLine = new DSwitchHeaderLine(this);
    m_headerLine->setExpand(expand());
    connect(m_headerLine, &DSwitchHeaderLine::checkedChanged, [ = ](bool arg) {
        DBaseExpand::setExpand(arg);
    });
    setHeader(m_headerLine);
}

/*!
 * \~chinese \brief DSwitchLineExpand::setTitle 用于设置控件的标题。
 * \~chinese \param title 为所要设置的标题。
 */
void DSwitchLineExpand::setTitle(const QString &title)
{
    m_headerLine->setTitle(title);
}

/*!
 * \~chinese \brief DSwitchLineExpand::setExpand 用于控制控件是否展开。
 * \~chinese \param value 为所要设置的展开状态，true 为展开，false 为合上。
 * \~chinese
 * \~chinese \note 这个函数同时会影响标题栏中开关按钮的状态。
 */
void DSwitchLineExpand::setExpand(bool value)
{
    //Header's arrow direction change here
    m_headerLine->setExpand(value);
}

/*!
 * \~chinese \brief DSwitchLineExpand::header 用来获取标题栏控件。
 * \~chinese \return 标题栏控件。
 */
DBaseLine *DSwitchLineExpand::header()
{
    return m_headerLine;
}

/*!
 * \~chinese \brief DSwitchLineExpand::setHeader 用于自定义标题栏的内容。
 * \~chinese \param header 为自定义的标题栏内容。
 * \~chinese
 * \~chinese \note 自定义的内容用于替代原来的标题显示部分，右侧的开关按钮不受影响。
 */
void DSwitchLineExpand::setHeader(QWidget *header)
{
    DBaseExpand::setHeader(header);
}

void DSwitchLineExpand::resizeEvent(QResizeEvent *e)
{
    m_headerLine->setFixedWidth(e->size().width());

    DBaseExpand::resizeEvent(e);
}

DWIDGET_END_NAMESPACE
