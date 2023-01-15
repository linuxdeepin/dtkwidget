// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DAPPLICATIONSETTINGS_H
#define DAPPLICATIONSETTINGS_H

#include <dtkwidget_global.h>
#include <DObject>

DWIDGET_BEGIN_NAMESPACE

class DApplicationSettingsPrivate;
class DApplicationSettings : public QObject, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DApplicationSettings)

public:
    explicit DApplicationSettings(QObject *parent = nullptr);

private:
    D_PRIVATE_SLOT(void _q_onChanged(const QString &))
    D_PRIVATE_SLOT(void _q_onPaletteTypeChanged())
};

DWIDGET_END_NAMESPACE

#endif // DAPPLICATIONSETTINGS_H
