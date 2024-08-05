// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DTOOLBUTTON_H
#define DTOOLBUTTON_H

#include <dtkwidget_global.h>

#include <QToolButton>

#include <DDciIcon>
#include <DDciIconPlayer>

DWIDGET_BEGIN_NAMESPACE
DGUI_USE_NAMESPACE

class DToolButtonPrivate;
class LIBDTKWIDGETSHARED_EXPORT DToolButton : public QToolButton, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    DToolButton(QWidget *parent = nullptr);
    void setAlignment(Qt::Alignment flag);
    Qt::Alignment alignment() const;
    void setDciIcon(const DDciIcon &dciIcon);

protected:
    void paintEvent(QPaintEvent *event) override;
    void initStyleOption(QStyleOptionToolButton *option) const;
    QSize sizeHint() const override;
    bool event(QEvent *e) override;

private:
    D_DECLARE_PRIVATE(DToolButton)
};

DWIDGET_END_NAMESPACE

#endif // DTOOLBUTTON_H
