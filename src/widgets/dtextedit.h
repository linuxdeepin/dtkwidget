/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DTEXTEDIT_H
#define DTEXTEDIT_H

#include "dtkwidget_global.h"

#include <dobject.h>

#include <QTextEdit>

DWIDGET_BEGIN_NAMESPACE

class DTextEditPrivate;
class DTextEdit : public QTextEdit, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

public:
    explicit DTextEdit(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DTextEdit)
    Q_DISABLE_COPY(DTextEdit)
};

DWIDGET_END_NAMESPACE

#endif // DTEXTEDIT_H
