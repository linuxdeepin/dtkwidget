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

#ifndef DSCROLLAREA_H
#define DSCROLLAREA_H

#include <QScrollArea>

#include "dtkwidget_global.h"
#include "dobject.h"

DWIDGET_BEGIN_NAMESPACE

class DScrollBar;
class DScrollAreaPrivate;
class D_DECL_DEPRECATED_X("Plase use QScrollArea") LIBDTKWIDGETSHARED_EXPORT DScrollArea : public QScrollArea, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool autoHideScrollBar READ autoHideScrollBar WRITE setAutoHideScrollBar NOTIFY autoHideScrollBarChanged)

public:
    explicit DScrollArea(QWidget *parent = 0);

    bool autoHideScrollBar() const;

public Q_SLOTS:
    void setAutoHideScrollBar(bool autoHideScrollBar);

Q_SIGNALS:
    void autoHideScrollBarChanged(bool autoHideScrollBar);

private Q_SLOTS:
    void hideScrollBar(QScrollBar *bar);
    void showScrollBar(QScrollBar *bar);

protected:
    DScrollArea(DScrollAreaPrivate &dd, QWidget *parent = 0);

private:
    D_DECLARE_PRIVATE(DScrollArea)
};

DWIDGET_END_NAMESPACE

#endif // DSCROLLAREA_H
