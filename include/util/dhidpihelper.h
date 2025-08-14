// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DHIDPIHELPER_H
#define DHIDPIHELPER_H

#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE

class D_DECL_DEPRECATED DHiDPIHelper
{
public:
    D_DECL_DEPRECATED_X("Use Dtk::Gui::DIcon::loadNxPixmap") static QPixmap loadNxPixmap(const QString &fileName);
};

DWIDGET_END_NAMESPACE

#endif // DHIDPIHELPER_H
