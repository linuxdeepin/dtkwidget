#ifndef DFILEDIALOG_H
#define DFILEDIALOG_H

#include <dtkwidget_global.h>

#include <QFileDialog>
#include <QLineEdit>

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DFileDialog : public QFileDialog
{
    Q_OBJECT

public:
    DFileDialog(QWidget *parent, Qt::WindowFlags f);
    explicit DFileDialog(QWidget *parent = Q_NULLPTR,
                         const QString &caption = QString(),
                         const QString &directory = QString(),
                         const QString &filter = QString());

    struct DComboBoxOptions {
        bool editable;
        QStringList data;
        QString defaultValue;
    };

    struct DLineEditOptions {
        int maxLength;
        QLineEdit::EchoMode echoMode;
        QString defaultValue;
        QString inputMask;
        QString placeholderText;
    };

    void addComboBox(const QString &text, const QStringList &data);
    void addComboBox(const QString &text, const DComboBoxOptions &options);
    void addLineEdit(const QString &text);
    void addLineEdit(const QString &text, const DLineEditOptions &options);
    void setAllowMixedSelection(bool on);

    QString getComboBoxValue(const QString &text) const;
    QString getLineEditValue(const QString &text) const;

    void setVisible(bool visible) override;
};

DWIDGET_END_NAMESPACE

#endif // DFILEDIALOG_H
