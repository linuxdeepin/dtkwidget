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

#ifndef DSCROLLBAR_H
#define DSCROLLBAR_H

#include <QScrollBar>

#include "dtkwidget_global.h"
#include "dobject.h"

DWIDGET_BEGIN_NAMESPACE

class DScrollBarPrivate;
class D_DECL_DEPRECATED_X("Plase use QScrollBar") LIBDTKWIDGETSHARED_EXPORT DScrollBar : public QScrollBar, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool autoHide READ autoHide WRITE setAutoHide NOTIFY autoHideChanged)

public:
    explicit DScrollBar(QWidget *parent = 0);

    bool autoHide() const;

public Q_SLOTS:
    void setAutoHide(bool autoHide);

Q_SIGNALS:
    void autoHideChanged(bool autoHide);

protected:
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(DScrollBar)
    D_DECLARE_PRIVATE(DScrollBar)
    D_PRIVATE_SLOT(void _q_hidden())
    D_PRIVATE_SLOT(void _q_updateOpacity())
};

DWIDGET_END_NAMESPACE

#endif // DSCROLLBAR_H
