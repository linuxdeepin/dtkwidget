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

#ifndef DPASSWORDEDIT_H
#define DPASSWORDEDIT_H

#include "dlineedit.h"

DWIDGET_BEGIN_NAMESPACE

class DPasswordEditPrivate;
class LIBDTKWIDGETSHARED_EXPORT DPasswordEdit : public DLineEdit
{
    Q_OBJECT
    Q_PROPERTY(bool isEchoMode READ isEchoMode NOTIFY echoModeChanged)

public:
    DPasswordEdit(QWidget *parent = nullptr);

    bool isEchoMode() const;
    void setEchoMode(EchoMode mode);

    void setEchoButtonIsVisible(bool visible);
    bool isEchoButtonIsVisible () const;

Q_SIGNALS:
    void echoModeChanged(bool echoOn);

protected:
    Q_DISABLE_COPY(DPasswordEdit)
    D_DECLARE_PRIVATE(DPasswordEdit)
    Q_PRIVATE_SLOT(d_func(), void _q_toggleEchoMode())
};

DWIDGET_END_NAMESPACE

#endif // DPASSWORDEDIT_H
