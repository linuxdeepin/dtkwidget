#ifndef DPLATFORMWINDOWHANDLE_H
#define DPLATFORMWINDOWHANDLE_H

#include "dwidget_global.h"

#include <QObject>
#include <QPainterPath>
#include <QColor>
#include <QRegion>

#include <functional>

QT_BEGIN_NAMESPACE
class QWindow;
class QWidget;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DPlatformWindowHandle : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int windowRadius READ windowRadius WRITE setWindowRadius NOTIFY windowRadiusChanged)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(int shadowRadius READ shadowRadius WRITE setShadowRadius NOTIFY shadowRadiusChanged)
    Q_PROPERTY(QPoint shadowOffset READ shadowOffset WRITE setShadowOffset NOTIFY shadowOffsetChanged)
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged)
    Q_PROPERTY(QPainterPath clipPath READ clipPath WRITE setClipPath NOTIFY clipPathChanged)
    Q_PROPERTY(QRegion frameMask READ frameMask WRITE setFrameMask NOTIFY frameMaskChanged)
    Q_PROPERTY(QMargins frameMargins READ frameMargins NOTIFY frameMarginsChanged)
    Q_PROPERTY(bool translucentBackground READ translucentBackground WRITE setTranslucentBackground NOTIFY translucentBackgroundChanged)
    Q_PROPERTY(bool enableSystemResize READ enableSystemResize WRITE setEnableSystemResize NOTIFY enableSystemResizeChanged)
    Q_PROPERTY(bool enableSystemMove READ enableSystemMove WRITE setEnableSystemMove NOTIFY enableSystemMoveChanged)
    Q_PROPERTY(bool enableBlurWindow READ enableBlurWindow WRITE setEnableBlurWindow NOTIFY enableBlurWindowChanged)
    Q_PROPERTY(bool autoInputMaskByClipPath READ autoInputMaskByClipPath WRITE setAutoInputMaskByClipPath NOTIFY autoInputMaskByClipPathChanged)

public:
    explicit DPlatformWindowHandle(QWindow *window, QObject *parent = 0);
    explicit DPlatformWindowHandle(QWidget *widget, QObject *parent = 0);

    static void enableDXcbForWindow(QWidget *widget);
    static void enableDXcbForWindow(QWindow *window);
    static bool isEnabledDXcb(const QWidget *widget);
    static bool isEnabledDXcb(const QWindow *window);

    struct WMBlurArea {
        qint32 x = 0;
        qint32 y = 0;
        qint32 width = 0;
        qint32 height = 0;
        qint32 xRadius = 0;
        qint32 yRaduis = 0;
    };

    static bool hasBlurWindow();
    static bool setWindowBlurAreaByWM(QWidget *widget, const QVector<WMBlurArea> &area);
    static bool setWindowBlurAreaByWM(QWidget *widget, const QList<QPainterPath> &paths);
    static bool setWindowBlurAreaByWM(QWindow *window, const QVector<WMBlurArea> &area);
    static bool setWindowBlurAreaByWM(QWindow *window, const QList<QPainterPath> &paths);
    static bool connectWindowManagerChangedSignal(QObject *object, std::function<void ()> slot);
    static bool connectHasBlurWindowChanged(QObject *object, std::function<void ()> slot);

    bool setWindowBlurAreaByWM(const QVector<WMBlurArea> &area);
    bool setWindowBlurAreaByWM(const QList<QPainterPath> &paths);

    int windowRadius() const;

    int borderWidth() const;
    QColor borderColor() const;

    int shadowRadius() const;
    QPoint shadowOffset() const;
    QColor shadowColor() const;

    QPainterPath clipPath() const;
    QRegion frameMask() const;
    QMargins frameMargins() const;

    bool translucentBackground() const;
    bool enableSystemResize() const;
    bool enableSystemMove() const;
    bool enableBlurWindow() const;
    bool autoInputMaskByClipPath() const;

public slots:
    void setWindowRadius(int windowRadius);

    void setBorderWidth(int borderWidth);
    void setBorderColor(const QColor &borderColor);

    void setShadowRadius(int shadowRadius);
    void setShadowOffset(const QPoint &shadowOffset);
    void setShadowColor(const QColor &shadowColor);

    void setClipPath(const QPainterPath &clipPath);
    void setFrameMask(const QRegion &frameMask);

    void setTranslucentBackground(bool translucentBackground);
    void setEnableSystemResize(bool enableSystemResize);
    void setEnableSystemMove(bool enableSystemMove);
    void setEnableBlurWindow(bool enableBlurWindow);
    void setAutoInputMaskByClipPath(bool autoInputMaskByClipPath);

signals:
    void frameMarginsChanged();
    void windowRadiusChanged();
    void borderWidthChanged();
    void borderColorChanged();
    void shadowRadiusChanged();
    void shadowOffsetChanged();
    void shadowColorChanged();
    void clipPathChanged();
    void frameMaskChanged();
    void translucentBackgroundChanged();
    void enableSystemResizeChanged();
    void enableSystemMoveChanged();
    void enableBlurWindowChanged();
    void autoInputMaskByClipPathChanged();

protected:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

private:
    QWindow *m_window;
};

inline DPlatformWindowHandle::WMBlurArea dMakeWMBlurArea(quint32 x, quint32 y, quint32 width, quint32 height, quint32 xr = 0, quint32 yr = 0)
{
    DPlatformWindowHandle::WMBlurArea a;

    a.x = x;
    a.y = y;
    a.width = width;
    a.height = height;
    a.xRadius = xr;
    a.yRaduis = yr;

    return a;
}

DWIDGET_END_NAMESPACE

QT_BEGIN_NAMESPACE
DWIDGET_USE_NAMESPACE
QDebug operator<<(QDebug deg, const DPlatformWindowHandle::WMBlurArea &area);
QT_END_NAMESPACE

Q_DECLARE_METATYPE(QPainterPath)
Q_DECLARE_METATYPE(QRegion)
Q_DECLARE_METATYPE(QMargins)

#endif // DPLATFORMWINDOWHANDLE_H
