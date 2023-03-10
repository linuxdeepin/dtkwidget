// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "daboutdialog.h"
#include "dfeaturedisplaydialog.h"
#include "private/daboutdialog_p.h"

#include <dwidgetutil.h>
#include <DSysInfo>
#include <DGuiApplicationHelper>
#include <DApplication>
#include <DFontSizeManager>
#include <DConfig>

#include <QUrl>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QKeyEvent>
#include <QImageReader>
#include <QScrollArea>
#include <QPainter>
#include <QPainterPath>

#ifdef Q_OS_UNIX
#include <unistd.h>
#include <pwd.h>
#endif

DCORE_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

const QString DAboutDialogPrivate::websiteLinkTemplate = "<a href='%1' style='text-decoration: none; font-size:13px; color: #004EE5;'>%2</a>";

DRedPointLabel::DRedPointLabel(QWidget *parent)
    : QLabel(parent)
{
}

void DRedPointLabel::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    QRectF rcf(0, 0, 4, 4);
    QPainterPath path;
    path.addEllipse(rcf);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillPath(path, QColor("#FF0000"));
    painter.setPen(QColor(0, 0, 0, 255 * 0.05));
    painter.drawEllipse(rcf);
}

DAboutDialogPrivate::DAboutDialogPrivate(DAboutDialog *qq)
    : DDialogPrivate(qq)
{

}

void DAboutDialogPrivate::init()
{
    D_Q(DAboutDialog);

    q->setMinimumWidth(360);

    // overwrite default info if distribution config file existed.
    loadDistributionInfo();

    logoLabel = new QLabel();
    logoLabel->setContentsMargins(0, 0, 0, 0);

    productNameLabel = new QLabel();
    productNameLabel->setObjectName("ProductNameLabel");
    DFontSizeManager *fontManager =  DFontSizeManager::instance();
    fontManager->bind(productNameLabel, DFontSizeManager::T5, QFont::DemiBold);

    versionLabel = new QLabel();
    versionLabel->setObjectName("VersionLabel");
    fontManager->bind(versionLabel, DFontSizeManager::T8, QFont::DemiBold);

    companyLogoLabel = new QLabel();
    companyLogoLabel->setPixmap(loadPixmap(logoPath));

    websiteLabel = new QLabel();
    websiteLabel->setObjectName("WebsiteLabel");
    websiteLabel->setContextMenuPolicy(Qt::NoContextMenu);
    websiteLabel->setOpenExternalLinks(false);
    updateWebsiteLabel();

    descriptionLabel = new QLabel();
    descriptionLabel->setObjectName("DescriptionLabel");
    descriptionLabel->setAlignment(Qt::AlignLeft);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    fontManager->bind(descriptionLabel, DFontSizeManager::T8, QFont::DemiBold);

    licenseLabel = new QLabel();
    licenseLabel->setObjectName("LicenseLabel");
    licenseLabel->setAlignment(Qt::AlignHCenter);
    licenseLabel->setWordWrap(true);
    licenseLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    licenseLabel->hide();
    fontManager->bind(licenseLabel, DFontSizeManager::T10, QFont::Medium);

    QLabel *versionTipLabel = new QLabel(QObject::tr("Version"));
    fontManager->bind(versionTipLabel, DFontSizeManager::T10, QFont::Normal);
    featureLabel = new QLabel(websiteLinkTemplate.arg(websiteLink).arg(QObject::tr("Features")));
    featureLabel->setContextMenuPolicy(Qt::NoContextMenu);
    featureLabel->setOpenExternalLinks(false);
    featureLabel->setVisible(!qApp->featureDisplayDialog()->isEmpty());
    redPointLabel = new DRedPointLabel();
    redPointLabel->setFixedSize(10, 10);
    QHBoxLayout *vFeatureLayout =  new QHBoxLayout;
    vFeatureLayout->setMargin(0);
    vFeatureLayout->setSpacing(0);
    vFeatureLayout->addWidget(featureLabel, 0, Qt::AlignLeft);
    vFeatureLayout->addWidget(redPointLabel, 0, Qt::AlignLeft);
    vFeatureLayout->addStretch(0);
    QLabel *homePageTipLabel = new QLabel(QObject::tr("Homepage"));
    fontManager->bind(homePageTipLabel, DFontSizeManager::T10, QFont::Normal);
    QLabel *descriptionTipLabel = new QLabel(QObject::tr("Description"));
    fontManager->bind(descriptionTipLabel, DFontSizeManager::T10, QFont::Normal);

    q->connect(websiteLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onLinkActivated(QString)));
    q->connect(featureLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onFeatureActivated(QString)));
    q->connect(descriptionLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onLinkActivated(QString)));
    q->connect(licenseLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onLinkActivated(QString)));

    QVBoxLayout *leftVLayout = new QVBoxLayout;
    leftVLayout->setContentsMargins(30, 3, 0, 20);
    leftVLayout->setSpacing(0);
    leftVLayout->addWidget(logoLabel, 0, Qt::AlignCenter);
    leftVLayout->addSpacing(8);
    leftVLayout->addWidget(productNameLabel, 0, Qt::AlignCenter);
    leftVLayout->addSpacing(31);
    leftVLayout->addWidget(companyLogoLabel, 0, Qt::AlignCenter);
    leftVLayout->addSpacing(3);
    leftVLayout->addWidget(licenseLabel, 0, Qt::AlignHCenter);
    leftVLayout->addStretch(0);

    QVBoxLayout *rightVLayout = new QVBoxLayout;
    rightVLayout->setContentsMargins(0, 3, 30, 20);
    rightVLayout->setSpacing(0);
    rightVLayout->addWidget(versionTipLabel, 0, Qt::AlignLeft);
    rightVLayout->addWidget(versionLabel, 0, Qt::AlignLeft);
    rightVLayout->addLayout(vFeatureLayout);
    rightVLayout->addSpacing(9);
    rightVLayout->addWidget(homePageTipLabel, 0, Qt::AlignLeft);
    rightVLayout->addWidget(websiteLabel, 0, Qt::AlignLeft);
    rightVLayout->addSpacing(10);
    rightVLayout->addWidget(descriptionTipLabel, 0, Qt::AlignLeft);
    rightVLayout->addWidget(descriptionLabel, 0, Qt::AlignLeft);
    rightVLayout->addStretch(0);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addLayout(leftVLayout);
    mainLayout->addLayout(rightVLayout);

    QScrollArea *mainScrollArea = new QScrollArea;
    QWidget  *mainContent = new QWidget;
    QPalette scrollPalette;

    scrollPalette.setBrush(QPalette::Background, Qt::transparent);
    mainScrollArea->setFrameShape(QFrame::NoFrame);
    mainScrollArea->setWidget(mainContent);
    mainScrollArea->setWidgetResizable(true);
    mainScrollArea->setPalette(scrollPalette);

    mainContent->setLayout(mainLayout);
    q->addContent(mainScrollArea);

    DConfig config("org.deepin.dtkwidget.feature-display");
    bool isUpdated = config.value("featureUpdated", false).toBool();
    redPointLabel->setVisible(isUpdated);
    // make active
    q->setFocus();
}

void DAboutDialogPrivate::loadDistributionInfo()
{
    logoPath = DSysInfo::distributionOrgLogo(DSysInfo::Distribution, DSysInfo::Light, ":/assets/images/deepin-logo.svg");
    auto websiteInfo = DSysInfo::distributionOrgWebsite(DSysInfo::Distribution); // will always return a valid result.
    websiteName = websiteInfo.first;
    websiteLink = websiteInfo.second;
}

void DAboutDialogPrivate::updateWebsiteLabel()
{
    QString websiteText = QString(websiteLinkTemplate).arg(websiteLink).arg(websiteName);
    websiteLabel->setText(websiteText);
}

void DAboutDialogPrivate::_q_onLinkActivated(const QString &link)
{
    DGUI_NAMESPACE::DGuiApplicationHelper::openUrl(link);
}

void DAboutDialogPrivate::_q_onFeatureActivated(const QString &)
{
    D_Q(DAboutDialog);
    DConfig config("org.deepin.dtkwidget.feature-display");
    if (config.value("featureUpdated", false).toBool()) {
        config.setValue("featureUpdated", false);
        redPointLabel->setVisible(false);
    }
    Q_EMIT q->featureActivated();
}

QPixmap DAboutDialogPrivate::loadPixmap(const QString &file)
{
    D_Q(DAboutDialog);

    qreal ratio = 1.0;

    const qreal devicePixelRatio = q->devicePixelRatioF();

    QPixmap pixmap;

    if (!qFuzzyCompare(ratio, devicePixelRatio)) {
        QImageReader reader;
        reader.setFileName(qt_findAtNxFile(file, devicePixelRatio, &ratio));
        if (reader.canRead()) {
            reader.setScaledSize(reader.size() * (devicePixelRatio / ratio));
            pixmap = QPixmap::fromImage(reader.read());
            pixmap.setDevicePixelRatio(devicePixelRatio);
        }
    } else {
        pixmap.load(file);
    }

    return pixmap;
}

/*!
@~english
  @class Dtk::Widget::DAboutDialog
  @ingroup dtkwidget
  @brief The DaboutDialog class provides the application about dialog boxes that specify all Deepin applications about window design specifications, which meets the deepin style.

  The windows created using dmainwindow can be made in the menu about the pop -up window. Generally, it is not necessary to create manually.

  In order to provide simple operations, the content of the dialog box can be set through dapplication。
 */

DAboutDialog::DAboutDialog(QWidget *parent)
    : DDialog(*new DAboutDialogPrivate(this), parent)
{
    D_D(DAboutDialog);

    d->init();
}

/*!
@~english
  @property DAboutDialog::windowTitle
  @brief the title of the dialog.
 */
QString DAboutDialog::windowTitle() const
{
    return title();
}

/*!
@~english
  @property DAboutDialog::productName

  @brief the product name to be shown on the dialog.
 */
QString DAboutDialog::productName() const
{
    D_DC(DAboutDialog);

    return d->productNameLabel->text();
}

/*!
@~english
  @property DAboutDialog::version

  @brief the version number to be shown on the dialog.
  @brief 返回关于对话框显示的版本.
 */
QString DAboutDialog::version() const
{
    D_DC(DAboutDialog);

    return d->versionLabel->text();
}

/*!
@~english
  @property DAboutDialog::description

  @brief the description to be show on the dialog.
  @brief 返回关于对话框显示的描述.
 */
QString DAboutDialog::description() const
{
    D_DC(DAboutDialog);

    return d->descriptionLabel->text();
}

/*!
@~english
  @brief the vendor logo to be shown on the dialog.
  @return 返回对话框中的公司/组织 logo 图片.
 */
const QPixmap *DAboutDialog::companyLogo() const
{
    D_DC(DAboutDialog);

    return d->companyLogoLabel->pixmap();
}

/*!
@~english
  @property DAboutDialog::websiteName
  @brief the vendor website name to be shown on the dialog.
  Usually be in form like www.deepin.org.
 */
QString DAboutDialog::websiteName() const
{
    D_DC(DAboutDialog);

    return d->websiteName;
}

/*!
@~english
  @property DAboutDialog::websiteLink
  @brief the corresponding web address of websiteName().
  @brief 返回 websiteName() 相应的网址.
  The website link will be open in the browser if the user clicks on
  the website text shown on the dialog.
 */
QString DAboutDialog::websiteLink() const
{
    D_DC(DAboutDialog);

    return d->websiteLink;
}

/*!
@~english
  @property DAboutDialog::acknowledgementLink
  @brief the web address to be open open when user clicks on the "Acknowlegement"
  text show on the dialog.
 */
QString DAboutDialog::acknowledgementLink() const
{
    return QString();
}

/*!
@~english
  @property DAboutDialog::license
  @brief the license to be shown on the dialog.
 */
QString DAboutDialog::license() const
{
    D_DC(DAboutDialog);

    return d->licenseLabel->text();
}

/*!
@~english
  @brief Set the title of the dialog box window.
  \a Window title string.
 */
void DAboutDialog::setWindowTitle(const QString &windowTitle)
{
    setTitle(windowTitle);
}

/*!
@~english
  @brief Set the icon icon displayed.

  In the icon of the dialog box display.
 */
void DAboutDialog::setProductIcon(const QIcon &icon)
{
    D_D(DAboutDialog);

    d->logoLabel->setPixmap(icon.pixmap(windowHandle(), QSize(128, 128)));
}

/*!
@~english
  @brief Set the application name.
 */
void DAboutDialog::setProductName(const QString &productName)
{
    D_D(DAboutDialog);

    d->productNameLabel->setText(productName);
}

/*!
@~english
  @brief This function is used to set the specified version information.
 */
void DAboutDialog::setVersion(const QString &version)
{
    D_D(DAboutDialog);

    d->versionLabel->setText(version);
}

/*!
@~english
  @brief This function is used to set the specified description description information.
 */
void DAboutDialog::setDescription(const QString &description)
{
    D_D(DAboutDialog);

    d->descriptionLabel->setText(description);
}

/*!
@~english
  @brief This function is used to set the specified CompanyLogo organization logo.
 */
void DAboutDialog::setCompanyLogo(const QPixmap &companyLogo)
{
    D_D(DAboutDialog);

    d->companyLogoLabel->setPixmap(companyLogo);
}

/*!
@~english
  @brief This function is used to set the specified websitename website name
 */
void DAboutDialog::setWebsiteName(const QString &websiteName)
{
    D_D(DAboutDialog);

    if (d->websiteName == websiteName) {
        return;
    }

    d->websiteName = websiteName;
    d->updateWebsiteLabel();
}

/*!
@~english
  @brief This function is used to set the specified WebSitelink website link
 */
void DAboutDialog::setWebsiteLink(const QString &websiteLink)
{
    D_D(DAboutDialog);

    if (d->websiteLink == websiteLink) {
        return;
    }

    d->websiteLink = websiteLink;
    d->updateWebsiteLabel();
}

/*!
@~english
  @brief This function is used to set the specified ACKNOWLEDGEMENTLINK Link
 */
void DAboutDialog::setAcknowledgementLink(const QString &)
{
}

/*!
@~english
  @brief This function is used to set the specified Visible settings to set the gratitude link to display
 */
void DAboutDialog::setAcknowledgementVisible(bool)
{
}

/*!
@~english
  @brief This function is used to set the specified License license.
 */
void DAboutDialog::setLicense(const QString &license)
{
    D_D(DAboutDialog);

    d->licenseLabel->setText(license);
    d->licenseLabel->setVisible(!license.isEmpty());
}

void DAboutDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();
        event->accept();
    }

    DDialog::keyPressEvent(event);
}

void DAboutDialog::showEvent(QShowEvent *event)
{
    DDialog::showEvent(event);

    if (minimumWidth() == maximumWidth()) {
        resize(width(), heightForWidth(width()));
    } else {
        adjustSize();
    }
}

DWIDGET_END_NAMESPACE

#include "moc_daboutdialog.cpp"
