/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DABOUTDIALOG_P_H
#define DABOUTDIALOG_P_H

#include "daboutdialog.h"
#include "ddialog_p.h"

DWIDGET_BEGIN_NAMESPACE

class DAboutDialogPrivate : public DDialogPrivate
{
public:
    DAboutDialogPrivate(DAboutDialog *qq);

    void init();
    void updateWebsiteLabel();
    void updateAcknowledgementLabel();
    void _q_onLinkActivated(const QString &link);
    QPixmap loadPixmap(const QString &file);

    static const QString websiteLinkTemplate;

    QPixmap windowIcon;
    QLabel *logoLabel;
    QLabel *productNameLabel;
    QLabel *versionLabel;
    QLabel *descriptionLabel;
    QLabel *licenseLabel;
    QLabel *companyLogoLabel;
    QLabel *websiteLabel;
    QLabel *acknowledgementLabel;

    QString websiteName;
    QString websiteLink;
    QString acknowledgementLink;

    Q_DECLARE_PUBLIC(DAboutDialog)
};

DWIDGET_END_NAMESPACE

#endif // DABOUTDIALOG_P_H
