#include "daboutdialog.h"

#include <dutility.h>

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QKeyEvent>

DWIDGET_BEGIN_NAMESPACE

const QString DAboutDialog::websiteLinkTemplate = "<a href='%1' style='text-decoration: none; font-size:13px; color: #004EE5;'>%2</a>";

DAboutDialog::DAboutDialog(const QPixmap &windowIcon,
                           const QPixmap &productIcon,
                           const QString &productName,
                           const QString &version,
                           const QString &description,
                           const QString &acknowledgementLink,
                           QWidget *parent): DWindow(parent)
{
    m_windowIcon = QPixmap(windowIcon);
    m_productIcon = QPixmap(productIcon);
    m_productName = productName;
    m_version = version;
    m_description = description;
    m_acknowledgementLink = acknowledgementLink;
    m_companyLogo = QPixmap(":/images/deepin-logo.png");
    m_websiteName = "www.deepin.org";
    m_websiteLink = "https://www.deepin.org";
    initUI();
}

DAboutDialog::DAboutDialog(
    const QString &windowIcon,
    const QString &productIcon,
    const QString &productName,
    const QString &version,
    const QString &description,
    QWidget *parent,
    const QString &companyLogo,
    const QString &websiteName,
    const QString &websiteLink): DWindow(parent)
{
    m_windowIcon = QPixmap(windowIcon);
    m_productIcon = QPixmap(productIcon);
    m_productName = productName;
    m_version = version;
    m_description = description;
    m_companyLogo = QPixmap(companyLogo);;
    m_websiteName = websiteName;
    m_websiteLink = websiteLink;
    initUI();
}

DAboutDialog::DAboutDialog(
    const QString &/*windowTitle*/,
    const QString &windowIcon,
    const QString &productIcon,
    const QString &productName,
    const QString &version,
    const QString &description,
    QWidget *parent,
    const QString &companyLogo,
    const QString &website,
    const QString &websiteLink)
    : DAboutDialog(windowIcon, productIcon, productName, version, description, parent, companyLogo, website, websiteLink)
{
    setTitle("");
}

void DAboutDialog::onLinkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void DAboutDialog::initUI()
{
    setTitlebarFixedHeight(30);
    setWindowFlags(windowFlags() & ~ Qt::WindowMinimizeButtonHint);
    setWindowFlags(windowFlags() & ~ Qt::WindowSystemMenuHint);
    setWindowFlags(windowFlags() & ~ Qt::WindowMaximizeButtonHint);
    setWindowModality(Qt::ApplicationModal);

    DWindow::setWindowIcon(m_windowIcon);

    QLabel *logoLabel = new QLabel("logo");
    logoLabel->setContentsMargins(0, 0, 0, 0);
    logoLabel->setFixedSize(96, 96);
    logoLabel->setPixmap(m_productIcon.scaled(logoLabel->size(), Qt::KeepAspectRatio));

    QLabel *productNameLabel = new QLabel(m_productName);
    productNameLabel->setStyleSheet("font-size:18px;");

    QLabel *versionLabel = new QLabel(m_version);
    versionLabel->setStyleSheet("font-size:12px; color: #666666");

    QLabel *companyLogoLabel = new QLabel();
    companyLogoLabel->setPixmap(m_companyLogo);
    companyLogoLabel->setFixedSize(m_companyLogo.size());

    QLabel *websiteLabel = new QLabel(m_websiteName);
    websiteLabel->setContextMenuPolicy(Qt::NoContextMenu);
    websiteLabel->setFixedHeight(24);
    websiteLabel->setStyleSheet("font-size:13px; color: #004EE5");
    websiteLabel->setOpenExternalLinks(false);
    QString websiteText = QString(websiteLinkTemplate).arg(m_websiteLink).arg(m_websiteName);
    websiteLabel->setText(websiteText);

    QLabel *acknowledgementLabel = new QLabel();
    acknowledgementLabel->setContextMenuPolicy(Qt::NoContextMenu);
    acknowledgementLabel->setFixedHeight(24);
    acknowledgementLabel->setStyleSheet("font-size:13px; color: #004EE5");
    acknowledgementLabel->setOpenExternalLinks(false);
    QString acknowledgementText = QString(websiteLinkTemplate).arg(m_acknowledgementLink).arg(tr("Acknowledgements"));
    acknowledgementLabel->setText(acknowledgementText);

    QLabel *descriptionLabel = new QLabel();
    descriptionLabel->setText(m_description + '\n');
    descriptionLabel->setAlignment(Qt::AlignHCenter);

    descriptionLabel->setStyleSheet("font-size:11px; color: #1A1A1A; border: none;");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->adjustSize();

    connect(websiteLabel, &QLabel::linkActivated, this, &DAboutDialog::onLinkActivated);
    connect(acknowledgementLabel, &QLabel::linkActivated, this, &DAboutDialog::onLinkActivated);
    connect(descriptionLabel, &QLabel::linkActivated, this, &DAboutDialog::onLinkActivated);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(38, 0, 38, 10);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(logoLabel);
    mainLayout->setAlignment(logoLabel, Qt::AlignCenter);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(productNameLabel);
    mainLayout->setAlignment(productNameLabel, Qt::AlignCenter);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(versionLabel);
    mainLayout->setAlignment(versionLabel, Qt::AlignCenter);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(companyLogoLabel);
    mainLayout->setAlignment(companyLogoLabel, Qt::AlignCenter);
    mainLayout->addSpacing(2);
    mainLayout->addWidget(websiteLabel);
    mainLayout->setAlignment(websiteLabel, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(acknowledgementLabel);
    mainLayout->setAlignment(acknowledgementLabel, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(descriptionLabel, Qt::AlignHCenter);

    setLayout(mainLayout);

    this->setFixedWidth(400);
    this->adjustSize();
    this->setFixedSize(this->size());
    if (parentWidget() && parentWidget()->isTopLevel()) {
        QPoint pCenterGlobal = mapToGlobal(parentWidget()->geometry().center());
        this->move(pCenterGlobal.x() - width() / 2,
                   pCenterGlobal.y() - height() / 2);
    }

    setFocus();

    connect(this, &DAboutDialog::windowTitleChanged, this, &DAboutDialog::setTitle);
    connect(this, &DAboutDialog::windowIconChanged, this, [=](QPixmap windowIcon){
        DWindow::setWindowIcon(windowIcon);
    });
    connect(this, &DAboutDialog::productIconChanged, logoLabel, &QLabel::setPixmap);
    connect(this, &DAboutDialog::productNameChanged, productNameLabel, &QLabel::setText);
    connect(this, &DAboutDialog::versionChanged, versionLabel, &QLabel::setText);
    connect(this, &DAboutDialog::descriptionChanged, descriptionLabel, &QLabel::setText);

    connect(this, &DAboutDialog::websiteNameChanged, this, [=](const QString websiteName){
        QString websiteText = QString(websiteLinkTemplate).arg(m_websiteLink).arg(websiteName);
        websiteLabel->setText(websiteText);
    });
    connect(this, &DAboutDialog::websiteLinkChanged, this, [=](const QString websiteLink){
        QString websiteText = QString(websiteLinkTemplate).arg(websiteLink).arg(m_websiteName);
        websiteLabel->setText(websiteText);
    });
    connect(this, &DAboutDialog::acknowledgementLinkChanged,  this, [=](const QString acknowledgementLink){
        QString websiteText = QString(websiteLinkTemplate).arg(acknowledgementLink).arg(tr("Acknowledgements"));
        acknowledgementLabel->setText(websiteText);
    });
}

void DAboutDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();
        event->accept();
    }

    DWindow::keyPressEvent(event);
}

DWIDGET_END_NAMESPACE
