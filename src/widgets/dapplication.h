/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DAPPLICATION_H
#define DAPPLICATION_H

#include <QApplication>

#include "dtkwidget_global.h"
#include "dobject.h"

DWIDGET_BEGIN_NAMESPACE

class DApplication;
class DApplicationPrivate;
class DAboutDialog;

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<DApplication *>(QCoreApplication::instance()))

class LIBDTKWIDGETSHARED_EXPORT DApplication : public QApplication, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DApplication)

public:
    DApplication(int &argc, char **argv);

    QString theme() const;
    void setTheme(const QString &theme);

    bool setSingleInstance(const QString &key);
    bool loadTranslator(QList<QLocale> localeFallback = QList<QLocale>() << QLocale::system());

    //! warning: Must call before QGuiApplication defined object
    static bool loadDXcbPlugin();
    static bool isDXcbPlatform();

    // meta information that necessary to create a about dialog for the application.
    QString productName() const;
    void setProductName(const QString &productName);

    const QIcon &productIcon() const;
    void setProductIcon(const QIcon &productIcon);

    QString applicationLicense() const;
    void setApplicationLicense(const QString &license);

    QString applicationDescription() const;
    void setApplicationDescription(const QString &description);

    QString applicationHomePage() const;
    void setApplicationHomePage(const QString &link);

    QString applicationAcknowledgementPage() const;
    void setApplicationAcknowledgementPage(const QString &link);

    DAboutDialog *aboutDialog();
    void setAboutDialog(DAboutDialog *aboutDialog);

Q_SIGNALS:
    void newInstanceStarted();

    //###(zccrs): Depend the Qt platform theme plugin(from the package: dde-qt5integration)
    void iconThemeChanged();

protected:
    virtual void handleHelpAction();
    virtual void handleAboutAction();
    virtual void handleQuitAction();

    bool notify(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

private:
    friend class DTitlebarPrivate;
};

DWIDGET_END_NAMESPACE

#endif // DAPPLICATION_H
