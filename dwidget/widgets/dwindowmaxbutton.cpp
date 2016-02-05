/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QDebug>

#include "dthememanager.h"

#include "dwindowmaxbutton.h"

DWIDGET_NAMESPACE_BEGIN

DWindowMaxButton::DWindowMaxButton(QWidget * parent) :
    DImageButton(parent)
{
    D_THEME_INIT_WIDGET(DWindowMaxButton);
}

QString DWindowMaxButton::normalImage() const
{
    return m_normalImage;
}

void DWindowMaxButton::setNormalImage(const QString &normalImage)
{
    m_normalImage = normalImage;

    this->setNormalPic(m_normalImage);
}

QString DWindowMaxButton::hoverImage() const
{
    return m_hoverImage;
}

void DWindowMaxButton::setHoverImage(const QString &hoverImage)
{
    m_hoverImage = hoverImage;

    this->setHoverPic(m_hoverImage);
}

QString DWindowMaxButton::pressedImage() const
{
    return m_pressedImage;
}

void DWindowMaxButton::setPressedImage(const QString &pressedImage)
{
    m_pressedImage = pressedImage;

    this->setPressPic(m_pressedImage);
}

DWIDGET_NAMESPACE_END


