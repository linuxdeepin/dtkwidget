#ifndef DABOUTDIALOG_H
#define DABOUTDIALOG_H

#include <ddialog.h>

DWIDGET_BEGIN_NAMESPACE

class DAboutDialogPrivate;
class DAboutDialog : public DDialog
{
    Q_OBJECT

    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle)
    Q_PROPERTY(QString productName READ productName WRITE setProductName)
    Q_PROPERTY(QString version READ version WRITE setVersion)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString license READ license WRITE setLicense)
    Q_PROPERTY(QString websiteName READ websiteName WRITE setWebsiteName NOTIFY websiteNameChanged)
    Q_PROPERTY(QString websiteLink READ websiteLink WRITE setWebsiteLink NOTIFY websiteLinkChanged)
    Q_PROPERTY(QString acknowledgementLink READ acknowledgementLink WRITE setAcknowledgementLink)

public:
    DAboutDialog(QWidget *parent = 0);

    QString windowTitle() const;
    QString productName() const;
    QString version() const;
    QString description() const;
    const QPixmap *companyLogo() const;
    QString websiteName() const;
    QString websiteLink() const;
    QString acknowledgementLink() const;
    QString license() const;

public slots:
    void setWindowTitle(const QString &windowTitle);
    void setProductIcon(const QIcon &icon);
    void setProductName(const QString &productName);
    void setVersion(const QString &version);
    void setDescription(const QString &description);
    void setCompanyLogo(const QPixmap &companyLogo);
    void setWebsiteName(const QString &websiteName);
    void setWebsiteLink(const QString &websiteLink);
    void setAcknowledgementLink(const QString &acknowledgementLink);
    void setLicense(const QString &license);

signals:
    void websiteNameChanged(QString websiteName);
    void websiteLinkChanged(QString websiteLink);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_onLinkActivated(const QString &link))

    Q_DISABLE_COPY(DAboutDialog)
    D_DECLARE_PRIVATE(DAboutDialog)
};

DWIDGET_END_NAMESPACE

#endif // DABOUTDIALOG_H
