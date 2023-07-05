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
@~english
  @class Dtk::Widget::DSpinBox
  @inmodule dtkwidget
  @brief The DSpinBox class provides deepin style QSpinBox.
  Like DLineEdit, this widget can be set on alert to warn the user that the input is not correct. In addition, there's a DSpinBox::defaultValue property can be used to set a default value on the widget.
  As shown in the figure below, the upper is normal state widget and the lower is a warning state widget:
  @image html DSpinBox.png
 */

// ========================SIGNAL START========================
/*!
@~english
  @fn void DSpinBox::alertChanged(bool alert)
  @brief The warning state has changed.
  @param[in] alert current warning state
 */

/*!
@~english
  @fn void DSpinBox::defaultValueChanged(int defaultValue)
  @brief The default value has changed.
  @param[in] defaultValue default value of widget
 */
// ========================SIGNAL END========================

/*!
@~english
  @brief DSpinBox::DSpinBox construct an instance of DSpinBox
  @param[in] parent passed to QSpinBox constructor
 */
DSpinBox::DSpinBox(QWidget *parent) :
    QSpinBox(parent),
    DObject(*new DSpinBoxPrivate(this))
{
    d_func()->init();
}

/*!
@~english
  @brief get the input line widget
  @return the QLineEdit used by this spin box
 */
QLineEdit *DSpinBox::lineEdit() const
{
    return QSpinBox::lineEdit();
}

/*!
@~english
  @property DSpinBox::alert
  @brief This property holds whether the widget on alert mode.
 */

/*!
@~english
  @brief It means whether the current widget is on alert mode.
  Use DSpinBox::isAlert to get the current state of the property, use DSpinBox::setAlert to set the state of the property.
  @return If the current state is warning, return true, otherwise, return false.
 */
bool DSpinBox::isAlert() const
{
    D_DC(DSpinBox);

    return d->lineEdit->isAlert();
}

/*!
@~english
  @brief Display the specified text message and the warning message will disappear if time out.
  @param[in] text warning text
  @param[in] duration show the length of time, unit in millisecond
 */
void DSpinBox::showAlertMessage(const QString &text, int duration)
{
    D_D(DSpinBox);

    d->lineEdit->showAlertMessage(text, duration);
}

/*!
@~english
  @brief Display the specified text message and the warning message will disappear if time out.
  @param[in] text warning text
  @param[in] follower object of the text message
  @param[in] duration show the length of time, unit in millisecond
 */
void DSpinBox::showAlertMessage(const QString &text, QWidget *follower, int duration)
{
    D_D(DSpinBox);

    d->lineEdit->showAlertMessage(text, follower, duration);
}

/*!
@~english
  @brief default value of widget
  Use DSpinBox::defaultValue to get the default value, use DSpinBox::setDefaultValue to set the default value, click the Reset button on the widget to set it to this value.
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
@~english
  @class Dtk::Widget::DDoubleSpinBox
  @inmodule dtkwidget
  @brief like DDoubleSpinBox
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
@~english
  @brief Display the specified text message and the warning message will disappear if time out.
  @param[in] text warning text
  @param[in] duration show the length of time, unit in millisecond
 */
void DDoubleSpinBox::showAlertMessage(const QString &text, int duration)
{
    D_D(DDoubleSpinBox);

    d->lineEdit->showAlertMessage(text, duration);
}

/*!
@~english
  @brief Display the specified text message and the warning message will disappear if time out.
  @param[in] text warning text
  @param[in] follower object of the text message
  @param[in] duration show the length of time, unit in millisecond
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
