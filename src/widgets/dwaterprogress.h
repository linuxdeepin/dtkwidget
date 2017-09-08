/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DWATERPROGRESS_H
#define DWATERPROGRESS_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QScopedPointer>
#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DWaterProgressPrivate;
class LIBDTKWIDGETSHARED_EXPORT DWaterProgress : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
public:
    explicit DWaterProgress(QWidget *parent = 0);
    ~DWaterProgress();

    int value() const;

Q_SIGNALS:
    void valueChanged();

public Q_SLOTS:
    void start();
    void stop();
    void setValue(int);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DWaterProgress)
};

DWIDGET_END_NAMESPACE

#endif // DWATERPROGRESS_H
