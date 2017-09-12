/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DBASEBUTTON_H
#define DBASEBUTTON_H

#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DBaseButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DBaseButton(QWidget * parent = 0);
    explicit DBaseButton(const QString & text, QWidget * parent = 0);
    explicit DBaseButton(const QIcon & icon, const QString & text, QWidget * parent = 0);

private:
    void initInsideFrame();
};

DWIDGET_END_NAMESPACE

#endif // DBASEBUTTON_H
