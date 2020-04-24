/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DPLATFORMWINDOWHANDLE_H
#define DPLATFORMWINDOWHANDLE_H

#include <dtkwidget_global.h>
#include <DPlatformHandle>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DPlatformWindowHandle : public DPlatformHandle
{
    Q_OBJECT

public:
    explicit DPlatformWindowHandle(QWidget *widget, QObject *parent = nullptr);

    static void enableDXcbForWindow(QWidget *widget);
    static void enableDXcbForWindow(QWidget *widget, bool redirectContent);
    static bool isEnabledDXcb(const QWidget *widget);

    static bool setWindowBlurAreaByWM(QWidget *widget, const QVector<WMBlurArea> &area);
    static bool setWindowBlurAreaByWM(QWidget *widget, const QList<QPainterPath> &paths);


    using DPlatformHandle::setWindowBlurAreaByWM;
};

DWIDGET_END_NAMESPACE

#endif // DPLATFORMWINDOWHANDLE_H
