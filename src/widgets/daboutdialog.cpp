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
  \class Dtk::Widget::DAboutDialog
  \inmodule dtkwidget
  \brief DAboutDialog 类提供了应用程序的关于对话框，规范所有 deepin 应用关于窗口设计规范，符合 Deepin 风格.
  
  使用 DMainWindow 创建的窗口都可以在菜单点关于弹出关于窗口，一般不需要手动创建。
  
  为了提供简便操作，可通过 DApplication 来设置关于对话框展示内容。
 */

DAboutDialog::DAboutDialog(QWidget *parent)
    : DDialog(*new DAboutDialogPrivate(this), parent)
{
    D_D(DAboutDialog);

    d->init();
}

/*!
  \property DAboutDialog::windowTitle
  
  \brief the title of the dialog.
  \brief 返回关于对话框窗口的标题.
 */
QString DAboutDialog::windowTitle() const
{
    return title();
}

/*!
  \property DAboutDialog::productName
  
  \brief the product name to be shown on the dialog.
  \brief 返回对话框显示的应用名称.
 */
QString DAboutDialog::productName() const
{
    D_DC(DAboutDialog);

    return d->productNameLabel->text();
}

/*!
  \property DAboutDialog::version
  
  \brief the version number to be shown on the dialog.
  \brief 返回关于对话框显示的版本.
 */
QString DAboutDialog::version() const
{
    D_DC(DAboutDialog);

    return d->versionLabel->text();
}

/*!
  \property DAboutDialog::description

  \brief the description to be show on the dialog.
  \brief 返回关于对话框显示的描述.
 */
QString DAboutDialog::description() const
{
    D_DC(DAboutDialog);

    return d->descriptionLabel->text();
}

/*!
  \brief the vendor logo to be shown on the dialog.
  \return 返回对话框中的公司/组织 logo 图片.
 */
const QPixmap *DAboutDialog::companyLogo() const
{
    D_DC(DAboutDialog);

    return d->companyLogoLabel->pixmap();
}

/*!
  \property DAboutDialog::websiteName
  \brief the vendor website name to be shown on the dialog.
  \brief 返回对话框中显示的公司/组织网站名称.
  
  Usually be in form like www.deepin.org.
  通常采用 www.deepin.org 等形式。
 */
QString DAboutDialog::websiteName() const
{
    D_DC(DAboutDialog);

    return d->websiteName;
}

/*!
  \property DAboutDialog::websiteLink
  \brief the corresponding web address of websiteName().
  \brief 返回 websiteName() 相应的网址.
  
  The website link will be open in the browser if the user clicks on
  the website text shown on the dialog.
  如果用户点击对话框中显示的网址，则会打开相应的链接。
 */
QString DAboutDialog::websiteLink() const
{
    D_DC(DAboutDialog);

    return d->websiteLink;
}

/*!
  \property DAboutDialog::acknowledgementLink

  \brief the web address to be open open when user clicks on the "Acknowlegement"
  text show on the dialog.
  \brief 返回鸣谢链接地址.
 */
QString DAboutDialog::acknowledgementLink() const
{
    D_DC(DAboutDialog);

    return d->acknowledgementLink;
}

/*!
  \property DAboutDialog::license

  \brief the license to be shown on the dialog.
  \brief 对话框显示的许可证.
 */
QString DAboutDialog::license() const
{
    D_DC(DAboutDialog);

    return d->licenseLabel->text();
}

/*!
  \brief 设置对话框窗口标题.

  \a windowTitle 窗口标题字符串.
 */
void DAboutDialog::setWindowTitle(const QString &windowTitle)
{
    setTitle(windowTitle);
}

/*!
  \brief 设置展示的 \a icon 图标.
  
  在关于对话框展示的图标.
 */
void DAboutDialog::setProductIcon(const QIcon &icon)
{
    D_D(DAboutDialog);

    d->logoLabel->setPixmap(icon.pixmap(windowHandle(), QSize(96, 96)));
}

/*!
  \brief 设置应用名称.
  \a productName 产品名称.
 */
void DAboutDialog::setProductName(const QString &productName)
{
    D_D(DAboutDialog);

    d->productNameLabel->setText(productName);
}

/*!
  \brief 此函数用于设置指定的 \a version 版本信息.
 */
void DAboutDialog::setVersion(const QString &version)
{
    D_D(DAboutDialog);

    d->versionLabel->setText(version);
}

/*!
  \brief 此函数用于设置指定的 \a description 描述信息.
 */
void DAboutDialog::setDescription(const QString &description)
{
    D_D(DAboutDialog);

    d->descriptionLabel->setText(description);
}

/*!
  \brief 此函数用于设置指定的 \a companyLogo 组织标志.
 */
void DAboutDialog::setCompanyLogo(const QPixmap &companyLogo)
{
    D_D(DAboutDialog);

    d->companyLogoLabel->setPixmap(companyLogo);
}

/*!
  \brief 此函数用于设置指定的 \a websiteName 网站名称
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
  \brief 此函数用于设置指定的 \a websiteLink 网站链接
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
  \brief 此函数用于设置指定的 \a acknowledgementLink 鸣谢链接
 */
void DAboutDialog::setAcknowledgementLink(const QString &acknowledgementLink)
{
    D_D(DAboutDialog);

    d->acknowledgementLink = acknowledgementLink;
    d->updateAcknowledgementLabel();
}

/*!
  \brief 此函数用于设置指定的 \a visible 设置鸣谢链接是否显示
 */
void DAboutDialog::setAcknowledgementVisible(bool visible)
{
    Q_UNUSED(visible)
    D_D(DAboutDialog);
//    d->acknowledgementLabel->setVisible(visible);
}

/*!
  \brief 此函数用于设置指定的 \a license 许可证.
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
