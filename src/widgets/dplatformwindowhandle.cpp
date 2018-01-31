/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dplatformwindowhandle.h"
#include "dapplication.h"
#include "util/dwindowmanagerhelper.h"

#include <QWindow>
#include <QWidget>
#include <QGuiApplication>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

#define DEFINE_CONST_CHAR(Name) const char _##Name[] = "_d_" #Name

DEFINE_CONST_CHAR(useDxcb);
DEFINE_CONST_CHAR(redirectContent);
DEFINE_CONST_CHAR(netWmStates);
DEFINE_CONST_CHAR(windowRadius);
DEFINE_CONST_CHAR(borderWidth);
DEFINE_CONST_CHAR(borderColor);
DEFINE_CONST_CHAR(shadowRadius);
DEFINE_CONST_CHAR(shadowOffset);
DEFINE_CONST_CHAR(shadowColor);
DEFINE_CONST_CHAR(clipPath);
DEFINE_CONST_CHAR(frameMask);
DEFINE_CONST_CHAR(frameMargins);
DEFINE_CONST_CHAR(translucentBackground);
DEFINE_CONST_CHAR(enableSystemResize);
DEFINE_CONST_CHAR(enableSystemMove);
DEFINE_CONST_CHAR(enableBlurWindow);
DEFINE_CONST_CHAR(windowBlurAreas);
DEFINE_CONST_CHAR(windowBlurPaths);
DEFINE_CONST_CHAR(autoInputMaskByClipPath);

// functions
DEFINE_CONST_CHAR(setWmBlurWindowBackgroundArea);
DEFINE_CONST_CHAR(setWmBlurWindowBackgroundPathList);
DEFINE_CONST_CHAR(setWmBlurWindowBackgroundMaskImage);
DEFINE_CONST_CHAR(setWindowProperty);
DEFINE_CONST_CHAR(pluginVersion);

static void setWindowProperty(QWindow *window, const char *name, const QVariant &value)
{
    if (!window)
        return;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    static QFunctionPointer setWindowProperty = qApp->platformFunction(_setWindowProperty);
#else
    constexpr QFunctionPointer setWindowProperty = nullptr;
#endif

    if (!setWindowProperty) {
        window->setProperty(name, value);

        return;
    }

    reinterpret_cast<void(*)(QWindow *, const char *, const QVariant &)>(setWindowProperty)(window, name, value);
}

DPlatformWindowHandle::DPlatformWindowHandle(QWindow *window, QObject *parent)
    : QObject(parent)
    , m_window(window)
{
    enableDXcbForWindow(window);

    window->installEventFilter(this);
}

DPlatformWindowHandle::DPlatformWindowHandle(QWidget *widget, QObject *parent)
    : QObject(parent)
    , m_window(Q_NULLPTR)
{
    enableDXcbForWindow(widget);

    m_window = widget->window()->windowHandle();

    if (m_window)
        m_window->installEventFilter(this);
}

QString DPlatformWindowHandle::pluginVersion()
{
    QFunctionPointer pv = 0;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    pv = qApp->platformFunction(_pluginVersion);
#endif

    if (Q_UNLIKELY(!pv))
        return QString();

    return reinterpret_cast<QString(*)()>(pv)();
}

void DPlatformWindowHandle::enableDXcbForWindow(QWidget *widget)
{
    if (!DApplication::isDXcbPlatform())
        return;

    QWidget *window = widget->window();
    QWindow *handle = window->windowHandle();

    if (!handle) {
        bool save_flag = qApp->testAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
        qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);
        window->setAttribute(Qt::WA_NativeWindow);
        handle = window->windowHandle();
        window->setAttribute(Qt::WA_NativeWindow, false);

        // dxcb version >= 1.1.6
        if (!pluginVersion().isEmpty()) {
            /// TODO: Avoid call parentWidget()->enforceNativeChildren().
            qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, save_flag);
        }

        Q_ASSERT_X(handle, "DPlatformWindowHandler:", "widget window handle is NULL.");

        if (Q_UNLIKELY(!handle))
            return;
    }

    if (handle->property(_useDxcb).toBool())
        return;

    Q_ASSERT_X(!handle->handle(), "DPlatformWindowHandler:",
               "Must be called before window handle has been created. See also QWidget::windowHandle()");

    handle->setProperty(_useDxcb, true);
}

void DPlatformWindowHandle::enableDXcbForWindow(QWindow *window)
{
    if (!DApplication::isDXcbPlatform())
        return;

    if (window->handle()) {
        Q_ASSERT_X(window->property(_useDxcb).toBool(), "DPlatformWindowHandler:",
                   "Must be called before window handle has been created. See also QWindow::handle()");
    } else {
        window->setProperty(_useDxcb, true);
    }
}

void DPlatformWindowHandle::enableDXcbForWindow(QWidget *widget, bool redirectContent)
{
    enableDXcbForWindow(widget);

    if (isEnabledDXcb(widget)) {
        widget->windowHandle()->setProperty(_redirectContent, redirectContent);
    }
}

void DPlatformWindowHandle::enableDXcbForWindow(QWindow *window, bool redirectContent)
{
    enableDXcbForWindow(window);

    if (isEnabledDXcb(window)) {
        window->setProperty(_redirectContent, redirectContent);
    }
}

bool DPlatformWindowHandle::isEnabledDXcb(const QWidget *widget)
{
    return widget->windowHandle() && widget->windowHandle()->property(_useDxcb).toBool();
}

bool DPlatformWindowHandle::isEnabledDXcb(const QWindow *window)
{
    return window->property(_useDxcb).toBool();
}

bool DPlatformWindowHandle::setWindowBlurAreaByWM(QWidget *widget, const QVector<DPlatformWindowHandle::WMBlurArea> &area)
{
    Q_ASSERT(widget);

    return widget->windowHandle() && setWindowBlurAreaByWM(widget->windowHandle(), area);
}

bool DPlatformWindowHandle::setWindowBlurAreaByWM(QWidget *widget, const QList<QPainterPath> &paths)
{
    Q_ASSERT(widget);

    return widget->windowHandle() && setWindowBlurAreaByWM(widget->windowHandle(), paths);
}

inline DPlatformWindowHandle::WMBlurArea operator *(const DPlatformWindowHandle::WMBlurArea &area, qreal scale)
{
    if (qFuzzyCompare(scale, 1.0))
        return area;

    DPlatformWindowHandle::WMBlurArea new_area;

    new_area.x = qRound64(area.x * scale);
    new_area.y = qRound64(area.y * scale);
    new_area.width = qRound64(area.width * scale);
    new_area.height = qRound64(area.height * scale);
    new_area.xRadius = qRound64(area.xRadius * scale);
    new_area.yRaduis = qRound64(area.yRaduis * scale);

    return new_area;
}

bool DPlatformWindowHandle::setWindowBlurAreaByWM(QWindow *window, const QVector<DPlatformWindowHandle::WMBlurArea> &area)
{
    if (!window) {
        return false;
    }

    if (isEnabledDXcb(window)) {
        setWindowProperty(window, _windowBlurAreas, QVariant::fromValue(*(reinterpret_cast<const QVector<quint32>*>(&area))));

        return true;
    }

    QFunctionPointer setWmBlurWindowBackgroundArea = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    setWmBlurWindowBackgroundArea = qApp->platformFunction(_setWmBlurWindowBackgroundArea);
#endif

    if (!setWmBlurWindowBackgroundArea) {
        qWarning("setWindowBlurAreaByWM is not support");

        return false;
    }

    QSurfaceFormat format = window->format();

    format.setAlphaBufferSize(8);
    window->setFormat(format);

    const qreal device_ratio = window->devicePixelRatio();

    if (qFuzzyCompare(device_ratio, 1.0)) {
        return reinterpret_cast<bool(*)(const quint32, const QVector<WMBlurArea>&)>(setWmBlurWindowBackgroundArea)(window->winId(), area);
    }

    QVector<WMBlurArea> new_areas;

    new_areas.reserve(area.size());

    for (const WMBlurArea &a : area) {
        new_areas.append(a * device_ratio);
    }

    return reinterpret_cast<bool(*)(const quint32, const QVector<WMBlurArea>&)>(setWmBlurWindowBackgroundArea)(window->winId(), new_areas);
}

inline QPainterPath operator *(const QPainterPath &path, qreal scale)
{
    if (qFuzzyCompare(1.0, scale))
        return path;

    QPainterPath new_path = path;

    for (int i = 0; i < path.elementCount(); ++i) {
        const QPainterPath::Element &e = path.elementAt(i);

        new_path.setElementPositionAt(i, qRound(e.x * scale), qRound(e.y * scale));
    }

    return new_path;
}

bool DPlatformWindowHandle::setWindowBlurAreaByWM(QWindow *window, const QList<QPainterPath> &paths)
{
    if (!window) {
        return false;
    }

    if (isEnabledDXcb(window)) {
        setWindowProperty(window, _windowBlurPaths, QVariant::fromValue(paths));

        return true;
    }

    QFunctionPointer setWmBlurWindowBackgroundPathList = Q_NULLPTR;

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    setWmBlurWindowBackgroundPathList = qApp->platformFunction(_setWmBlurWindowBackgroundPathList);
#endif

    if (!setWmBlurWindowBackgroundPathList) {
        qWarning("setWindowBlurAreaByWM is not support");

        return false;
    }

    QSurfaceFormat format = window->format();

    format.setAlphaBufferSize(8);
    window->setFormat(format);

    const qreal device_ratio = window->devicePixelRatio();

    if (qFuzzyCompare(device_ratio, 1.0)) {
        return reinterpret_cast<bool(*)(const quint32, const QList<QPainterPath>&)>(setWmBlurWindowBackgroundPathList)(window->winId(), paths);
    }

    QList<QPainterPath> new_paths;

    new_paths.reserve(paths.size());

    for (const QPainterPath &p : paths) {
        new_paths.append(p * device_ratio);
    }

    return reinterpret_cast<bool(*)(const quint32, const QList<QPainterPath>&)>(setWmBlurWindowBackgroundPathList)(window->winId(), new_paths);
}

bool DPlatformWindowHandle::connectWindowManagerChangedSignal(QObject *object, std::function<void ()> slot)
{
    if (object) {
        return QObject::connect(DWindowManagerHelper::instance(), &DWindowManagerHelper::windowManagerChanged, object, slot);
    }

    return QObject::connect(DWindowManagerHelper::instance(), &DWindowManagerHelper::windowManagerChanged, slot);
}

bool DPlatformWindowHandle::connectHasBlurWindowChanged(QObject *object, std::function<void ()> slot)
{
    if (object) {
        return QObject::connect(DWindowManagerHelper::instance(), &DWindowManagerHelper::hasBlurWindowChanged, object, slot);
    }

    return QObject::connect(DWindowManagerHelper::instance(), &DWindowManagerHelper::hasBlurWindowChanged, slot);
}

bool DPlatformWindowHandle::setWindowBlurAreaByWM(const QVector<DPlatformWindowHandle::WMBlurArea> &area)
{
    return setWindowBlurAreaByWM(m_window, area);
}

bool DPlatformWindowHandle::setWindowBlurAreaByWM(const QList<QPainterPath> &paths)
{
    return setWindowBlurAreaByWM(m_window, paths);
}

int DPlatformWindowHandle::windowRadius() const
{
    return m_window->property(_windowRadius).toInt();
}

int DPlatformWindowHandle::borderWidth() const
{
    return m_window->property(_borderWidth).toInt();
}

QColor DPlatformWindowHandle::borderColor() const
{
    return qvariant_cast<QColor>(m_window->property(_borderColor));
}

int DPlatformWindowHandle::shadowRadius() const
{
    return m_window->property(_shadowRadius).toInt();
}

QPoint DPlatformWindowHandle::shadowOffset() const
{
    return m_window->property(_shadowOffset).toPoint();
}

QColor DPlatformWindowHandle::shadowColor() const
{
    return qvariant_cast<QColor>(m_window->property(_shadowColor));
}

QPainterPath DPlatformWindowHandle::clipPath() const
{
    return qvariant_cast<QPainterPath>(m_window->property(_clipPath));
}

QRegion DPlatformWindowHandle::frameMask() const
{
    return qvariant_cast<QRegion>(m_window->property(_frameMask));
}

QMargins DPlatformWindowHandle::frameMargins() const
{
    return qvariant_cast<QMargins>(m_window->property(_frameMargins));
}

bool DPlatformWindowHandle::translucentBackground() const
{
    return m_window->property(_translucentBackground).toBool();
}

bool DPlatformWindowHandle::enableSystemResize() const
{
    return m_window->property(_enableSystemResize).toBool();
}

bool DPlatformWindowHandle::enableSystemMove() const
{
    return m_window->property(_enableSystemMove).toBool();
}

bool DPlatformWindowHandle::enableBlurWindow() const
{
    return m_window->property(_enableBlurWindow).toBool();
}

bool DPlatformWindowHandle::autoInputMaskByClipPath() const
{
    return m_window->property(_autoInputMaskByClipPath).toBool();
}

WId DPlatformWindowHandle::realWindowId() const
{
    return qvariant_cast<WId>(m_window->property("_d_real_content_window"));
}

void DPlatformWindowHandle::setWindowRadius(int windowRadius)
{
    setWindowProperty(m_window, _windowRadius, windowRadius);
}

void DPlatformWindowHandle::setBorderWidth(int borderWidth)
{
    setWindowProperty(m_window, _borderWidth, borderWidth);
}

void DPlatformWindowHandle::setBorderColor(const QColor &borderColor)
{
    setWindowProperty(m_window, _borderColor, QVariant::fromValue(borderColor));
}

void DPlatformWindowHandle::setShadowRadius(int shadowRadius)
{
    setWindowProperty(m_window, _shadowRadius, shadowRadius);
}

void DPlatformWindowHandle::setShadowOffset(const QPoint &shadowOffset)
{
    setWindowProperty(m_window, _shadowOffset, shadowOffset);
}

void DPlatformWindowHandle::setShadowColor(const QColor &shadowColor)
{
    setWindowProperty(m_window, _shadowColor, QVariant::fromValue(shadowColor));
}

void DPlatformWindowHandle::setClipPath(const QPainterPath &clipPath)
{
    setWindowProperty(m_window, _clipPath, QVariant::fromValue(clipPath));
}

void DPlatformWindowHandle::setFrameMask(const QRegion &frameMask)
{
    setWindowProperty(m_window, _frameMask, QVariant::fromValue(frameMask));
}

void DPlatformWindowHandle::setTranslucentBackground(bool translucentBackground)
{
    setWindowProperty(m_window, _translucentBackground, translucentBackground);
}

void DPlatformWindowHandle::setEnableSystemResize(bool enableSystemResize)
{
    setWindowProperty(m_window, _enableSystemResize, enableSystemResize);
}

void DPlatformWindowHandle::setEnableSystemMove(bool enableSystemMove)
{
    setWindowProperty(m_window, _enableSystemMove, enableSystemMove);
}

void DPlatformWindowHandle::setEnableBlurWindow(bool enableBlurWindow)
{
    setWindowProperty(m_window, _enableBlurWindow, enableBlurWindow);
}

void DPlatformWindowHandle::setAutoInputMaskByClipPath(bool autoInputMaskByClipPath)
{
    setWindowProperty(m_window, _autoInputMaskByClipPath, autoInputMaskByClipPath);
}

bool DPlatformWindowHandle::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_window) {
        if (event->type() == QEvent::DynamicPropertyChange) {
            QDynamicPropertyChangeEvent *e = static_cast<QDynamicPropertyChangeEvent *>(event);

            if (e->propertyName() == _windowRadius) {
                Q_EMIT windowRadiusChanged();
            } else if (e->propertyName() == _borderWidth) {
                Q_EMIT borderWidthChanged();
            } else if (e->propertyName() == _borderColor) {
                Q_EMIT borderColorChanged();
            } else if (e->propertyName() == _shadowRadius) {
                Q_EMIT shadowRadiusChanged();
            } else if (e->propertyName() == _shadowOffset) {
                Q_EMIT shadowOffsetChanged();
            } else if (e->propertyName() == _shadowColor) {
                Q_EMIT shadowColorChanged();
            } else if (e->propertyName() == _clipPath) {
                Q_EMIT clipPathChanged();
            } else if (e->propertyName() == _frameMask) {
                Q_EMIT frameMaskChanged();
            } else if (e->propertyName() == _frameMargins) {
                Q_EMIT frameMarginsChanged();
            } else if (e->propertyName() == _translucentBackground) {
                Q_EMIT translucentBackgroundChanged();
            } else if (e->propertyName() == _enableSystemResize) {
                Q_EMIT enableSystemResizeChanged();
            } else if (e->propertyName() == _enableSystemMove) {
                Q_EMIT enableSystemMoveChanged();
            } else if (e->propertyName() == _enableBlurWindow) {
                Q_EMIT enableBlurWindowChanged();
            } else if (e->propertyName() == _autoInputMaskByClipPath) {
                Q_EMIT autoInputMaskByClipPathChanged();
            }
        }
    }

    return false;
}

DWIDGET_END_NAMESPACE

QT_BEGIN_NAMESPACE
QDebug operator<<(QDebug deg, const DPlatformWindowHandle::WMBlurArea &area)
{
    QDebugStateSaver saver(deg);
    Q_UNUSED(saver)

    deg.setAutoInsertSpaces(true);
    deg << "x:" << area.x
        << "y:" << area.y
        << "width:" << area.width
        << "height:" << area.height
        << "xRadius:" << area.xRadius
        << "yRadius:" << area.yRaduis;

    return deg;
}
QT_END_NAMESPACE
