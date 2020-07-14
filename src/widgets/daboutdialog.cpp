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

#include "daboutdialog.h"
#include "private/daboutdialog_p.h"

#include <dwidgetutil.h>
#include <DSysInfo>

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QKeyEvent>
#include <QApplication>
#include <QImageReader>
#include <DSysInfo>
#include <QScrollArea>

#ifdef Q_OS_UNIX
#include <unistd.h>
#include <pwd.h>
#endif

DCORE_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

#ifdef Q_OS_UNIX
class EnvReplaceGuard
{
public:
    EnvReplaceGuard(const int uid);
    ~EnvReplaceGuard();

    char *m_backupLogName;
    char *m_backupHome;
};

EnvReplaceGuard::EnvReplaceGuard(const int uid)
{
    m_backupLogName = getenv("LOGNAME");
    m_backupHome = getenv("HOME");

    struct passwd *pwd = getpwuid(uid);

    setenv("LOGNAME", pwd->pw_name, 1);
    setenv("HOME", pwd->pw_dir, 1);
}

EnvReplaceGuard::~EnvReplaceGuard()
{
    setenv("LOGNAME", m_backupLogName, 1);
    setenv("HOME", m_backupHome, 1);
}
#endif

const QString DAboutDialogPrivate::websiteLinkTemplate = "<a href='%1' style='text-decoration: none; font-size:13px; color: #004EE5;'>%2</a>";

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

    versionLabel = new QLabel();
    versionLabel->setObjectName("VersionLabel");

    companyLogoLabel = new QLabel();
    companyLogoLabel->setPixmap(loadPixmap(logoPath));

    websiteLabel = new QLabel();
    websiteLabel->setObjectName("WebsiteLabel");
    websiteLabel->setContextMenuPolicy(Qt::NoContextMenu);
    websiteLabel->setOpenExternalLinks(false);
    updateWebsiteLabel();

    acknowledgementLabel = new QLabel();
    acknowledgementLabel->setObjectName("AcknowledgementLabel");
    acknowledgementLabel->setContextMenuPolicy(Qt::NoContextMenu);
    acknowledgementLabel->setOpenExternalLinks(false);
    updateAcknowledgementLabel();

    descriptionLabel = new QLabel();
    descriptionLabel->setObjectName("DescriptionLabel");
    descriptionLabel->setAlignment(Qt::AlignHCenter);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    licenseLabel = new QLabel();
    licenseLabel->setObjectName("LicenseLabel");
    licenseLabel->setAlignment(Qt::AlignHCenter);
    licenseLabel->setWordWrap(true);
    licenseLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    licenseLabel->hide();

    q->connect(websiteLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onLinkActivated(QString)));
    q->connect(acknowledgementLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onLinkActivated(QString)));
    q->connect(descriptionLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onLinkActivated(QString)));
    q->connect(licenseLabel, SIGNAL(linkActivated(QString)), q, SLOT(_q_onLinkActivated(QString)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(11, 20, 11, 10);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(logoLabel);
    mainLayout->setAlignment(logoLabel, Qt::AlignCenter);
    mainLayout->addSpacing(3);
    mainLayout->addWidget(productNameLabel);
    mainLayout->setAlignment(productNameLabel, Qt::AlignCenter);
    mainLayout->addSpacing(6);
    mainLayout->addWidget(versionLabel);
    mainLayout->setAlignment(versionLabel, Qt::AlignCenter);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(companyLogoLabel);
    mainLayout->setAlignment(companyLogoLabel, Qt::AlignCenter);
//    mainLayout->addSpacing(6);
    mainLayout->addWidget(websiteLabel);
    mainLayout->setAlignment(websiteLabel, Qt::AlignCenter);
    mainLayout->addSpacing(5);
//    mainLayout->addWidget(acknowledgementLabel);
//    mainLayout->setAlignment(acknowledgementLabel, Qt::AlignCenter);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(descriptionLabel, Qt::AlignHCenter);
    mainLayout->addSpacing(7);
    mainLayout->addWidget(licenseLabel, Qt::AlignHCenter);

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

void DAboutDialogPrivate::updateAcknowledgementLabel()
{
    QString acknowledgementText = QString(websiteLinkTemplate).arg(acknowledgementLink).arg(QApplication::translate("DAboutDialog", "Acknowledgements"));
    acknowledgementLabel->setText(acknowledgementText);
}

void DAboutDialogPrivate::_q_onLinkActivated(const QString &link)
{
#ifdef Q_OS_UNIX
    // workaround for pkexec apps
    bool ok = false;
    const int pkexecUid = qEnvironmentVariableIntValue("PKEXEC_UID", &ok);

    if (ok)
    {
        EnvReplaceGuard _env_guard(pkexecUid);
        Q_UNUSED(_env_guard);

        QDesktopServices::openUrl(QUrl(link));
    }
    else
#endif
    {
        QDesktopServices::openUrl(QUrl(link));
    }
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
 * \~chinese \class DAboutDialog
 * \~chinese \brief DAboutDialog 类提供了应用程序的关于对话框，规范所有 deepin 应用关于窗口设计规范，符合 Deepin 风格。
 *
 * \~chinese 使用 DMainWindow 创建的窗口都可以在菜单点关于弹出关于窗口，一般不需要手动创建。
 *
 * \~chinese 为了提供简便操作，可通过 DApplication 来设置关于对话框展示内容。
 */

DAboutDialog::DAboutDialog(QWidget *parent)
    : DDialog(*new DAboutDialogPrivate(this), parent)
{
    D_D(DAboutDialog);

    d->init();
}

/*!
 * \~english \property DAboutDialog::windowTitle
 *
 * \~english \brief the title of the dialog.
 */

/*!
 * \~chinese \property DAboutDialog::windowTitle
 *
 * \~chinese \brief 返回关于对话框窗口的标题
 */
QString DAboutDialog::windowTitle() const
{
    return title();
}

/*!
 * \~english \property DAboutDialog::productName
 *
 * \~english \brief the product name to be shown on the dialog.
 */

/*!
 * \~chinese \property DAboutDialog::productName
 *
 * \~chinese \brief 返回对话框显示的应用名称
 */
QString DAboutDialog::productName() const
{
    D_DC(DAboutDialog);

    return d->productNameLabel->text();
}

/*!
 * \~english \property DAboutDialog::version
 *
 * \~english \brief the version number to be shown on the dialog.
 */

/*!
 * \~chinese \property DAboutDialog::version
 *
 * \~chinese \brief 返回关于对话框显示的版本
 */
QString DAboutDialog::version() const
{
    D_DC(DAboutDialog);

    return d->versionLabel->text();
}

/*!
 * \~english \property DAboutDialog::description
 * \~english \brief the description to be show on the dialog.
 */

/*!
 * \~chinese \property DAboutDialog::description
 * \~chinese \brief 返回关于对话框显示的描述
 */
QString DAboutDialog::description() const
{
    D_DC(DAboutDialog);

    return d->descriptionLabel->text();
}

/*!
 * \~english \property DAboutDialog::companyLogo
 * \~english \brief the vendor logo to be shown on the dialog.
 */

/*!
 * \~chinese \property DAboutDialog::companyLogo
 * \~chinese \brief 返回对话框中的公司/组织 logo 图片
 */
const QPixmap *DAboutDialog::companyLogo() const
{
    D_DC(DAboutDialog);

    return d->companyLogoLabel->pixmap();
}

/*!
 * \~english \property DAboutDialog::websiteName
 * \~english \brief the vendor website name to be shown on the dialog.
 *
 * \~english Usually be in form like www.deepin.org.
 */

/*!
 * \~chinese \property DAboutDialog::websiteName
 * \~chinese \brief 返回对话框中显示的公司/组织网站名称
 *
 * \~chinese 通常采用 www.deepin.org 等形式。
 */
QString DAboutDialog::websiteName() const
{
    D_DC(DAboutDialog);

    return d->websiteName;
}

/*!
 * \~english \property DAboutDialog::websiteLink
 * \~english \brief the corresponding web address of websiteName()
 *
 * \~english The website link will be open in the browser if the user clicks on
 * \~english the website text shown on the dialog.
 */

/*!
 * \~chinese \property DAboutDialog::websiteLink
 * \~chinese \brief 返回 websiteName() 相应的网址
 *
 * \~chinese 如果用户点击对话框中显示的网址，则会打开相应的链接。
 */
QString DAboutDialog::websiteLink() const
{
    D_DC(DAboutDialog);

    return d->websiteLink;
}

/*!
 * \~english \property DAboutDialog::acknowledgementLink
 * \~english \brief the web address to be open open when user clicks on the "Acknowlegement"
 * \~english text show on the dialog.
 */

/*!
 * \~chinese \property DAboutDialog::acknowledgementLink
 * \~chinese \brief 返回鸣谢链接地址
 */
QString DAboutDialog::acknowledgementLink() const
{
    D_DC(DAboutDialog);

    return d->acknowledgementLink;
}

/*!
 * \~english \property DAboutDialog::license
 * \~english \brief the license to be shown on the dialog.
 */

/*!
 * \~chinese \property DAboutDialog::license
 * \~chinese \brief 对话框显示的许可证
 */
QString DAboutDialog::license() const
{
    D_DC(DAboutDialog);

    return d->licenseLabel->text();
}

/*!
 * \~chinese \property DAboutDialog::setWindowTitle
 * \~chinese \brief 设置对话框窗口标题
 */
void DAboutDialog::setWindowTitle(const QString &windowTitle)
{
    setTitle(windowTitle);
}

/*!
 * \~chinese \property DAboutDialog::setProductIcon
 * \~chinese \brief 设置展示的图标
 *
 * \~chinese 在关于对话框展示的图标
 */
void DAboutDialog::setProductIcon(const QIcon &icon)
{
    D_D(DAboutDialog);

    d->logoLabel->setPixmap(icon.pixmap(windowHandle(), QSize(96, 96)));
}

/*!
 * \~chinese \property DAboutDialog::setProductName
 * \~chinese \brief 设置应用名称
 */
void DAboutDialog::setProductName(const QString &productName)
{
    D_D(DAboutDialog);

    d->productNameLabel->setText(productName);
}

/*!
 * \~chinese \property DAboutDialog::setVersion
 * \~chinese \brief 设置版本
 */
void DAboutDialog::setVersion(const QString &version)
{
    D_D(DAboutDialog);

    d->versionLabel->setText(version);
}

/*!
 * \~chinese \property DAboutDialog::setDescription
 * \~chinese \brief 设置描述信息
 */
void DAboutDialog::setDescription(const QString &description)
{
    D_D(DAboutDialog);

    d->descriptionLabel->setText(description);
}

/*!
 * \~chinese \property DAboutDialog::setCompanyLogo
 * \~chinese \brief 设置组织标志
 */
void DAboutDialog::setCompanyLogo(const QPixmap &companyLogo)
{
    D_D(DAboutDialog);

    d->companyLogoLabel->setPixmap(companyLogo);
}

/*!
 * \~chinese \property DAboutDialog::setWebsiteName
 * \~chinese \brief 设置网站名称
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
 * \~chinese \property DAboutDialog::setWebsiteLink
 * \~chinese \brief 设置网站链接
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
 * \~chinese \property DAboutDialog::setAcknowledgementLink
 * \~chinese \brief 设置鸣谢链接
 */
void DAboutDialog::setAcknowledgementLink(const QString &acknowledgementLink)
{
    D_D(DAboutDialog);

    d->acknowledgementLink = acknowledgementLink;
    d->updateAcknowledgementLabel();
}

/*!
 * \~chinese \property DAboutDialog::setAcknowledgementVisible
 * \~chinese \brief 设置鸣谢链接是否显示
 */
void DAboutDialog::setAcknowledgementVisible(bool visible)
{
    Q_UNUSED(visible)
    D_D(DAboutDialog);
//    d->acknowledgementLabel->setVisible(visible);
}

/*!
 * \~chinese \property DAboutDialog::setLicense
 * \~chinese \brief 设置许可证
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
