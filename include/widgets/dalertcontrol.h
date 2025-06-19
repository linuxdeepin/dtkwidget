// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DALERTCONTROL_H
#define DALERTCONTROL_H

#include <QObject>
#include <QPointer>

#include <DFloatingWidget>
#include <DToolTip>

DWIDGET_BEGIN_NAMESPACE
class DAlertControlPrivate;
class LIBDTKWIDGETSHARED_EXPORT DAlertControl : public QObject, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DAlertControl)
    D_DECLARE_PRIVATE(DAlertControl)
    Q_PROPERTY(bool alert READ isAlert WRITE setAlert NOTIFY alertChanged)
    Q_PROPERTY(QColor alertColor READ alertColor WRITE setAlertColor)

public:
    explicit DAlertControl(QWidget *target, QObject *parent = nullptr);
    ~DAlertControl() override;

    void setAlert(bool isAlert);
    bool isAlert() const;
    void setAlertColor(QColor c);
    QColor alertColor() const;
    QColor defaultAlertColor() const;
    // alert对齐方式
    void setAlertAlignment(Qt::Alignment alignment);
    Qt::Alignment alertAlignment() const;
    // 文本对齐方式
    void setMessageAlignment(Qt::Alignment alignment);
    Qt::Alignment messageAlignment() const;
    void showAlertMessage(const QString &text, int duration = 3000);
    void showAlertMessage(const QString &text, QWidget *follower, int duration = 3000);
    void hideAlertMessage();

Q_SIGNALS:
    void alertChanged(bool alert) const;

protected:
    DAlertControl(DAlertControlPrivate &d, QObject *parent);
    bool eventFilter(QObject *watched, QEvent *event) override;

};

DWIDGET_END_NAMESPACE
#endif // DALERTCONTROL_H
