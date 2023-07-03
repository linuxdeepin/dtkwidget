// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DABOUTDIALOG_H
#define DABOUTDIALOG_H

#include <DDialog>

DWIDGET_BEGIN_NAMESPACE

class DAboutDialogPrivate;
class DAboutDialog : public DDialog
{
    Q_OBJECT

    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle)
    Q_PROPERTY(QString productName READ productName WRITE setProductName)
    Q_PROPERTY(QString version READ version WRITE setVersion)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString license READ license WRITE setLicense)
    Q_PROPERTY(QString websiteName READ websiteName WRITE setWebsiteName)
    Q_PROPERTY(QString websiteLink READ websiteLink WRITE setWebsiteLink)
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    Q_PROPERTY(QString acknowledgementLink READ acknowledgementLink WRITE setAcknowledgementLink)
#endif

public:
    DAboutDialog(QWidget *parent = 0);

    QString windowTitle() const;
    QString productName() const;
    QString version() const;
    QString description() const;
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    D_DECL_DEPRECATED_X("this method will be deprecated after dtk6") const QPixmap *companyLogo() const;
#else
    QPixmap companyLogo() const;
#endif
    QString websiteName() const;
    QString websiteLink() const;
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    D_DECL_DEPRECATED_X("acknowledgement is no longer used") QString acknowledgementLink() const;
#endif
    QString license() const;
    void setLicenseEnabled(bool enabled);

Q_SIGNALS:
    void featureActivated();
    void licenseActivated();

public Q_SLOTS:
    void setWindowTitle(const QString &windowTitle);
    void setProductIcon(const QIcon &icon);
    void setProductName(const QString &productName);
    void setVersion(const QString &version);
    void setDescription(const QString &description);
    void setCompanyLogo(const QPixmap &companyLogo);
    void setWebsiteName(const QString &websiteName);
    void setWebsiteLink(const QString &websiteLink);
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
    D_DECL_DEPRECATED_X("acknowledgement is no longer used") void setAcknowledgementLink(const QString &acknowledgementLink);
#endif
    void setAcknowledgementVisible(bool visible);
    void setLicense(const QString &license);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_onLinkActivated(const QString &link))
    Q_PRIVATE_SLOT(d_func(), void _q_onFeatureActivated(const QString &link))
    Q_PRIVATE_SLOT(d_func(), void _q_onLicenseActivated(const QString &link))

    Q_DISABLE_COPY(DAboutDialog)
    D_DECLARE_PRIVATE(DAboutDialog)
};

DWIDGET_END_NAMESPACE

#endif // DABOUTDIALOG_H
