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
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    Q_PROPERTY(int defaultValue READ defaultValue WRITE setDefaultValue NOTIFY defaultValueChanged)
#endif
    
public:
    explicit DSpinBox(QWidget *parent = nullptr);

    QLineEdit *lineEdit() const;

    bool isAlert() const;
    void showAlertMessage(const QString &text, int duration = 3000);
    void showAlertMessage(const QString &text, QWidget *follower, int duration = 3000);
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    D_DECL_DEPRECATED int defaultValue() const;
#endif

    void setEnabledEmbedStyle(bool enabled);

public Q_SLOTS:
    void setAlert(bool alert);
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    D_DECL_DEPRECATED void setDefaultValue(int defaultValue);
#endif

Q_SIGNALS:
    void alertChanged(bool alert);
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    D_DECL_DEPRECATED void defaultValueChanged(int defaultValue);
#endif

private:
    D_DECLARE_PRIVATE(DSpinBox)
};

class DDoubleSpinBoxPrivate;
class LIBDTKWIDGETSHARED_EXPORT DDoubleSpinBox : public QDoubleSpinBox, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool alert READ isAlert WRITE setAlert NOTIFY alertChanged)
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    Q_PROPERTY(double defaultValue READ defaultValue WRITE setDefaultValue NOTIFY defaultValueChanged)
#endif

public:
    explicit DDoubleSpinBox(QWidget *parent = nullptr);

    bool isAlert() const;
    void showAlertMessage(const QString &text, int duration = 3000);
    void showAlertMessage(const QString &text, QWidget *follower, int duration = 3000);
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    D_DECL_DEPRECATED double defaultValue() const;
#endif

    QLineEdit *lineEdit() const;
    void setEnabledEmbedStyle(bool enabled);

public Q_SLOTS:
    void setAlert(bool alert);
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    D_DECL_DEPRECATED void setDefaultValue(double defaultValue);
#endif

Q_SIGNALS:
    void alertChanged(bool alert);
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    D_DECL_DEPRECATED void defaultValueChanged(double defaultValue);
#endif

private:
    D_DECLARE_PRIVATE(DDoubleSpinBox)
};

DWIDGET_END_NAMESPACE

#endif // DSPINBOX_H
