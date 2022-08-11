// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DDIALOG_P_H
#define DDIALOG_P_H

#include <QPointer>
#include <QPushButton>
#include <QSpacerItem>
#include <DDialog>
#include "dabstractdialogprivate_p.h"

QT_BEGIN_NAMESPACE
class QHBoxLayout;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DIconButton;
class DTitlebar;
class DVBoxWidget;
class DHBoxWidget;

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

    QLabel* messageLabel;
    QLabel* titleLabel;

    DTitlebar   *titleBar = nullptr;
    QHBoxLayout *iconLayout;
    QVBoxLayout *contentLayout;
    QWidget *contentWidget = nullptr;
    QHBoxLayout *buttonLayout;
    const QScreen *targetScreen = nullptr;
    QSpacerItem *spacer = nullptr;

    QList<QAbstractButton*> buttonList;
    QList<QWidget*> contentList;

    QPointer<QAbstractButton> defaultButton;

    int clickedButtonIndex;

    void init();
    const QScreen *getScreen() const;
    QString trimTag(QString origin) const;
    QMap<int, QString> scanTags(const QString origin) const;
    QString elideString(QString str, const QFontMetrics &fm, int width) const;

    void updateSize();
    void setSpacer(int height);

    void _q_onButtonClicked();
    void _q_defaultButtonTriggered();

    Q_DECLARE_PUBLIC(DDialog)
};

DWIDGET_END_NAMESPACE

#endif // DDIALOG_P_H

