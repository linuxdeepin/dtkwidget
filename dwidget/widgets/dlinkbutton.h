/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLINKBUTTON_H
#define DLINKBUTTON_H

#include <QPushButton>

#include "libdui_global.h"

DWIDGET_NAMESPACE_BEGIN

class LIBDUISHARED_EXPORT DLinkButton : public QPushButton
{
    Q_OBJECT

public:
    DLinkButton(const QString & text = QString(), QWidget * parent = 0);
};

DWIDGET_NAMESPACE_END

#endif // DLINKBUTTON_H
