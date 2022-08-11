// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSWITCHBUTTON_H
#define DSWITCHBUTTON_H

#include <QVariantAnimation>
#include <QAbstractButton>

#include <dtkwidget_global.h>
#include <DObject>

DWIDGET_BEGIN_NAMESPACE

class DSwitchButtonPrivate;
class DStyleOptionButton;
class LIBDTKWIDGETSHARED_EXPORT DSwitchButton : public QAbstractButton, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

public:
    explicit DSwitchButton(QWidget *parent = Q_NULLPTR);

    QSize sizeHint() const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void checkedChanged(bool arg);

protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void initStyleOption(DStyleOptionButton *option) const;

private:
    D_DECLARE_PRIVATE(DSwitchButton)
};

DWIDGET_END_NAMESPACE

#endif // DSWITCHBUTTON_H

