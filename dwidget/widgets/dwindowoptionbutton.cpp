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

#include "dwindowoptionbutton.h"

DWIDGET_BEGIN_NAMESPACE

DWindowOptionButton::DWindowOptionButton(QWidget * parent) :
    DImageButton(parent)
{
    D_THEME_INIT_WIDGET(DWindowOptionButton);
}

QString DWindowOptionButton::normalImage() const
{
    return m_normalImage;
}

void DWindowOptionButton::setNormalImage(const QString &normalImage)
{
    m_normalImage = normalImage;

    this->setNormalPic(m_normalImage);
}

QString DWindowOptionButton::hoverImage() const
{
    return m_hoverImage;
}

void DWindowOptionButton::setHoverImage(const QString &hoverImage)
{
    m_hoverImage = hoverImage;

    this->setHoverPic(m_hoverImage);
}

QString DWindowOptionButton::pressedImage() const
{
    return m_pressedImage;
}

void DWindowOptionButton::setPressedImage(const QString &pressedImage)
{
    m_pressedImage = pressedImage;

    this->setPressPic(m_pressedImage);
}

DWIDGET_END_NAMESPACE


