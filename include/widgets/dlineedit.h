// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DLINEEDIT_H
#define DLINEEDIT_H

#include <dobject.h>
#include <dtkwidget_global.h>

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>

DWIDGET_BEGIN_NAMESPACE

class DLineEditPrivate;
class DStyleOptionLineEdit;
class LIBDTKWIDGETSHARED_EXPORT DLineEdit : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DLineEdit)
    D_DECLARE_PRIVATE(DLineEdit)
    Q_PROPERTY(bool alert READ isAlert WRITE setAlert NOTIFY alertChanged)

public:
    DLineEdit(QWidget *parent = nullptr);
    virtual ~DLineEdit() override;

    QLineEdit *lineEdit() const;
    void setPlaceholderText(const QString &);

    void setAlert(bool isAlert);
    bool isAlert() const;
    void showAlertMessage(const QString &text, int duration = 3000);
    void showAlertMessage(const QString &text, QWidget *follower, int duration = 3000);
    void setAlertMessageAlignment(Qt::Alignment alignment);
    Qt::Alignment alertMessageAlignment() const;
    void hideAlertMessage();

    void setLeftWidgets(const QList<QWidget *> &list);
    void setRightWidgets(const QList<QWidget *> &list);

    void setLeftWidgetsVisible(bool visible);
    void setRightWidgetsVisible(bool visible);

    void setClearButtonEnabled(bool enable);
    bool isClearButtonEnabled() const;

    void setText(const QString &text);
    QString text();

    void clear();

    QLineEdit::EchoMode echoMode() const;
    void setEchoMode(QLineEdit::EchoMode mode);

    void setContextMenuPolicy(Qt::ContextMenuPolicy policy);

    bool speechToTextIsEnabled() const;
    void setSpeechToTextEnabled(bool enable);

    bool textToSpeechIsEnabled() const;
    void setTextToSpeechEnabled(bool enable);

    bool textToTranslateIsEnabled() const;
    void setTextToTranslateEnabled(bool enable);

    bool copyEnabled() const;
    void setCopyEnabled(bool enable);

    bool cutEnabled() const;
    void setCutEnabled(bool enable);

    bool pasteEnabled() const;
    void setPasteEnabled(bool enable);

Q_SIGNALS:
    void alertChanged(bool alert) const;
    void focusChanged(bool onFocus) const;

    void textChanged(const QString &);
    void textEdited(const QString &);
    void cursorPositionChanged(int, int);
    void returnPressed();
    void editingFinished();
    void selectionChanged();

protected:
    DLineEdit(DLineEditPrivate &q, QWidget *parent);
    bool eventFilter(QObject *watched, QEvent *event) override;
    bool event(QEvent *event) override;

    friend class DStyleOptionLineEdit;
};

DWIDGET_END_NAMESPACE

#endif // DLINEEDIT_H
