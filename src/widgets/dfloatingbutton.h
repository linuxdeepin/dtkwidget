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
#ifndef DFLOATINGBUTTON_H
#define DFLOATINGBUTTON_H

#include <dtkwidget_global.h>
#include <DObject>

#include <QAbstractButton>

DWIDGET_BEGIN_NAMESPACE

class DStyleOptionButton;
class DFloatingButtonPrivate;
class DFloatingButton : public QAbstractButton, DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DFloatingButton)

    Q_PROPERTY(bool flat READ isFlat WRITE setFlat)

public:
    explicit DFloatingButton(QWidget *parent = nullptr);
    explicit DFloatingButton(const QString &text, QWidget *parent = nullptr);
    DFloatingButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    bool isFlat() const;

public Q_SLOTS:
    void setFlat(bool flat);

protected:
    void initStyleOption(DStyleOptionButton *option) const;

private:
    void paintEvent(QPaintEvent *event) override;
};

DWIDGET_END_NAMESPACE

#endif // DFLOATINGBUTTON_H
