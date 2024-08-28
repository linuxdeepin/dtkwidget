// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DTICKEFFECT_H
#define DTICKEFFECT_H

#include <dtkwidget_global.h>
#include <DObject>

#include <QGraphicsEffect>

DWIDGET_BEGIN_NAMESPACE

class DTickEffectPrivate;
class LIBDTKWIDGETSHARED_EXPORT DTickEffect : public QGraphicsEffect, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DTickEffect(QWidget *widget, QWidget *parent = 0);

    enum Direction{
        LeftToRight,
        RightToLeft,
        TopToBottom,
        BottomToTop
    };

    void play();
    void stop();
    void pause();
    void resume();

    void setDirection(Direction direction);
    void setFixedPixelMove(const int pixel);

Q_SIGNALS:
    void finished();
    void stateChanged();

protected:
    void draw(QPainter *painter) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DTickEffect)
};

DWIDGET_END_NAMESPACE

#endif // DTICKEFFECT_H
