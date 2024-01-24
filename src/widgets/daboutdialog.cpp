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
#include <QWindow>
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

const QString DAboutDialogPrivate::websiteLinkTemplate = "<a href='%1' style='text-decoration: none; font-size:12px; color: rgba(0,129,255,0.9);'>%2</a>";

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

    q->setFixedSize(540, 290);

    // overwrite default info if distribution config file existed.
    loadDistributionInfo();

    logoLabel = new QLabel();
    logoLabel->setContentsMargins(0, 0, 0, 0);
    logoLabel->setAlignment(Qt::AlignCenter);

    productNameLabel = new QLabel();
    productNameLabel->setForegroundRole(QPalette::BrightText);
    productNameLabel->setObjectName("ProductNameLabel");
    productNameLabel->setWordWrap(true);
    productNameLabel->setAlignment(Qt::AlignCenter);
    DFontSizeManager *fontManager =  DFontSizeManager::instance();
    fontManager->bind(productNameLabel, DFontSizeManager::T5, QFont::Medium);

    versionLabel = new QLabel();
    versionLabel->setObjectName("VersionLabel");
    versionLabel->setForegroundRole(QPalette::BrightText);
    fontManager->bind(versionLabel, DFontSizeManager::T8, QFont::Medium);

    companyLogoLabel = new QLabel();
    companyLogoLabel->setPixmap(loadPixmap(logoPath));
    companyLogoLabel->hide();

    websiteLabel = new QLabel();
    websiteLabel->setObjectName("WebsiteLabel");
    websiteLabel->setContextMenuPolicy(Qt::NoContextMenu);
    websiteLabel->setOpenExternalLinks(false);
    updateWebsiteLabel();

    descriptionLabel = new QLabel();
    descriptionLabel->setForegroundRole(QPalette::BrightText);
    descriptionLabel->setFixedWidth(280);
    descriptionLabel->setObjectName("DescriptionLabel");
    descriptionLabel->setAlignment(Qt::AlignLeft);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    fontManager->bind(descriptionLabel, DFontSizeManager::T8, QFont::Medium);

    licenseLabel = new QLabel();
    licenseLabel->setObjectName("LicenseLabel");
    licenseLabel->setWordWrap(true);
    licenseLabel->setForegroundRole(QPalette::BrightText);
    licenseLabel->setFixedWidth(280);
    licenseLabel->setAlignment(Qt::AlignLeft);
    licenseLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    fontManager->bind(licenseLabel, DFontSizeManager::T8, QFont::Medium);
    licenseLabel->hide();

    QLabel *versionTipLabel = new QLabel(QObject::tr("Version"));
    fontManager->bind(versionTipLabel, DFontSizeManager::T10, QFont::Thin);
    featureLabel = new QLabel(websiteLinkTemplate.arg(websiteLink).arg(QObject::tr("Features")));
    featureLabel->setContextMenuPolicy(Qt::NoContextMenu);
    featureLabel->setOpenExternalLinks(false);
    if (qobject_cast<DApplication *>(qApp))
      featureLabel->setVisible(!qApp->featureDisplayDialog()->isEmpty());
    else
      featureLabel->setVisible(false);
    redPointLabel = new DRedPointLabel();
    redPointLabel->setFixedSize(10, 10);
    QHBoxLayout *vFeatureLayout =  new QHBoxLayout;
    vFeatureLayout->setContentsMargins(0, 0, 0, 0);
    vFeatureLayout->setSpacing(0);
    vFeatureLayout->addWidget(featureLabel, 0, Qt::AlignLeft);
    vFeatureLayout->addWidget(redPointLabel, 0, Qt::AlignLeft);
    vFeatureLayout->addStretch(0);
    QLabel *homePageTipLabel = new QLabel(QObject::tr("Homepage"));
    fontManager->bind(homePageTipLabel, DFontSizeManager::T10, QFont::Thin);
    QLabel *descriptionTipLabel = new QLabel(QObject::tr("Description"));
    fontManager->bind(descriptionTipLabel, DFontSizeManager::T10, QFont::Thin);
    acknowledgementTipLabel = new QLabel(QObject::tr("Acknowledgements"));
    fontManager->bind(acknowledgementTipLabel, DFontSizeManager::T10, QFont::Thin);
    licenseTipLabel = new QLabel(QObject::tr("License"));
    fontManager->bind(licenseTipLabel, DFontSizeManager::T10, QFont::Thin);
    licenseTipLabel->hide();

    acknowledgementLabel = new QLabel(QObject::tr("Sincerely appreciate the open-source software used."));
    acknowledgementLabel->setForegroundRole(QPalette::BrightText);
    acknowledgementLabel->setFixedWidth(280);
    acknowledgementLabel->setWordWrap(true);
    acknowledgementLabel->setContextMenuPolicy(Qt::NoContextMenu);
    acknowledgementLabel->setOpenExternalLinks(false);
    fontManager->bind(acknowledgementLabel, DFontSizeManager::T8, QFont::Medium);

    q->connect(websiteLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onLinkActivated(QString)));
    q->connect(featureLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onFeatureActivated(QString)));
    q->connect(descriptionLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onLinkActivated(QString)));
    q->connect(licenseLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onLinkActivated(QString)));
    q->connect(acknowledgementLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onLicenseActivated(QString)));

    QScrollArea *productNameScrollArea = new QScrollArea;
    productNameScrollArea->setMaximumHeight(50);
    productNameScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    productNameScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    QPalette scrollPalette;

    scrollPalette.setBrush(QPalette::Window, Qt::transparent);
    productNameScrollArea->setFrameShape(QFrame::NoFrame);
    productNameScrollArea->setWidget(productNameLabel);
    productNameScrollArea->setWidgetResizable(true);
    productNameScrollArea->setPalette(scrollPalette);

    QVBoxLayout *leftVLayout = new QVBoxLayout;
    leftVLayout->setContentsMargins(0, 10, 0, 0);
    leftVLayout->addWidget(logoLabel);
    leftVLayout->addSpacing(8);
    leftVLayout->addWidget(productNameScrollArea);
    leftVLayout->addSpacing(16);
    leftVLayout->addWidget(companyLogoLabel);
    leftVLayout->addSpacing(3);
    leftVLayout->addStretch(0);

    QVBoxLayout *rightVLayout = new QVBoxLayout;
    rightVLayout->setContentsMargins(0, 0, 10, 0);
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
    rightVLayout->addSpacing(10);
    rightVLayout->addWidget(acknowledgementTipLabel, 0, Qt::AlignLeft);
    rightVLayout->addWidget(acknowledgementLabel, 0, Qt::AlignLeft);
    rightVLayout->addSpacing(10);
    rightVLayout->addWidget(licenseTipLabel, 0, Qt::AlignLeft);
    rightVLayout->addWidget(licenseLabel, 0, Qt::AlignLeft);
    rightVLayout->addStretch(0);

    QScrollArea *rightScrollArea = new QScrollArea;
    rightScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    rightScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    QWidget  *rightContent = new QWidget;
    rightContent->setLayout(rightVLayout);

    rightScrollArea->setFrameShape(QFrame::NoFrame);
    rightScrollArea->setWidget(rightContent);
    rightScrollArea->setWidgetResizable(true);
    rightScrollArea->setPalette(scrollPalette);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addLayout(leftVLayout, 2);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(rightScrollArea, 3);

    QWidget  *mainContent = new QWidget;

    mainContent->setLayout(mainLayout);
    q->addContent(mainContent);
    q->setContentsMargins(0, 0, 0, 10);

    DConfig config("org.deepin.dtk.preference");
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
    DConfig config("org.deepin.dtk.preference");
    if (config.value("featureUpdated", false).toBool()) {
        config.setValue("featureUpdated", false);
        redPointLabel->setVisible(false);
    }
    Q_EMIT q->featureActivated();
}

void DAboutDialogPrivate::_q_onLicenseActivated(const QString &)
{
    D_Q(DAboutDialog);
    Q_EMIT q->licenseActivated();
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
const QPixmap *DAboutDialog::companyLogo() const
#else
QPixmap DAboutDialog::companyLogo() const
#endif
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
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
QString DAboutDialog::acknowledgementLink() const
{
    return QString();
}
#endif
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

void DAboutDialog::setLicenseEnabled(bool enabled)
{
    D_D(DAboutDialog);
    QString ack = QObject::tr("Sincerely appreciate the open-source software used.");
    if (enabled) {
        QString tmp = QObject::tr("open-source software");
        ack = ack.replace(tmp, d->websiteLinkTemplate.arg(d->websiteLink).arg(tmp));
    }
    d->acknowledgementLabel->setText(ack);
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    d->logoLabel->setPixmap(icon.pixmap(windowHandle(), QSize(128, 128)));
#else
    winId(); // TODO: wait for checking
    auto window = windowHandle();
    d->logoLabel->setPixmap(icon.pixmap(window->baseSize(), window->screen()->devicePixelRatio()));
#endif
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
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
void DAboutDialog::setAcknowledgementLink(const QString &)
{
}
#endif
/*!
@~english
  @brief This function is used to set the specified Visible settings to set the gratitude link to display
 */
void DAboutDialog::setAcknowledgementVisible(bool isVisible)
{
    D_D(DAboutDialog);
    d->acknowledgementTipLabel->setVisible(isVisible);
    d->acknowledgementLabel->setVisible(isVisible);
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
    d->licenseTipLabel->setVisible(!license.isEmpty());
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
