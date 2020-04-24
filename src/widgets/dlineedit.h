/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
