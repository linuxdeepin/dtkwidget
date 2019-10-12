/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     wp <wangpeng_cm@deepin.com>
 *
 * Maintainer: wp <wangpeng_cm@deepin.com>
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

#ifndef DTIPLABEL_H
#define DTIPLABEL_H

#include <dobject.h>
#include <dstyle.h>
#include <dtkwidget_global.h>

#include <QLabel>

DWIDGET_BEGIN_NAMESPACE

class DTipLabelPrivate;
class LIBDTKWIDGETSHARED_EXPORT DTipLabel : public QLabel, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DTipLabel)
    D_DECLARE_PRIVATE(DTipLabel)
public:
    DTipLabel(const QString &text, QWidget *parent = nullptr);
    ~DTipLabel();

    void show(const QPoint &pos);

    void setForegroundRole(DPalette::ColorType color);

protected:
    void initPainter(QPainter *painter) const override;
    void paintEvent(QPaintEvent *event) override;
};
DWIDGET_END_NAMESPACE

#endif // DTIPLABEL_H
