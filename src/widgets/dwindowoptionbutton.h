// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DWINDOWOPTIONBUTTON_H
#define DWINDOWOPTIONBUTTON_H

#include <DIconButton>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DWindowOptionButton : public DIconButton
{
    Q_OBJECT
public:
    DWindowOptionButton(QWidget * parent = 0);

    QSize sizeHint() const override;

protected:
    void initStyleOption(DStyleOptionButton *option) const override;
};

DWIDGET_END_NAMESPACE

#endif // DWINDOWOPTIONBUTTON_H
