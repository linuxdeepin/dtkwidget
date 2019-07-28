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
 * \~english \class DThemeManager
 * \~english \brief The DThemeManager class provides helper to manage theme files of widgets.
 *
 * \~english The DThemeManager is singleton class, DThemeManager::instance is used to
 * \~english get an instance of DThemeManager.
 *
 * \~english DThemeManager can be used to get the current application theme, find the
 * \~english corresponding theme file of a widget.
 *
 *
 * \~chinese \class DThemeManager
 * \~chinese \brief DThemeManager 类提供了管理控件主题文件的工具函数。
 * \~chinese
 * \~chinese 在 DTK 中，目前只有 light 和 dark 两种风格的主题，主题文件是为单个控件所创建
 * \~chinese 的 QSS 文件，并且以 .theme 结尾，每个控件都应该有对应的 light 和 dark 两套主题文件。
 * \~chinese
 * \~chinese 主题在应用范围上分为程序级别的主题和控件级别的主题，程序中的控件默认使用程序级别的
 * \~chinese 主题 theme()， 控件也可以使用 setTheme(QWidget *widget, const QString theme)单独设置不同的主题风格，
 * \~chinese 这种情况下控件的子控件都会继承其父控件设置的主题，而不再使用程序级别的主题。
 * \~chinese
 * \~chinese \note DThemeManager 是单实例的，也就是说通常情况下使用者无法创建 DThemeManager 对象，
 * \~chinese 而是通过 DThemeManager::instance 方法来获取到全局的 DThemeManager 实例。
 * \~chinese
 * \~chinese \warning 只有使用 DThemeManager::registerWidget 进行注册的控件才会纳入 DThemeManager
 * \~chinese 的正常管理流程。
 */

/*!
 *
 * \~chinese \enum DThemeManager::ThemeType
 * \~chinese DThemeManager::ThemeType 主题的类型，分为“浅色”、“深色”两种类型。默认情况下，DApplication
 * \~chinese 会根据应用程序调色板中的窗口背景色自动设置应用程序的主题类型。
 *
 * \~chinese \var DThemeManager::UnknownType DThemeManager::UnknownType
 * \~chinese 未知的主题类型
 *
 * \~chinese \var DThemeManager::LightType DThemeManager::LightType
 * \~chinese 浅色主题
 *
 * \~chinese \var DApplication::SingleScope DApplication::SystemScope
 * \~chinese 深色主题
 *
 * \~chinese \sa DApplication
 * \~chinese \sa QPalette
 * \~chinese \sa QApplication::palette
 */

/**
 * \~chinese \property DThemeManager::themeType
 * \~chinese \brief themeType 属性表示应用程序主题的类型
 */

/*!
 * \~chinese \fn DThemeManager::themeChanged
 * \~chinese \brief themeChanged 信号在程序主题发生改变时触发。
 */
/*!
 * \~chinese \fn DThemeManager::widgetThemeChanged
 * \~chinese \brief widgetThemeChanged 信号在特定控件主题发生改变时触发。
 */
/*!
 * \~chinese \fn DThemeManager::themeTypeChanged
 * \~chinese \param themeType 新的主题类型
 * \~chinese \brief themeTypeChanged 信号在应用程序主题发生变化时触发。默认情况下，DApplication
 * \~chinese 会在处理 QEvent::ApplicationPaletteChange 事件时自动更新应用程序的主题类型，此操作可能
 * \~chinese 会导致此信号的触发。
 *
 * \~chinese \sa DThemeManager::setThemeType
 */
/*!
 * \~chinese \fn DThemeManager::windowThemeTypeChanged
 * \~chinese \param window 主题类型发生变化的顶级窗口
 * \~chinese \param themeType 新的主题类型
 * \~chinese \brief windowThemeTypeChanged 信号在窗口的主题类型发现变化时触发。默认情况下，DApplication
 * \~chinese 会在处理 QEvent::PaletteChange 事件时自动更新顶级窗口的主题类型，此操作可能会导致此信号的触发。
 * \~chinese 另外，如果窗口未自定义过 QPalette 并且未指定主题类型，窗口的主题类型会跟随应用程序改变而改变。
 *
 * \~chinese \sa QWidget::setPalette
 * \~chinese \sa DThemeManager::setThemeType(QWidget*, DThemeManager::ThemeType)
 */

/*!
 * \~chinese \brief DThemeManager::instance 用于获取 DThemeManager 的全局实例。
 * \~chinese \return DThemeManager 全局实例。
 */
DThemeManager *DThemeManager::instance()
{
    // 正在初始化时返回空对象，否则会导致当前线程陷入死锁
    if (Q_QGS_DThemeManagerStatic::guard.load() == QtGlobalStatic::Initializing) {
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
    QMap<QWidget *, QMap<QString, QString> > watchedDynamicPropertys;
    DThemeManager::ThemeType themeType = DThemeManager::UnknownType;

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

    void registerWidget(QWidget *widget, const QString &filename, const QStringList &propertys)
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
 * \~english \brief DThemeManager::theme
 * \~english \return the theme name currently be used by this application, the possible
 * \~english value are "light" and "dark".
 *
 *
 * \~chinese \brief DThemeManager::theme 用于获取当前程序使用的主题名称。
 * \~chinese \return 程序使用的主题名称。
 */
QString DThemeManager::theme() const
{
    D_DC(DThemeManager);
    return d->themeName;
}

/*!
 * \~chinese \brief DThemeManager::theme 用于找到 widget 距离最近、并且设置了主题的上级控件，
 * \~chinese 并返回这个主题的名称。如果上级控件都没有设置主题，则返回程序级别的主题名称。
 * \~chinese \param widget 目标控件。
 * \~chinese \param baseWidget 待返回的上级控件。
 * \~chinese \return 指定控件应该使用的主题名称。
 */
QString DThemeManager::theme(const QWidget *widget, QWidget **baseWidget) const
{
    D_DC(DThemeManager);
    return d->fallbackWidgetThemeName(widget, baseWidget);
}

/*!
 * \~english \brief DThemeManager::setTheme sets the application theme.
 * \~english \param theme is the theme name to be set.
 *
 *
 * \~chinese \brief DThemeManager::setTheme 用于设置当前程序的主题。
 * \~chinese \param theme 为指定的主题名称。
 */
void DThemeManager::setTheme(const QString theme)
{
    D_D(DThemeManager);
    d->setTheme(theme);
}

/*!
 * \~english \brief DThemeManager::setTheme sets theme on a widget.
 * \~english \param widget is the target widget.
 * \~english \param theme is the name of theme to be set.
 *
 *
 * \~chinese \brief DThemeManager::setTheme 用于给特定的控件设置主题。
 * \~chinese \note 控件的子控件也会继承设置的主题风格。
 * \~chinese \param widget 为需要设置主题的控件。
 * \~chinese \param theme 为指定的主题名称。
 */
void DThemeManager::setTheme(QWidget *widget, const QString theme)
{
    D_D(DThemeManager);
    d->setTheme(widget, theme);
}

/*!
 * \~chinese \brief DThemeManager::themeType 返回应用程序的主题类型。DApplication 对象构造时会
 * \~chinese 调用 setThemeType 初始化应用程序的主题类型，未初始化时返回 UnknownType。
 * \~chinese \return 主题类型的枚举值
 *
 * \~chinese \sa \class DApplication
 * \~chinese \sa DThemeManager::setThemeType
 */
DThemeManager::ThemeType DThemeManager::themeType() const
{
    D_DC(DThemeManager);

    return d->themeType;
}

/*!
 * \~chinese \brief DThemeManager::themeType 返回窗口的主题类型。当窗口未设置主题类型时，将返回
 * \~chinese 应用程序的主题类型。
 * \~chinese \param window 要获取主题类型的顶级窗口对象
 * \~chinese \return 主题类型的枚举值
 *
 * \~chinese \sa DThemeManager::setThemeType
 * \~chinese \sa DThemeManager::setThemeType(QWidget*, DThemeManager::ThemeType)
 */
DThemeManager::ThemeType DThemeManager::themeType(const QWidget *window) const
{
    ThemeType type = static_cast<ThemeType>(window->property("_d_dtk_theme_type").toInt());

    if (type == UnknownType) {
        type = themeType();
    }

    return type;
}

/*!
 * \~chinese \brief DThemeManager::setThemeType 设置窗口的主题类型。
 * \~chinese \param window 要设置主题类型的顶级窗口对象
 * \~chinese \param 要设置的主题类型枚举值，值为 UnknownType 时将清空 window 对象的主题类型设置
 *
 * \~chinese \sa DThemeManager::setThemeType()
 * \~chinese \note 针对窗口设置的主题类型将存储在窗口对象的 "_d_dtk_theme_type" 属性中
 */
void DThemeManager::setThemeType(QWidget *window, DThemeManager::ThemeType type)
{
    ThemeType old_type = themeType(window);

    if (type == UnknownType) {
        window->setProperty("_d_dtk_theme_type", QVariant());
        type = themeType();
    } else {
        window->setProperty("_d_dtk_theme_type", type);
    }

    if (old_type != type) {
        Q_EMIT windowThemeTypeChanged(window, type);
    }
}

/*!
 * \~english \brief DThemeManager::getQssForWidget searches for the theme file of one class in a specific theme.
 * \~english \param className is the name of the class.
 * \~english \param theme is the name of the theme to be applied.
 * \~english \return The content of the theme file.
 *
 * \~english \note The class name should have no namespace prefixes.
 *
 *
 * \~chinese \brief DThemeManager::getQssForWidget 用于获取特定类在特定主题下的样式内容。
 * \~chinese \param className 为指定的类名称，className 需要跟控件对应的主题文件名对应。
 * \~chinese \param theme 为指定的主题名称。
 * \~chinese \return 主题文件的内容。
 */
QString DThemeManager::getQssForWidget(const QString className, const QString &theme) const
{
    D_DC(DThemeManager);
    QString qss;

    QString themeName = theme.isEmpty() ? d->themeName : theme;
    QString themeURL = QString(":/%1/%2.theme").arg(themeName).arg(className);
    return d->getQssContent(themeURL);
}

QString DThemeManager::getQssForWidget(const QString className, const QWidget *widget) const
{
    Q_ASSERT(widget);

    return getQssForWidget(className, theme(widget));
}

/*!
 * \~english \brief DThemeManager::getQssForWidget This function is overloaded.
 * \~english \param widget is the target widget.
 * \~english \return The content of the theme file to be applied on the widget.
 *
 *
 * \~chinese \brief DThemeManager::getQssForWidget 控件的主题样式内容。
 * \~chinese \param widget 为指定的控件。
 * \~chinese \return 主题文件的内容。
 */
QString DThemeManager::getQssForWidget(const QWidget *widget) const
{
    return getQssForWidget(getObjectClassName(widget), widget);
}

/*!
 * \~chinese \brief DThemeManager::registerWidget 用于将控件注册到 DThemeManager 的管理范围内。
 * \~chinese 只有通过 registerWidget() 注册的控件才会受到 DThemeManager 的自动管理，
 * \~chinese 通常注册的动作都会在实例的构造过程中完成。\ref propertys 参数用来注册属性，
 * \~chinese 注册的属性发生变化时 DThemeManager 会自动刷新控件的样式。
 * \~chinese \param widget 需要注册的控件。
 * \~chinese \param propertys 需要注册的控件属性。
 */
void DThemeManager::registerWidget(QWidget *widget, QStringList propertys)
{
    auto dtm = DThemeManager::instance();
    auto fileName = dtm->d_func()->fallbackWidgetThemeFilename(widget);
    registerWidget(widget, fileName, propertys);
}

/*!
 * \~chinese \brief DThemeManager::registerWidget 用于将控件注册到 DThemeManager 的管理范围内。
 * \~chinese \param widget 需要注册的控件。
 * \~chinese \param filename 需要注册的控件所对应的主题文件名。
 * \~chinese \param propertys 需要注册的控件属性。
 *
 *
 * \sa registerWidget(QWidget *widget, QStringList propertys)
 */
void DThemeManager::registerWidget(QWidget *widget, const QString &filename, const QStringList &propertys)
{
    auto dtm = DThemeManager::instance();
    dtm->d_func()->registerWidget(widget, filename, propertys);
}

/*!
 * \~chinese \brief DThemeManager::toThemeType 获取颜色的明亮度，将其转换为主题类型的枚举值。
 * \~chinese 转换的策略为：先将颜色转换为rgb格式，再根据 Y = 0.299R + 0.587G + 0.114B 的公式
 * \~chinese 计算出颜色的亮度，亮度大于 191 时认为其为浅色，否则认为其为深色。
 * \~chinese \param color 需要转换为主题的类型的颜色
 * \~chinese \return 主题类型的枚举值
 */
DThemeManager::ThemeType DThemeManager::toThemeType(const QColor &color)
{
    QColor rgb_color = color.toRgb();
    // 获取rgb颜色的亮度
    float luminance = 0.299 * rgb_color.redF() + 0.587 * rgb_color.greenF() + 0.114 * rgb_color.blueF();

    if (qRound(luminance * 255) > 191) {
        return LightType;
    }

    return DarkType;
}

/*!
 * \~english \brief DThemeManager::updateQss This slot repolishes the theme applied on
 * \~english the sender widget.
 *
 * \~english This slot is triggered when the property of a registered widget changes,
 * \~english it's useful because Qt has no support of repolishing the style if some
 * \~english custom properties used in the QSS file has changed.
 *
 *
 * \~chinese \brief DThemeManager::updateQss 槽函数会刷新信号发送者的样式。
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

/*!
 * \~chinese \brief DThemeManager::updateThemeOnParentChanged 槽函数用于在控件的
 * \~chinese 父控件发生改变时更新控件的主题样式。
 * \~chinese \param widget 为待刷新样式的控件。
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

/*!
 * \~chinese \brief DThemeManager::setThemeType 设置应用程序的主题类型。
 * \~chinese \param themeType 新的主题类型
 */
void DThemeManager::setThemeType(DThemeManager::ThemeType themeType)
{
    D_D(DThemeManager);

    if (d->themeType == themeType)
        return;

    d->themeType = themeType;
    Q_EMIT themeTypeChanged(themeType);

    // 未自定义主题类型的窗口应该跟随应用程序的主题类型改变，此处需要为这些窗口发送通知
    for (QWidget *window : qApp->topLevelWidgets()) {
        if (!window->property("_d_dtk_theme_type").isValid()) {
            Q_EMIT windowThemeTypeChanged(window, themeType);
        }
    }
}

DWIDGET_END_NAMESPACE
