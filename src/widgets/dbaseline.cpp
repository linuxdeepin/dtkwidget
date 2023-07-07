// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dbaseline.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

/*!
@~english
    @class Dtk::Widget::DBaseLine
    @ingroup dtkwidget

    @brief Use the DBaseLine class to quickly create a row widget with two controls positioned on the left and right sides.
The DBaseLine class provides a fixed-height widget that contains two displayable controls positioned on the left and 
right sides, respectively. DBaseLine can be used to build widgets with a fixed row height and custom left and right 
controls, such as a top navigation bar.For example, DHeaderLine utilizes DBaseLine to construct a widget with a text 
title and an optional content control.
    @sa DHeaderLine
*/

DBaseLine::DBaseLine(QWidget *parent) : QLabel(parent)
{
    this->setFixedHeight(CONTENT_HEADER_HEIGHT);
    m_leftLayout = new QHBoxLayout();
    m_leftLayout->setContentsMargins(0,0,0,0);
    m_leftLayout->setSpacing(0);
    m_rightLayout = new QHBoxLayout();
    m_rightLayout->setContentsMargins(0, 0, 0, 0);
    m_rightLayout->setSpacing(0);

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->setContentsMargins(m_leftMargin, 0, m_rightMargin, 0);
    m_mainLayout->addLayout(m_leftLayout);
    m_mainLayout->addStretch();
    m_mainLayout->addLayout(m_rightLayout);

    this->setLayout(m_mainLayout);
}

/*!
@~english
  @brief set left widget
  @param[in] content widget
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
@~english
  @brief set right widget
  @param[in] content widget
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
@~english
  @brief get left layout
 */
QBoxLayout *DBaseLine::leftLayout()
{
    return m_leftLayout;
}

/*!
@~english
  @brief get right layout
 */
QBoxLayout *DBaseLine::rightLayout()
{
    return m_rightLayout;
}

/*!
@~english
  @brief set left margin
  @param[in] margin the length of margin
 */
void DBaseLine::setLeftMargin(int margin)
{
    m_leftMargin = margin;
    m_mainLayout->setContentsMargins(m_leftMargin, 0, m_rightMargin, 0);
}

/*!
@~english
  @brief set right margin
  @param[in] margin the length of margin
 */
void DBaseLine::setRightMargin(int margin)
{
    m_rightMargin = margin;
    m_mainLayout->setContentsMargins(m_leftMargin, 0, m_rightMargin, 0);
}

/*!
@~english
  @brief get left margin
 */
int DBaseLine::leftMargin() const
{
    return m_leftMargin;
}

/*!
@~english
  @brief get right margin
 */
int DBaseLine::rightMargin() const
{
    return m_rightMargin;
}

DWIDGET_END_NAMESPACE
