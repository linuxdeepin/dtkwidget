/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DGRAPHICSCLIPEFFECT_H
#define DGRAPHICSCLIPEFFECT_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QGraphicsEffect>

DWIDGET_BEGIN_NAMESPACE

class DGraphicsClipEffectPrivate;
class DGraphicsClipEffect : public QGraphicsEffect, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(QMargins margins READ margins WRITE setMargins NOTIFY marginsChanged)
    Q_PROPERTY(QPainterPath clipPath READ clipPath WRITE setClipPath NOTIFY clipPathChanged)

public:
    explicit DGraphicsClipEffect(QObject *parent = Q_NULLPTR);

    QMargins margins() const;
    QPainterPath clipPath() const;

public Q_SLOTS:
    void setMargins(const QMargins &margins);
    void setClipPath(const QPainterPath &clipPath);

Q_SIGNALS:
    void marginsChanged(QMargins margins);
    void clipPathChanged(QPainterPath clipPath);

protected:
    void draw(QPainter *painter) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DGraphicsClipEffect)
};

DWIDGET_END_NAMESPACE

#endif // DGRAPHICSCLIPEFFECT_H
