// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSPINBOX_H
#define DSPINBOX_H

#include <dtkwidget_global.h>
#include <DObject>

#include <QSpinBox>

DWIDGET_BEGIN_NAMESPACE

class DSpinBoxPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSpinBox : public QSpinBox, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool alert READ isAlert WRITE setAlert NOTIFY alertChanged)
    Q_PROPERTY(int defaultValue READ defaultValue WRITE setDefaultValue NOTIFY defaultValueChanged)

public:
    explicit DSpinBox(QWidget *parent = nullptr);

    QLineEdit *lineEdit() const;

    bool isAlert() const;
    void showAlertMessage(const QString &text, int duration = 3000);
    void showAlertMessage(const QString &text, QWidget *follower, int duration = 3000);
    D_DECL_DEPRECATED int defaultValue() const;

    void setEnabledEmbedStyle(bool enabled);

public Q_SLOTS:
    void setAlert(bool alert);
    D_DECL_DEPRECATED void setDefaultValue(int defaultValue);

Q_SIGNALS:
    void alertChanged(bool alert);
    D_DECL_DEPRECATED void defaultValueChanged(int defaultValue);

private:
    D_DECLARE_PRIVATE(DSpinBox)
};

class DDoubleSpinBoxPrivate;
class LIBDTKWIDGETSHARED_EXPORT DDoubleSpinBox : public QDoubleSpinBox, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool alert READ isAlert WRITE setAlert NOTIFY alertChanged)
    Q_PROPERTY(double defaultValue READ defaultValue WRITE setDefaultValue NOTIFY defaultValueChanged)

public:
    explicit DDoubleSpinBox(QWidget *parent = nullptr);

    bool isAlert() const;
    void showAlertMessage(const QString &text, int duration = 3000);
    void showAlertMessage(const QString &text, QWidget *follower, int duration = 3000);
    D_DECL_DEPRECATED double defaultValue() const;

    QLineEdit *lineEdit() const;
    void setEnabledEmbedStyle(bool enabled);

public Q_SLOTS:
    void setAlert(bool alert);
    D_DECL_DEPRECATED void setDefaultValue(double defaultValue);

Q_SIGNALS:
    void alertChanged(bool alert);
    D_DECL_DEPRECATED void defaultValueChanged(double defaultValue);

private:
    D_DECLARE_PRIVATE(DDoubleSpinBox)
};

DWIDGET_END_NAMESPACE

#endif // DSPINBOX_H
