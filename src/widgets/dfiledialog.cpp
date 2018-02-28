#include "dfiledialog.h"

#include <QTextStream>

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
    QString value;
    QTextStream stream(&value);

    stream.setCodec("utf-8");
    stream << text;
    stream << options.editable;
    stream << options.data.size();

    for (const QString &i : options.data)
        stream << i;

    stream << options.defaultValue;

    QStringList list = property("_dtk_widget_custom_combobox_list").toStringList();

    list << value;
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
    QString value;
    QTextStream stream(&value);

    stream.setCodec("utf-8");
    stream << text;
    stream << options.maxLength;
    stream << options.echoMode;
    stream << options.defaultValue;
    stream << options.inputMask;
    stream << options.placeholderText;

    QStringList list = property("_dtk_widget_custom_lineedit_list").toStringList();

    list << value;
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

DWIDGET_END_NAMESPACE



