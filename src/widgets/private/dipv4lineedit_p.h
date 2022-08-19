// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DIPV4LINEEDIT_P_H
#define DIPV4LINEEDIT_P_H

#include <DIpv4LineEdit>
#include <DImageButton>
#include <DLineEdit>
#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DIpv4LineEditPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DIpv4LineEdit)

public:
    DIpv4LineEditPrivate(DIpv4LineEdit *parent);

    void init();

    QLabel *getLabel();
    QLineEdit *getEdit();

    void setFocus(bool focus);

    void _q_updateLineEditText();
    void _q_setIpLineEditText(const QString &text);

    QList<QLineEdit*> editList;
    QWidget *editMainWidget;

    bool focus = false;
};

DWIDGET_END_NAMESPACE

#endif // DIPV4LINEEDIT_P_H

