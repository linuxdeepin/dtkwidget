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

#include "dwindowrestorebutton.h"

DWIDGET_BEGIN_NAMESPACE

DWindowRestoreButton::DWindowRestoreButton(QWidget * parent) :
    DImageButton(parent)
{
    D_THEME_INIT_WIDGET(DWindowRestoreButton);
}

QString DWindowRestoreButton::normalImage() const
{
    return m_normalImage;
}

void DWindowRestoreButton::setNormalImage(const QString &normalImage)
{
    m_normalImage = normalImage;

    this->setNormalPic(m_normalImage);
}

QString DWindowRestoreButton::hoverImage() const
{
    return m_hoverImage;
}

void DWindowRestoreButton::setHoverImage(const QString &hoverImage)
{
    m_hoverImage = hoverImage;

    this->setHoverPic(m_hoverImage);
}

QString DWindowRestoreButton::pressedImage() const
{
    return m_pressedImage;
}

void DWindowRestoreButton::setPressedImage(const QString &pressedImage)
{
    m_pressedImage = pressedImage;

    this->setPressPic(m_pressedImage);
}

DWIDGET_END_NAMESPACE


