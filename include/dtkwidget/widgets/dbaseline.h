// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DBASELINE_H
#define DBASELINE_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include <dtkwidget_global.h>
#include <dconstants.h>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DBaseLine : public QLabel
{
    Q_OBJECT
public:
    explicit DBaseLine(QWidget *parent = 0);

    void setLeftContent(QWidget *content);
    void setRightContent(QWidget *content);

    QBoxLayout *leftLayout();
    QBoxLayout *rightLayout();

    void setLeftMargin(int margin);
    void setRightMargin(int margin);
    int leftMargin() const;
    int rightMargin() const;

private:
    QHBoxLayout *m_mainLayout = NULL;
    QHBoxLayout *m_leftLayout= NULL;
    QHBoxLayout *m_rightLayout = NULL;

    int m_leftMargin = 10;
    int m_rightMargin = HEADER_RIGHT_MARGIN;
};

DWIDGET_END_NAMESPACE

#endif // DBASELINE_H
