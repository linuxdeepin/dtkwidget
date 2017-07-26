/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
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
    ~DWindowManagerHelper();

    static DWindowManagerHelper *instance();

    bool hasBlurWindow() const;
    bool hasComposite() const;

    QList<DForeignWindow*> currentWorkspaceWindows() const;

Q_SIGNALS:
    void windowManagerChanged();
    void hasBlurWindowChanged();
    void hasCompositeChanged();
    void windowListChanged();

protected:
    explicit DWindowManagerHelper(QObject *parent = 0);

private:
    D_DECLARE_PRIVATE(DWindowManagerHelper)
};

DWIDGET_END_NAMESPACE

#endif // DWINDOWMANAGERHELPER_H
