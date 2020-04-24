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
 * \~chinese \class DInputDialog
 * \~chinese \brief 快速创建一个获取可输入数据的对话框
 *
 * \~chinese DInputDialog 一般直接调用其静态函数 DInputDialog::getText DInputDialog::getItem DInputDialog::getInt DInputDialog::getDouble 来直接获取文字，选项，整数，浮点数
 */





/*!
 * \~chinese \fn DInputDialog::textValueSelected
 * \~chinese \brief 确认按钮被点击时调用此信号
 *
 * \~chinese 如果InputMode是 DInputDialog::TextInput 或者 DInputDialog::ComboBox ，则确认按钮被点击时调用此信号发送当前文本输入框或多选框中当前的值
 *
 * \~chinese \param value 要发送的值
 */

/*!
 * \~chinese \fn DInputDialog::intValueSelected
 * \~chinese \brief 确认按钮被点击时调用此信号
 *
 * \~chinese 如果InputMode是 DInputDialog::IntInput ，则确认按钮被点击时调用此信号发送当前的整数值
 *
 * \~chinese \param value 要发送的值
 */

/*!
 * \~chinese \fn DInputDialog::doubleValueSelected
 * \~chinese \brief 确认按钮被点击时调用此信号
 *
 * \~chinese 如果InputMode是 DInputDialog::DoubleInput ，则确认按钮被点击时调用此信号发送当前的浮点数值
 *
 * \~chinese \param value 要发送的值
 */

/*!
 * \~chinese \fn DInputDialog::cancelButtonClicked
 * \~chinese \brief 取消按钮被点击时调用此信号
 */

/*!
 * \~chinese \fn DInputDialog::okButtonClicked
 * \~chinese \brief 确认按钮被点击时调用此信号
 */





/*!
 * \~chinese \brief 获取 DInputDialog::DInputDialog 实例
 * \~chinese \param parent 作为 DInputDialog::DInputDialog 实例的父控件
 */
DInputDialog::DInputDialog(QWidget *parent) :
    DDialog(*new DInputDialogPrivate(this), parent)
{
    d_func()->init();
}

/*!
 * \~chinese \sa DInputDialog::inputMode
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
 * \~chinese \property DInputDialog::inputMode
 * \~chinese \brief 这个属性保存对话框当前的输入模式
 *
 * \~chinese 查看 DInputDialog::InputMode 以了解对话框支持的多种输入模式
 *
 * \~chinese Getter: DInputDialog::inputMode , Setter: DInputDialog::setInputMode
 * \~chinese \sa DInputDialog::InputMode
 */
DInputDialog::InputMode DInputDialog::inputMode() const
{
    D_DC(DInputDialog);

    return d->inputMode;
}

/*!
 * \~chinese \sa DInputDialog::textValue
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
 * \~chinese \fn DInputDialog::textValueChanged
 * \~chinese \brief 当属性 DInputDialog::textValue 发生变化时调用此信号
 * \~chinese \sa DInputDialog::textValue
 */

/*!
 * \~chinese \property DInputDialog::textValue
 * \~chinese \brief 这个属性保存对话框当前的输入文本
 *
 * \~chinese 如果当前的输入模式是 DInputDialog::ComboBox 则返回当前已选择的ComboBox选项，否则返回输入框的内容
 *
 * \~chinese Getter: DInputDialog::textValue , Setter: DInputDialog::setTextValue , Signal: DInputDialog::textValueChanged
 *
 * \~chinese \sa DInputDialog::InputMode
 */
QString DInputDialog::textValue() const
{
    D_DC(DInputDialog);

    return inputMode() == ComboBox ? d->comboBox->currentText() : d->lineEdit->text();
}

/*!
 * \~chinese \sa DInputDialog::textEchoMode
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
 * \~chinese \property DInputDialog::textEchoMode
 * \~chinese \brief 这个属性保存对话框中的输入框文字显示模式
 *
 * \~chinese Getter: DInputDialog::textEchoMode , Setter: DInputDialog::setTextEchoMode
 *
 * \~chinese \sa QLineEdit::EchoMode
 */
QLineEdit::EchoMode DInputDialog::textEchoMode() const
{
    D_DC(DInputDialog);

    return d->lineEdit->echoMode();
}

/*!
 * \sa DInputDialog::comboBoxEditable
 */
void DInputDialog::setComboBoxEditable(bool editable)
{
    D_D(DInputDialog);

    d->comboBox->setEditable(editable);

    if (editable)
        connect(d->comboBox->lineEdit(), &QLineEdit::textChanged, this, &DInputDialog::textValueChanged, Qt::UniqueConnection);
}

/*!
 * \~chinese \property DInputDialog::comboBoxEditable
 * \~chinese \brief 这个属性表示对话框中的多选框是否可以编辑
 *
 * \~chinese Getter: DInputDialog::isComboBoxEditable , Setter: DInputDialog::setComboBoxEditable
 */

/*!
 * \sa DInputDialog::comboBoxEditable
 */
bool DInputDialog::isComboBoxEditable() const
{
    D_DC(DInputDialog);

    return d->comboBox->isEditable();
}

/*!
 * \sa DInputDialog::comboBoxEditable
 */
void DInputDialog::setComboBoxItems(const QStringList &items)
{
    D_D(DInputDialog);

    d->comboBox->clear();
    d->comboBox->addItems(items);
}

/*!
 * \~chinese \property DInputDialog::comboBoxItems
 * \~chinese \brief 这个属性保存对话框中多选框的所有可选值
 *
 * \~chinese Getter: DInputDialog::comboBoxItems , Setter: DInputDialog::setComboBoxItems
 */

/*!
 * \~chinese \sa DInputDialog::comboBoxItems
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
 * \~chinese \fn DInputDialog::comboBoxCurrentIndexChanged
 * \~chinese \brief 当属性 DInputDialog::comboBoxCurrentIndex 发生变化时调用此信号
 * \~chinese \sa DInputDialog::comboBoxCurrentIndex
 */

/*!
 * \~chinese \property DInputDialog::comboBoxCurrentIndex
 * \~chinese \brief 这个属性保存对话框中多选框当前的索引
 *
 * \~chinese Getter: DInputDialog::comboBoxCurrentIndex , Setter: DInputDialog::setComboBoxCurrentIndex , Signal: DInputDialog::comboBoxCurrentIndexChanged
 */
int DInputDialog::comboBoxCurrentIndex() const
{
    D_DC(DInputDialog);

    return d->comboBox->currentIndex();
}

/*!
 * \~chinese \sa DInputDialog::comboBoxCurrentIndex
 */
void DInputDialog::setComboBoxCurrentIndex(int comboBoxCurrentIndex)
{
    D_D(DInputDialog);

    d->comboBox->setCurrentIndex(comboBoxCurrentIndex);
}

/*!
 * \sa DInputDialog::intValue
 */
void DInputDialog::setIntValue(int value)
{
    D_DC(DInputDialog);

    d->spinBox->setValue(value);
}

/*!
 * \~chinese \fn DInputDialog::intValueChanged
 * \~chinese \brief 当属性 DInputDialog::intValue 发生变化时调用此信号
 * \~chinese \sa DInputDialog::intValue
 */

/*!
 * \~chinese \property DInputDialog::intValue
 * \~chinese \brief 这个属性保存对话框当前的整型值
 *
 * \~chinese Getter: DInputDialog::intValue , Setter: DInputDialog::setIntValue , Signal: DInputDialog::intValueChanged
 */
int DInputDialog::intValue() const
{
    D_DC(DInputDialog);

    return d->spinBox->value();
}

/*!
 * \~chinese \sa DInputDialog::intMinimum
 */
void DInputDialog::setIntMinimum(int min)
{
    D_D(DInputDialog);

    d->spinBox->setMinimum(min);
}

/*!
 * \~chinese \property DInputDialog::intMinimum
 * \~chinese \brief 这个属性保存对话框中的整形可选输入框SpinBox的最小值
 *
 * \~chinese Getter: DInputDialog::intMinimum , Setter: DInputDialog::setIntMinimum
 *
 * \~chinese \sa DInputDialog::intMaximum
 */
int DInputDialog::intMinimum() const
{
    D_DC(DInputDialog);

    return d->spinBox->minimum();
}

/*!
 * \~chinese \sa DInputDialog::intMaximum
 */
void DInputDialog::setIntMaximum(int max)
{
    D_D(DInputDialog);

    d->spinBox->setMaximum(max);
}

/*!
 * \~chinese \property DInputDialog::intMaximum
 * \~chinese \brief 这个属性保存对话框中的整形可选输入框SpinBox的最大值
 *
 * \~chinese Getter: DInputDialog::intMaximum , Setter: DInputDialog::setIntMaximum
 *
 * \~chinese \sa DInputDialog::intMinimum
 */
int DInputDialog::intMaximum() const
{
    D_DC(DInputDialog);

    return d->spinBox->maximum();
}

/*!
 * \~chinese \brief 设置对话框中的整形可选输入框SpinBox的可选范围（最小值和最大值）
 *
 * \~chinese \sa DInputDialog::intMinimum DInputDialog::intMaximum
 */
void DInputDialog::setIntRange(int min, int max)
{
    D_D(DInputDialog);

    d->spinBox->setRange(min, max);
}

/*!
 * \~chinese \sa DInputDialog::intStep
 */
void DInputDialog::setIntStep(int step)
{
    D_D(DInputDialog);

    d->spinBox->setSingleStep(step);
}

/*!
 * \~chinese \property DInputDialog::intStep
 * \~chinese \brief 这个属性保存对话框中的可选输入框点击调增按钮时的步进值
 *
 * \~chinese \brief 步进值即点击一次增大或减小多少
 *
 * \~chinese Getter: DInputDialog::intStep , Setter: DInputDialog::setIntStep
 */
int DInputDialog::intStep() const
{
    D_DC(DInputDialog);

    return d->spinBox->singleStep();
}

/*!
 * \~chinese \sa DInputDialog::doubleValue
 */
void DInputDialog::setDoubleValue(double value)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setValue(value);
}

/*!
 * \~chinese \fn DInputDialog::doubleValueChanged
 * \~chinese \brief 当属性 DInputDialog::doubleValue 发生变化时调用此信号
 * \~chinese \sa DInputDialog::doubleValue
 */

/*!
 * \~chinese \property DInputDialog::doubleValue
 * \~chinese \brief 这个属性保存对话框当前的浮点数值
 *
 * \~chinese Getter: DInputDialog::doubleValue , Setter: DInputDialog::setDoubleValue , Signal: DInputDialog::doubleValueChanged
 */
double DInputDialog::doubleValue() const
{
    D_DC(DInputDialog);

    return d->doubleSpinBox->value();
}

/*!
 * \~chinese \sa DInputDialog::doubleMinimum
 */
void DInputDialog::setDoubleMinimum(double min)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setMinimum(min);
}

/*!
 * \~chinese \property DInputDialog::doubleMinimum
 * \~chinese \brief 这个属性保存对话框中的浮点型可选输入框SpinBox的最小值
 *
 * \~chinese Getter: DInputDialog::doubleMinimum , Setter: DInputDialog::setDoubleMinimum
 *
 * \~chinese \sa DInputDialog::doubleMaximum
 */
double DInputDialog::doubleMinimum() const
{
    D_DC(DInputDialog);

    return d->doubleSpinBox->minimum();
}

/*!
 * \~chinese \sa DInputDialog::doubleMaximum
 */
void DInputDialog::setDoubleMaximum(double max)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setMaximum(max);
}

/*!
 * \~chinese \property DInputDialog::doubleMaximum
 * \~chinese \brief 这个属性保存对话框中的浮点型可选输入框SpinBox的最大值
 *
 * \~chinese Getter: DInputDialog::doubleMaximum , Setter: DInputDialog::setDoubleMaximum
 *
 * \~chinese \sa DInputDialog::doubleMinimum
 */
double DInputDialog::doubleMaximum() const
{
    D_DC(DInputDialog);

    return d->doubleSpinBox->maximum();
}

/*!
 * \~chinese \brief 设置对话框中的浮点型可选输入框SpinBox的可选范围（最小值和最大值）
 *
 * \~chinese \sa DInputDialog::doubleMinimum DInputDialog::doubleMaximum
 */
void DInputDialog::setDoubleRange(double min, double max)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setRange(min, max);
}

/*!
 * \~chinese \sa DInputDialog::doubleDecimals
 */
void DInputDialog::setDoubleDecimals(int decimals)
{
    D_D(DInputDialog);

    d->doubleSpinBox->setDecimals(decimals);
}

/*!
 * \~chinese \property DInputDialog::doubleDecimals
 * \~chinese \brief 这个属性保存对话框中的浮点型可选输入框SpinBox的精度
 *
 * \~chinese 精度即处理多少位小数
 *
 * \~chinese Getter: DInputDialog::doubleDecimals , Setter: DInputDialog::setDoubleDecimals
 */
int DInputDialog::doubleDecimals() const
{
    D_DC(DInputDialog);

    return d->doubleSpinBox->decimals();
}

/*!
 * \~chinese \sa DInputDialog::okButtonText
 */
void DInputDialog::setOkButtonText(const QString &text)
{
    D_DC(DInputDialog);

    d->okButton->setText(text);
}

/*!
 * \~chinese \property DInputDialog::okButtonText
 * \~chinese \brief 这个属性保存对话框的确认按钮文字
 *
 * \~chinese Getter: DInputDialog::okButtonText , Setter: DInputDialog::setOkButtonText
 */
QString DInputDialog::okButtonText() const
{
    D_DC(DInputDialog);

    return d->okButton->text();
}

/*!
 * \~chinese \brief 设置确认按钮是否可以点击
 * \~chinese \param enable 为 true 即为可以点击，反之则反
 */
void DInputDialog::setOkButtonEnabled(const bool enable)
{
    D_DC(DInputDialog);

    d->okButton->setEnabled(enable);
}

/*!
 * \~chinese \brief 获取确认按钮是否可以点击
 * \~chinese \return true 即为可以点击，反之则反
 */
bool DInputDialog::okButtonIsEnabled() const
{
    D_DC(DInputDialog);

    return d->okButton->isEnabled();
}

/*!
 * \~chinese \sa DInputDialog::cancelButtonText
 */
void DInputDialog::setCancelButtonText(const QString &text)
{
    D_DC(DInputDialog);

    d->cancelButton->setText(text);
}

/*!
 * \~chinese \property DInputDialog::cancelButtonText
 * \~chinese \brief 这个属性保存对话框的取消按钮文字
 *
 * \~chinese Getter: DInputDialog::cancelButtonText , Setter: DInputDialog::setCancelButtonText
 */
QString DInputDialog::cancelButtonText() const
{
    D_DC(DInputDialog);

    return d->cancelButton->text();
}

/*!
 * \~chinese \sa DInputDialog::textAlert
 */
void DInputDialog::setTextAlert(bool textAlert)
{
    D_D(DInputDialog);

    d->lineEdit->setAlert(textAlert);
}

/*!
 * \~chinese \fn DInputDialog::textAlertChanged
 * \~chinese \brief 当属性 DInputDialog::textAlert 发生变化时调用此信号
 * \~chinese \sa DInputDialog::textAlert
 */

/*!
 * \~chinese \property DInputDialog::textAlert
 * \~chinese \brief 这个属性保存对话框是否处于警告状态
 *
 * \~chinese 当对话框处于警告状态时，输入框的边框将会显示不同的颜色，一般警告告诉用户当前输入框的内容不合法
 *
 * \~chinese Getter: DInputDialog::textAlert , Setter: DInputDialog::setTextAlert , Signal: DInputDialog::textAlertChanged
 */

/*!
 * \~chinese \sa DInputDialog::textAlert
 */
bool DInputDialog::isTextAlert() const
{
    D_DC(DInputDialog);

    return d->lineEdit->isAlert();
}

/*!
 * \brief 快速创建一个可以输入文字的对话框
 * \param parent 作为对话框的父控件
 * \param title 作为对话框的标题
 * \param message 作为对话框提示信息
 * \param echo 设置文本输入框文字的显示模式
 * \param text 设置文本输入框文字默认的内容
 * \param ok 用于获取对话框是否点击了确认的指针
 * \param flags 设置对话框的窗口属性，一般无需设置
 * \param inputMethodHints 设置输入法相关属性，一般无需设置
 * \return 返回输入的字符串
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
 * \brief 快速创建一个多选框的对话框
 * \param parent 作为对话框的父控件
 * \param title 作为对话框的标题
 * \param message 作为对话框提示信息
 * \param items 设置所有可选项
 * \param current 设置当前的可选项
 * \param editable 设置多选框是否可编辑
 * \param ok 用于获取对话框是否点击了确认的指针
 * \param flags 设置对话框的窗口属性，一般无需设置
 * \param inputMethodHints 设置输入法相关属性，一般无需设置
 * \return 返回选中的项
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
 * \brief 快速创建一个获取整数的对话框
 * \param parent 作为对话框的父控件
 * \param title 作为对话框的标题
 * \param message 作为对话框提示信息
 * \param value 设置默认的值
 * \param minValue 设置最小值
 * \param maxValue 设置最大值
 * \param step 设置步进值
 * \param ok 用于获取对话框是否点击了确认的指针
 * \param flags 设置对话框的窗口属性，一般无需设置
 * \return 返回整数值
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
 * \brief 快速创建一个获取浮点数的对话框
 * \param parent 作为对话框的父控件
 * \param title 作为对话框的标题
 * \param message 作为对话框提示信息
 * \param value 设置默认的值
 * \param minValue 设置最小值
 * \param maxValue 设置最大值
 * \param decimals 设置精度
 * \param ok 用于获取对话框是否点击了确认的指针
 * \param flags 设置对话框的窗口属性，一般无需设置
 * \return 返回整数值
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
