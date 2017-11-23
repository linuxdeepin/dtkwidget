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

#ifndef DWINDOW_H
#define DWINDOW_H

#include <QWidget>
#include <dobject.h>
#include <dtkwidget_global.h>

#include "dwidget.h"

class QMenu;

DWIDGET_BEGIN_NAMESPACE

class DWindowPrivate;

class D_DECL_DEPRECATED_X("Use DMainWindow instead.") DWindow : public DWidget
{
    Q_OBJECT
public:
    explicit DWindow(QWidget *parent = 0);

    void setContentLayout(QLayout *l);
    void setContentWidget(QWidget *w);

    void setParent(QWidget *parent);
    QWidget *parentWidget() const;

protected Q_SLOTS:

private:
    D_DECLARE_PRIVATE(DWindow)
    Q_DISABLE_COPY(DWindow)
};

DWIDGET_END_NAMESPACE

#endif // DWINDOW_H
