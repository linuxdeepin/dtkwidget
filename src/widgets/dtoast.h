/*
 * Copyright (C) 2016 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
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


#pragma once

#include <QScopedPointer>
#include <QFrame>
#include <QIcon>

#include "dtkwidget_global.h"
#include <DObject>

DWIDGET_BEGIN_NAMESPACE

class DToastPrivate;
class DToast : public QFrame, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(QBrush background READ background WRITE setBackground)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
public:
    explicit DToast(QWidget *parent = 0);
    ~DToast();

    QString text() const;
    QIcon icon() const;

public Q_SLOTS:
    void pop();
    void pack();

    void setText(QString text);
    void setIcon(QString icon);
    void setIcon(QIcon icon, QSize defaultSize = QSize(20, 20));

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    qreal opacity() const;
    QColor borderColor() const;
    QBrush background() const;
    int radius() const;

    void setBorderColor(QColor borderColor);
    void setOpacity(qreal);
    void setBackground(QBrush background);
    void setRadius(int radius);

    D_DECLARE_PRIVATE(DToast)
};


DWIDGET_END_NAMESPACE
