/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QPointer>
#include <QScopedPointer>

#include <DSettings>

#include "dabstractdialog.h"

DWIDGET_BEGIN_NAMESPACE

class DSettingsDialogPrivate;
class DSettingsDialog : public DAbstractDialog
{
    Q_OBJECT
public:
    DSettingsDialog(QWidget *parent = 0);
    ~DSettingsDialog();

public Q_SLOTS:
    void updateSettings(DTK_CORE_NAMESPACE::DSettings *settings);

private:
    QScopedPointer<DSettingsDialogPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), DSettingsDialog)
};

DWIDGET_END_NAMESPACE
