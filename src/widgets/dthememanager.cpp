// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
DWIDGET_BEGIN_NAMESPACE

class DThemeManagerStaticPrivate : public DThemeManager
{

};

Q_GLOBAL_STATIC(DThemeManagerStaticPrivate, DThemeManagerStatic)

/*!
  \class Dtk::Widget::DThemeManager
  \inmodule dtkwidget

  \brief DThemeManager 类提供了管理控件主题文件的工具函数.
  \brief The DThemeManager class provides helper to manage theme files of widgets.
  
  The DThemeManager is singleton class, DThemeManager::instance is used to
  get an instance of DThemeManager.
  
  DThemeManager can be used to get the current application theme, find the
  corresponding theme file of a widget.
  
  在 DTK 中，目前只有 light 和 dark 两种风格的主题，主题文件是为单个控件所创建
  的 QSS 文件，并且以 .theme 结尾，每个控件都应该有对应的 light 和 dark 两套主题文件。
  
  主题在应用范围上分为程序级别的主题和控件级别的主题，程序中的控件默认使用程序级别的
  主题 theme()， 控件也可以使用 setTheme(QWidget *widget, const QString theme)单独设置不同的主题风格，
  这种情况下控件的子控件都会继承其父控件设置的主题，而不再使用程序级别的主题。
  
  \note DThemeManager 是单实例的，也就是说通常情况下使用者无法创建 DThemeManager 对象，
  而是通过 DThemeManager::instance 方法来获取到全局的 DThemeManager 实例。
  
  \warning 只有使用 DThemeManager::registerWidget 进行注册的控件才会纳入 DThemeManager
  的正常管理流程。
 */

/*!
  \fn void DThemeManager::themeChanged(QString theme)

  \a theme 新的主题类型
  \brief themeTypeChanged 信号在应用程序主题发生变化时触发.

  默认情况下，DApplication 会在处理 QEvent::ApplicationPaletteChange
  事件时自动更新应用程序的主题类型，此操作可能会导致此信号的触发。

  \sa DThemeManager::setTheme
 */
/*!
  \fn void DThemeManager::widgetThemeChanged(QWidget *widget, QString theme)
  \brief widgetThemeChanged 信号在特定控件主题发生改变时触发.

  \a widget 对应的控件指针， \a theme 切换的主题类型。

 */

/*!
  \brief DThemeManager::instance 用于获取 DThemeManager 的全局实例。
  \return DThemeManager 全局实例。
 */
DThemeManager *DThemeManager::instance()
{
    // 正在初始化时返回空对象，否则会导致当前线程陷入死锁
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    if (Q_QGS_DThemeManagerStatic::guard.loadRelaxed() == QtGlobalStatic::Initializing)
#else
    if (Q_QGS_DThemeManagerStatic::guard.load() == QtGlobalStatic::Initializing)
#endif
    {
        return nullptr;
    }

    Q_QGS_DThemeManagerStatic::guard.testAndSetRelaxed(QtGlobalStatic::Uninitialized, QtGlobalStatic::Initializing);

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
    QMap<QWidget *, QMap<QString, QString> > watchedDynamicProperties;

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

        QString className = getThemeNameByClassName(w);
        if (!className.isEmpty()) {
            fallbackList << className;
        }

        QString rawClassName = getThemeNameByRawClassName(w);
        if (!rawClassName.isEmpty()) {
            fallbackList << rawClassName;
        }

        auto themeName = fallbackWidgetThemeName(w, nullptr);
        for (auto filename : fallbackList) {
            if (themeFileExist(themeURL(themeName, filename))) {
                return filename;
            }
        }

        return "";
    }

    void registerWidget(QWidget *widget, const QString &filename, const QStringList &properties)
    {
        if (filename.isEmpty()) {
            qWarning() << "can not load qss with out filename" << widget;
            return;
        }

        auto themeurl = themeURL(fallbackWidgetThemeName(widget), filename);

        auto dtm = DThemeManager::instance();
        widget->setStyleSheet(widget->styleSheet() + dtm->d_func()->getQssContent(themeurl));
        widget->style()->polish(widget);

        auto reloadTheme = [this, dtm](QWidget * widget, const QString & filename, const QString & themename) {
            const char *baseClassReloadThemeProp = "_dtk_theme_base_calss_reload_theme";
            auto themeurl = themeURL(fallbackWidgetThemeName(widget), filename);
            auto reloadTheme = widget->property(baseClassReloadThemeProp).toString();
            if (reloadTheme != themename) {
                widget->setStyleSheet(dtm->d_func()->getQssContent(themeurl));
                widget->setProperty(baseClassReloadThemeProp, themename);
            } else {
                widget->setStyleSheet(widget->styleSheet() + dtm->d_func()->getQssContent(themeurl));
            }
        };

        dtm->connect(dtm, &DThemeManager::themeChanged,
        widget, [reloadTheme, widget, filename](QString theme) {
            reloadTheme(widget, filename, theme);
        });

        dtm->connect(dtm, &DThemeManager::widgetThemeChanged, widget,
        [reloadTheme, widget, filename](QWidget * w, QString theme) {
            if (widget == w) {
                reloadTheme(widget, filename, theme);
            }
        });

        auto meta = widget->metaObject();
        QMap<QString, QString> dynamicProperties;
        for (auto &prop : properties) {
            auto propIndex = meta->indexOfProperty(prop.toLatin1().data());
            if (propIndex < 0) {
                dynamicProperties.insert(prop, prop);
                continue;
            }
            dtm->connect(widget, meta->property(propIndex).notifySignal(),
                         dtm, dtm->metaObject()->method(dtm->metaObject()->indexOfMethod("updateQss()")));
        }

        if (!dynamicProperties.isEmpty()) {
            widget->installEventFilter(dtm);
            if (dtm->d_func()->watchedDynamicProperties.contains(widget)) {
                QMap<QString, QString> oldProps = dtm->d_func()->watchedDynamicProperties.value(widget);
                for (auto &key : oldProps.keys()) {
                    dynamicProperties.insert(key, oldProps.value(key));
                }
            }
            dtm->d_func()->watchedDynamicProperties.insert(widget, dynamicProperties);

            dtm->connect(widget, &QObject::destroyed, dtm, [ = ]() {
                dtm->d_func()->watchedDynamicProperties.remove(widget);
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
  \brief DThemeManager::theme
  
  \brief DThemeManager::theme 用于获取当前程序使用的主题名称。
  value are "light" and "dark".
  
  \return 程序使用的主题名称。
  \return the theme name currently be used by this application, the possible
 */
QString DThemeManager::theme() const
{
    D_DC(DThemeManager);
    return d->themeName;
}

/*!
  \brief DThemeManager::theme 用于找到 widget 距离最近、并且设置了主题的上级控件，
  并返回这个主题的名称。如果上级控件都没有设置主题，则返回程序级别的主题名称。

  \a widget 目标控件。
  \a baseWidget 待返回的上级控件。
  \return 指定控件应该使用的主题名称。
 */
QString DThemeManager::theme(const QWidget *widget, QWidget **baseWidget) const
{
    D_DC(DThemeManager);
    return d->fallbackWidgetThemeName(widget, baseWidget);
}

/*!
  \brief DThemeManager::setTheme sets the application theme.
  \a theme is the theme name to be set.
  
  
  \brief DThemeManager::setTheme 用于设置当前程序的主题。
  \a theme 为指定的主题名称。
 */
void DThemeManager::setTheme(const QString theme)
{
    D_D(DThemeManager);
    d->setTheme(theme);
}

/*!
  \brief DThemeManager::setTheme sets theme on a widget.
  \a widget is the target widget.
  \a theme is the name of theme to be set.
  
  
  \brief DThemeManager::setTheme 用于给特定的控件设置主题。
  \note 控件的子控件也会继承设置的主题风格。
  \a widget 为需要设置主题的控件。
  \a theme 为指定的主题名称。
 */
void DThemeManager::setTheme(QWidget *widget, const QString theme)
{
    D_D(DThemeManager);
    d->setTheme(widget, theme);
}

/*!
  \brief DThemeManager::getQssForWidget searches for the theme file of one class in a specific theme.
  \a className is the name of the class.
  \a theme is the name of the theme to be applied.
  \return The content of the theme file.
  
  \note The class name should have no namespace prefixes.
  
  
  \brief DThemeManager::getQssForWidget 用于获取特定类在特定主题下的样式内容。
  \a className 为指定的类名称，className 需要跟控件对应的主题文件名对应。
  \a theme 为指定的主题名称。
  \return 主题文件的内容。
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
  \brief DThemeManager::getQssForWidget This function is overloaded.
  \a widget is the target widget.
  \return The content of the theme file to be applied on the widget.
  
  
  \brief DThemeManager::getQssForWidget 控件的主题样式内容。
  \a widget 为指定的控件。
  \return 主题文件的内容。
 */
QString DThemeManager::getQssForWidget(const QWidget *widget) const
{
    return getQssForWidget(getObjectClassName(widget), theme(widget));
}

/*!
  \brief DThemeManager::registerWidget 用于将控件注册到 DThemeManager 的管理范围内。
  只有通过 registerWidget() 注册的控件才会受到 DThemeManager 的自动管理，
  通常注册的动作都会在实例的构造过程中完成。properties 参数用来注册属性，
  注册的属性发生变化时 DThemeManager 会自动刷新控件的样式。
  \a widget 需要注册的控件。
  \a properties 需要注册的控件属性。
 */
void DThemeManager::registerWidget(QWidget *widget, QStringList properties)
{
    auto dtm = DThemeManager::instance();
    auto fileName = dtm->d_func()->fallbackWidgetThemeFilename(widget);
    registerWidget(widget, fileName, properties);
}

/*!
  \brief DThemeManager::registerWidget 用于将控件注册到 DThemeManager 的管理范围内。
  \a widget 需要注册的控件。
  \a filename 需要注册的控件所对应的主题文件名。
  \a properties 需要注册的控件属性。
  
  
  \sa registerWidget(QWidget *widget, QStringList properties)
 */
void DThemeManager::registerWidget(QWidget *widget, const QString &filename, const QStringList &properties)
{
    auto dtm = DThemeManager::instance();
    dtm->d_func()->registerWidget(widget, filename, properties);
}

/*!
  \brief DThemeManager::updateQss This slot repolishes the theme applied on
  the sender widget.
  
  This slot is triggered when the property of a registered widget changes,
  it's useful because Qt has no support of repolishing the style if some
  custom properties used in the QSS file has changed.
  
  
  \brief DThemeManager::updateQss 槽函数会刷新信号发送者的样式。
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
    qApp->setStyle("chameleon");
}

bool DThemeManager::eventFilter(QObject *watched, QEvent *event)
{
    D_D(DThemeManager);
    if (event->type() != QEvent::DynamicPropertyChange) {
        return QObject::eventFilter(watched, event);
    }

    auto widget = qobject_cast<QWidget *>(watched);
    if (!d->watchedDynamicProperties.contains(widget)) {
        return QObject::eventFilter(watched, event);
    }

    auto propEvent = reinterpret_cast<QDynamicPropertyChangeEvent *>(event);
    if (!propEvent) {
        return QObject::eventFilter(watched, event);
    }

    auto props = d->watchedDynamicProperties.value(widget);
    auto propName = QString::fromLatin1(propEvent->propertyName().data());
    if (props.contains(propName) && widget) {
        widget->setStyleSheet(widget->styleSheet());
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
        widget->update();
    }

    return QObject::eventFilter(watched, event);
}

/*!
  \brief DThemeManager::updateThemeOnParentChanged 槽函数用于在控件的
  父控件发生改变时更新控件的主题样式。
  \a widget 为待刷新样式的控件。
 */
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

#endif
