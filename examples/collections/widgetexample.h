// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef WIDGETEXAMPLE_H
#define WIDGETEXAMPLE_H

#include <dtkwidget_global.h>
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

#include <DGuiApplicationHelper>

#include <QWidget>
#include <QLabel>
#include <QAbstractTableModel>
#include <QDate>

DWIDGET_USE_NAMESPACE

DGUI_USE_NAMESPACE

class QTableView;

class WidgetExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit WidgetExampleWindow(QWidget *parent = nullptr);
};

class DCalendarWidgetExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DCalendarWidgetExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class DTableWidgetExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DTableWidgetExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;

private:
    QTableView *tableView;
};

class DMPRISControlWidgetExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DMPRISControlWidgetExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;
};

class CalendarModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CalendarModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    QStringList header;
    QVector<QVector<QDate>> m_tableData; // row = 6, col = 7
};

#endif // WIDGETEXAMPLE_H
