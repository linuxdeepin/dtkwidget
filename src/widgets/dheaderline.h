// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DHEADERLINE_H
#define DHEADERLINE_H

#include <QWidget>
#include <QLabel>

#include <dtkwidget_global.h>
#include <dbaseline.h>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DHeaderLine : public DBaseLine
{
    Q_OBJECT
public:
    explicit DHeaderLine(QWidget *parent = 0);
    void setTitle(const QString &title);
    void setContent(QWidget *content);

    QString title() const;

private:
    void setLeftContent(QWidget *content);
    void setRightContent(QWidget *content);

private:
    QLabel *m_titleLabel = NULL;
};

DWIDGET_END_NAMESPACE

#endif // DHEADERLINE_H
