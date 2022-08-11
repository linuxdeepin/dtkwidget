// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSWITCHLINEEXPAND_H
#define DSWITCHLINEEXPAND_H

#include <QWidget>

#include <dtkwidget_global.h>
#include <dbaseexpand.h>
#include <DSwitchButton>
#include <dheaderline.h>

DWIDGET_BEGIN_NAMESPACE

class DSwitchHeaderLine : public DHeaderLine
{
    Q_OBJECT
public:
    DSwitchHeaderLine(QWidget *parent = 0);
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
    DSwitchHeaderLine *m_headerLine = NULL;
};

DWIDGET_END_NAMESPACE

#endif // DSWITCHLINEEXPAND_H
