// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPICTURESEQUENCEVIEW_H
#define DPICTURESEQUENCEVIEW_H

#include <dtkwidget_global.h>
#include <DObject>

#include <QtGlobal>
#include <QPair>
#include <QGraphicsView>

DWIDGET_BEGIN_NAMESPACE

class DPictureSequenceViewPrivate;
class LIBDTKWIDGETSHARED_EXPORT DPictureSequenceView : public QGraphicsView, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(bool singleShot READ singleShot WRITE setSingleShot)

public:
    DPictureSequenceView(QWidget *parent = nullptr);

    void setPictureSequence(const QString &srcFormat, const QPair<int, int> &range, const int fieldWidth = 0, const bool autoScale = false);
    void setPictureSequence(const QStringList &sequence, const bool autoScale = false);
    void setPictureSequence(const QList<QPixmap> &sequence, const bool autoScale = false);
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
