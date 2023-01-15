// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DTABLETWINDOWOPTIONBUTTON_H
#define DTABLETWINDOWOPTIONBUTTON_H

#include <DIconButton>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DTabletWindowOptionButton : public DIconButton
{
    Q_OBJECT
public:
    DTabletWindowOptionButton(QWidget *parent = 0);

    QSize sizeHint() const override;

protected:
    void initStyleOption(DStyleOptionButton *option) const override;
};

DWIDGET_END_NAMESPACE

#endif // DTABLETWINDOWOPTIONBUTTON_H
