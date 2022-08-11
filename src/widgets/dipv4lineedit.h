// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DIPV4LINEEDIT_H
#define DIPV4LINEEDIT_H

#include <dtkwidget_global.h>
#include <DObject>

#include <QLineEdit>

DWIDGET_BEGIN_NAMESPACE

class DIpv4LineEditPrivate;
class LIBDTKWIDGETSHARED_EXPORT DIpv4LineEdit : public QLineEdit, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_DISABLE_COPY(DIpv4LineEdit)
    D_DECLARE_PRIVATE(DIpv4LineEdit)
    Q_PROPERTY(QString displayText READ displayText)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition)
    Q_PROPERTY(Qt::Alignment alignment READ alignment)
    Q_PROPERTY(QString selectedText READ selectedText)
    Q_PROPERTY(bool acceptableInput READ hasAcceptableInput)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(QString placeholderText READ placeholderText)
    Q_PROPERTY(bool clearButtonEnabled READ isClearButtonEnabled)

public:
    explicit DIpv4LineEdit(QWidget *parent = 0);

    QString displayText() const;
    int cursorPosition() const;
    Qt::Alignment alignment() const;
    bool hasAcceptableInput() const;
    bool isReadOnly() const;

public Q_SLOTS:
    void setCursorPosition(int cursorPosition);
    void setReadOnly(bool readOnly);
    void setSelection(int start, int length);
    void selectAll();

Q_SIGNALS:
    void focusChanged(bool focus);

protected:
    bool eventFilter(QObject *obj, QEvent *e) Q_DECL_OVERRIDE;

private:
    DIpv4LineEdit(DIpv4LineEditPrivate &q, QWidget *parent);
    void setPlaceholderText(QString placeholderText);
    void setClearButtonEnabled(bool clearButtonEnabled);

    Q_PRIVATE_SLOT(d_func(), void _q_updateLineEditText())
    Q_PRIVATE_SLOT(d_func(), void _q_setIpLineEditText(const QString &))

protected:
    void resizeEvent(QResizeEvent *event) override;
};

DWIDGET_END_NAMESPACE

#endif // DIPV4LINEEDIT_H
