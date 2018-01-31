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
class LIBDTKWIDGETSHARED_EXPORT DToast : public QFrame, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
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

private:
    qreal opacity() const;
    void setOpacity(qreal);

    D_DECLARE_PRIVATE(DToast)
};


DWIDGET_END_NAMESPACE
