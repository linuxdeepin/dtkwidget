#include "daboutdialog.h"

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>

DWIDGET_BEGIN_NAMESPACE

const QString DAboutDialog::websiteLinkTemplate = "<a href='%1' style='text-decoration: none;'>%2</a>";

DAboutDialog::DAboutDialog(
    const QString &windowIcon,
    const QString &productIcon,
    const QString &productName,
    const QString &version,
    const QString &description,
    QWidget *parent,
    const QString &companyLogo,
    const QString &website,
    const QString &websiteLink): DWindow(parent)
{
    setWindowFlags(windowFlags() & ~ Qt::WindowMinimizeButtonHint);
    setWindowFlags(windowFlags() & ~ Qt::WindowSystemMenuHint);
    setWindowFlags(windowFlags() & ~ Qt::WindowMaximizeButtonHint);

    setWindowIcon(QIcon(windowIcon));

    QLabel *logoLabel = new QLabel("logo");
    logoLabel->setFixedSize(96, 96);
    logoLabel->setPixmap(QPixmap(productIcon).scaled(logoLabel->size(), Qt::KeepAspectRatio));


    QLabel *productNameLabel = new QLabel(productName);
    productNameLabel->setStyleSheet("font-size:18px;");

    QLabel *versionLabel = new QLabel(version);
    versionLabel->setStyleSheet("font-size:12px; color: #666666");

    QPixmap companyLogoPixmap(companyLogo);
    QLabel *companyLogoLabel = new QLabel(companyLogo);
    companyLogoLabel->setPixmap(companyLogoPixmap);
    companyLogoLabel->setFixedSize(companyLogoPixmap.size());

    QLabel *websiteLabel = new QLabel(website);
    websiteLabel->setStyleSheet("font-size:13px; color: #004EE5");
    websiteLabel->setOpenExternalLinks(false);
    QString websiteText = QString(websiteLinkTemplate).arg(websiteLink).arg(website);
    websiteLabel->setText(websiteText);
    connect(websiteLabel, SIGNAL(linkActivated(QString)),
            this, SLOT(onLogLinkActivated(QString)));

    QString textFormat = "<p style='text-indent: 24px;'>%1</p>";
    QString descriptionText =  textFormat.arg(description);
    QLabel *descriptionLabel = new QLabel(descriptionText);
    descriptionLabel->setStyleSheet("font-size:12px; color: #1A1A1A; border: 0px solid;");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->adjustSize();
    descriptionLabel->setFixedSize(400 - 38 * 2, descriptionLabel->height() + 16);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(10);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(logoLabel);
    mainLayout->setAlignment(logoLabel, Qt::AlignCenter);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(productNameLabel);
    mainLayout->setAlignment(productNameLabel, Qt::AlignCenter);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(versionLabel);
    mainLayout->setAlignment(versionLabel, Qt::AlignCenter);
    mainLayout->addSpacing(16);
    mainLayout->addWidget(companyLogoLabel);
    mainLayout->setAlignment(companyLogoLabel, Qt::AlignCenter);
    mainLayout->addSpacing(6);
    mainLayout->addWidget(websiteLabel);
    mainLayout->setAlignment(websiteLabel, Qt::AlignCenter);
    mainLayout->addSpacing(16);
    mainLayout->addWidget(descriptionLabel);
    mainLayout->setAlignment(descriptionLabel, Qt::AlignCenter);
    mainLayout->addSpacing(16);
    mainLayout->addStretch();

    setLayout(mainLayout);

    this->setFixedWidth(420);
    this->adjustSize();
    this->setFixedSize(this->size());

    setFocus();
}

void DAboutDialog::focusOutEvent(QFocusEvent */*e*/)
{
    hide();
}

void DAboutDialog::onLogLinkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));

}

DWIDGET_END_NAMESPACE
