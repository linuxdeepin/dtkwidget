/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     wangpeng <wangpeng@uniontech.com>
*
* Maintainer: wangpeng <wangpeng@uniontech.com>
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

protected:
    void paintEvent(QPaintEvent *event) override;
    void initStyleOption(QStyleOptionToolButton *option) const;
    QSize sizeHint() const override;
};

DWIDGET_END_NAMESPACE

#endif // DTOOLBUTTON_H
