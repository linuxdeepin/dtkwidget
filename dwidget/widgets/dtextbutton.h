/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DTEXTBUTTON_H
#define DTEXTBUTTON_H

#include <QPushButton>
#include <QPainter>

#include "libdui_global.h"

DWIDGET_NAMESPACE_BEGIN

class LIBDUISHARED_EXPORT DTextButton : public QPushButton
{
    Q_OBJECT
public:
    DTextButton(const QString & text, QWidget * parent = 0);
    ~DTextButton();
};

DWIDGET_NAMESPACE_END

#endif // DTEXTBUTTON_H
