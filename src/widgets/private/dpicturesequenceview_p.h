// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPICTURESEQUENCEVIEW_P_H
#define DPICTURESEQUENCEVIEW_P_H

#include <dpicturesequenceview.h>

#include <DObjectPrivate>

#include <QList>
#include <QGraphicsScene>
#include <QTimer>

DWIDGET_BEGIN_NAMESPACE

class DPictureSequenceViewPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DPictureSequenceView)

public:
    DPictureSequenceViewPrivate(DPictureSequenceView *q);
    ~DPictureSequenceViewPrivate();

    void init();
    void play();

    QPixmap loadPixmap(const QString &path);

public:
    void _q_refreshPicture();

public:
    int lastItemPos = 0;
    bool singleShot = false;

    QGraphicsScene *scene;
    QTimer *refreshTimer;
    QList<QGraphicsPixmapItem*> pictureItemList;
};

DWIDGET_END_NAMESPACE

#endif // DPICTURESEQUENCEVIEW_P_H

