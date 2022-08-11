// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <QScopedPointer>
#include <QFrame>

class ButtonGroupPrivate;
class ButtonGroup : public QFrame
{
    Q_OBJECT
public:
    explicit ButtonGroup(QWidget *parent = 0);
    ~ButtonGroup();

Q_SIGNALS:
    void buttonChecked(int id);

public Q_SLOTS:
    void setCheckedButton(int id);
    void setButtons(const QStringList &texts);

private:
    QScopedPointer<ButtonGroupPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), ButtonGroup)
};

