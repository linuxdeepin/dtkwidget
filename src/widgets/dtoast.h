// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <QScopedPointer>
#include <QFrame>
#include <QIcon>

#include <dtkwidget_global.h>
#include <DObject>

DWIDGET_BEGIN_NAMESPACE

class DToastPrivate;
class LIBDTKWIDGETSHARED_EXPORT D_DECL_DEPRECATED_X("Use DMessageManager") DToast : public QFrame, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(qreal duration READ duration WRITE setDuration)
public:
    explicit DToast(QWidget *parent = 0);
    ~DToast();

    QString text() const;
    QIcon icon() const;
    int duration() const;

Q_SIGNALS:
    void visibleChanged(bool isVisible);

public Q_SLOTS:
    void pop();
    void pack();
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

    void setText(QString text);
    void setIcon(QString icon);
    void setIcon(QIcon icon, QSize defaultSize = QSize(20, 20));
    void setDuration(int duration);

private:
    qreal opacity() const;
    void setOpacity(qreal);

    D_DECLARE_PRIVATE(DToast)
};


DWIDGET_END_NAMESPACE
