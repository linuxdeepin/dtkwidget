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

DFileDialog::DFileDialog(QWidget *parent, Qt::WindowFlags f)
    : QFileDialog(parent, f)
{

}

DFileDialog::DFileDialog(QWidget *parent, const QString &caption,
                         const QString &directory, const QString &filter)
    : QFileDialog(parent, caption, directory, filter)
{

}

void DFileDialog::addComboBox(const QString &text, const QStringList &data)
{
    if (data.isEmpty())
        return;

    addComboBox(text, DComboBoxOptions {
                    false, data, data.first()
                });
}

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

void DFileDialog::addLineEdit(const QString &text)
{
    addLineEdit(text, DLineEditOptions {
                    -1, QLineEdit::Normal, QString(), QString(), QString()
                });
}

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

QString DFileDialog::getComboBoxValue(const QString &text) const
{
    return property(QString("_dtk_widget_combobox_%1_value").arg(text).toUtf8()).toString();
}

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



