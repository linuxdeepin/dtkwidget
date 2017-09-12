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

#ifndef DSWITCHLINEEXPAND_H
#define DSWITCHLINEEXPAND_H

#include <QWidget>

#include "dtkwidget_global.h"
#include "dbaseexpand.h"
#include "dswitchbutton.h"
#include "dheaderline.h"

DWIDGET_BEGIN_NAMESPACE

class SwitchHeaderLine : public DHeaderLine
{
    Q_OBJECT
public:
    SwitchHeaderLine(QWidget *parent = 0);
    void setExpand(bool value);

Q_SIGNALS:
    void checkedChanged(bool arg);

protected:
    void mousePressEvent(QMouseEvent *);

private:
    void reverseArrowDirection();
    DSwitchButton *m_switchButton = NULL;

};

class LIBDTKWIDGETSHARED_EXPORT DSwitchLineExpand : public DBaseExpand
{
    Q_OBJECT
public:
    explicit DSwitchLineExpand(QWidget *parent = 0);
    void setTitle(const QString &title);
    void setExpand(bool value);

    DBaseLine *header();

private:
    void setHeader(QWidget *header);
    void resizeEvent(QResizeEvent *e);

private:
    SwitchHeaderLine *m_headerLine = NULL;
};

DWIDGET_END_NAMESPACE

#endif // DSWITCHLINEEXPAND_H
