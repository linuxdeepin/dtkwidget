// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DCOMBOBOX_H
#define DCOMBOBOX_H

#include <dtkwidget_global.h>
#include <DObject>
#include <QComboBox>

DWIDGET_BEGIN_NAMESPACE

class DComboBoxPrivate;
class LIBDTKWIDGETSHARED_EXPORT DComboBox : public QComboBox, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DComboBox)

public:
    explicit DComboBox(QWidget *parent = nullptr);

protected:
    DComboBox(DComboBoxPrivate &dd, QWidget *parent);

    // QComboBox interface
public:
    virtual void showPopup() override;
};

DWIDGET_END_NAMESPACE

#endif // DCOMBOBOX_H
