/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DPICTURESEQUENCEVIEW_P_H
#define DPICTURESEQUENCEVIEW_P_H

#include "dpicturesequenceview.h"

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

    void init();
    void play();

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

