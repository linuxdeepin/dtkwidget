/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DBOXWIDGET_H
#define DBOXWIDGET_H

#include "dtkwidget_global.h"

#include <dobject.h>

#include <QFrame>
#include <QBoxLayout>

DWIDGET_BEGIN_NAMESPACE

class DBoxWidgetPrivate;
class DBoxWidget : public QFrame, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(QBoxLayout::Direction direction READ direction WRITE setDirection NOTIFY directionChanged)

public:
    explicit DBoxWidget(QBoxLayout::Direction direction, QWidget *parent = 0);

    QBoxLayout::Direction direction() const;
    QBoxLayout *layout() const;

    void addWidget(QWidget *widget);
    QSize sizeHint() const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void setDirection(QBoxLayout::Direction direction);

Q_SIGNALS:
    void sizeChanged(QSize size);
    void directionChanged(QBoxLayout::Direction direction);

protected:
    virtual void updateSize(const QSize &size);
    bool event(QEvent *ee) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DBoxWidget)
};

class DHBoxWidget : public DBoxWidget
{
    Q_OBJECT
public:
    explicit DHBoxWidget(QWidget *parent = 0);
};

class DVBoxWidget : public DBoxWidget
{
    Q_OBJECT
public:
    explicit DVBoxWidget(QWidget *parent = 0);
};

DWIDGET_END_NAMESPACE

#endif // DBOXWIDGET_H
