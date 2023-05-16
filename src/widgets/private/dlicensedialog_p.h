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

    bool load();
    bool isValid() const;

    class DComponentInfoPrivate;
    class LIBDTKCORESHARED_EXPORT DComponentInfo : public DObject
    {
    public:
        explicit DComponentInfo(DObject *parent = nullptr);
        ~DComponentInfo() override;

        QString name() const;
        QString version() const;
        QString copyRight() const;
        QString licenseName() const;

    private:
        D_DECLARE_PRIVATE(DComponentInfo)
        friend class DLicenseDialogPrivate;
    };
    using DComponentInfos = QVector<DComponentInfo*>;
    DComponentInfos componentInfos() const;
protected:
    void hideEvent(QHideEvent *) override;

private:
    D_DECLARE_PRIVATE(DLicenseDialog)
};

DWIDGET_END_NAMESPACE

#endif // DLICENSEDIALOG_H
