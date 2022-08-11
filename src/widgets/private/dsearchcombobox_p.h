// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSEARCHCOMBOBOX_P_H
#define DSEARCHCOMBOBOX_P_H

#include "dsearchcombobox.h"
#include "private/dcombobox_p.h"
#include <DObjectPrivate>
#include <DSearchEdit>
#include <QSortFilterProxyModel>


DWIDGET_BEGIN_NAMESPACE

class DSearchComboBox;
class DSearchComboBoxPrivate : public DComboBoxPrivate
{
    Q_DECLARE_PUBLIC(DSearchComboBox)
public:
    explicit DSearchComboBoxPrivate(DSearchComboBox *q);

private:
    DSearchEdit *searchEdit;
    QCompleter *completer;
    QSortFilterProxyModel *proxyModel;
    int height;
};

DWIDGET_END_NAMESPACE

#endif // DSEARCHCOMBOBOX_P_H
