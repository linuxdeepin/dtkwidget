/*
 * Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
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
#ifndef DFLOATINGBUTTON_H
#define DFLOATINGBUTTON_H

#include <DIconButton>

DWIDGET_BEGIN_NAMESPACE

class DFloatingButton : public DIconButton
{
    Q_OBJECT

public:
    explicit DFloatingButton(QWidget *parent = nullptr);
    explicit DFloatingButton(QStyle::StandardPixmap iconType = static_cast<QStyle::StandardPixmap>(-1), QWidget *parent = nullptr);
    explicit DFloatingButton(DStyle::StandardPixmap iconType = static_cast<DStyle::StandardPixmap>(-1), QWidget *parent = nullptr);
    explicit DFloatingButton(const QString &text, QWidget *parent = nullptr);
    DFloatingButton(const QIcon& icon, const QString &text = QString(), QWidget *parent = nullptr);

protected:
    DStyleOptionButton baseStyleOption() const override;
    void initStyleOption(DStyleOptionButton *option) const override;
};

DWIDGET_END_NAMESPACE

#endif // DFLOATINGBUTTON_H
