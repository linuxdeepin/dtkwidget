// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSEARCHCOMBOBOX_H
#define DSEARCHCOMBOBOX_H

#include <dobject.h>
#include <dtkwidget_global.h>

#include <DComboBox>

DWIDGET_BEGIN_NAMESPACE

class DSearchComboBoxPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSearchComboBox : public DComboBox
{
    Q_OBJECT
    Q_DISABLE_COPY(DSearchComboBox)
    D_DECLARE_PRIVATE(DSearchComboBox)
public:
    explicit DSearchComboBox(QWidget *parent = nullptr);
    void setEditable(bool editable);

protected:
    void showPopup() override;
};

DWIDGET_END_NAMESPACE

#endif // DSEARCHCOMBOBOX_H
