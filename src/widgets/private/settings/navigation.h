// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <QScopedPointer>
#include <QFrame>

#include <dtkwidget_global.h>

DCORE_BEGIN_NAMESPACE
class DSettings;
DCORE_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class NavigationPrivate;
class LIBDTKWIDGETSHARED_EXPORT Navigation : public QFrame
{
    Q_OBJECT
public:
    explicit Navigation(QWidget *parent = 0);
    ~Navigation();

    bool groupIsVisible(const QString &key) const;
    void setGroupVisible(const QString &key, bool visible);

Q_SIGNALS:
    void selectedGroup(const QString &key);

public Q_SLOTS:
    void onSelectGroup(const QString &key);
    void updateSettings(const QByteArray &translateContext, QPointer<DTK_CORE_NAMESPACE::DSettings> settings);

private:
    QScopedPointer<NavigationPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), Navigation)
};

DWIDGET_END_NAMESPACE
