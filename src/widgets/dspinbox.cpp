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

#include <QResizeEvent>
#include <QHBoxLayout>
#include <QDebug>

#include "dspinbox.h"
#include "private/dspinbox_p.h"

DWIDGET_BEGIN_NAMESPACE

DSpinBoxPrivate::DSpinBoxPrivate(DSpinBox *parent) :
    DObjectPrivate(parent)
{

}

void DSpinBoxPrivate::init()
{
    D_Q(DSpinBox);


}

/*!
 * \~english \class DSpinBox
 * \~english \brief The DSpinBox class provides deepin style QSpinBox.
 *
 * \~english Like DLineEdit, this widget can be set on alert to warn the user that the
 * \~english input is not correct. In addition, there's a DSpinBox::defaultValue property
 * \~english can be used to set a default value on the widget.
 */

/*!
 * \~chinese \class DSpinBox
 * \~chinese \brief Deepin风格的QSpinBox。
 *
 * \~chinese 类似 DLineEdit ，这个控件也提供了警告功能，用于提醒用户当前输入的数据不正确，另外，还可以设置一个默认的值，并在默认值发生改变时发出信号。
 *
 * \~chinese 如下图上面的是正常状态的控件, 下面的是处于警告状态的控件:
 *
 * \~chinese \image html DSpinBox.png
 */

// ========================SIGNAL START========================
/**
 * \~chinese \fn DSpinBox::alertChanged
 * \~chinese \brief 警告状态发生了变化。
 * \~chinese \param alert 当前的警告状态。
 */

/**
 * \~chinese \fn DSpinBox::defaultValueChanged
 * \~chinese \brief 默认值发生了变化。
 * \~chinese \param defaultValue 控件的默认值。
 */
// ========================SIGNAL END========================

/*!
 * \~english \brief DSpinBox::DSpinBox constructs an instance of DSpinBox
 * \~english \param parent is passed to QSpinBox constructor
 */

/*!
 * \~chinese \brief 构造一个 DSpinBox 实例。
 * \~chinese \param parent 作为该实例的父控件。
 */
DSpinBox::DSpinBox(QWidget *parent) :
    QSpinBox(parent),
    DObject(*new DSpinBoxPrivate(this))
{
    d_func()->init();
}

/*!
 * \~english \brief DSpinBox::lineEdit
 * \~english \return the QLineEdit used by this spin box.
 */

/*!
 * \~chinese \brief 获取输入框控件。
 * \~chinese \return 返回正在使用的输入库控件对象。
 */
QLineEdit *DSpinBox::lineEdit() const
{
    return QSpinBox::lineEdit();
}

/*!
 * \~english \property DSpinBox::alert
 * \~english \brief This property holds whether the widget on alert mode.
 */

/*!
 * \~chinese \property DSpinBox::alert
 * \~chinese \brief 表示当前控件是否处于警告状态的属性。
 *
 * \~chinese 使用 DSpinBox::isAlert 获取属性当前状态，使用 DSpinBox::setAlert 设置属性的状态。
 */
bool DSpinBox::isAlert() const
{
    D_DC(DSpinBox);

    return d->alert;
}

/*!
 * \~english \property DSpinBox::defaultValue
 * \~english \brief This property holds the default value of this spin box.
 */

/*!
 * \~chinese \property DSpinBox::defaultValue
 * \~chinese \brief 这个属性的值是控件的默认值。
 *
 * \~chinese 使用 DSpinBox::defaultValue 获取默认值，使用 DSpinBox::setDefaultValue 设置默认值, 点击控件上的 reset 按钮会设置为此值。
 */
int DSpinBox::defaultValue() const
{
    return d_func()->defaultValue;
}

void DSpinBox::setAlert(bool alert)
{
    D_D(DSpinBox);

    if(alert == d->alert)
        return;

    d->alert = alert;

    Q_EMIT alertChanged(alert);
}

void DSpinBox::setDefaultValue(int defaultValue)
{
    D_D(DSpinBox);

    if (d->defaultValue == defaultValue)
        return;

    d->defaultValue = defaultValue;

    Q_EMIT defaultValueChanged(defaultValue);
}

DDoubleSpinBoxPrivate::DDoubleSpinBoxPrivate(DDoubleSpinBox *parent) :
    DObjectPrivate(parent)
{

}

void DDoubleSpinBoxPrivate::init()
{
    D_Q(DDoubleSpinBox);


}

/*!
 * \~chinese \class DDoubleSpinBox
 * \~chinese \brief 类似 DDoubleSpinBox 。
 */

DDoubleSpinBox::DDoubleSpinBox(QWidget *parent) :
    QDoubleSpinBox(parent),
    DObject(*new DDoubleSpinBoxPrivate(this))
{
    d_func()->init();
}

bool DDoubleSpinBox::isAlert() const
{
    D_DC(DDoubleSpinBox);

    return d->alert;
}

double DDoubleSpinBox::defaultValue() const
{
    return d_func()->defaultValue;
}

void DDoubleSpinBox::setAlert(bool alert)
{
    D_D(DDoubleSpinBox);

    if(alert == d->alert)
        return;

    d->alert = alert;

    Q_EMIT alertChanged(alert);
}

void DDoubleSpinBox::setDefaultValue(double defaultValue)
{
    D_D(DDoubleSpinBox);

    if (d->defaultValue == defaultValue)
        return;

    d->defaultValue = defaultValue;

    Q_EMIT defaultValueChanged(defaultValue);
}

DWIDGET_END_NAMESPACE
