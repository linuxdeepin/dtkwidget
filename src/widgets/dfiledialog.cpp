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
  \~english \class DFileDialog
  \~english \brief The DFileDialog class provides a dialog that allow users to select files or directories.

  \~english \image html dfiledialog.png

  \~english You can also add extra ComboBox and LineEdit widget via addComboBox() and addLineEdit() to allowed
  user fill more field when needed. Values of these extra fields can be accessed via getComboBoxValue() and
  getLineEditValue() .
*/

/*!
  \~chinese \class DFileDialog
  \~chinese \brief DFileDialog 类提供了一个可供用户选择文件或目录的对话框.

  \~chinese \image html dfiledialog.png

  \~chinese 你也可以通过 addComboBox() 和 addLineEdit() 来为文件选择框增加额外的输入内容控件，并通过 getComboBoxValue()
  和 getLineEditValue() 来得到用户所输入的值。
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
 * \~english \brief Add an extra ComboBox widget to the DFileDialog
 * \~chinese \brief 为文件选择框追加一个下拉单选框
 *
 * \~english \param text ComboBox description text (as key for getting value).
 * \~english \param data ComboBox options in a string list
 *
 * \~chinese \param text 追加选项的描述文字（作为键）
 * \~chinese \param data 多选框的选项列表
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
 * \~english \brief Add an extra ComboBox widget to the DFileDialog
 * \~chinese \brief 为文件选择框追加一个下拉单选框
 *
 * \~english \param text ComboBox description text (as key for getting value).
 * \~english \param options ComboBox data
 *
 * \~chinese \param text 追加选项的描述文字（作为键）
 * \~chinese \param options 多选框的属性信息
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
 * \~english \brief Add an extra LineEdit widget to the DFileDialog
 * \~chinese \brief 为文件选择框追加一个输入框
 *
 * \~english \param text LineEdit description text (as key for getting value).
 * \~chinese \param text 追加选项的描述文字（作为键）
 */
void DFileDialog::addLineEdit(const QString &text)
{
    addLineEdit(text, DLineEditOptions {
                    -1, QLineEdit::Normal, QString(), QString(), QString()
                });
}

/*!
 * \~english \brief Add an extra LineEdit widget to the DFileDialog
 * \~chinese \brief 为文件选择框追加一个输入框
 *
 * \~english \param text LineEdit description text (as key for getting value).
 * \~english \param options LineEdit data
 *
 * \~chinese \param text 追加选项的描述文字（作为键）
 * \~chinese \param options 输入框的属性信息
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
 * \brief Allow mixed selection
 *
 * Allow user choose files and folders at the same time when selecting multiple files. By
 * default user can only select files (folder not included) when selecting multiple files.
 *
 * Notice that this option only works when file mode is QFileDialog::ExistingFiles
 *
 * \param on enable this feature or not.
 */
void DFileDialog::setAllowMixedSelection(bool on)
{
    setProperty("_dtk_widget_filedialog_mixed_selection", on);
}

/*!
 * \~english \brief Get the added extra ComboBox value
 * \~chinese \brief 获得所追加的额外多选框的值
 *
 * \~english \param text The description (key) of the ComboBox.
 * \~chinese \param text 所追加的多选框的描述名（作为键）
 *
 * \sa addComboBox()
 */
QString DFileDialog::getComboBoxValue(const QString &text) const
{
    return property(QString("_dtk_widget_combobox_%1_value").arg(text).toUtf8()).toString();
}

/*!
 * \~english \brief Get the added extra LineEdit value
 * \~chinese \brief 获得所追加的额外输入框的值
 *
 * \~english \param text The description (key) of the ComboBox.
 * \~chinese \param text 所追加的多选框的描述名（作为键）
 *
 * \sa addLineEdit()
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



