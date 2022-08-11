// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSEARCHEDIT_H
#define DSEARCHEDIT_H

#include <DLineEdit>

DWIDGET_BEGIN_NAMESPACE

class DSearchEditPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSearchEdit : public DLineEdit
{
    Q_OBJECT
    Q_PROPERTY(bool voiceInput READ isVoiceInput NOTIFY voiceChanged)

public:
    explicit DSearchEdit(QWidget *parent = nullptr);
    ~DSearchEdit();

    void setPlaceHolder(QString placeHolder);
    QString placeHolder() const;

    void clear();
    void clearEdit();

    bool isVoiceInput() const;

    void setPlaceholderText(const QString &text);
    QString placeholderText() const;

Q_SIGNALS:
    void voiceInputFinished();
    void searchAborted();
    void voiceChanged();

protected:
    Q_DISABLE_COPY(DSearchEdit)
    D_DECLARE_PRIVATE(DSearchEdit)
    Q_PRIVATE_SLOT(d_func(), void _q_toEditMode(bool))
    D_PRIVATE_SLOT(void _q_onVoiceActionTrigger(bool))
    D_PRIVATE_SLOT(void _q_clearFocus())
};

DWIDGET_END_NAMESPACE

#endif // DSEARCHEDIT_H
