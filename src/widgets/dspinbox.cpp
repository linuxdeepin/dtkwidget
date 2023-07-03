// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QDebug>

#include "dspinbox.h"
#include "private/dspinbox_p.h"
#include "dlineedit.h"

DWIDGET_BEGIN_NAMESPACE

DSpinBoxPrivate::DSpinBoxPrivate(DSpinBox *parent) :
    DObjectPrivate(parent)
{

}

void DSpinBoxPrivate::init()
{
    D_Q(DSpinBox);

    lineEdit = new DLineEdit(q);
    lineEdit->setAccessibleName("DSpinBoxChildDLineEdit");
    lineEdit->setClearButtonEnabled(false);
    q->setLineEdit(lineEdit->lineEdit());
    q->connect(lineEdit, &DLineEdit::alertChanged, q, &DSpinBox::alertChanged);
}

/*!
  \class Dtk::Widget::DSpinBox
  \inmodule dtkwidget

  \brief Deepin风格的QSpinBox.
  \brief The DSpinBox class provides deepin style QSpinBox.
  
  类似 DLineEdit ，这个控件也提供了警告功能，用于提醒用户当前输入的数据不正确，另外，还可以设置一个默认的值，并在默认值发生改变时发出信号。
  如下图上面的是正常状态的控件, 下面的是处于警告状态的控件:
  Like DLineEdit, this widget can be set on alert to warn the user that the
  input is not correct. In addition, there's a DSpinBox::defaultValue property
  can be used to set a default value on the widget.

  \image html DSpinBox.png
 */

// ========================SIGNAL START========================
/*!
  \fn void DSpinBox::alertChanged(bool alert)
  \brief 警告状态发生了变化.

  \a alert 当前的警告状态。
 */

/*!
  \fn void DSpinBox::defaultValueChanged(int defaultValue)
  \brief 默认值发生了变化.

  \a defaultValue 控件的默认值。
 */
// ========================SIGNAL END========================

/*!
  \brief 构造一个 DSpinBox 实例。
  \brief DSpinBox::DSpinBox constructs an instance of DSpinBox

  \a parent 作为该实例的父控件。
  \a parent is passed to QSpinBox constructor
 */
DSpinBox::DSpinBox(QWidget *parent) :
    QSpinBox(parent),
    DObject(*new DSpinBoxPrivate(this))
{
    d_func()->init();
}

/*!
  \brief 获取输入框控件。

  \return 返回正在使用的输入库控件对象。
  \return the QLineEdit used by this spin box.
 */
QLineEdit *DSpinBox::lineEdit() const
{
    return QSpinBox::lineEdit();
}

/*!
  \property DSpinBox::alert

  \brief This property holds whether the widget on alert mode.
 */

/*!
  \brief 表示当前控件是否处于警告状态的属性.

  使用 DSpinBox::isAlert 获取属性当前状态，使用 DSpinBox::setAlert 设置属性的状态。
  
  \return 当前状态处于警告状态，返回 true，否则返回 false.
 */
bool DSpinBox::isAlert() const
{
    D_DC(DSpinBox);

    return d->lineEdit->isAlert();
}

/*!
  \brief 显示指定的文本消息，超过指定时间后警告消息消失.

   \a text text警告的文本
   \a duration 显示的时间长度，单位毫秒
 */
void DSpinBox::showAlertMessage(const QString &text, int duration)
{
    D_D(DSpinBox);

    d->lineEdit->showAlertMessage(text, duration);
}

/*!
  \brief 显示指定的文本消息，超过指定时间后警告消息消失.

  \a text text警告的文本
  \a follower 指定文本消息跟随的对象
  \a duration 显示的时间长度，单位毫秒
 */
void DSpinBox::showAlertMessage(const QString &text, QWidget *follower, int duration)
{
    D_D(DSpinBox);

    d->lineEdit->showAlertMessage(text, follower, duration);
}

/*!
  \brief 这个属性的值是控件的默认值。
  
  使用 DSpinBox::defaultValue 获取默认值，使用 DSpinBox::setDefaultValue 设置默认值, 点击控件上的 reset 按钮会设置为此值。
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
int DSpinBox::defaultValue() const
{
    return 0;
}
#endif

void DSpinBox::setEnabledEmbedStyle(bool enabled)
{
    setProperty("_d_dtk_spinBox", enabled);
}

void DSpinBox::setAlert(bool alert)
{
    D_D(DSpinBox);

    d->lineEdit->setAlert(alert);
}

#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
void DSpinBox::setDefaultValue(int defaultValue)
{
    Q_UNUSED(defaultValue)
}
#endif

DDoubleSpinBoxPrivate::DDoubleSpinBoxPrivate(DDoubleSpinBox *parent) :
    DObjectPrivate(parent)
{

}

void DDoubleSpinBoxPrivate::init()
{
    D_Q(DDoubleSpinBox);

    lineEdit = new DLineEdit(q);
    lineEdit->setClearButtonEnabled(false);
    q->setLineEdit(lineEdit->lineEdit());
    q->connect(lineEdit, &DLineEdit::alertChanged, q, &DDoubleSpinBox::alertChanged);
}

/*!
  \class Dtk::Widget::DDoubleSpinBox
  \inmodule dtkwidget

  \brief 类似 DDoubleSpinBox.
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

    return d->lineEdit->isAlert();
}

/*!
  \brief 显示指定的文本消息，超过指定时间后警告消息消失.

   \a text text警告的文本
   \a duration 显示的时间长度，单位毫秒
 */
void DDoubleSpinBox::showAlertMessage(const QString &text, int duration)
{
    D_D(DDoubleSpinBox);

    d->lineEdit->showAlertMessage(text, duration);
}

/*!
  \brief 显示指定的文本消息，超过指定时间后警告消息消失.

  \a text text警告的文本
  \a follower 指定文本消息跟随的对象
  \a duration 显示的时间长度，单位毫秒
 */
void DDoubleSpinBox::showAlertMessage(const QString &text, QWidget *follower, int duration)
{
    D_D(DDoubleSpinBox);

    d->lineEdit->showAlertMessage(text, follower, duration);
}

#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
double DDoubleSpinBox::defaultValue() const
{
    return 0;
}
#endif

QLineEdit *DDoubleSpinBox::lineEdit() const
{
    return QDoubleSpinBox::lineEdit();
}

void DDoubleSpinBox::setEnabledEmbedStyle(bool enabled)
{
    setProperty("_d_dtk_spinBox", enabled);
}

void DDoubleSpinBox::setAlert(bool alert)
{
    D_D(DDoubleSpinBox);

    d->lineEdit->setAlert(alert);
}
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
void DDoubleSpinBox::setDefaultValue(double defaultValue)
{
    Q_UNUSED(defaultValue)
}
#endif

DWIDGET_END_NAMESPACE
