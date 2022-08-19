// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFILECHOOSEREDIT_H
#define DFILECHOOSEREDIT_H

#include <DLineEdit>
#include <QFileDialog>

DWIDGET_BEGIN_NAMESPACE

class DFileChooserEditPrivate;
class LIBDTKWIDGETSHARED_EXPORT DFileChooserEdit : public DLineEdit
{
    Q_OBJECT

    Q_ENUMS(DialogDisplayPosition)

public:
    enum DialogDisplayPosition {
        FollowParentWindow,
        CurrentMonitorCenter
    };

    DFileChooserEdit(QWidget *parent = nullptr);

    void setFileMode(QFileDialog::FileMode mode);
    QFileDialog::FileMode fileMode() const;

    void setNameFilters(const QStringList &filters);
    QStringList nameFilters() const;

    void setDirectoryUrl(const QUrl &directory);
    QUrl directoryUrl();

    void setDialogDisplayPosition(DialogDisplayPosition dialogDisplayPosition);
    DFileChooserEdit::DialogDisplayPosition dialogDisplayPosition() const;

    void setFileDialog(QFileDialog *fileDialog);
    QFileDialog *fileDialog() const;

    void initDialog();

Q_SIGNALS:
    void fileChoosed(const QString &fileName);
    void dialogOpened();
    void dialogClosed(int code);

protected:
    Q_DISABLE_COPY(DFileChooserEdit)
    D_DECLARE_PRIVATE(DFileChooserEdit)
    Q_PRIVATE_SLOT(d_func(), void _q_showFileChooserDialog())
};

DWIDGET_END_NAMESPACE

#endif // DFILECHOOSEREDIT_H
