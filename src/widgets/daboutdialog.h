/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DABOUTDIALOG_H
#define DABOUTDIALOG_H

#include "ddialog.h"

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

    Q_DISABLE_COPY(DAboutDialog)
    D_DECLARE_PRIVATE(DAboutDialog)
};

DWIDGET_END_NAMESPACE

#endif // DABOUTDIALOG_H
