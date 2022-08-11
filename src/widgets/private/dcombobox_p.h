// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DCOMBOBOX_P_H
#define DCOMBOBOX_P_H

#include "dcombobox.h"
#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DComboBoxPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DComboBox)
public:
    explicit DComboBoxPrivate(DComboBox* q);

    void init();

    // 重写 QComboBoxPrivate类的popupGeometry
    QRect popupGeometry();

    // 重写 QComboBoxPrivate类的computeWidthHint
    int computeWidthHint() const;

    // 最大显示项数
    static const int MaxVisibleItems = 16;
};

DWIDGET_END_NAMESPACE

#endif // DCOMBOBOX_P_H

