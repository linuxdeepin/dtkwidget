// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfloatingbutton.h"
#include "dstyleoption.h"
#include "dobject_p.h"
#include "dstyle.h"

#include <private/qabstractbutton_p.h>

DWIDGET_BEGIN_NAMESPACE

/*!
@~english
  @brief DFloatingButton::DFloatingButton 用于定制化的button，根据传入的图标参数具体调整
  @a parent
 */
DFloatingButton::DFloatingButton(QWidget *parent)
    : DIconButton(parent)
{
    setBackgroundRole(QPalette::Highlight);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

DFloatingButton::DFloatingButton(QStyle::StandardPixmap iconType, QWidget *parent)
    : DIconButton(iconType, parent)
{
    setBackgroundRole(QPalette::Highlight);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

DFloatingButton::DFloatingButton(DStyle::StandardPixmap iconType, QWidget *parent)
    : DIconButton(iconType, parent)
{
    setBackgroundRole(QPalette::Highlight);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

DFloatingButton::DFloatingButton(const QString &text, QWidget *parent)
    : DFloatingButton(parent)
{
    setText(text);
}

DFloatingButton::DFloatingButton(const QIcon &icon, const QString &text, QWidget *parent)
    : DFloatingButton(parent)
{
    setIcon(icon);
    setText(text);
}

DFloatingButton::DFloatingButton(const DDciIcon &icon, const QString &text, QWidget *parent)
    : DFloatingButton(text, parent)
{
    setIcon(icon);
}

DStyleOptionButton DFloatingButton::baseStyleOption() const
{
    DStyleOptionButton opt;
    opt.features = QStyleOptionButton::ButtonFeature(DStyleOptionButton::FloatingButton);

    return opt;
}

void DFloatingButton::initStyleOption(DStyleOptionButton *option) const
{
    DIconButton::initStyleOption(option);
    option->features = QStyleOptionButton::ButtonFeature(DStyleOptionButton::FloatingButton);
}

DWIDGET_END_NAMESPACE
