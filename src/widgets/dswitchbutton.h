/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

