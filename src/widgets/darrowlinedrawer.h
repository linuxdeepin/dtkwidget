// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DARROWLINEDRAWER_H
#define DARROWLINEDRAWER_H

#include <DDrawer>

DWIDGET_BEGIN_NAMESPACE

class DBaseLine;
class DArrowLineDrawerPrivate;
class LIBDTKWIDGETSHARED_EXPORT DArrowLineDrawer : public DDrawer
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DArrowLineDrawer)

public:
    explicit DArrowLineDrawer(QWidget *parent = nullptr);
    void setTitle(const QString &title);
    void setExpand(bool value);
    D_DECL_DEPRECATED DBaseLine *headerLine();

private:
    void setHeader(QWidget *header);
    void resizeEvent(QResizeEvent *e);
};

DWIDGET_END_NAMESPACE

#endif // DARROWLINEDRAWER_H
