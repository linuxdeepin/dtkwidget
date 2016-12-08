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
