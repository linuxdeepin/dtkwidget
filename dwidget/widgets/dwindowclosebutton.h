/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DWINDOWCLOSEBUTTON_H
#define DWINDOWCLOSEBUTTON_H

#include <QObject>
#include <QString>

#include "dimagebutton.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DWindowCloseButton : public DImageButton
{
    Q_OBJECT
public:
    DWindowCloseButton(QWidget * parent = 0);

    Q_PROPERTY(QString normalImage READ normalImage WRITE setNormalImage)
    Q_PROPERTY(QString hoverImage READ hoverImage WRITE setHoverImage)
    Q_PROPERTY(QString pressedImage READ pressedImage WRITE setPressedImage)

    QString normalImage() const;
    void setNormalImage(const QString &normalImage);

    QString hoverImage() const;
    void setHoverImage(const QString &hoverImage);

    QString pressedImage() const;
    void setPressedImage(const QString &pressedImage);

private:
    QString m_normalImage;
    QString m_hoverImage;
    QString m_pressedImage;
};

DWIDGET_END_NAMESPACE

#endif // DWINDOWCLOSEBUTTON_H
