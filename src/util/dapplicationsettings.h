/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
