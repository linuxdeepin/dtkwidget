/*
 * Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DICONBUTTON_H
#define DICONBUTTON_H

#include <dtkwidget_global.h>
#include <DObject>
#include <DStyle>

#include <QAbstractButton>

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
    explicit DIconButton(QStyle::StandardPixmap iconType = static_cast<QStyle::StandardPixmap>(-1), QWidget *parent = nullptr);
    explicit DIconButton(DStyle::StandardPixmap iconType = static_cast<DStyle::StandardPixmap>(-1), QWidget *parent = nullptr);
    ~DIconButton() override;

    void setIcon(const QIcon &icon);
    void setIcon(QStyle::StandardPixmap iconType);
    void setIcon(DStyle::StandardPixmap iconType);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    QSize iconSize() const;

    bool isFlat() const;

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
