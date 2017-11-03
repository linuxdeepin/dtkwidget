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

#ifndef DPICTURESEQUENCEVIEW_H
#define DPICTURESEQUENCEVIEW_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QtGlobal>
#include <QPair>
#include <QGraphicsView>

DWIDGET_BEGIN_NAMESPACE

class DPictureSequenceViewPrivate;
class DPictureSequenceView : public QGraphicsView, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(bool singleShot READ singleShot WRITE setSingleShot)

public:
    DPictureSequenceView(QWidget *parent = nullptr);

    void setPictureSequence(const QString &src, const QPair<int, int> &range, const int fieldWidth = 0, const bool auto_scale = false);
    void setPictureSequence(const QStringList &sequence, const bool auto_scale = false);
    void play();
    void pause();
    void stop();

    int speed() const;
    void setSpeed(int speed);

    bool singleShot() const;
    void setSingleShot(bool singleShot);

Q_SIGNALS:
    void speedChanged(int speed) const;
    void playEnd() const;

private:
    D_PRIVATE_SLOT(void _q_refreshPicture())

    Q_DISABLE_COPY(DPictureSequenceView)
    D_DECLARE_PRIVATE(DPictureSequenceView)
};

DWIDGET_END_NAMESPACE

#endif // DPICTURESEQUENCEVIEW_H
