/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DTEXTEDIT_P_H
#define DTEXTEDIT_P_H

#include "dtextedit.h"

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DTextEditPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    DTextEditPrivate(DTextEdit *qq);

    void init();

    QFrame *insideFrame;

    D_DECLARE_PUBLIC(DTextEdit)
};

DWIDGET_END_NAMESPACE

#endif // DTEXTEDIT_P_H
