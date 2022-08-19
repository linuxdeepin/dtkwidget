// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DACCESSIBILITYCHECKER_H
#define DACCESSIBILITYCHECKER_H

#include "dtkwidget_global.h"

#include <DObject>

DWIDGET_BEGIN_NAMESPACE

class DAccessibilityCheckerPrivate;
class LIBDTKWIDGETSHARED_EXPORT DAccessibilityChecker : public QObject, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DAccessibilityChecker)
    Q_PROPERTY(OutputFormat outputFormat READ outputFormat WRITE setOutputFormat)

public:
    enum OutputFormat {
       AssertFormat,
       FullFormat
    };

    enum Role {
        Widget,
        ViewItem
    };

    explicit DAccessibilityChecker(QObject *parent = nullptr);

    void setOutputFormat(OutputFormat format);
    OutputFormat outputFormat() const;

    bool check();
    void start(int msec = 3000);

protected:
    virtual bool isIgnore(Role role, const QWidget *w);

private:
    D_PRIVATE_SLOT(void _q_checkTimeout())
};

DWIDGET_END_NAMESPACE
#endif // DACCESSIBILITYCHECKER_H
