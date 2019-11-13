/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef DAPPLICATIONHELPER_H
#define DAPPLICATIONHELPER_H

#include <dtkwidget_global.h>
#include <DGuiApplicationHelper>
#include <DPalette>

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DApplicationHelper : public DGuiApplicationHelper
{
    Q_OBJECT

public:
    static DApplicationHelper *instance();

    DPalette palette(const QWidget *widget, const QPalette &base = QPalette()) const;
    void setPalette(QWidget *widget, const DPalette &palette);
    void resetPalette(QWidget *widget);

private:
    DApplicationHelper();
    ~DApplicationHelper();

    bool eventFilter(QObject *watched, QEvent *event) override;
    bool event(QEvent *event) override;

    friend class _DApplicationHelper;
};

DWIDGET_END_NAMESPACE

#endif // DAPPLICATIONHELPER_H
