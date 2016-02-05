/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DSEPARATORVERTICAL_H
#define DSEPARATORVERTICAL_H

#include <QWidget>
#include <QHBoxLayout>

#include "libdui_global.h"

DWIDGET_NAMESPACE_BEGIN

class LIBDUISHARED_EXPORT DSeparatorVertical : public QWidget
{
    Q_OBJECT
public:
    explicit DSeparatorVertical(QWidget *parent = 0);

private:
    QWidget *m_leftRec = NULL;
    QWidget *m_rightRec = NULL;
};

DWIDGET_NAMESPACE_END

#endif // DSEPARATORVERTICAL_H
