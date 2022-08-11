// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DICONBUTTON_H
#define DICONBUTTON_H

#include <dtkwidget_global.h>
#include <DObject>
#include <DStyle>
#include <DDciIcon>

#include <QAbstractButton>

DGUI_BEGIN_NAMESPACE
class DDciIcon;
DGUI_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DIconButtonPrivate;
class DStyleOptionButton;
class DIconButton : public QAbstractButton, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DIconButton)

    Q_PROPERTY(bool flat READ isFlat WRITE setFlat)

public:
    explicit DIconButton(QWidget *parent = nullptr);
    explicit DIconButton(QStyle::StandardPixmap iconType, QWidget *parent = nullptr);
    explicit DIconButton(DStyle::StandardPixmap iconType, QWidget *parent = nullptr);
    explicit DIconButton(const DDciIcon &dciIcon, QWidget *parent = nullptr);
    ~DIconButton() override;

    void setIcon(const QIcon &icon);
    void setIcon(QStyle::StandardPixmap iconType);
    void setIcon(DStyle::StandardPixmap iconType);
    void setIcon(const DDciIcon &icon);

    DDciIcon dciIcon() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    QSize iconSize() const;

    bool isFlat() const;

    void setEnabledCircle(bool status);
    bool enabledCircle() const;
    void setNewNotification(const bool set_new);

public Q_SLOTS:
    void setFlat(bool flat);

protected:
    using QAbstractButton::setText;
    using QAbstractButton::text;

    DIconButton(DIconButtonPrivate &dd, QWidget *parent = nullptr);
    virtual DStyleOptionButton baseStyleOption() const;
    virtual void initStyleOption(DStyleOptionButton *option) const;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *e) override;
};

DWIDGET_END_NAMESPACE

#endif // DICONBUTTON_H
