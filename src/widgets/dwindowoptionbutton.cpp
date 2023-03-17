// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dwindowoptionbutton.h"
#include "dstyleoption.h"

DWIDGET_BEGIN_NAMESPACE

/*!
@~english
  @class Dtk::Widget::DWindowOptionButton
  \inmodule dtkwidget
  @brief The DWindowOptionButton class is used as the unified window option button.
  
  
  It's actually a special DImageButton which has the appearance of option button.
  After clicking the button, the main menu of the program will be displayed by default, which contains items such as “About” and “Help”.
  
 */

/*!
@~english
  @fn DWindowOptionButton::DWindowOptionButton(QWidget * parent)
  @brief DWindowOptionButton::DWindowOptionButton is the constructor of DWindowOptionButton, which returns a DWindowOptionButton object. 
  Normal programs do not need to use it.
  \a parent is the parent widget of the created object.
 */
DWindowOptionButton::DWindowOptionButton(QWidget * parent)
    : DIconButton(parent)
{
    //QStyle::SP_TitleBarMenuButton
    auto iconEngine = new DStyledIconEngine(DDrawUtils::drawTitleBarMenuButton, QStringLiteral("TitleBarMenuButton"));
    setIcon(QIcon(iconEngine));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    setFlat(true);
}

QSize DWindowOptionButton::sizeHint() const
{
    return iconSize();
}

void DWindowOptionButton::initStyleOption(DStyleOptionButton *option) const
{
    DIconButton::initStyleOption(option);

    option->features |= QStyleOptionButton::ButtonFeature(DStyleOptionButton::TitleBarButton);
}

DWIDGET_END_NAMESPACE


