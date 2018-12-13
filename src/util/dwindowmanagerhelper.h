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

#ifndef DWINDOWMANAGERHELPER_H
#define DWINDOWMANAGERHELPER_H

#include <QWindow>

#include <DObject>

#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DForeignWindow;
class DWindowManagerHelperPrivate;
class DWindowManagerHelper : public QObject, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool hasBlurWindow READ hasBlurWindow NOTIFY hasBlurWindowChanged)
    Q_PROPERTY(bool hasComposite READ hasComposite NOTIFY hasCompositeChanged)

public:
    enum MotifFunction {
        FUNC_RESIZE   = (1L << 1),
        FUNC_MOVE     = (1L << 2),
        FUNC_MINIMIZE = (1L << 3),
        FUNC_MAXIMIZE = (1L << 4),
        FUNC_CLOSE    = (1L << 5),
        FUNC_ALL      = FUNC_RESIZE | FUNC_MOVE | FUNC_MINIMIZE | FUNC_MAXIMIZE | FUNC_CLOSE
    };
    Q_DECLARE_FLAGS(MotifFunctions, MotifFunction)

    enum MotifDecoration {
        DECOR_BORDER   = (1L << 1),
        DECOR_RESIZEH  = (1L << 2),
        DECOR_TITLE    = (1L << 3),
        DECOR_MENU     = (1L << 4),
        DECOR_MINIMIZE = (1L << 5),
        DECOR_MAXIMIZE = (1L << 6),
        DECOR_ALL      = DECOR_BORDER | DECOR_RESIZEH | DECOR_TITLE | DECOR_MENU | DECOR_MINIMIZE | DECOR_MAXIMIZE
    };
    Q_DECLARE_FLAGS(MotifDecorations, MotifDecoration)

    enum WMName {
        OtherWM,
        DeepinWM,
        KWinWM
    };
    Q_ENUM(WMName)

    ~DWindowManagerHelper();

    static DWindowManagerHelper *instance();

    static void setMotifFunctions(const QWindow *window, MotifFunctions hints);
    static MotifFunctions setMotifFunctions(const QWindow *window, MotifFunctions hints, bool on);
    static MotifFunctions getMotifFunctions(const QWindow *window);
    static void setMotifDecorations(const QWindow *window, MotifDecorations hints);
    static MotifDecorations setMotifDecorations(const QWindow *window, MotifDecorations hints, bool on);
    static MotifDecorations getMotifDecorations(const QWindow *window);

    static void popupSystemWindowMenu(const QWindow *window);

    bool hasBlurWindow() const;
    bool hasComposite() const;
    QString windowManagerNameString() const;
    WMName windowManagerName() const;

    QVector<quint32> allWindowIdList() const;
    QVector<quint32> currentWorkspaceWindowIdList() const;
    QList<DForeignWindow*> currentWorkspaceWindows() const;

Q_SIGNALS:
    void windowManagerChanged();
    void hasBlurWindowChanged();
    void hasCompositeChanged();
    void windowListChanged();
    void windowMotifWMHintsChanged(quint32 winId);

protected:
    explicit DWindowManagerHelper(QObject *parent = 0);

private:
    D_DECLARE_PRIVATE(DWindowManagerHelper)
};

DWIDGET_END_NAMESPACE

Q_DECLARE_OPERATORS_FOR_FLAGS(DTK_WIDGET_NAMESPACE::DWindowManagerHelper::MotifFunctions)
Q_DECLARE_OPERATORS_FOR_FLAGS(DTK_WIDGET_NAMESPACE::DWindowManagerHelper::MotifDecorations)

#endif // DWINDOWMANAGERHELPER_H
