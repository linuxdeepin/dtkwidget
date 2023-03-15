// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DMAINWINDOW_P_H
#define DMAINWINDOW_P_H

#include <dtkwidget_global.h>
#include <DMainWindow>
#include <DShadowLine>

#include <DObjectPrivate>
class QShortcut;
DWIDGET_BEGIN_NAMESPACE

class DSidebarHelper;
class DPlatformWindowHandle;
class DTitlebar;
class DMainWindowPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DMainWindowPrivate(DMainWindow *qq);

    void init();
    void updateTitleShadowGeometry();

    DPlatformWindowHandle   *handle     = Q_NULLPTR;
    DTitlebar               *titlebar   = Q_NULLPTR;
    DShadowLine             *titleShadow = nullptr;
    QShortcut               *help       = Q_NULLPTR;
    DSidebarHelper          *sidebarHelper = nullptr;
    QWidget                 *sidebarWidget = nullptr;
    QToolBar                *tb = nullptr;

private:
    D_DECLARE_PUBLIC(DMainWindow)
    void _q_autoShowFeatureDialog();
};

DWIDGET_END_NAMESPACE

#endif // DMAINWINDOW_P_H
