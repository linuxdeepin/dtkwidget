// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DKEYSEQUENCEEDIT_P_H
#define DKEYSEQUENCEEDIT_P_H

#include <DKeySequenceEdit>

#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DKeyWidget;
class DKeySequenceEditPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DKeySequenceEdit)
public:
    DKeySequenceEditPrivate(DKeySequenceEdit *q);

    void init();
    QString replaceWriting(QString writing);
private:
    DKeyWidget *rightWidget;
    QKeySequence sequencekey;
    QMap<QString , QString> copywritingList;
};

DWIDGET_END_NAMESPACE

#endif // DKEYSEQUENCEEDIT_P_H
