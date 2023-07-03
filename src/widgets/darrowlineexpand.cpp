// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "darrowlineexpand.h"
#include "dthememanager.h"

#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)

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

/*!
@~english
  \class Dtk::Widget::DArrowLineExpand
  \inmodule dtkwidget
  @brief a beautiful expandable control.
  
  DArrowLineExpand inherited from DBaseExpand and provide ArrowHeaderLine (a button marked with an arrow)as its fixed title control，that is to say, compared with DBaseExpand, the step of providing title control is omitted，you only need to provide content controls，If you need a custom title control, you should use the DBaseExpand class。
  \sa  DBaseExpand
 */

/*!
@~english
  @brief Construct a DArrowLineExpand instance
  
  \a parent Is the parent control of the instance
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

/*!
@~english
  @brief Set the text for the title to display
  
  \a title Title content
 */
void DArrowLineExpand::setTitle(const QString &title)
{
    m_headerLine->setTitle(title);
}

/*!
@~english
  @brief Sets whether to expand to display the content control
  
  \a value If it is true, it will be displayed, and vice versa
 */
void DArrowLineExpand::setExpand(bool value)
{
    //Header's arrow direction change here
    m_headerLine->setExpand(value);
    DBaseExpand::setExpand(value);
}

/*!
@~english
  @brief get the title control
  \return title control
  \sa DHeaderLine DBaseLine
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

#endif
