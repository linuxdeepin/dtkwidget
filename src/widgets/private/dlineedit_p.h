// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DLINEEDIT_P_H
#define DLINEEDIT_P_H

#include <DLineEdit>
#include <DAlertControl>

#include <DObjectPrivate>
#include <QPointer>
#include <DFloatingWidget>

DWIDGET_BEGIN_NAMESPACE

class DToolTip;
class DLineEditPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DLineEdit)

public:
    DLineEditPrivate(DLineEdit *q);
    void updateTooltipPos();

    void init();

    DAlertControl *control{nullptr};

    QWidget *leftWidget;
    QWidget *rightWidget;

    QLineEdit *lineEdit;
    QHBoxLayout *hLayout;

    bool bEnableCut = true;
    bool bEnableCopy = true;
    bool bSpeechToText = true;
    bool bTextToSpeech = true;
    bool bTextToTranslate = true;
};

DWIDGET_END_NAMESPACE

#endif // DLINEEDIT_P_H

