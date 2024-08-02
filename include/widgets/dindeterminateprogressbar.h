// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DINDETERMINATEPROGRESSBAR_H
#define DINDETERMINATEPROGRESSBAR_H

#include <DObject>
#include <QWidget>

class DIndeterminateProgressbarPrivate;
class DIndeterminateProgressbar : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DIndeterminateProgressbar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:
    D_DECLARE_PRIVATE(DIndeterminateProgressbar)
};

#endif // DINDETERMINATEPROGRESSBAR_H
