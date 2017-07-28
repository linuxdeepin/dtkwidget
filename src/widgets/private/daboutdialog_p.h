/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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
