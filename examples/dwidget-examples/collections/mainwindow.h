/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStackedWidget>
#include <qtypetraits.h>
#include <DListView>

#include "widgets/dmainwindow.h"
#include "dtkwidget_global.h"

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
