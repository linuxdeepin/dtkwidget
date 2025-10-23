// SPDX-FileCopyrightText: 2015 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPASSWORDEDIT_H
#define DPASSWORDEDIT_H

#include <DLineEdit>

DWIDGET_BEGIN_NAMESPACE

class DPasswordEditPrivate;
class LIBDTKWIDGETSHARED_EXPORT DPasswordEdit : public DLineEdit
{
    Q_OBJECT
    Q_PROPERTY(bool isEchoMode READ isEchoMode NOTIFY echoModeChanged)

public:
    DPasswordEdit(QWidget *parent = nullptr);

    bool isEchoMode() const;
    void setEchoMode(QLineEdit::EchoMode mode);

    void setEchoButtonIsVisible(bool visible);
    bool echoButtonIsVisible () const;

Q_SIGNALS:
    void echoModeChanged(bool echoOn);

protected:
    Q_DISABLE_COPY(DPasswordEdit)
    D_DECLARE_PRIVATE(DPasswordEdit)
    Q_PRIVATE_SLOT(d_func(), void _q_toggleEchoMode())

    void changeEvent(QEvent *event) override;
    bool eventFilter(QObject* watcher, QEvent* event) override;
};

DWIDGET_END_NAMESPACE

#endif // DPASSWORDEDIT_H
