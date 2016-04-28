/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DPICTURESEQUENCEVIEW_H
#define DPICTURESEQUENCEVIEW_H

#include "dwidget_global.h"
#include "dobject.h"

#include <QGraphicsView>

DWIDGET_BEGIN_NAMESPACE

class DPictureSequenceViewPrivate;
class DPictureSequenceView : public QGraphicsView, public DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DPictureSequenceView)
    D_DECLARE_PRIVATE(DPictureSequenceView)
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(bool singleShot READ singleShot WRITE setSingleShot)

public:
    DPictureSequenceView(QWidget *parent = nullptr);

    void setPictureSequence(const QStringList &sequence);
    void play();

    int speed() const;
    void setSpeed(int speed);

    bool singleShot() const;
    void setSingleShot(bool singleShot);

Q_SIGNALS:
    void speedChanged(int speed) const;
    void playEnd() const;
};

DWIDGET_END_NAMESPACE

#endif // DPICTURESEQUENCEVIEW_H
