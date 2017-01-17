/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DDIALOG_P_H
#define DDIALOG_P_H

#include <QPointer>
#include <QPushButton>
#include "ddialog.h"
#include "dabstractdialogprivate_p.h"

QT_BEGIN_NAMESPACE
class QHBoxLayout;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DVBoxWidget;
class DHBoxWidget;

class DialogButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(int buttonType READ buttonType WRITE setButtonType NOTIFY buttonTypeChanged)

public:
    explicit DialogButton(const QString &text, QWidget *parent = 0);
    int buttonType() const;

public slots:
    void setButtonType(int buttonType);

signals:
    void buttonTypeChanged(int buttonType);

private:
    int m_buttonType;
};

class DDialogPrivate : public DAbstractDialogPrivate
{
protected:
    DDialogPrivate(DDialog *qq);

    QBoxLayout *getContentLayout();

private:
    bool onButtonClickedClose = true;
    QIcon icon;
    QString title;
    QString message;
    Qt::TextFormat textFormat;

    QLabel* iconLabel;
    QLabel* messageLabel;
    QLabel* titleLabel;

    QPushButton* closeButton = Q_NULLPTR;
    QHBoxLayout *iconLayout;
    QVBoxLayout *contentLayout;
    QHBoxLayout *buttonLayout;
    const QScreen *targetScreen = nullptr;

    QList<QAbstractButton*> buttonList;
    QList<QWidget*> contentList;

    QPointer<QAbstractButton> defaultButton;

    int clickedButtonIndex;

    QStyle *fixedStyle;

    void init();
    const QScreen *getScreen() const;
    QString trimTag(QString origin) const;
    QMap<int, QString> scanTags(const QString origin) const;
    QString elideString(QString str, const QFontMetrics &fm, int width) const;

    void updateSize();

    void _q_onButtonClicked();
    void _q_defaultButtonTriggered();

    Q_DECLARE_PUBLIC(DDialog)
};

DWIDGET_END_NAMESPACE

#endif // DDIALOG_P_H

