// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DTOOLBUTTON_H
#define DTOOLBUTTON_H

#include <QToolButton>
#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DToolButton : public QToolButton
{
    Q_OBJECT
public:
    DToolButton(QWidget *parent = nullptr);
    void setAlignment(Qt::Alignment flag);
    Qt::Alignment alignment() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void initStyleOption(QStyleOptionToolButton *option) const;
    QSize sizeHint() const override;
};

DWIDGET_END_NAMESPACE

#endif // DTOOLBUTTON_H
