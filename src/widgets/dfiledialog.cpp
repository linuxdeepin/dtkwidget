// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfiledialog.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#ifndef foreach
#define foreach Q_FOREACH
#endif
#include <private/qfiledialog_p.h>
#include <qpa/qplatformdialoghelper.h>

DWIDGET_BEGIN_NAMESPACE

/*!
@~english
@class Dtk::Widget::DFileDialog

  @brief The DFileDialog class provides a dialog that allow users to select
  files or directories.
  You can also add extra ComboBox and LineEdit widget via addComboBox() and
  addLineEdit() to allowed user fill more field when needed. Values of these
  extra fields can be accessed via getComboBoxValue() and getLineEditValue() .

@image html DFileDialog.png
*/

DFileDialog::DFileDialog(QWidget *parent, Qt::WindowFlags f)
    : QFileDialog(parent, f)
{

}

DFileDialog::DFileDialog(QWidget *parent, const QString &caption,
                         const QString &directory, const QString &filter)
    : QFileDialog(parent, caption, directory, filter)
{

}

/*!
@~english
  @brief Add an extra ComboBox widget to the DFileDialog
   text ComboBox description text (as key for getting value).
   data ComboBox options in a string list
 */
void DFileDialog::addComboBox(const QString &text, const QStringList &data)
{
    if (data.isEmpty())
        return;

    addComboBox(text, DComboBoxOptions {
                    false, data, data.first()
                });
}

/*!
@~english
  @brief Add an extra ComboBox widget to the DFileDialog
   text ComboBox description text (as key for getting value).
   options ComboBox data
 */
void DFileDialog::addComboBox(const QString &text, const DFileDialog::DComboBoxOptions &options)
{
    QJsonObject json;

    json["text"] = text;
    json["editable"] = options.editable;
    json["data"] = QJsonArray::fromStringList(options.data);
    json["defaultValue"] = options.defaultValue;

    QStringList list = property("_dtk_widget_custom_combobox_list").toStringList();

    list << QJsonDocument(json).toJson(QJsonDocument::Compact);
    setProperty("_dtk_widget_custom_combobox_list", list);
}

/*!
@~english
  @brief Add an extra LineEdit widget to the DFileDialog
   text LineEdit description text (as key for getting value).
 */
void DFileDialog::addLineEdit(const QString &text)
{
    addLineEdit(text, DLineEditOptions {
                    -1, QLineEdit::Normal, QString(), QString(), QString()
                });
}

/*!
@~english
  @brief Add an extra LineEdit widget to the DFileDialog
   text LineEdit description text (as key for getting value).
   options LineEdit data
 */
void DFileDialog::addLineEdit(const QString &text, const DFileDialog::DLineEditOptions &options)
{
    QJsonObject json;

    json["text"] = text;
    json["maxLength"] = options.maxLength;
    json["echoMode"] = options.echoMode;
    json["defaultValue"] = options.defaultValue;
    json["inputMask"] = options.inputMask;
    json["placeholderText"] = options.placeholderText;

    QStringList list = property("_dtk_widget_custom_lineedit_list").toStringList();

    list << QJsonDocument(json).toJson(QJsonDocument::Compact);
    setProperty("_dtk_widget_custom_lineedit_list", list);
}

/*!
@~english
  @brief Allow mixed selection
  @details
  Allow user choose files and folders at the same time when selecting multiple files. By
  default user can only select files (folder not included) when selecting multiple files.
  Notice that this option only works when file mode is QFileDialog::ExistingFiles
   on enable this feature or not.
 */
void DFileDialog::setAllowMixedSelection(bool on)
{
    setProperty("_dtk_widget_filedialog_mixed_selection", on);
}

/*!
@~english
  @brief Get the added extra ComboBox value
   text The description (key) of the ComboBox.
  @sa addComboBox()
 */
QString DFileDialog::getComboBoxValue(const QString &text) const
{
    return property(QString("_dtk_widget_combobox_%1_value").arg(text).toUtf8()).toString();
}

/*!
@~english
  @brief Get the added extra LineEdit value
   text The description (key) of the ComboBox.
  @sa addLineEdit()
 */
QString DFileDialog::getLineEditValue(const QString &text) const
{
    return property(QString("_dtk_widget_lineedit_%1_value").arg(text).toUtf8()).toString();
}

void DFileDialog::setVisible(bool visible)
{
    QFileDialogPrivate *d = static_cast<QFileDialogPrivate*>(d_ptr.data());

    if (d->canBeNativeDialog()) {
        QPlatformDialogHelper *helper = d->platformHelper();

        if (helper)
            helper->setProperty("_dtk_widget_QFileDialog", reinterpret_cast<quintptr>(this));
    }

    QFileDialog::setVisible(visible);
}

DWIDGET_END_NAMESPACE
