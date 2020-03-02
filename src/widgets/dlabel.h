/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef DLABEL_H
#define DLABEL_H

#include <dobject.h>
#include <dstyle.h>
#include <dtkwidget_global.h>

#include <QLabel>

DWIDGET_BEGIN_NAMESPACE

class DLabelPrivate;
class LIBDTKWIDGETSHARED_EXPORT DLabel : public QLabel, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DLabel)
    D_DECLARE_PRIVATE(DLabel)
public:
    explicit DLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    DLabel(const QString &text, QWidget *parent = nullptr);
    ~DLabel();

    void setForegroundRole(QPalette::ColorRole role);
    void setForegroundRole(DPalette::ColorType color);
    void setElideMode(Qt::TextElideMode elideMode);
    Qt::TextElideMode elideMode() const;

protected:
    DLabel(DLabelPrivate &dd, QWidget *parent = nullptr);

    void initPainter(QPainter *painter) const override;
    void paintEvent(QPaintEvent *event) override;
};

DWIDGET_END_NAMESPACE

#endif // DLABEL_H
