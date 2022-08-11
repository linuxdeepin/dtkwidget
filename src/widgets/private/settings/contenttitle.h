// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <QScopedPointer>
#include <QLabel>

class ContentTitlePrivate;
class ContentTitle : public QWidget
{
    Q_OBJECT
public:
    explicit ContentTitle(QWidget *parent = 0);
    ~ContentTitle();

    void setSpacing(int width);
    QLabel *label() const;

public Q_SLOTS:
    void setTitle(const QString &title);

private:
    QScopedPointer<ContentTitlePrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), ContentTitle)
};

