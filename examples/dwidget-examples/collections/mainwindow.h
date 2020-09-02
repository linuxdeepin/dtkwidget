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
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QStandardItemModel>
#include <QStackedWidget>
#include <qtypetraits.h>
#include <DListView>

#include "widgets/dmainwindow.h"

#include "bartab.h"
#include "buttontab.h"
#include "inputtab.h"
#include "linetab.h"
#include "slidertab.h"
#include "indicatortab.h"
#include "widgetstab.h"


class QAction;

DWIDGET_USE_NAMESPACE

struct ItemInfo {
    ItemInfo(const QString &itemName)
        : m_itemName(itemName)
        , m_pModel(nullptr)
        , m_pPageWindow(nullptr)
    {
    }

    ~ItemInfo()
    {
        for (auto pItem : m_itemVector) {
            delete pItem;
        }
    }

    QString m_itemName;
    QVector<ItemInfo *> m_itemVector;
    QStandardItemModel *m_pModel;
    QWidget *m_pPageWindow;
};

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected Q_SLOTS:
    void menuItemInvoked(QAction *action);
    void onPrimaryIndexChanged(const QModelIndex &modelIndex);
    void onSubIndexChanged(const QModelIndex &modelIndex);

private:
    void initListView();
    void registerPage(const QString &primaryMenuName, const QString &subMenuName, QWidget *pPageWindow);
    void initModel();

private:
    QVector<ItemInfo *> m_primaryMenu;
    QStackedWidget *m_pStackedWidget;
    DListView *m_pPrimaryListView;
    DListView *m_pSubListView;
    QStandardItemModel *m_pPrimaryMenuModel;
};

#endif // MAINWINDOW_H
