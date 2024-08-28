// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <functional>

#include <QObject>
#include <QScopedPointer>

#include <dtkwidget_global.h>

DCORE_BEGIN_NAMESPACE
class DSettingsOption;
DCORE_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DSettingsWidgetFactoryPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSettingsWidgetFactory : public QObject
{
    Q_OBJECT
public:
    typedef QWidget *(WidgetCreateHandler)(QObject *);
    typedef QPair<QWidget*, QWidget*> (ItemCreateHandler)(QObject *);

    explicit DSettingsWidgetFactory(QObject *parent = Q_NULLPTR);
    ~DSettingsWidgetFactory();

    void registerWidget(const QString &viewType, std::function<WidgetCreateHandler> handler);
    void registerWidget(const QString &viewType, std::function<ItemCreateHandler> handler);

    QWidget *createWidget(QPointer<DTK_CORE_NAMESPACE::DSettingsOption> option);
    QWidget *createWidget(const QByteArray &translateContext, QPointer<DTK_CORE_NAMESPACE::DSettingsOption> option);
    QPair<QWidget*, QWidget*> createItem(QPointer<DTK_CORE_NAMESPACE::DSettingsOption> option) const;
    QPair<QWidget*, QWidget*> createItem(const QByteArray &translateContext, QPointer<DTK_CORE_NAMESPACE::DSettingsOption> option) const;

    D_DECL_DEPRECATED static QWidget *createTwoColumWidget(DTK_CORE_NAMESPACE::DSettingsOption *option, QWidget *rightWidget);
    D_DECL_DEPRECATED static QWidget *createTwoColumWidget(const QByteArray &translateContext, DTK_CORE_NAMESPACE::DSettingsOption *option, QWidget *rightWidget);
    static QPair<QWidget*, QWidget*> createStandardItem(const QByteArray &translateContext, DTK_CORE_NAMESPACE::DSettingsOption *option, QWidget *rightWidget);

private:
    QScopedPointer<DSettingsWidgetFactoryPrivate> dd_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(dd_ptr), DSettingsWidgetFactory)
};

DWIDGET_END_NAMESPACE
