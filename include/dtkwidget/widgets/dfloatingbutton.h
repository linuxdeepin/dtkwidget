// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFLOATINGBUTTON_H
#define DFLOATINGBUTTON_H
#include <DDciIcon>
#include <DIconButton>

DWIDGET_BEGIN_NAMESPACE

class DFloatingButton : public DIconButton
{
    Q_OBJECT

public:
    explicit DFloatingButton(QWidget *parent = nullptr);
    explicit DFloatingButton(QStyle::StandardPixmap iconType, QWidget *parent = nullptr);
    explicit DFloatingButton(DStyle::StandardPixmap iconType, QWidget *parent = nullptr);
    explicit DFloatingButton(const QString &text, QWidget *parent = nullptr);
    DFloatingButton(const QIcon& icon, const QString &text = QString(), QWidget *parent = nullptr);
    DFloatingButton(const DDciIcon &icon, const QString &text = QString(), QWidget *parent = nullptr);

protected:
    DStyleOptionButton baseStyleOption() const override;
    void initStyleOption(DStyleOptionButton *option) const override;
};

DWIDGET_END_NAMESPACE

#endif // DFLOATINGBUTTON_H
