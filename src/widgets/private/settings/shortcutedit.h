// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <QScopedPointer>
#include <QWidget>

class ShortcutEditPrivate;
class ShortcutEdit : public QWidget
{
    Q_OBJECT
public:
    explicit ShortcutEdit(QWidget *parent = 0);
    ~ShortcutEdit();

    void setShortCut(Qt::KeyboardModifiers modifier, Qt::Key key);
    void setShortCut(const QString &sequenceString);

Q_SIGNALS:
    // WARINNG: just connect one of this on one time
    // these two signal will send at them same time
    void shortcutChanged(Qt::KeyboardModifiers modifier, Qt::Key key);
    void shortcutStringChanged(const QString &sequenceString);

protected:
    virtual void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    virtual void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;
    virtual void focusOutEvent(QFocusEvent *e) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void notifyShortcutChanged(Qt::KeyboardModifiers modifier, Qt::Key key);

private:
    QScopedPointer<ShortcutEditPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), ShortcutEdit)
};

