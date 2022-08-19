// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DWINDOWQUITFULLBUTTON_H
#define DWINDOWQUITFULLBUTTON_H

#include <DIconButton>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DWindowQuitFullButton : public DIconButton
{
    Q_OBJECT

public:
    DWindowQuitFullButton(QWidget * parent = nullptr);

    QSize sizeHint() const override;

protected:
    void initStyleOption(DStyleOptionButton *option) const override;
};

DWIDGET_END_NAMESPACE

#endif // DWINDOWQUITFULLBUTTON_H
