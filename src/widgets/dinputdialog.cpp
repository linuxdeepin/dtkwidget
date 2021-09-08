/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include <QAbstractButton>
#include <QHBoxLayout>
#include <QShowEvent>
#include <QApplication>
#include <QComboBox>

#include "dinputdialog.h"
#include "private/dinputdialog_p.h"
#include "dspinbox.h"

#include "dpasswordedit.h"

DWIDGET_BEGIN_NAMESPACE

DInputDialogPrivate::DInputDialogPrivate(DInputDialog *qq) :
    DDialogPrivate(qq)
{

}

void DInputDialogPrivate::init()
{
    D_Q(DInputDialog);

    q->addButtons(QStringList() << QApplication::translate("DInputDialog", "Cancel")
                                << QApplication::translate("DInputDialog", "Confirm"));
    q->setOnButtonClickedClose(false);
    q->setDefaultButton(1);

    cancelButton = q->getButton(0);
    okButton = q->getButton(1);

    lineEdit = new DLineEdit;
    spinBox = new DSpinBox;
    doubleSpinBox = new DDoubleSpinBox;
    comboBox = new QComboBox;

    q->addSpacing(10);
    q->addContent(lineEdit);
    q->addContent(spinBox);
    q->addContent(doubleSpinBox);
    q->addContent(comboBox);

    q->setInputMode(DInputDialog::TextInput);

    q->connect(lineEdit, &DLineEdit::textChanged, q, &DInputDialog::textValueChanged);
    q->connect(lineEdit, &DLineEdit::alertChanged, q, &DInputDialog::textAlertChanged);
    q->connect(comboBox, &QComboBox::currentTextChanged, q, &DInputDialog::textValueChanged);
    q->connect(comboBox, SIGNAL(currentTextChanged(QString)), q, SIGNAL(textValueChanged(QString)));
    q->connect(spinBox, SIGNAL(valueChanged(int)), q, SIGNAL(intValueChanged(int)));
    q->connect(doubleSpinBox, SIGNAL(valueChanged(double)), q, SIGNAL(doubleValueChanged(double)));
    q->connect(cancelButton, &QAbstractButton::clicked, q, [q] {
        q->done(QDialog::Rejected);
        q->cancelButtonClicked();
    });
    q->connect(okButton, &QAbstractButton::clicked, q, [q, this] {
        q->done(QDialog::Accepted);
        q->okButtonClicked();

        switch (inputMode) {
        case DInputDialog::TextInput:
            q->textValueSelected(lineEdit->text());
            break;
        case DInputDialog::ComboBox:
            q->textValueSelected(comboBox->currentText());
            break;
        case DInputDialog::IntInput:
            q->intValueSelected(spinBox->value());
            break;
        case DInputDialog::DoubleInput:
            q->doubleValueSelected(doubleSpinBox->value());
            break;
        default:
            break;
        }
    });
}

/*!
  \class Dtk::Widget::DInputDialog
  \inmodule dtkwidget
  \obsolete

  Use QInputDialog instead.

  \brief 快速创建一个获取可输入数据的对话框.
  
  DInputDialog 一般直接调用其静态函数 DInputDialog::getText DInputDialog::getItem DInputDialog::getInt DInputDialog::getDouble 来直接获取文字，选项，整数，浮点数
 */

/*!
  \fn void DInputDialog::textValueSelected(const QString &text)
  \brief 确认按钮被点击时调用此信号.
  
  如果InputMode是 DInputDialog::TextInput 或者 DInputDialog::ComboBox ，则确认按钮被点击时调用此信号发送当前文本输入框或多选框中当前的值
  
  \a value 要发送的值
 */

/*!
  \fn void DInputDialog::intValueSelected(int value)
  \brief 确认按钮被点击时调用此信号
  
  如果InputMode是 DInputDialog::IntInput ，则确认按钮被点击时调用此信号发送当前的整数值
  
  \a value 要发送的值
 */

/*!
  \fn DInputDialog::doubleValueSelected(double value)
  \brief 确认按钮被点击时调用此信号
  
  如果InputMode是 DInputDialog::DoubleInput ，则确认按钮被点击时调用此信号发送当前的浮点数值
  
  \a value 要发送的值
 */

/*!
  \fn void DInputDialog::cancelButtonClicked()
  \brief 取消按钮被点击时调用此信号
 */

/*!
  \fn void DInputDialog::okButtonClicked()
  \brief 确认按钮被点击时调用此信号
 */

/*!
  \brief 获取 DInputDialog::DInputDialog 实例
  \a parent 作为 DInputDialog::DInputDialog 实例的父控件
 */
DInputDialog::DInputDialog(QWidget *parent) :
    DDialog(*new DInputDialogPrivate(this), parent)
{
    d_func()->init();
}

/*!
  \sa DInputDialog::inputMode
 */
void DInputDialog::setInputMode(DInputDialog::InputMode mode)
{
    D_D(DInputDialog);

    d->inputMode = mode;

    switch (mode) {
    case InputMode::TextInput:
        d->lineEdit->show();
        d->comboBox->hide();
        d->spinBox->hide();
        d->doubleSpinBox->hide();
        break;
    case InputMode::ComboBox:
        d->comboBox->show();
        d->lineEdit->hide();
        d->spinBox->hide();
        d->doubleSpinBox->hide();
        break;
    case InputMode::IntInput:
        d->spinBox->show();
        d->lineEdit->hide();
        d->comboBox->hide();
        d->doubleSpinBox->hide();
        break;
    case InputMode::DoubleInput:
        d->doubleSpinBox->show();
        d->lineEdit->hide();
        d->comboBox->hide();
        d->spinBox->hide();
        break;
    default:
        break;
    }
}

/*!
  \property DInputDialog::inputMode
  \brief 这个属性保存对话框当前的输入模式
  
  查看 DInputDialog::InputMode 以了解对话框支持的多种输入模式
  
  Getter: DInputDialog::inputMode , Setter: DInputDialog::setInputMode
  \sa DInputDialog::InputMode
 */
DInputDialog::InputMode DInputDialog::inputMode() const
{
    D_DC(DInputDialog);

    return d->inputMode;
}

/*!
  \sa DInputDialog::textValue
 */
void DInputDialog::setTextValue(const QString &text)
{
    D_D(DInputDialog);

    if(inputMode() == ComboBox)
        d->comboBox->setCurrentText(text);
    else
        d->lineEdit->setText(text);
}

/*!
  \fn DInputDialog::textValueChanged(const QString &text)
  \brief 当属性 DInputDialog::textValue 发生变化时调用此信号
  \sa DInputDialog::textValue
 */

/*!
  \property DInputDialog::textValue
  \brief 这个属性保存对话框当前的输入文本
  
  如果当前的输入模式是 DInputDialog::ComboBox 则返回当前已选择的ComboBox选项，否则返回输入框的内容
  
  Getter: DInputDialog::textValue , Setter: DInputDialog::setTextValue , Signal: DInputDialog::textValueChanged
  
  \sa DInputDialog::InputMode
 */
QString DInputDialog::textValue() const
{
    D_DC(DInputDialog);

    return inputMode() == ComboBox ? d->comboBox->currentText() : d->lineEdit->text();
}

/*!
  \sa DInputDialog::textEchoMode
 */
void DInputDialog::setTextEchoMode(QLineEdit::EchoMode mode)
{
    D_D(DInputDialog);

    if(mode == d->lineEdit->echoMode())
        return;

    DLineEdit *edit;

    if(mode == QLineEdit::Normal) {
        edit = new DLineEdit;
    } else {
        edit = new DPasswordEdit;

        edit->setEchoMode(mode);
    }

    edit->setClearButtonEnabled(true);

    disconnect(d->lineEdit, &DLineEdit::alertChanged, this, &DInputDialog::textAlertChanged);
    disconnect(d->lineEdit, &DLineEdit::textChanged, this, &DInputDialog::textValueChanged);
    connect(edit, &DLineEdit::alertChanged, this, &DInputDialog::textAlertChanged);
    connect(edit, &DLineEdit::textChanged, this, &DInputDialog::textValueChanged);

    edit->setText(d->lineEdit->text());
    insertContent(d->getContentLayout()->indexOf(d->lineEdit), edit);
    removeContent(d->lineEdit);

    d->lineEdit = edit;
}

/*!
  \property DInputDialog::textEchoMode
  \brief 这个属性保存对话框中的输入框文字显示模式
  
  Getter: DInputDialog::textEchoMode , Setter: DInputDialog::setTextEchoMode
  
  \sa QLineEdit::EchoMode
 */
QLineEdit::EchoMode DInputDialog::textEchoMode() const
{
    D_DC(DInputDialog);

    return d->lineEdit->echoMode();
}

/*!
  \sa DInputDialog::comboBoxEditable
 */
void DInputDialog::setComboBoxEditable(bool editable)
{
    D_D(DInputDialog);

    d->comboBox->setEditable(editable);

    if (editable)
        connect(d->comboBox->lineEdit(), &QLineEdit::textChanged, this, &DInputDialog::textValueChanged, Qt::UniqueConnection);
}

/*!
  \property DInputDialog::comboBoxEditable
  \brief 这个属性表示对话框中的多选框是否可以编辑
  
  Getter: DInputDialog::isComboBoxEditable , Setter: DInputDialog::setComboBoxEditable
 */

/*!
  \sa DInputDialog::comboBoxEditable
 */
bool DInputDialog::isComboBoxEditable() const
{
    D_DC(DInputDialog);

    return d->comboBox->isEditable();
}

/*!
  \sa DInputDialog::comboBoxEditable
 */
void DInputDialog::setComboBoxItems(const QStringList &items)
{
    D_D(DInputDialog);

    d->comboBox->clear();
    d->comboBox->addItems(items);
}

/*!
  \property DInputDialog::comboBoxItems
  \brief 这个属性保存对话框中多选框的所有可选值
  
  Getter: DInputDialog::comboBoxItems , Setter: DInputDialog::setComboBoxItems
 */

/*!
  \sa DInputDialog::comboBoxItems
 */
QStringList DInputDialog::comboBoxItems() const
{
    D_DC(DInputDialog);

    QStringList list;

    for(int i = 0; i < d->comboBox->count(); ++i) {
        list << d->comboBox->itemText(i);
    }

    return list;
}

/*!
  \fn void DInputDialog::comboBoxCurrentIndexChanged(int comboBoxCurrentIndex)
  \brief 当属性 DInputDialog::comboBoxCurrentIndex 发生变化时调用此信号
  \sa DInputDialog::comboBoxCurrentIndex
 */

/*!
  \property DInputDialog::comboBoxCurrentIndex
  \brief 这个属性保存对话框中多选框当前的索引
  
  Getter: DInputDialog::comboBoxCurrentIndex , Setter: DInputDialog::setComboBoxCurrentIndex , Signal: DInputDialog::comboBoxCurrentIndexChanged
 */
int DInputDialog::comboBoxCurrentIndex() const
{
    D_DC(DInputDialog);

    return d->comboBox->currentIndex();
}

/*!
  \sa DInputDialog::comboBoxCurrentIndex
 */
void DInputDialog::setComboBoxCurrentIndex(int comboBoxCurrentIndex)
{
    D_D(DInputDialog);

    d->comboBox->setCurrentIndex(comboBoxCurrentIndex);
}

/*!
  \sa DInputDialog::intValue
 */
void DInputDialog::setIntValue(int value)
{
    D_DC(DInputDialog);

    d->spinBox->setValue(value);
}

/*!
  \fn void DInputDialog::intValueChanged(int value)
  \brief 当属性 DInputDialog::intValue 发生变化时调用此信号
  \sa DInputDialog::intValue
 */

/*!
  \property DInputDialog::intValue
  \brief 这个属性保存对话框当前的整型值
  
  Getter: DInputDialog::intValue , Setter: DInputDialog::setIntValue , Signal: DInputDialog::intValueChanged
 */
int DInputDialog::intValue() const
{
    D_DC(DInputDialog);

    return d->spinBox->value();
}

/*!
  \sa DInputDialog::intMinimum
 */
void DInputDialog::setIntMinimum(int min)
{
    D_D(DInputDialog);

    d->spinBox->setMinimum(min);
}

/*!
  \property DInputDialog::intMinimum
  \brief 这个属性保存对话框中的整形可选输入框SpinBox的最小值
  
  Getter: DInputDialog::intMinimum , Setter: DInputDialog::setIntMinimum
  
  \sa DInputDialog::intMaximum
 */
int DInputDialog::intMinimum() const
{
    D_DC(DInputDialog);

    return d->spinBox->minimum();
}

/*!
  \sa DInputDialog::intMaximum
 */
void DInputDialog::setIntMaximum(int max)
{
    D_D(DInputDialog);

    d->spinBox->setMaximum(max);
}

/*!
  \property DInputDialog::intMaximum
  \brief 这个属性保存对话框中的整形可选输入框SpinBox的最大值
  
  Getter: DInputDialog::intMaximum , Setter: DInputDialog::setIntMaximum
  
  \sa DInputDialog::intMinimum
 */
int DInputDialog::intMaximum() const
{
    D_DC(DInputDialog);

    return d->spinBox->maximum();
}

/*!
  \brief 设置对话框中的整形可选输入框SpinBox的可选范围（最小值和最大值）
  
  \sa DInputDialog::intMinimum DInputDialog::intMaximum
 */
void DInputDialog::setIntRange(int min, int max)
{
    D_D(DInputDialog);

    d->spinBox->setRange(min, max);
}

/*!
  \sa DInputDialog::intStep
 */
void DInputDialog::setIntStep(int step)
{
    D_D(DInputDialog);

    d->spinBox->setSingleStep(step);
}

/*!
  \property DInputDialog::intStep
  \brief 这个属性保存对话框中的可选输入框点击调增按钮时的步进值
  
  \brief 步进值即点击一次增大或减小多少
  
  Getter: DInputDialog::intStep , Setter: DInputDialog::setIntStep
 */
int DInputDialog::intStep() const
{
    D_DC(DInputDialog);

    return d->spinBox->singleStep();
}

/*!
  \sa DInputDialog::doubleValue
 */
void DInputDialog::setDoubleValue(double value)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setValue(value);
}

/*!
  \fn void DInputDialog::doubleValueChanged(int value)
  \brief 当属性 DInputDialog::doubleValue 发生变化时调用此信号
  \sa DInputDialog::doubleValue
 */

/*!
  \property DInputDialog::doubleValue
  \brief 这个属性保存对话框当前的浮点数值
  
  Getter: DInputDialog::doubleValue , Setter: DInputDialog::setDoubleValue , Signal: DInputDialog::doubleValueChanged
 */
double DInputDialog::doubleValue() const
{
    D_DC(DInputDialog);

    return d->doubleSpinBox->value();
}

/*!
  \sa DInputDialog::doubleMinimum
 */
void DInputDialog::setDoubleMinimum(double min)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setMinimum(min);
}

/*!
  \property DInputDialog::doubleMinimum
  \brief 这个属性保存对话框中的浮点型可选输入框SpinBox的最小值
  
  Getter: DInputDialog::doubleMinimum , Setter: DInputDialog::setDoubleMinimum
  
  \sa DInputDialog::doubleMaximum
 */
double DInputDialog::doubleMinimum() const
{
    D_DC(DInputDialog);

    return d->doubleSpinBox->minimum();
}

/*!
  \sa DInputDialog::doubleMaximum
 */
void DInputDialog::setDoubleMaximum(double max)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setMaximum(max);
}

/*!
  \property DInputDialog::doubleMaximum
  \brief 这个属性保存对话框中的浮点型可选输入框SpinBox的最大值
  
  Getter: DInputDialog::doubleMaximum , Setter: DInputDialog::setDoubleMaximum
  
  \sa DInputDialog::doubleMinimum
 */
double DInputDialog::doubleMaximum() const
{
    D_DC(DInputDialog);

    return d->doubleSpinBox->maximum();
}

/*!
  \brief 设置对话框中的浮点型可选输入框SpinBox的可选范围（最小值和最大值）
  
  \sa DInputDialog::doubleMinimum DInputDialog::doubleMaximum
 */
void DInputDialog::setDoubleRange(double min, double max)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setRange(min, max);
}

/*!
  \sa DInputDialog::doubleDecimals
 */
void DInputDialog::setDoubleDecimals(int decimals)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setDecimals(decimals);
}

/*!
  \property DInputDialog::doubleDecimals
  \brief 这个属性保存对话框中的浮点型可选输入框SpinBox的精度
  
  精度即处理多少位小数
  
  Getter: DInputDialog::doubleDecimals , Setter: DInputDialog::setDoubleDecimals
 */
int DInputDialog::doubleDecimals() const
{
    D_DC(DInputDialog);

    return d->doubleSpinBox->decimals();
}

/*!
  \sa DInputDialog::okButtonText
 */
void DInputDialog::setOkButtonText(const QString &text)
{
    D_DC(DInputDialog);

    d->okButton->setText(text);
}

/*!
  \property DInputDialog::okButtonText
  \brief 这个属性保存对话框的确认按钮文字
  
  Getter: DInputDialog::okButtonText , Setter: DInputDialog::setOkButtonText
 */
QString DInputDialog::okButtonText() const
{
    D_DC(DInputDialog);

    return d->okButton->text();
}

/*!
  \brief 设置确认按钮是否可以点击
  \a enable 为 true 即为可以点击，反之则反
 */
void DInputDialog::setOkButtonEnabled(const bool enable)
{
    D_DC(DInputDialog);

    d->okButton->setEnabled(enable);
}

/*!
  \brief 获取确认按钮是否可以点击
  \return true 即为可以点击，反之则反
 */
bool DInputDialog::okButtonIsEnabled() const
{
    D_DC(DInputDialog);

    return d->okButton->isEnabled();
}

/*!
  \sa DInputDialog::cancelButtonText
 */
void DInputDialog::setCancelButtonText(const QString &text)
{
    D_DC(DInputDialog);

    d->cancelButton->setText(text);
}

/*!
  \property DInputDialog::cancelButtonText
  \brief 这个属性保存对话框的取消按钮文字
  
  Getter: DInputDialog::cancelButtonText , Setter: DInputDialog::setCancelButtonText
 */
QString DInputDialog::cancelButtonText() const
{
    D_DC(DInputDialog);

    return d->cancelButton->text();
}

/*!
  \sa DInputDialog::textAlert
 */
void DInputDialog::setTextAlert(bool textAlert)
{
    D_D(DInputDialog);

    d->lineEdit->setAlert(textAlert);
}

/*!
  \fn DInputDialog::textAlertChanged(bool textAlert)
  \brief 当属性 DInputDialog::textAlert 发生变化时调用此信号
  \sa DInputDialog::textAlert
 */

/*!
  \property DInputDialog::textAlert
  \brief 这个属性保存对话框是否处于警告状态
  
  当对话框处于警告状态时，输入框的边框将会显示不同的颜色，一般警告告诉用户当前输入框的内容不合法
  
  Getter: DInputDialog::textAlert , Setter: DInputDialog::setTextAlert , Signal: DInputDialog::textAlertChanged
 */

/*!
  \sa DInputDialog::textAlert
 */
bool DInputDialog::isTextAlert() const
{
    D_DC(DInputDialog);

    return d->lineEdit->isAlert();
}

/*!
  \brief 快速创建一个可以输入文字的对话框
  \a parent 作为对话框的父控件
  \a title 作为对话框的标题
  \a message 作为对话框提示信息
  \a echo 设置文本输入框文字的显示模式
  \a text 设置文本输入框文字默认的内容
  \a ok 用于获取对话框是否点击了确认的指针
  \a flags 设置对话框的窗口属性，一般无需设置
  \a inputMethodHints 设置输入法相关属性，一般无需设置
  \return 返回输入的字符串
 */
QString DInputDialog::getText(QWidget *parent, const QString &title, const QString &message,
                              QLineEdit::EchoMode echo, const QString &text, bool *ok,
                              Qt::WindowFlags flags, Qt::InputMethodHints inputMethodHints)
{
    DInputDialog dialog(parent);

    dialog.setTitle(title);
    dialog.setMessage(message);
    dialog.setTextEchoMode(echo);
    dialog.setTextValue(text);
    dialog.setWindowFlags(flags);
    dialog.setInputMethodHints(inputMethodHints);
    dialog.setInputMode(InputMode::TextInput);

    ok ? *ok = dialog.exec() == QDialog::Accepted : dialog.exec();

    return dialog.textValue();
}

/*!
  \brief 快速创建一个多选框的对话框
  \a parent 作为对话框的父控件
  \a title 作为对话框的标题
  \a message 作为对话框提示信息
  \a items 设置所有可选项
  \a current 设置当前的可选项
  \a editable 设置多选框是否可编辑
  \a ok 用于获取对话框是否点击了确认的指针
  \a flags 设置对话框的窗口属性，一般无需设置
  \a inputMethodHints 设置输入法相关属性，一般无需设置
  \return 返回选中的项
 */
QString DInputDialog::getItem(QWidget *parent, const QString &title, const QString &message,
                              const QStringList &items, int current, bool editable, bool *ok,
                              Qt::WindowFlags flags, Qt::InputMethodHints inputMethodHints)
{
    DInputDialog dialog(parent);

    dialog.setComboBoxItems(items);
    dialog.setComboBoxEditable(editable);
    dialog.setComboBoxCurrentIndex(current);
    dialog.setTitle(title);
    dialog.setMessage(message);
    dialog.setWindowFlags(flags);
    dialog.setInputMethodHints(inputMethodHints);
    dialog.setInputMode(InputMode::ComboBox);

    ok ? *ok = dialog.exec() == QDialog::Accepted : dialog.exec();

    return dialog.textValue();
}

/*!
  \brief 快速创建一个获取整数的对话框
  \a parent 作为对话框的父控件
  \a title 作为对话框的标题
  \a message 作为对话框提示信息
  \a value 设置默认的值
  \a minValue 设置最小值
  \a maxValue 设置最大值
  \a step 设置步进值
  \a ok 用于获取对话框是否点击了确认的指针
  \a flags 设置对话框的窗口属性，一般无需设置
  \return 返回整数值
 */
int DInputDialog::getInt(QWidget *parent, const QString &title, const QString &message,
                         int value, int minValue, int maxValue, int step, bool *ok,
                         Qt::WindowFlags flags)
{
    DInputDialog dialog(parent);

    dialog.setIntValue(value);
    dialog.setIntRange(minValue, maxValue);
    dialog.setIntStep(step);
    dialog.setTitle(title);
    dialog.setMessage(message);
    dialog.setWindowFlags(flags);
    dialog.setInputMode(InputMode::IntInput);

    ok ? *ok = dialog.exec() == QDialog::Accepted : dialog.exec();

    return dialog.intValue();
}

/*!
  \brief 快速创建一个获取浮点数的对话框
  \a parent 作为对话框的父控件
  \a title 作为对话框的标题
  \a message 作为对话框提示信息
  \a value 设置默认的值
  \a minValue 设置最小值
  \a maxValue 设置最大值
  \a decimals 设置精度
  \a ok 用于获取对话框是否点击了确认的指针
  \a flags 设置对话框的窗口属性，一般无需设置
  \return 返回整数值
 */
double DInputDialog::getDouble(QWidget *parent, const QString &title, const QString &message,
                               double value, double minValue, double maxValue, int decimals,
                               bool *ok, Qt::WindowFlags flags)
{
    DInputDialog dialog(parent);

    dialog.setDoubleValue(value);
    dialog.setDoubleRange(minValue, maxValue);
    dialog.setDoubleDecimals(decimals);
    dialog.setTitle(title);
    dialog.setMessage(message);
    dialog.setWindowFlags(flags);
    dialog.setInputMode(InputMode::DoubleInput);

    ok ? *ok = dialog.exec() == QDialog::Accepted : dialog.exec();

    return dialog.doubleValue();
}

void DInputDialog::showEvent(QShowEvent *e)
{
    DDialog::showEvent(e);

    Q_D(DInputDialog);
    d->lineEdit->setFocus();
}

DWIDGET_END_NAMESPACE
