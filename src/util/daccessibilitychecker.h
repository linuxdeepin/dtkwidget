/*
 * Copyright (C) 2021 ~ 2021 Uniontech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
