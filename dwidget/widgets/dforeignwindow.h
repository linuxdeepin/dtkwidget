/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#ifndef DFOREIGNWINDOW_H
#define DFOREIGNWINDOW_H

#include "dobject.h"
#include "dwidget_global.h"

#include <QWindow>

DWIDGET_BEGIN_NAMESPACE

class DForeignWindowPrivate;
class DForeignWindow : public QWindow, public DObject
{
    Q_OBJECT
    Q_PROPERTY(QString wmClass READ wmClass NOTIFY wmClassChanged)
    Q_PROPERTY(quint32 pid READ pid NOTIFY pidChanged)

public:
    explicit DForeignWindow(QWindow *parent = 0);

    static DForeignWindow *fromWinId(WId id);

    QString wmClass() const;
    quint32 pid() const;

Q_SIGNALS:
    void wmClassChanged();
    void pidChanged();

protected:
    bool event(QEvent *) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DForeignWindow)
};

DWIDGET_END_NAMESPACE

#endif // DFOREIGNWINDOW_H
