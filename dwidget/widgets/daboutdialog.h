#pragma once

#include <DWindow>

DWIDGET_BEGIN_NAMESPACE

class DAboutDialog : public DWindow
{
    Q_OBJECT

    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle NOTIFY windowTitleChanged)
    Q_PROPERTY(QPixmap windowIcon READ windowIcon WRITE setWindowIcon NOTIFY windowIconChanged)
    Q_PROPERTY(QPixmap productIcon READ productIcon WRITE setProductIcon NOTIFY productIconChanged)
    Q_PROPERTY(QString productName READ productName WRITE setProductName NOTIFY productNameChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QPixmap companyLogo READ companyLogo WRITE setCompanyLogo NOTIFY companyLogoChanged)
    Q_PROPERTY(QString websiteName READ websiteName WRITE setWebsiteName NOTIFY websiteNameChanged)
    Q_PROPERTY(QString websiteLink READ websiteLink WRITE setWebsiteLink NOTIFY websiteLinkChanged)
    Q_PROPERTY(QString acknowledgementLink READ acknowledgementLink WRITE setAcknowledgementLink NOTIFY acknowledgementLinkChanged)


public:
    DAboutDialog(const QPixmap &windowIcon,
                 const QPixmap &productIcon,
                 const QString &productName,
                 const QString &version,
                 const QString &description,
                 const QString &acknowledgementLink,
                 QWidget *parent = nullptr);


    DAboutDialog(const QString &windowIcon,
                 const QString &productIcon,
                 const QString &productName,
                 const QString &version,
                 const QString &description,
                 QWidget *parent = nullptr,
                 const QString &companyLogo = ":/images/deepin-logo.png",
                 const QString &websiteName = "www.deepin.org",
                 const QString &websiteLink = "https://www.deepin.org");

    DAboutDialog(
        const QString &windowTitle,
        const QString &windowIcon,
        const QString &productIcon,
        const QString &productName,
        const QString &version,
        const QString &description,
        QWidget *parent = nullptr,
        const QString &companyLogo = ":/images/deepin-logo.png",
        const QString &website = "www.deepin.org",
        const QString &websiteLink = "https://www.deepin.org");

public:
    QString windowTitle() const
    {
        return m_windowTitle;
    }

    QPixmap windowIcon() const
    {
        return m_windowIcon;
    }

    QPixmap productIcon() const
    {
        return m_productIcon;
    }

    QString productName() const
    {
        return m_productName;
    }

    QString version() const
    {
        return m_version;
    }

    QString description() const
    {
        return m_description;
    }

    QPixmap companyLogo() const
    {
        return m_companyLogo;
    }

    QString websiteName() const
    {
        return m_websiteName;
    }

    QString websiteLink() const
    {
        return m_websiteLink;
    }

    QString acknowledgementLink() const
    {
        return m_acknowledgementLink;
    }

public slots:
    void setWindowTitle(QString windowTitle)
    {
        if (m_windowTitle == windowTitle) {
            return;
        }

        m_windowTitle = windowTitle;
        emit windowTitleChanged(windowTitle);
    }

    void setWindowIcon(QPixmap windowIcon)
    {
//        if (m_windowIcon == windowIcon) {
//            return;
//        }

        m_windowIcon = windowIcon;
        emit windowIconChanged(windowIcon);
    }

    void setProductIcon(QPixmap productIcon)
    {
//        if (m_productIcon == productIcon) {
//            return;
//        }

        m_productIcon = productIcon;
        emit productIconChanged(productIcon);
    }

    void setProductName(QString productName)
    {
        if (m_productName == productName) {
            return;
        }

        m_productName = productName;
        emit productNameChanged(productName);
    }

    void setVersion(QString version)
    {
        if (m_version == version) {
            return;
        }

        m_version = version;
        emit versionChanged(version);
    }

    void setDescription(QString description)
    {
        if (m_description == description) {
            return;
        }

        m_description = description;
        emit descriptionChanged(description);
    }

    void setCompanyLogo(QPixmap companyLogo)
    {
//        if (m_companyLogo == companyLogo) {
//            return;
//        }

        m_companyLogo = companyLogo;
        emit companyLogoChanged(companyLogo);
    }

    void setWebsiteName(QString websiteName)
    {
        if (m_websiteName == websiteName) {
            return;
        }

        m_websiteName = websiteName;
        emit websiteNameChanged(websiteName);
    }

    void setWebsiteLink(QString websiteLink)
    {
        if (m_websiteLink == websiteLink) {
            return;
        }

        m_websiteLink = websiteLink;
        emit websiteLinkChanged(websiteLink);
    }

    void setAcknowledgementLink(QString acknowledgementLink)
    {
        if (m_acknowledgementLink == acknowledgementLink) {
            return;
        }

        m_acknowledgementLink = acknowledgementLink;
        emit acknowledgementLinkChanged(acknowledgementLink);
    }

signals:
    void windowTitleChanged(QString windowTitle);
    void windowIconChanged(QPixmap windowIcon);
    void productIconChanged(QPixmap productIcon);
    void productNameChanged(QString productName);
    void versionChanged(QString version);
    void descriptionChanged(QString description);
    void companyLogoChanged(QPixmap companyLogo);
    void websiteNameChanged(QString websiteName);
    void websiteLinkChanged(QString websiteLink);
    void acknowledgementLinkChanged(QString acknowledgementLink);

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void onLinkActivated(const QString &link);

private:
    void initUI();
    static const QString websiteLinkTemplate;

    QString m_windowTitle;
    QPixmap m_windowIcon;
    QPixmap m_productIcon;
    QString m_productName;
    QString m_version;
    QString m_description;
    QPixmap m_companyLogo;
    QString m_websiteName;
    QString m_websiteLink;
    QString m_acknowledgementLink;
};

DWIDGET_END_NAMESPACE
