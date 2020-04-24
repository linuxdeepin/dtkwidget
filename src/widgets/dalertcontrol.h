/*
 * Copyright (C) 2020 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     ck <chenke@uniontech.com>
 *
 * Maintainer: ck <chenke@uniontech.com>
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
