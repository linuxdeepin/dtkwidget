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

SwitchHeaderLine::SwitchHeaderLine(QWidget *parent) :
    DHeaderLine(parent)
{
    m_switchButton = new DSwitchButton(this);
    connect(m_switchButton, &DSwitchButton::checkedChanged, this, &SwitchHeaderLine::checkedChanged);
    setContent(m_switchButton);
}

void SwitchHeaderLine::setExpand(bool value)
{
    m_switchButton->setChecked(value);
}

void SwitchHeaderLine::mousePressEvent(QMouseEvent *)
{
    m_switchButton->setChecked(!m_switchButton->checked());
//    Q_EMIT mousePress();
}

DSwitchLineExpand::DSwitchLineExpand(QWidget *parent) :
    DBaseExpand(parent)
{
    m_headerLine = new SwitchHeaderLine(this);
    m_headerLine->setExpand(expand());
    connect(m_headerLine, &SwitchHeaderLine::checkedChanged, [ = ](bool arg) {
        DBaseExpand::setExpand(arg);
    });
    setHeader(m_headerLine);
}

void DSwitchLineExpand::setTitle(const QString &title)
{
    m_headerLine->setTitle(title);
}

void DSwitchLineExpand::setExpand(bool value)
{
    //Header's arrow direction change here
    m_headerLine->setExpand(value);
}

DBaseLine *DSwitchLineExpand::header()
{
    return m_headerLine;
}

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
