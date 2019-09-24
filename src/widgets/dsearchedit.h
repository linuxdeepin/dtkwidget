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

#ifndef DSEARCHEDIT_H
#define DSEARCHEDIT_H

#include <DLineEdit>

DWIDGET_BEGIN_NAMESPACE

class DSearchEditPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSearchEdit : public DLineEdit
{
    Q_OBJECT

public:
    explicit DSearchEdit(QWidget *parent = nullptr);
    ~DSearchEdit();

    void setPlaceHolder(QString placeHolder);
    QString placeHolder() const;

protected:
    Q_DISABLE_COPY(DSearchEdit)
    D_DECLARE_PRIVATE(DSearchEdit)
    Q_PRIVATE_SLOT(d_func(), void _q_toEditMode(bool))
};

DWIDGET_END_NAMESPACE

#endif // DSEARCHEDIT_H
