// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DWINDOWMAXBUTTON_H
#define DWINDOWMAXBUTTON_H

#include <DIconButton>

DWIDGET_BEGIN_NAMESPACE

class DWindowMaxButtonPrivate;

class LIBDTKWIDGETSHARED_EXPORT DWindowMaxButton : public DIconButton
{
    Q_OBJECT
public:
    DWindowMaxButton(QWidget * parent = 0);

    Q_PROPERTY(bool isMaximized READ isMaximized WRITE setMaximized NOTIFY maximizedChanged)

    bool isMaximized() const;
    QSize sizeHint() const override;

public Q_SLOTS:
    void setMaximized(bool isMaximized);

Q_SIGNALS:
    void maximizedChanged(bool isMaximized);

protected:
    void initStyleOption(DStyleOptionButton *option) const override;

private:
    D_DECLARE_PRIVATE(DWindowMaxButton)
};

DWIDGET_END_NAMESPACE

#endif // DWINDOWMAXBUTTON_H
