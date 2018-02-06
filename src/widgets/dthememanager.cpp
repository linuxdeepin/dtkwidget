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

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QWidget>
#include <QStyle>
#include <QStyleFactory>

#include <DObjectPrivate>

#include <private/qstylesheetstyle_p.h>

#include "dthememanager.h"
#include "dapplication.h"

DWIDGET_BEGIN_NAMESPACE

class DThemeManagerStaticPrivate : public DThemeManager
{

};

Q_GLOBAL_STATIC(DThemeManagerStaticPrivate, DThemeManagerStatic)


/*!
 * \class DThemeManager
 * \brief The DThemeManager class provides helper to manage theme files of widgets.
 *
 * The DThemeManager is singleton class, DThemeManager::instance is used to
 * get an instance of DThemeManager.
 *
 * DThemeManager can be used to get the current application theme, find the
 * corresponding theme file of a widget.
 */

DThemeManager *DThemeManager::instance()
{
    return DThemeManagerStatic;
}

static QString getObjectClassName(const QObject *obj)
{
    const QString &type_name = QString::fromLocal8Bit(obj->metaObject()->className());
    const QStringList &widget_type_list = type_name.split("::");

    return widget_type_list.isEmpty() ? type_name : widget_type_list.last();
}

// TODO: just for fix some bug, remove after 2.0.7
static QString getThemeNameByRawClassName(const QObject *obj)
{
    return QString::fromLocal8Bit(obj->metaObject()->className());
}

static QString getThemeNameByClassName(const QObject *obj)
{
    QString type_name = QString::fromLocal8Bit(obj->metaObject()->className());
    return type_name.replace("::", "--");
}

static void emitThemeChanged(DThemeManager *manager, QWidget *widget, const QString &theme)
{
    Q_EMIT manager->widgetThemeChanged(widget, theme);

    for (QObject *child : widget->children()) {
        if (QWidget *cw = qobject_cast<QWidget *>(child)) {
            if (cw->property("_d_dtk_theme").isValid()) {
                continue;
            }

            emitThemeChanged(manager, cw, theme);
        }
    }
}

static void setStyle(QWidget *widget, QStyle *style)
{
    widget->setStyle(style);

    for (QObject *child : widget->children()) {
        if (QWidget *cw = qobject_cast<QWidget *>(child)) {
            if (cw->property("_d_dtk_theme").isValid()) {
                continue;
            }

            setStyle(cw, style);
        }
    }
}

static void inseritStyle(QWidget *widget, const QWidget *baseWidget)
{
    if (widget == baseWidget) {
        return;
    }

    QStyle *base_style = baseWidget ? baseWidget->style() : qApp->style();

    if (base_style->inherits("QStyleSheetStyle")) {
        base_style = static_cast<QStyleSheetStyle *>(base_style)->base;
    }

    widget->setStyle(base_style);
}

static void updateWidgetTheme(DThemeManager *manager, QWidget *widget, QWidget *baseWidget, const QString &theme)
{
    inseritStyle(widget, baseWidget);

    Q_EMIT manager->widgetThemeChanged(widget, theme);

    for (QObject *child : widget->children()) {
        if (QWidget *cw = qobject_cast<QWidget *>(child)) {
            if (widget->property("_d_dtk_theme").isValid()) {
                return;
            }

            updateWidgetTheme(manager, cw, baseWidget, theme);
        }
    }
}

class DThemeManagerPrivate : public DCORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DThemeManager)

    QString themeName;
    QMap<QWidget *, QMap<QString, QString> > watchedDynamicPropertys;

public:
    DThemeManagerPrivate(DThemeManager *qq)
        : DObjectPrivate(qq) {}

    QString getQssContent(const QString &themeURL) const
    {
        QString qss;
        QFile themeFile(themeURL);
        if (themeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qss = themeFile.readAll();
            themeFile.close();
        } else {
            /// !!! if do not privode qss file, do not register it!!!
            qWarning() << "open qss file failed" << themeURL << themeFile.errorString();
        }
        return qss;
    }

    bool themeFileExist(const QString &filename) const
    {
        QFileInfo fi(filename);
        return  fi.exists();
    }

    inline QString themeURL(const QString &themename, const QString &filename) const
    {
        if (themename.isEmpty() || filename.isEmpty()) {
            return "";
        }
        return QString(":/%1/%2.theme").arg(themename).arg(filename);
    }

    QString fallbackWidgetThemeName(const QWidget *widget, QWidget **baseWidget = nullptr) const
    {
        QString theme;

        if (baseWidget) {
            *baseWidget = nullptr;
        }

        do {
            theme = widget->property("_d_dtk_theme").toString();
            if (!theme.isEmpty()) {
                if (baseWidget) {
                    *baseWidget = const_cast<QWidget *>(widget);
                }

                break;
            }
            widget = widget->isWindow() ? 0 : widget->parentWidget();
        } while (widget);

        return theme.isEmpty() ? themeName : theme;
    }

    QString fallbackWidgetThemeFilename(QWidget *w) const
    {
        QStringList fallbackList;

        QString overriveName = w->property("_d_dtk_theme_filename").toString();
        if (!overriveName.isEmpty()) {
            fallbackList << overriveName;
        }

        QString rawClassName = getThemeNameByRawClassName(w);
        if (!rawClassName.isEmpty()) {
            fallbackList << rawClassName;
        }

        QString className = getThemeNameByClassName(w);
        if (!className.isEmpty()) {
            fallbackList << className;
        }

        auto themeName = fallbackWidgetThemeName(w, nullptr);
        for (auto filename : fallbackList) {
            if (themeFileExist(themeURL(themeName, filename))) {
                return filename;
            }
        }

        return "";
    }

    void registerWidget(QWidget *widget, const QString &filename, const QStringList &propertys)
    {
        if (filename.isEmpty()) {
            return;
        }

        auto themeurl = themeURL(fallbackWidgetThemeName(widget), filename);

        auto dtm = DThemeManager::instance();
        widget->setStyleSheet(dtm->d_func()->getQssContent(themeurl));

        dtm->connect(dtm, &DThemeManager::themeChanged,
        widget, [this, widget, dtm, filename](QString) {
            auto themeurl = themeURL(fallbackWidgetThemeName(widget), filename);
            widget->setStyleSheet(dtm->d_func()->getQssContent(themeurl));
        });

        dtm->connect(dtm, &DThemeManager::widgetThemeChanged, widget,
        [this, widget, dtm, filename](QWidget * w, QString) {
            if (widget == w) {
                auto themeurl = themeURL(fallbackWidgetThemeName(widget), filename);
                widget->setStyleSheet(dtm->d_func()->getQssContent(themeurl));
            }
        });

        auto meta = widget->metaObject();
        QMap<QString, QString> dynamicPropertys;
        for (auto &prop : propertys) {
            auto propIndex = meta->indexOfProperty(prop.toLatin1().data());
            if (propIndex < 0) {
                dynamicPropertys.insert(prop, prop);
                continue;
            }
            dtm->connect(widget, meta->property(propIndex).notifySignal(),
                         dtm, dtm->metaObject()->method(dtm->metaObject()->indexOfMethod("updateQss()")));
        }

        if (!dynamicPropertys.isEmpty()) {
            widget->installEventFilter(dtm);
            if (dtm->d_func()->watchedDynamicPropertys.contains(widget)) {
                QMap<QString, QString> oldProps = dtm->d_func()->watchedDynamicPropertys.value(widget);
                for (auto &key : oldProps.keys()) {
                    dynamicPropertys.insert(key, oldProps.value(key));
                }
            }
            dtm->d_func()->watchedDynamicPropertys.insert(widget, dynamicPropertys);

            dtm->connect(widget, &QObject::destroyed, dtm, [ = ]() {
                dtm->d_func()->watchedDynamicPropertys.remove(widget);
            });
        }
    }

    void setTheme(const QString theme)
    {
        D_Q(DThemeManager);
        if (themeName != theme) {
            QStyle *style = Q_NULLPTR;

            // TODO: remove this shit in the future.
            // It's just a trick to make all DApplications use dde qt5 styles,
            // if dlight or ddark style is set to default style of dde, those
            // ugly code will no longer needed.
            if (theme == "light") {
                style = QStyleFactory::create("dlight");
                themeName = theme;
            } else if (theme == "dark") {
                style = QStyleFactory::create("ddark");
                themeName = theme;
            } else if (theme == "semilight") {
                style = QStyleFactory::create("dsemilight");
                themeName = "light";
            } else if (theme == "semidark") {
                style = QStyleFactory::create("dsemidark");
                themeName = "dark";
            }

            if (style) {
                qApp->setStyle(style);
            }

            Q_EMIT q->themeChanged(themeName);
        }
    }

    void setTheme(QWidget *widget, const QString theme)
    {
        D_Q(DThemeManager);

        Q_ASSERT(widget);

        if (theme.isEmpty()) {
            QString old_theme = fallbackWidgetThemeName(widget);

            widget->setProperty("_d_dtk_theme", QVariant());

            QWidget *baseWidget = nullptr;

            if (fallbackWidgetThemeName(widget, &baseWidget) != old_theme) {
                emitThemeChanged(q, widget, fallbackWidgetThemeName(widget));
            }

            inseritStyle(widget, baseWidget);

            return;
        }

        const QString &old_theme = fallbackWidgetThemeName(widget);
        QStyle *style = Q_NULLPTR;

        // TODO: remove this shit in the future.
        // It's just a trick to make all DApplications use dde qt5 styles,
        // if dlight or ddark style is set to default style of dde, those
        // ugly code will no longer needed.
        if (theme == "light") {
            style = QStyleFactory::create("dlight");
            widget->setProperty("_d_dtk_theme", theme);
        } else if (theme == "dark") {
            style = QStyleFactory::create("ddark");
            widget->setProperty("_d_dtk_theme", theme);
        } else if (theme == "semilight") {
            style = QStyleFactory::create("dsemilight");
            widget->setProperty("_d_dtk_theme", "light");
        } else if (theme == "semidark") {
            style = QStyleFactory::create("dsemidark");
            widget->setProperty("_d_dtk_theme", "dark");
        }

        if (style) {
            setStyle(widget, style);
        }

        if (old_theme != theme) {
            emitThemeChanged(q, widget, theme);
        }
    }
};

/*!
 * \brief DThemeManager::theme
 * \return the theme name currently be used by this application, the possible
 * value are "light" and "dark".
 */
QString DThemeManager::theme() const
{
    D_DC(DThemeManager);
    return d->themeName;
}

QString DThemeManager::theme(const QWidget *widget, QWidget **baseWidget) const
{
    D_DC(DThemeManager);
    return d->fallbackWidgetThemeName(widget, baseWidget);
}

/*!
 * \brief DThemeManager::setTheme sets the application theme.
 * \param theme is the theme name to be set.
 */
void DThemeManager::setTheme(const QString theme)
{
    D_D(DThemeManager);
    d->setTheme(theme);
}

/*!
 * \brief DThemeManager::setTheme sets theme on a widget.
 * \param widget is the target widget.
 * \param theme is the name of theme to be set.
 */
void DThemeManager::setTheme(QWidget *widget, const QString theme)
{
    D_D(DThemeManager);
    d->setTheme(widget, theme);
}

/*!
 * \brief DThemeManager::getQssForWidget searches for the theme file of one class in a specific theme.
 * \param className is the name of the class.
 * \param theme is the name of the theme to be applied.
 * \return The content of the theme file.
 *
 * \note The class name should have no namespace prefixes.
 */
QString DThemeManager::getQssForWidget(const QString className, const QString &theme) const
{
    D_DC(DThemeManager);
    QString qss;

    QString themeName = theme.isEmpty() ? d->themeName : theme;
    QString themeURL = QString(":/%1/%2.theme").arg(themeName).arg(className);
    return d->getQssContent(themeURL);
}

/*!
 * \brief DThemeManager::getQssForWidget This function is overloaded.
 * \param className is the name of the target class.
 * \param widget is the widget from which to read the theme name.
 * \return The content of the theme file.
 *
 * \note The class name should have no namespace prefixes.
 */
QString DThemeManager::getQssForWidget(const QString className, const QWidget *widget) const
{
    Q_ASSERT(widget);

    return getQssForWidget(className, theme(widget));
}

/*!
 * \brief DThemeManager::getQssForWidget This function is overloaded.
 * \param widget is the target widget.
 * \return The content of the theme file to be applied on the widget.
 */
QString DThemeManager::getQssForWidget(const QWidget *widget) const
{
    return getQssForWidget(getObjectClassName(widget), widget);
}

/*!
    \since 2.0
    \relates DThemeManager
    Find QSS file on the qrc by getQssForWidget, className is form metaObject
 */
void DThemeManager::registerWidget(QWidget *widget, QStringList propertys)
{
    auto dtm = DThemeManager::instance();
    auto fileName = dtm->d_func()->fallbackWidgetThemeFilename(widget);
    registerWidget(widget, fileName, propertys);
}

/*!
 * \brief DThemeManager::registerWidget
 * \param widget
 * \param filename
 * \param propertys
 */
void DThemeManager::registerWidget(QWidget *widget, const QString &filename, const QStringList &propertys)
{
    auto dtm = DThemeManager::instance();
    dtm->d_func()->registerWidget(widget, filename, propertys);
}

/*!
 * \brief DThemeManager::updateQss This slot repolishes the theme applied on
 * the sender widget.
 *
 * This slot is triggered when the property of a registered widget changes,
 * it's useful because Qt has no support of repolishing the style if some
 * custom properties used in the QSS file has changed.
 */
void DThemeManager::updateQss()
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    if (w) {
        w->setStyleSheet(w->styleSheet());
    }
}

DThemeManager::DThemeManager()
    : QObject()
    , DObject(*new DThemeManagerPrivate(this))
{
    setTheme("light");
}

bool DThemeManager::eventFilter(QObject *watched, QEvent *event)
{
    D_D(DThemeManager);
    if (event->type() != QEvent::DynamicPropertyChange) {
        return QObject::eventFilter(watched, event);
    }

    auto widget = qobject_cast<QWidget *>(watched);
    if (!d->watchedDynamicPropertys.contains(widget)) {
        return QObject::eventFilter(watched, event);
    }

    auto propEvent = reinterpret_cast<QDynamicPropertyChangeEvent *>(event);
    if (!propEvent) {
        return QObject::eventFilter(watched, event);
    }

    auto props = d->watchedDynamicPropertys.value(widget);
    auto propName = QString::fromLatin1(propEvent->propertyName().data());
    if (props.contains(propName) && widget) {
        widget->setStyleSheet(widget->styleSheet());
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
        widget->update();
    }

    return QObject::eventFilter(watched, event);
}

void DThemeManager::updateThemeOnParentChanged(QWidget *widget)
{
    if (widget->property("_d_dtk_theme").isValid()) {
        return;
    }

    QWidget *base_widget = nullptr;
    const QString &theme = this->theme(widget, &base_widget);

    if (!base_widget) {
        return;
    }

    updateWidgetTheme(this, widget, base_widget, theme);
}


DWIDGET_END_NAMESPACE
