// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DLICENSEDIALOG_H
#define DLICENSEDIALOG_H

#include <DDialog>

DWIDGET_BEGIN_NAMESPACE

class DLicenseDialogPrivate;
class DLicenseDialog : public DAbstractDialog
{
    Q_OBJECT
public:
    explicit DLicenseDialog(QWidget *parent = nullptr);
    ~DLicenseDialog() override;

    void setContent(const QByteArray &content);
    void setFile(const QString &file);
    void setLicenseSearchPath(const QString &path);
    bool load();
    bool isValid() const;

protected:
    void hideEvent(QHideEvent *) override;

private:
    D_DECLARE_PRIVATE(DLicenseDialog)
};

DWIDGET_END_NAMESPACE

#endif // DLICENSEDIALOG_H
