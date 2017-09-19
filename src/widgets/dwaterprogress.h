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

#ifndef DWATERPROGRESS_H
#define DWATERPROGRESS_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QScopedPointer>
#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DWaterProgressPrivate;
class LIBDTKWIDGETSHARED_EXPORT DWaterProgress : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
public:
    explicit DWaterProgress(QWidget *parent = 0);
    ~DWaterProgress();

    int value() const;

Q_SIGNALS:
    void valueChanged();

public Q_SLOTS:
    void start();
    void stop();
    void setValue(int value);
    void setTextVisible(bool visible);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DWaterProgress)
};

DWIDGET_END_NAMESPACE

#endif // DWATERPROGRESS_H
