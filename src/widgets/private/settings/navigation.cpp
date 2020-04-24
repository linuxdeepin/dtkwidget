/*
 * Copyright (C) 2016 ~ 2017 Deepin Technology Co., Ltd.
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

#include "navigation.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QListView>
#include <QStandardItemModel>
#include <QCoreApplication>

#include <DSettings>
#include <DSettingsGroup>
#include <DListView>
#include <DApplicationHelper>

#include "navigationdelegate.h"

DWIDGET_BEGIN_NAMESPACE

class NavigationPrivate
{
public:
    NavigationPrivate(Navigation *parent) : q_ptr(parent) {}
    QModelIndex indexOfGroup(const QString &key) const
    {
        for (int i = 0; i < navbarModel->rowCount(); ++i) {
            auto index = navbarModel->index(i, 0);
            if (index.data(NavigationDelegate::NavKeyRole).toString() == key) {
                return index;
            }
        }

        return QModelIndex();
    }

    DListView           *navbar         = nullptr;
    QStandardItemModel  *navbarModel    = nullptr;

    Navigation *q_ptr;
    Q_DECLARE_PUBLIC(Navigation)
};

Navigation::Navigation(QWidget *parent) :
    QFrame(parent), d_ptr(new NavigationPrivate(this))
{
    Q_D(Navigation);

    setObjectName("Navigation");

    setContentsMargins(0, 0, 0, 0);
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    d->navbar = new DListView(this);
    d->navbar->setObjectName("NavigationBar");
    d->navbar->setContentsMargins(0, 0, 0, 0);
    d->navbar->setAutoFillBackground(true);
    d->navbar->setViewportMargins(10, 0, 10, 0);
    DPalette pa = DApplicationHelper::instance()->palette(d->navbar);
    pa.setBrush(DPalette::ItemBackground, Qt::transparent);
    DApplicationHelper::instance()->setPalette(d->navbar, pa);

    d->navbar->setSelectionMode(QListView::SingleSelection);
    d->navbar->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    d->navbarModel = new QStandardItemModel;

    d->navbar->setModel(d->navbarModel);

    d->navbar->setEditTriggers(QAbstractItemView::NoEditTriggers);
    d->navbar->setItemDelegate(new NavigationDelegate(d->navbar));

    layout->addWidget(d->navbar);

    connect(d->navbar->selectionModel(), &QItemSelectionModel::currentChanged, this, [=] (const QModelIndex &current) {
        const QString &key = current.data(NavigationDelegate::NavKeyRole).toString();
        if (!key.isEmpty()) {
            Q_EMIT selectedGroup(key);
        }
    });
    d->navbar->setFixedWidth(190);
}

Navigation::~Navigation()
{
}

bool Navigation::groupIsVisible(const QString &key) const
{
    Q_D(const Navigation);

    const QModelIndex &index = d->indexOfGroup(key);

    return index.isValid() && d->navbar->isRowHidden(index.row());
}

void Navigation::setGroupVisible(const QString &key, bool visible)
{
    Q_D(Navigation);

    const QModelIndex &index = d->indexOfGroup(key);

    if (index.isValid()) {
        d->navbar->setRowHidden(index.row(), !visible);
    }
}

void Navigation::onSelectGroup(const QString &key)
{
    Q_D(Navigation);

    const QModelIndex &index = d->indexOfGroup(key);

    if (index.isValid()) {
        d->navbar->setCurrentIndex(index);
    }
}

void Navigation::updateSettings(const QByteArray &translateContext, QPointer<DTK_CORE_NAMESPACE::DSettings> settings)
{
    Q_D(Navigation);

    for (auto groupKey : settings->groupKeys()) {
        auto group = settings->group(groupKey);
        if (group->isHidden()) {
            continue;
        }
        auto item = new QStandardItem;
        auto trName = translateContext.isEmpty() ? QObject::tr(group->name().toStdString().c_str())
                                                 : qApp->translate(translateContext.constData(), group->name().toStdString().c_str());
//        qDebug() << settings->group(groupKey)->name() << trName;
//        QFont font(item->data(Qt::FontRole).value<QFont>());
//        QFontMetrics fm(font);
//        int width = fm.width(trName);
//        qDebug() << ">>>>>>>>>>> " << font << trName << width;

        item->setData(trName, Qt::DisplayRole);
        item->setData(NavigationDelegate::Level1, NavigationDelegate::NavLevelRole);
        item->setData(groupKey, NavigationDelegate::NavKeyRole);
        d->navbarModel->appendRow(item);

        for (auto subgroup : settings->group(groupKey)->childGroups()) {
            if (subgroup->isHidden()) {
                continue;
            }

            if (subgroup->name().isEmpty()) {
                continue;
            }

            auto item = new QStandardItem;
            auto trName = translateContext.isEmpty() ? QObject::tr(subgroup->name().toStdString().c_str())
                                                     : qApp->translate(translateContext.constData(), subgroup->name().toStdString().c_str());
//            qDebug() << subgroup->name() << trName;
//            QFont font(item->data(Qt::FontRole).value<QFont>());
//            QFontMetrics fm(font);
//            int width = fm.width(trName);
//            qDebug() << "<<<<<<<<<<<< " << font << trName << width;
            item->setData(trName, Qt::DisplayRole);
            item->setData(NavigationDelegate::Level2, NavigationDelegate::NavLevelRole);
            item->setData(subgroup->key(), NavigationDelegate::NavKeyRole);
            d->navbarModel->appendRow(item);
        }
    }

    d->navbar->setCurrentIndex(d->navbarModel->index(0, 0));
}

DWIDGET_END_NAMESPACE
