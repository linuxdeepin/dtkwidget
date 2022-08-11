// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DARROWLINEEXPAND_H
#define DARROWLINEEXPAND_H

#include <QWidget>

#include <dtkwidget_global.h>
#include <dbaseexpand.h>
#include <DIconButton>
#include <dbaseline.h>
#include <dheaderline.h>
DWIDGET_BEGIN_NAMESPACE

class D_DECL_DEPRECATED ArrowHeaderLine : public DHeaderLine
{
    Q_OBJECT
public:
    ArrowHeaderLine(QWidget *parent = nullptr);
    void setExpand(bool value);

Q_SIGNALS:
    void mousePress();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:
    void reverseArrowDirection();
    bool m_isExpanded = false;
    DIconButton *m_arrowButton = nullptr;
};

class LIBDTKWIDGETSHARED_EXPORT D_DECL_DEPRECATED_X("Use DArrowLineDrawer") DArrowLineExpand : public DBaseExpand
{
    Q_OBJECT
public:
    explicit DArrowLineExpand(QWidget *parent = nullptr);
    void setTitle(const QString &title);
    void setExpand(bool value);
    DBaseLine *headerLine();

private:
    void setHeader(QWidget *header);
    void resizeEvent(QResizeEvent *e);

private:
    ArrowHeaderLine *m_headerLine = nullptr;
};

DWIDGET_END_NAMESPACE

#endif // DARROWLINEEXPAND_H
