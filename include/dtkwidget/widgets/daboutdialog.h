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
    Q_PROPERTY(QString acknowledgementLink READ acknowledgementLink WRITE setAcknowledgementLink)

public:
    DAboutDialog(QWidget *parent = 0);

    QString windowTitle() const;
    QString productName() const;
    QString version() const;
    QString description() const;
    const QPixmap *companyLogo() const;
    QString websiteName() const;
    QString websiteLink() const;
    QString acknowledgementLink() const;
    QString license() const;

Q_SIGNALS:
    void featureActivated();

public Q_SLOTS:
    void setWindowTitle(const QString &windowTitle);
    void setProductIcon(const QIcon &icon);
    void setProductName(const QString &productName);
    void setVersion(const QString &version);
    void setDescription(const QString &description);
    void setCompanyLogo(const QPixmap &companyLogo);
    void setWebsiteName(const QString &websiteName);
    void setWebsiteLink(const QString &websiteLink);
    void setAcknowledgementLink(const QString &acknowledgementLink);
    void setAcknowledgementVisible(bool visible);
    void setLicense(const QString &license);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_onLinkActivated(const QString &link))
    Q_PRIVATE_SLOT(d_func(), void _q_onFeatureActivated(const QString &link))

    Q_DISABLE_COPY(DAboutDialog)
    D_DECLARE_PRIVATE(DAboutDialog)
};

DWIDGET_END_NAMESPACE

#endif // DABOUTDIALOG_H
