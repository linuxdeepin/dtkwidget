/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DTICKEFFECT_H
#define DTICKEFFECT_H

#include "dtkwidget_global.h"
#include "dobject.h"

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
    void setDuration(const int duration);

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
