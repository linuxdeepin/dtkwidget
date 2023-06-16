// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStackedWidget>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <qtypetraits.h>
#endif
#include <DListView>

#include "dmainwindow.h"
#include <dtkwidget_global.h>

class PageWindowInterface;

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected Q_SLOTS:
    void menuItemInvoked(QAction *action);
    void onCurrentIndexChanged(const QModelIndex &index);

private:
    void initModel();
    void registerPage(const QString &pageName, PageWindowInterface *pPageWindow, const QIcon &icon = QIcon());

private:
    QStackedWidget *m_pStackedWidget;
    DListView *m_pListView;
    QStandardItemModel *m_pListViewModel;
};

#endif // MAINWINDOW_H
