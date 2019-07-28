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

#ifndef DTHEMEMANAGER_H
#define DTHEMEMANAGER_H

#include <QObject>
#include <QString>
#include <QGlobalStatic>

#include <DObject>
#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DThemeManagerPrivate;
class LIBDTKWIDGETSHARED_EXPORT DThemeManager : public QObject, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(ThemeType themeType READ themeType WRITE setThemeType NOTIFY themeTypeChanged)

public:
    enum ThemeType {
        UnknownType,
        LightType,
        DarkType
    };
    Q_ENUM(ThemeType)

    static DThemeManager *instance();

    QString theme() const;
    QString theme(const QWidget *widget, QWidget **baseWidget = nullptr) const;
    void setTheme(const QString theme);
    void setTheme(QWidget *widget, const QString theme);

    ThemeType themeType() const;
    ThemeType themeType(const QWidget *window) const;
    void setThemeType(QWidget *window, ThemeType type);

    QString getQssForWidget(const QString className, const QString &theme = QString()) const;
    Q_DECL_DEPRECATED QString getQssForWidget(const QString className, const QWidget *widget) const;
    QString getQssForWidget(const QWidget *widget) const;

    static void registerWidget(QWidget *widget, QStringList propertys = QStringList());
    // TODO: use blow instead, the only thing should do is rebuilding
    // static void registerWidget(QWidget *widget, const QStringList &propertys = QStringList());
    static void registerWidget(QWidget *widget, const QString &filename, const QStringList &propertys = QStringList());

    static ThemeType toThemeType(const QColor &color);

public Q_SLOTS:
    void updateQss();
    void updateThemeOnParentChanged(QWidget *widget);

    void setThemeType(ThemeType themeType);

Q_SIGNALS:
    void themeChanged(QString theme);
    void widgetThemeChanged(QWidget *widget, QString theme);

    void themeTypeChanged(ThemeType themeType);
    void windowThemeTypeChanged(QWidget *window, ThemeType type);

protected:
    DThemeManager();
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    friend class DApplication;
    D_DECLARE_PRIVATE(DThemeManager)
};

DWIDGET_END_NAMESPACE

#endif // DTHEMEMANAGER_H
