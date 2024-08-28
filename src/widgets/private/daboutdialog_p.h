// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DABOUTDIALOG_P_H
#define DABOUTDIALOG_P_H

#include <DAboutDialog>
#include <QLabel>

#include "ddialog_p.h"

DWIDGET_BEGIN_NAMESPACE

class DRedPointLabel : public QLabel
{
public:
    DRedPointLabel(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *e) override;
};

class DAboutDialogPrivate : public DDialogPrivate
{
public:
    DAboutDialogPrivate(DAboutDialog *qq);

    void init();
    void loadDistributionInfo();
    void updateWebsiteLabel();
    void _q_onLinkActivated(const QString &link);
    void _q_onFeatureActivated(const QString &link);

    QPixmap loadPixmap(const QString &file);

    static const QString websiteLinkTemplate;

    QPixmap windowIcon;
    QLabel *logoLabel = nullptr;
    QLabel *productNameLabel = nullptr;
    QLabel *versionLabel = nullptr;
    QLabel *descriptionLabel = nullptr;
    QLabel *licenseLabel = nullptr;
    QLabel *companyLogoLabel = nullptr;
    QLabel *websiteLabel = nullptr;
    QLabel *featureLabel = nullptr;
    DRedPointLabel *redPointLabel = nullptr;
    QString logoPath;
    QString websiteName;
    QString websiteLink;
    Q_DECLARE_PUBLIC(DAboutDialog)
};

DWIDGET_END_NAMESPACE

#endif // DABOUTDIALOG_P_H
