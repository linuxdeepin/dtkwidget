/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef DFRAME_H
#define DFRAME_H

#include <dtkwidget_global.h>
#include <DPalette>
#include <DObject>

#include <QFrame>

DWIDGET_BEGIN_NAMESPACE

class DFramePrivate;
class DFrame : public QFrame, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DFrame)

public:
    explicit DFrame(QWidget *parent = nullptr);

    void setFrameRounded(bool on);
    void setBackgroundRole(DGUI_NAMESPACE::DPalette::ColorType type);
    using QFrame::setBackgroundRole;

protected:
    DFrame(DFramePrivate &dd, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
};

class DHorizontalLine : public QFrame
{
    Q_OBJECT
public:
    explicit DHorizontalLine(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QFrame(parent, f)
    {
        setFrameShape(HLine);
    }
};

class DVerticalLine : public QFrame
{
    Q_OBJECT
public:
    explicit DVerticalLine(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QFrame(parent, f)
    {
        setFrameShape(VLine);
    }
};

DWIDGET_END_NAMESPACE

#endif // DFRAME_H
