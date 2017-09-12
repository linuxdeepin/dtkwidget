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

#ifndef DENHANCEDWIDGET_H
#define DENHANCEDWIDGET_H

#include <QWidget>

#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DEnhancedWidgetPrivate;
class DEnhancedWidget: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QWidget *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit DEnhancedWidget(QWidget *target, QObject *parent = 0);
    ~DEnhancedWidget();

    QWidget *target() const;
    bool enabled() const;

public Q_SLOTS:
    void setTarget(QWidget *target);
    void setEnabled(bool enabled);

Q_SIGNALS:
    void xChanged(int x);
    void yChanged(int y);
    void positionChanged(const QPoint &point);
    void widthChanged(int width);
    void heightChanged(int height);
    void sizeChanged(const QSize &size);
    void targetChanged(QWidget *target);
    void enabledChanged(bool enabled);
    void showed();

protected:
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;

private:
    explicit DEnhancedWidget(DEnhancedWidgetPrivate *dd, QWidget *w, QObject *parent = 0);

    DEnhancedWidgetPrivate *d_ptr;

    Q_DECLARE_PRIVATE(DEnhancedWidget)
};

DWIDGET_END_NAMESPACE

#endif // DENHANCEDWIDGET_H
