// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DWINDOWMINBUTTON_H
#define DWINDOWMINBUTTON_H

#include <DIconButton>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DWindowMinButton : public DIconButton
{
    Q_OBJECT

public:
    DWindowMinButton(QWidget * parent = 0);

    QSize sizeHint() const override;

protected:
    void initStyleOption(DStyleOptionButton *option) const override;
};

DWIDGET_END_NAMESPACE

#endif // DWINDOWMINBUTTON_H
