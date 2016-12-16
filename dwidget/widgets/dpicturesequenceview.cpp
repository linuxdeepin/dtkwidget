/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dpicturesequenceview.h"
#include "private/dpicturesequenceview_p.h"

#include <QGraphicsPixmapItem>

DWIDGET_BEGIN_NAMESPACE

DPictureSequenceViewPrivate::DPictureSequenceViewPrivate(DPictureSequenceView *q) :
    DObjectPrivate(q)
{

}

void DPictureSequenceViewPrivate::init()
{
    D_Q(DPictureSequenceView);

    scene = new QGraphicsScene(q);
    refreshTimer = new QTimer(q);
    refreshTimer->setInterval(33);

    q->setScene(scene);
    q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setFrameShape(QFrame::NoFrame);

    q->connect(refreshTimer, SIGNAL(timeout()), q, SLOT(_q_refreshPicture()));
}

void DPictureSequenceViewPrivate::play()
{
    D_Q(DPictureSequenceView);

    refreshTimer->start();
}

void DPictureSequenceViewPrivate::_q_refreshPicture()
{
    QGraphicsPixmapItem *item = pictureItemList.value(lastItemPos++);

    if (item)
        item->hide();

    if (lastItemPos == pictureItemList.count()) {
        lastItemPos = 0;

        if (singleShot)
            refreshTimer->stop();

        D_QC(DPictureSequenceView);

        emit q->playEnd();
    }

    item = pictureItemList.value(lastItemPos);

    if (item)
        item->show();
}

DPictureSequenceView::DPictureSequenceView(QWidget *parent) :
    QGraphicsView(parent),
    DObject(*new DPictureSequenceViewPrivate(this))
{
    D_D(DPictureSequenceView);

    d->init();
}

void DPictureSequenceView::setPictureSequence(const QStringList &sequence)
{
    setPictureSequence(sequence, NormalMode);
}

void DPictureSequenceView::setPictureSequence(const QStringList &sequence, PaintMode paintMode)
{
    D_D(DPictureSequenceView);

    stop();
    d->scene->clear();
    d->pictureItemList.clear();

    for (const QString &path : sequence) {
        QPixmap pixmap(path);

        if (paintMode == DPictureSequenceView::AutoScaleMode)
            pixmap = pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        d->pictureItemList << d->scene->addPixmap(pixmap);
        d->pictureItemList.last()->hide();
    }

    if (!d->pictureItemList.isEmpty())
        d->pictureItemList.first()->show();

    setStyleSheet("background-color:transparent;");
}

void DPictureSequenceView::play()
{
    D_D(DPictureSequenceView);

    d->play();
}

void DPictureSequenceView::pause()
{
    D_D(DPictureSequenceView);

    d->refreshTimer->stop();
}

void DPictureSequenceView::stop()
{
    D_D(DPictureSequenceView);

    d->refreshTimer->stop();
    if (d->pictureItemList.count() > d->lastItemPos)
        d->pictureItemList[d->lastItemPos]->hide();
    if (!d->pictureItemList.isEmpty())
        d->pictureItemList[0]->show();
    d->lastItemPos = 0;
}

int DPictureSequenceView::speed() const
{
    D_DC(DPictureSequenceView);

    return d->refreshTimer->interval();
}

void DPictureSequenceView::setSpeed(int speed)
{
    D_D(DPictureSequenceView);

    d->refreshTimer->setInterval(speed);
}

bool DPictureSequenceView::singleShot() const
{
    D_DC(DPictureSequenceView);

    return d->singleShot;
}

void DPictureSequenceView::setSingleShot(bool singleShot)
{
    D_D(DPictureSequenceView);

    d->singleShot = singleShot;
}

DWIDGET_END_NAMESPACE

#include "moc_dpicturesequenceview.cpp"
