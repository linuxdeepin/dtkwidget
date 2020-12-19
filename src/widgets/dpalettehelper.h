/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
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

#ifndef DPALETTEHELPER_H
#define DPALETTEHELPER_H

#include <dtkwidget_global.h>
#include <DPalette>
#include <DObject>

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DPaletteHelperPrivate;
class DPaletteHelper : public QObject
    , public DCORE_NAMESPACE::DObject
{
    Q_OBJECT

public:
    static DPaletteHelper *instance();

    DPalette palette(const QWidget *widget, const QPalette &base = QPalette()) const;
    void setPalette(QWidget *widget, const DPalette &palette);
    void resetPalette(QWidget *widget);

private:
    DPaletteHelper(QObject *parent = nullptr);
    ~DPaletteHelper() override;

    bool eventFilter(QObject *watched, QEvent *event) override;
    bool event(QEvent *event) override;

    D_DECLARE_PRIVATE(DPaletteHelper)
};

DWIDGET_END_NAMESPACE

#endif // DPALETTEHELPER_H
