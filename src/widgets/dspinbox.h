/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DSPINBOX_H
#define DSPINBOX_H

#include <QSpinBox>

#include "dtkwidget_global.h"
#include "dobject.h"

DWIDGET_BEGIN_NAMESPACE

class DSpinBoxPrivate;
class LIBDTKWIDGETSHARED_EXPORT DSpinBox : public QSpinBox, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool alert READ isAlert WRITE setAlert NOTIFY alertChanged)
    Q_PROPERTY(int defaultValue READ defaultValue WRITE setDefaultValue NOTIFY defaultValueChanged)

public:
    explicit DSpinBox(QWidget *parent = 0);

    QLineEdit *lineEdit() const;

    bool isAlert() const;
    int defaultValue() const;

public Q_SLOTS:
    void setAlert(bool alert);
    void setDefaultValue(int defaultValue);

Q_SIGNALS:
    void alertChanged(bool alert);
    void defaultValueChanged(int defaultValue);

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DSpinBox)
};

class DDoubleSpinBoxPrivate;
class DDoubleSpinBox : public QDoubleSpinBox, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool alert READ isAlert WRITE setAlert NOTIFY alertChanged)
    Q_PROPERTY(double defaultValue READ defaultValue WRITE setDefaultValue NOTIFY defaultValueChanged)

public:
    explicit DDoubleSpinBox(QWidget *parent = 0);

    bool isAlert() const;
    double defaultValue() const;

public Q_SLOTS:
    void setAlert(bool alert);
    void setDefaultValue(double defaultValue);

Q_SIGNALS:
    void alertChanged(bool alert);
    void defaultValueChanged(double defaultValue);

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DDoubleSpinBox)
    double m_defaultValue;
};

DWIDGET_END_NAMESPACE

#endif // DSPINBOX_H
