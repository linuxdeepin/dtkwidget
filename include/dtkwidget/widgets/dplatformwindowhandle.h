// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPLATFORMWINDOWHANDLE_H
#define DPLATFORMWINDOWHANDLE_H

#include <dtkwidget_global.h>
#include <dtkgui_global.h>
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
    static bool setWindowWallpaperParaByWM(QWidget *widget, const QRect &area, WallpaperScaleMode sMode, WallpaperFillMode fMode);

    using DPlatformHandle::setWindowBlurAreaByWM;
    using DPlatformHandle::setWindowWallpaperParaByWM;
};

DWIDGET_END_NAMESPACE

#endif // DPLATFORMWINDOWHANDLE_H
