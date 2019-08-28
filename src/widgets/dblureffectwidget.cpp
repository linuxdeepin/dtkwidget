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

#include "dblureffectwidget.h"
#include "private/dblureffectwidget_p.h"
#include "dplatformwindowhandle.h"

#include <DWindowManagerHelper>

#include <QPainter>
#include <QBackingStore>
#include <QPaintEvent>
#include <QDebug>

#include <qpa/qplatformbackingstore.h>

#define MASK_COLOR_ALPHA_DEFAULT 204

QT_BEGIN_NAMESPACE
Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

DGUI_USE_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

QMultiHash<QWidget *, const DBlurEffectWidget *> DBlurEffectWidgetPrivate::blurEffectWidgetHash;
QHash<const DBlurEffectWidget *, QWidget *> DBlurEffectWidgetPrivate::windowOfBlurEffectHash;

DBlurEffectWidgetPrivate::DBlurEffectWidgetPrivate(DBlurEffectWidget *qq)
    : DObjectPrivate(qq)
{

}

bool DBlurEffectWidgetPrivate::isBehindWindowBlendMode() const
{
    D_QC(DBlurEffectWidget);

    return blendMode == DBlurEffectWidget::BehindWindowBlend
           || q->isTopLevel();
}

bool DBlurEffectWidgetPrivate::isFull() const
{
    D_QC(DBlurEffectWidget);

    return full || (q->isTopLevel() && !(blurRectXRadius * blurRectYRadius) && maskPath.isEmpty());
}

void DBlurEffectWidgetPrivate::addToBlurEffectWidgetHash()
{
    D_QC(DBlurEffectWidget);

    QWidget *oldTopLevelWidget = windowOfBlurEffectHash.value(q);

    if (oldTopLevelWidget) {
        blurEffectWidgetHash.remove(oldTopLevelWidget, q);
        updateWindowBlurArea(oldTopLevelWidget);
    }

    QWidget *topLevelWidget = q->topLevelWidget();

    blurEffectWidgetHash.insertMulti(topLevelWidget, q);
    windowOfBlurEffectHash[q] = topLevelWidget;
    updateWindowBlurArea(topLevelWidget);
}

void DBlurEffectWidgetPrivate::removeFromBlurEffectWidgetHash()
{
    D_QC(DBlurEffectWidget);

    QWidget *topLevelWidget = windowOfBlurEffectHash.value(q);

    if (!topLevelWidget) {
        return;
    }

    blurEffectWidgetHash.remove(topLevelWidget, q);
    windowOfBlurEffectHash.remove(q);
    updateWindowBlurArea(topLevelWidget);
}

bool DBlurEffectWidgetPrivate::updateWindowBlurArea()
{
    D_QC(DBlurEffectWidget);

    QWidget *topLevelWidget = windowOfBlurEffectHash.value(q);

    return topLevelWidget && updateWindowBlurArea(topLevelWidget);
}

void DBlurEffectWidgetPrivate::setMaskAlpha(const quint8 alpha) {
    maskAlpha = alpha;

    // refresh alpha
    setMaskColor(maskColor);
}

void DBlurEffectWidgetPrivate::setMaskColor(const QColor &color)
{
    maskColor = color;

    if (isBehindWindowBlendMode()) {
        maskColor.setAlpha(DWindowManagerHelper::instance()->hasBlurWindow() ? maskAlpha : MASK_COLOR_ALPHA_DEFAULT);
    }

    D_Q(DBlurEffectWidget);

    q->update();
}

bool DBlurEffectWidgetPrivate::updateWindowBlurArea(QWidget *topLevelWidget)
{
    if (!topLevelWidget->isVisible()) {
        return false;
    }

    QList<const DBlurEffectWidget *> blurEffectWidgetList = blurEffectWidgetHash.values(topLevelWidget);

    bool isExistMaskPath = false;

    Q_FOREACH (const DBlurEffectWidget *w, blurEffectWidgetList) {
        if (!w->d_func()->blurEnabled) {
            continue;
        }

        // 当存在一个模糊区域填充整个窗口时，不需要再考虑其它的控件，直接对此窗口开启全模糊即可
        if (w->d_func()->isFull() && w->isVisible()) {
            DPlatformWindowHandle handle(topLevelWidget);

            if (!handle.enableBlurWindow()) {
                handle.setEnableBlurWindow(true);
            }

            return true;
        }

        if (!w->d_func()->maskPath.isEmpty() && w->isVisible()) {
            isExistMaskPath = true;
            break;
        }
    }

    // 清理窗口背景模糊是否完全开启的状态
    DPlatformWindowHandle handle(topLevelWidget);

    if (handle.enableBlurWindow()) {
        handle.setEnableBlurWindow(false);
    }

    bool ok = false;

    if (isExistMaskPath) {
        QList<QPainterPath> pathList;

        Q_FOREACH (const DBlurEffectWidget *w, blurEffectWidgetList) {
            if (!w->d_func()->blurEnabled) {
                continue;
            }

            if (!w->isVisible()) {
                continue;
            }

            QPainterPath p;
            QRect r = w->rect();

            r.moveTopLeft(w->mapTo(topLevelWidget, r.topLeft()));
            p.addRoundedRect(r, w->blurRectXRadius(), w->blurRectYRadius());

            if (!w->d_func()->maskPath.isEmpty()) {
                p &= w->d_func()->maskPath.translated(r.topLeft());
            }

            pathList << p;
        }

        ok = handle.setWindowBlurAreaByWM(pathList);
    } else {
        QVector<DPlatformWindowHandle::WMBlurArea> areaList;

        areaList.reserve(blurEffectWidgetList.size());

        Q_FOREACH (const DBlurEffectWidget *w, blurEffectWidgetList) {
            if (!w->d_func()->blurEnabled) {
                continue;
            }

            if (!w->isVisible()) {
                continue;
            }

            QRect r = w->rect();

            r.moveTopLeft(w->mapTo(topLevelWidget, r.topLeft()));

            areaList << dMakeWMBlurArea(r.x(), r.y(), r.width(), r.height(), w->blurRectXRadius(), w->blurRectYRadius());
        }

        ok = handle.setWindowBlurAreaByWM(areaList);
    }

    if (blurEffectWidgetList.isEmpty()) {
        blurEffectWidgetHash.remove(topLevelWidget);
    }

    return ok;
}

/*!
 * \~english \class DBlurEffectWidget
 * \~english \brief The DBlurEffectWidget class provides widget that backgrounds are blurred and semitranslucent.
 *
 * \~english DBlurEffectWidget is QWidget that has blurry background. With different
 * \~english blend mode set, DBlurEffectWidget can do in-window-blend, which means the
 * \~english the widget is blended with the widgets between the top level window and the
 * \~english widget itself, and behind-window-blend, which means the widget is blended with
 * \~english the scene behind the window (with the help of WM).
 *
 * \~english The effect has optional styles can choose from: DBlurEffectWidget::DarkColor, DBlurEffectWidget::LightColor, and
 * \~english DBlurEffectWidget::CustomColor. Usually the first two are sufficient, you can also choose
 * \~english CustomColor and set the color you want by setting DBlurEffectWidget::maskColor.
 *
 * \~english \note DBlurEffectWidget with BehindWindowBlend mode will become opaque if
 * \~english WM supports no X11 composite protocol.
 */

/*!
 * \~chinese \class DBlurEffectWidget
 * \~chinese \brief 用于实现主窗口或控件背景的实时模糊效果。
 *
 * \~chinese 分为主窗口模糊 DBlurEffectWidget::BehindWindowBlend 和控件模糊 DBlurEffectWidget::InWindowBlend
 * \~chinese 这两种不同的模式，主窗口模糊效果依赖于窗口管理器的实现，目前仅支持 deepin-wm 和 kwin，
 * \~chinese 可以使用DWindowManagerHelper::hasBlurWindow 判断当前运行环境中的窗口管理器是否支持
 * \~chinese 模糊特效，使用 DPlatformWindowHandle::setWindowBlurAreaByWM 设置主窗口背景的模糊
 * \~chinese 区域。如果在一个主窗口内同时使用了多个 DBlurEffectWidget 控件，则在更新主窗口模糊区域
 * \~chinese 时会自动将所有控件的区域合并到 QVector<WMBlurArea> 或者 QList<QPainterPath>
 * \~chinese （如果有任意一个控件设置了 DBlurEffectWidget::radius 属性）中统一设置。
 * \~chinese 控件模糊效果的原理和 DClipEffectWidget 类似，在控件绘制时先使用 QPlatformBackingStore::toImage()
 * \~chinese 获取主窗口上控件对应区域内已经绘制的内容，此时获取的内容为此控件下层控件所绘制的内容，将
 * \~chinese 获得的 QImage 对象使用软件算法进行模糊处理，然后再将模糊之后的图像绘制到控件上。由于实现
 * \~chinese 原理限制，不适用于主窗口为OpenGL绘制模式的窗口（如：QOpenGLWindow ），且控件下面使用
 * \~chinese OpenGL绘制的内容也生成模糊的效果（如 QOpenGLWidget ）
 */

/*!
 * \~chinese \property DBlurEffectWidget::radius
 * \~chinese \brief 模糊效果计算时的取样范围，模糊的原理为：将此像素点和周围像素点合成计算得到得到新的值，
 * \~chinese 此属性表示像素点周围 radius 距离的所有像素点参与计算
 * \~chinese \note 可读可写
 * \~chinese \note 只在模式为 DBlurEffectWidget::InWindowBlend 时有效
 */

/*!
 * \~chinese \property DBlurEffectWidget::mode
 * \~chinese \brief 所采用的模糊算法，默认值为 \a GaussianBlur
 * \~chinese \note 可读可写
 * \~chinese \note 只在模式为 DBlurEffectWidget::InWindowBlend 时有效
 */

/*!
 * \~chinese \property DBlurEffectWidget::blendMode
 * \~chinese \brief 模糊的应用场景，默认会根据有没有父控件自动判断使用哪种模式
 * \~chinese \note 可读可写
 * \~chinese \note 父对象为空时设置模式为 DBlurEffectWidget::InWindowBlend 没有意义
 */

/*!
 * \~chinese \property DBlurEffectWidget::blurRectXRadius
 * \~chinese \brief 模糊区域在x轴方向上的圆角半径，默认值为0
 * \~chinese \note 可读可写
 * \~chinese \see DBlurEffectWidget::blurRectYRadius DBlurEffectWidget::setMaskPath QPainterPath::addRoundedRect
 */

/*!
 * \~chinese \property DBlurEffectWidget::blurRectYRadius
 * \~chinese \brief 模糊区域在y轴方向上的圆角半径，默认值为0
 * \~chinese \note 可读可写
 * \~chinese \see DBlurEffectWidget::blurRectXRadius DBlurEffectWidget::setMaskPath QPainterPath::addRoundedRect
 */

/*!
 * \~chinese \property DBlurEffectWidget::maskColor
 * \~chinese \brief 画在控件模糊背景之上的颜色，此颜色的alpha通道值会被 \a maskAlpha 属性影响
 * \~chinese 修改此属性会自动将颜色模式设置为 CustomColor，设置后将不能再自动根据窗口管理器是否
 * \~chinese 支持混成和模糊来自动使用最佳的颜色
 * \~chinese \note 可读可写
 * \~chinese \see DBlurEffectWidget::blurRectXRadius DBlurEffectWidget::setMaskColor(MaskColorType)
 */

/*!
 * \~chinese \property DBlurEffectWidget::maskAlpha
 * \~chinese \brief maskColor 的alpha通道值。当前窗口管理器支持混成（窗口背景透明）时默认值为102，否则为204
 * \~chinese \note 可读可写
 * \~chinese \see DBlurEffectWidget::maskColor DPlatformWindowHandle::hasBlurWindow
 */

/*!
 * \~chinese \property DBlurEffectWidget::full
 * \~chinese \brief 如果值为true，将模糊此控件所在顶层窗口的整个背景，而无论控件的大小和位置，否则使用控件的位置和大小
 * \~chinese 设置顶层窗口的模糊区域。需要注意的时，当控件本身就是顶层窗口且未设置 blurRectXRadius 和 blurRectYRadius
 * \~chinese 属性的情况下，无论 full 属性的值为多少，都将和值为 true 时的行为保持一致。
 * \~chinese \note 可读可写
 * \~chinese \note 此属性不影响蒙版的绘制区域
 * \~chinese \note 只在模糊的混合模式为 BehindWindowBlend 时生效
 * \~chinese \see DBlurEffectWidget::blurRectXRadius BlurEffectWidget::blurRectYRadius
 * \~chinese \see DBlurEffectWidget::maskColor
 * \~chinese \see DBlurEffectWidget::blendMode
 */

/*!
 * \~chinese \property DBlurEffectWidget::blurEnabled
 * \~chinese \brief 如果值为 true 则此控件的模糊设置生效，否则不生效
 * \~chinese \note 可读可写
 */

/*!
 * \~chinese \fn DBlurEffectWidget::radiusChanged
 * \~chinese \brief 信号会在 radius 属性的值改变时被发送
 * \~chinese \fn DBlurEffectWidget::modeChanged
 * \~chinese \brief 信号会在 mode 属性的值改变时被发送
 * \~chinese \fn DBlurEffectWidget::blendModeChanged
 * \~chinese \brief 信号会在 blendMode 属性的值改变时被发送
 * \~chinese \fn DBlurEffectWidget::blurRectXRadiusChanged
 * \~chinese \brief 信号会在 blurRectXRadius 属性的值改变时被发送
 * * \~chinese \fn DBlurEffectWidget::blurRectYRadiusChanged
 * \~chinese \brief 信号会在 blurRectYRadius 属性的值改变时被发送
 * \~chinese \fn DBlurEffectWidget::maskAlphaChanged
 * \~chinese \brief 信号会在 maskAlpha 属性的值改变时被发送
 * \~chinese \fn DBlurEffectWidget::maskColorChanged
 * \~chinese \brief 信号会在 maskColor 属性的值改变时被发送
 */

/*!
 * \~chinese \enum DBlurEffectWidget::BlurMode
 * \~chinese DBlurEffectWidget::BlurMode 模糊算法
 *
 * \~chinese \var DBlurEffectWidget::GaussianBlur DBlurEffectWidget::GaussianBlur
 * \~chinese \href{https://zh.wikipedia.org/wiki/高斯模糊,高斯模糊算法}
 */

/*!
 * \~chinese \enum DBlurEffectWidget::BlendMode
 * \~chinese DBlurEffectWidget::BlendMode 模糊模式
 * \~chinese \image html blur-effect.png
 *
 * \~chinese \var DBlurEffectWidget::InWindowBlend DBlurEffectWidget::InWindowBlend
 * \~chinese 以窗口内部控件作为模糊背景
 *
 * \~chinese \var DBlurEffectWidget::BehindWindowBlend DBlurEffectWidget::BehindWindowBlend
 * \~chinese 以外部的其它窗口作为模糊背景
 */

/*!
 * \~chinese \enum DBlurEffectWidget::MaskColorType
 * \~chinese DBlurEffectWidget::MaskColorType 内置的几种颜色模式。分为三种情况：
 * \~chinese \arg \c A：模式为 DBlurEffectWidget::InWindowBlend 或当前窗口管理器支持混成且支持窗口背景模糊
 * \~chinese \arg \c B：模式为 DBlurEffectWidget::BehindWindowBlend 窗口管理器不支持混成
 * \~chinese \arg \c C：模式为 DBlurEffectWidget::BehindWindowBlend 窗口管理器不支持模糊
 * \~chinese \see DBlurEffectWidget::maskAlpha
 *
 * \~chinese \var DBlurEffectWidget::DarkColor DBlurEffectWidget::DarkColor
 * \~chinese 深色，三种情况下的值分别为：
 *
 * \~chinese \arg \c A：\color{black,#000000}（alpha通道值为非定值）
 * \~chinese \arg \c B：\color{#373F47,#373F47}
 * \~chinese \arg \c C：\color{rgba(0\,0\,0\,0.8),#CC000000}
 *
 * \~chinese \var DBlurEffectWidget::LightColor DBlurEffectWidget::LightColor
 * \~chinese 浅色，三种情况下的值分别为：
 * \~chinese \arg \c A：\color{#FFFFFF,#FFFFFF}（alpha通道值为非定值）
 * \~chinese \arg \c B：\color{#FCFCFC,#FCFCFC}
 * \~chinese \arg \c C：\color{rgba(255\,255\,255\,0.8),#CCFFFFFF}
 *
 * \~chinese \var DBlurEffectWidget::AutoColor DBlurEffectWidget::AutoColor
 * \~chinese 自动颜色，以当前应用的主题的背景色作为叠加色（alpha通道值为非定值）
 *
 * \~chinese \var DBlurEffectWidget::CustomColor DBlurEffectWidget::CustomColor
 * \~chinese 自定义颜色，使用 DBlurEffectWidget::setMaskColor 设置的颜色
 */

/*!
 * \~english \brief DBlurEffectWidget::DBlurEffectWidget constructs an instance of DBlurEffectWidget.
 * \~english \param parent is passed to QWidget constructor.
 */
/*!
 * \~chinese \brief 和普通控件使用方式一样，可以作为任何控件的子控件。默认会开启 Qt::WA_TranslucentBackground，
 * \~chinese 必须设置控件为背景透明，否则控件所在区域的内容重绘时，在此控件下方的区域不会被下层控件重新绘制
 * \~chinese \param 当父对象为空时，默认使用 DBlurEffectWidget::BehindWindowBlend 模式，否则使用 DBlurEffectWidget::InWindowBlend 模式
 */
DBlurEffectWidget::DBlurEffectWidget(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DBlurEffectWidgetPrivate(this))
{
    setAttribute(Qt::WA_TranslucentBackground);

    if (!parent) {
        D_D(DBlurEffectWidget);

        d->addToBlurEffectWidgetHash();
    }

    QObject::connect(DWindowManagerHelper::instance(), &DWindowManagerHelper::windowManagerChanged, this, [this] {
        D_D(DBlurEffectWidget);

        d->updateWindowBlurArea();
    });
    QObject::connect(DWindowManagerHelper::instance(), &DWindowManagerHelper::hasBlurWindowChanged, this, [this] {
        D_D(DBlurEffectWidget);

        d->setMaskColor(d->maskColor);
    });
    QObject::connect(DWindowManagerHelper::instance(), &DWindowManagerHelper::hasCompositeChanged, this, [this] {
        D_D(const DBlurEffectWidget);

        if (d->maskColorType != CustomColor)
            update();
    });
}

DBlurEffectWidget::~DBlurEffectWidget()
{
    D_D(DBlurEffectWidget);

    if (d->isBehindWindowBlendMode()) {
        d->removeFromBlurEffectWidgetHash();
    }
}

/*!
 * \~english \property DBlurEffectWidget::radius
 * \~english \brief This property holds the radius of the blur effect.
 *
 * \~english \note This property has no effect with the DBlurEffectWidget::blendMode set
 * \~english to DBlurEffectWidget::BehindWindowBlend.
 */
int DBlurEffectWidget::radius() const
{
    D_DC(DBlurEffectWidget);

    return d->radius;
}

/*!
 * \~english \property DBlurEffectWidget::mode
 * \~english \brief This property holds which blur alghorithm to be used.
 *
 * \~english Currently it only supports DBlurEffectWidget::GaussianBlur.
 */
DBlurEffectWidget::BlurMode DBlurEffectWidget::mode() const
{
    D_DC(DBlurEffectWidget);

    return d->mode;
}

/*!
 * \~english \property DBlurEffectWidget::blendMode
 * \~english \brief This property holds which mode is used to blend the widget and its background scene.
 */
DBlurEffectWidget::BlendMode DBlurEffectWidget::blendMode() const
{
    D_DC(DBlurEffectWidget);

    return d->blendMode;
}

/*!
 * \~english \property DBlurEffectWidget::blurRectXRadius
 * \~english \brief This property holds the xRadius of the effective background.
 *
 * \~english The xRadius and yRadius specify the radius of the ellipses defining
 * \~english the corners of the effective background.
 *
 * \~english \see DBlurEffectWidget::blurRectYRadius
 */
int DBlurEffectWidget::blurRectXRadius() const
{
    D_DC(DBlurEffectWidget);

    return d->blurRectXRadius;
}

/*!
 * \~english \property DBlurEffectWidget::blurRectYRadius
 * \~english \brief This property holds the yRadius of the effective background.
 *
 * \~english The xRadius and yRadius specify the radius of the ellipses defining
 * \~english the corners of the effective background.
 *
 * \~english \see DBlurEffectWidget::blurRectXRadius
 */
int DBlurEffectWidget::blurRectYRadius() const
{
    D_DC(DBlurEffectWidget);

    return d->blurRectYRadius;
}

/*!
 * \~english \property DBlurEffectWidget::maskColor
 * \~english \brief This property holds the background color of this widget.
 *
 * \~english It returns predefined colors if the DBlurEffectWidget::maskColorType is set
 * \~english to DBlurEffectWidget::DarkColor or BlurEffectWidget::LightColor, returns
 * \~english the color set by DBlurEffectWidget::setMaskColor if
 * \~english DBlurEffectWidget::maskColorType is set to BlurEffectWidget::CustomColor.
 */
QColor DBlurEffectWidget::maskColor() const
{
    D_DC(DBlurEffectWidget);

    switch ((int)d->maskColorType) {
    case DarkColor: {
        if (!d->isBehindWindowBlendMode()) {
            return QColor(0, 0, 0, d->maskAlpha);
        }

        if (DWindowManagerHelper::instance()->hasComposite()) {
            return QColor(0, 0, 0, DWindowManagerHelper::instance()->hasBlurWindow() ? d->maskAlpha : MASK_COLOR_ALPHA_DEFAULT);
        } else {
            return QColor("#373F47");
        }
    }
    case LightColor: {
        if (!d->isBehindWindowBlendMode()) {
            return QColor(255, 255, 255, d->maskAlpha);
        }

        if (DWindowManagerHelper::instance()->hasComposite()) {
            return QColor(255, 255, 255, DWindowManagerHelper::instance()->hasBlurWindow() ? d->maskAlpha : MASK_COLOR_ALPHA_DEFAULT);
        } else {
            return QColor("#FCFCFC");
        }
    }
    case AutoColor: {
        QColor color = palette().color(backgroundRole());
        if (!d->isBehindWindowBlendMode()) {
            color.setAlpha(d->maskAlpha);
            return color;
        }

        if (DWindowManagerHelper::instance()->hasComposite()) {
            color.setAlpha(DWindowManagerHelper::instance()->hasBlurWindow() ? d->maskAlpha : MASK_COLOR_ALPHA_DEFAULT);
        }
        return color;
    }
    }

    return d->maskColor;
}

quint8 DBlurEffectWidget::maskAlpha() const {
    D_DC(DBlurEffectWidget);

    return d->maskAlpha;
}

/*!
 * \~english \brief DBlurEffectWidget::setMaskPath set custom area as the effective background.
 * \~english \param path a QPainterPath to be used as the effectvie background.
 */
/*!
 * \~chinese \brief 设置模糊区域的路径，当未调用过此接口时，模糊区域为整个控件所占据的空间，此路径坐标原点为控件左上角
 * \~chinese \param path
 * \~chinese \warning 设置自定义路径之后，控件大小改变时不会再自动调整模糊区域大小
 * \~chinese \see DBlurEffectWidget::blurRectXRadius DBlurEffectWidget::blurRectYRadius
 */
void DBlurEffectWidget::setMaskPath(const QPainterPath &path)
{
    D_D(DBlurEffectWidget);

    if (d->maskPath == path) {
        return;
    }

    d->maskPath = path;

    update();
}

bool DBlurEffectWidget::isFull() const
{
    D_DC(DBlurEffectWidget);

    return d->full;
}

bool DBlurEffectWidget::blurEnabled() const
{
    D_DC(DBlurEffectWidget);

    return d->blurEnabled;
}

void DBlurEffectWidget::setRadius(int radius)
{
    D_D(DBlurEffectWidget);

    if (d->radius == radius) {
        return;
    }

    d->radius = radius;
    d->sourceImage = QImage();

    update();

    Q_EMIT radiusChanged(radius);
}

void DBlurEffectWidget::setMode(DBlurEffectWidget::BlurMode mode)
{
    D_D(DBlurEffectWidget);

    if (d->mode == mode) {
        return;
    }

    d->mode = mode;

    Q_EMIT modeChanged(mode);
}

void DBlurEffectWidget::setBlendMode(DBlurEffectWidget::BlendMode blendMode)
{
    D_D(DBlurEffectWidget);

    if (d->blendMode == blendMode) {
        return;
    }

    if (blendMode == BehindWindowBlend) {
        d->addToBlurEffectWidgetHash();
    } else {
        if (blendMode == InWindowBlend) {
            d->maskColor.setAlpha(d->maskAlpha);
        }

        if (d->blendMode == BehindWindowBlend) {
            d->removeFromBlurEffectWidgetHash();
        }
    }

    d->blendMode = blendMode;

    update();

    Q_EMIT blendModeChanged(blendMode);
}

void DBlurEffectWidget::setBlurRectXRadius(int blurRectXRadius)
{
    D_D(DBlurEffectWidget);

    if (d->blurRectXRadius == blurRectXRadius) {
        return;
    }

    d->blurRectXRadius = blurRectXRadius;

    update();

    Q_EMIT blurRectXRadiusChanged(blurRectXRadius);
}

void DBlurEffectWidget::setBlurRectYRadius(int blurRectYRadius)
{
    D_D(DBlurEffectWidget);

    if (d->blurRectYRadius == blurRectYRadius) {
        return;
    }

    d->blurRectYRadius = blurRectYRadius;

    update();

    Q_EMIT blurRectYRadiusChanged(blurRectYRadius);
}

void DBlurEffectWidget::setMaskAlpha(quint8 alpha) {
    D_D(DBlurEffectWidget);

    if (alpha == d->maskAlpha) return;

    d->setMaskAlpha(alpha);

    Q_EMIT maskAlphaChanged(alpha);
}

void DBlurEffectWidget::setMaskColor(QColor maskColor)
{
    D_D(DBlurEffectWidget);

    if (!maskColor.isValid()) {
        maskColor = Qt::transparent;
    }

    if (d->maskColor == maskColor) {
        return;
    }

    d->maskColorType = CustomColor;
    d->setMaskColor(maskColor);

    Q_EMIT maskColorChanged(maskColor);
}

/*!
 * \~chinese \brief 设置控件的颜色模式，默认值为 \a CustomColor
 * \~chinese \param type
 */
void DBlurEffectWidget::setMaskColor(DBlurEffectWidget::MaskColorType type)
{
    D_D(DBlurEffectWidget);

    if (d->maskColorType == type) {
        return;
    }

    d->maskColorType = type;

    update();
}

void DBlurEffectWidget::setFull(bool full)
{
    D_D(DBlurEffectWidget);

    if (d->full == full)
        return;

    d->full = full;
    d->updateWindowBlurArea();

    Q_EMIT fullChanged(full);
}

void DBlurEffectWidget::setBlurEnabled(bool blurEnabled)
{
    D_D(DBlurEffectWidget);

    if (d->blurEnabled == blurEnabled)
        return;

    d->blurEnabled = blurEnabled;
    d->updateWindowBlurArea();
    update();

    Q_EMIT blurEnabledChanged(d->blurEnabled);
}

DBlurEffectWidget::DBlurEffectWidget(DBlurEffectWidgetPrivate &dd, QWidget *parent)
    : QWidget(parent)
    , DObject(dd)
{

}

inline QRect operator *(const QRect &rect, qreal scale)
{
    return QRect(rect.left() * scale, rect.top() * scale, rect.width() * scale, rect.height() * scale);
}

void DBlurEffectWidget::paintEvent(QPaintEvent *event)
{
    D_D(DBlurEffectWidget);

    if (!d->blurEnabled)
        return;

    QPainter pa(this);

    if (d->blurRectXRadius > 0 || d->blurRectYRadius > 0) {
        QPainterPath path;

        path.addRoundedRect(rect(), d->blurRectXRadius, d->blurRectYRadius);
        pa.setRenderHint(QPainter::Antialiasing);
        pa.setClipPath(path);
    }

    if (!d->maskPath.isEmpty()) {
        QPainterPath path = pa.clipPath();

        if (path.isEmpty()) {
            path = d->maskPath;
        } else {
            path &= d->maskPath;
        }

        pa.setClipPath(path);
    }

    if (d->isBehindWindowBlendMode()) {
        pa.setCompositionMode(QPainter::CompositionMode_Source);
    } else {
        int radius = d->radius;
        QPoint point_offset = mapTo(window(), QPoint(0, 0));
        const QRect &paintRect = event->rect();
        qreal device_pixel_ratio = devicePixelRatioF();

        if (d->sourceImage.isNull()) {
            const QRect &tmp_rect = rect().translated(point_offset).adjusted(-radius, -radius, radius, radius);

            d->sourceImage = window()->backingStore()->handle()->toImage().copy(tmp_rect * device_pixel_ratio);
            d->sourceImage = d->sourceImage.scaledToWidth(d->sourceImage.width() / device_pixel_ratio);
        } else {
            QPainter pa_image(&d->sourceImage);

            pa_image.setCompositionMode(QPainter::CompositionMode_Source);

            for (const QRect &rect : event->region().rects()) {
                if (device_pixel_ratio > 1) {
                    const QRect &tmp_rect = this->rect().translated(point_offset);
                    const QImage &area = window()->backingStore()->handle()->toImage().copy(tmp_rect * device_pixel_ratio);

                    pa_image.drawImage(rect.topLeft() + QPoint(radius, radius), area.scaledToWidth(area.width() / device_pixel_ratio).copy(rect));
                } else {
                    pa_image.drawImage(rect.topLeft() + QPoint(radius, radius),
                                       window()->backingStore()->handle()->toImage().copy(rect.translated(point_offset)));
                }
            }

            pa_image.end();
        }

        QImage image = d->sourceImage.copy(paintRect.adjusted(0, 0, 2 * radius, 2 * radius));

        QTransform old_transform = pa.transform();
        pa.translate(paintRect.topLeft() - QPoint(radius, radius));
        qt_blurImage(&pa, image, radius, false, false);
        pa.setTransform(old_transform);
    }

    pa.fillRect(rect(), maskColor());
}

void DBlurEffectWidget::moveEvent(QMoveEvent *event)
{
    D_D(DBlurEffectWidget);

    if (isTopLevel()) {
        return QWidget::moveEvent(event);
    }

    if (d->blendMode == DBlurEffectWidget::InWindowBlend) {
        d->sourceImage = QImage();

        return QWidget::moveEvent(event);
    }

    d->updateWindowBlurArea();

    QWidget::moveEvent(event);
}

void DBlurEffectWidget::resizeEvent(QResizeEvent *event)
{
    D_D(DBlurEffectWidget);

    d->sourceImage = QImage();

    if (!d->isBehindWindowBlendMode()) {
        return QWidget::resizeEvent(event);
    }

    d->updateWindowBlurArea();

    QWidget::resizeEvent(event);
}

void DBlurEffectWidget::showEvent(QShowEvent *event)
{
    D_D(DBlurEffectWidget);

    if (!d->isBehindWindowBlendMode()) {
        return QWidget::showEvent(event);
    }

    d->addToBlurEffectWidgetHash();

    QWidget::showEvent(event);
}

void DBlurEffectWidget::hideEvent(QHideEvent *event)
{
    D_D(DBlurEffectWidget);

    if (!d->isBehindWindowBlendMode()) {
        return QWidget::hideEvent(event);
    }

    d->removeFromBlurEffectWidgetHash();

    QWidget::hideEvent(event);
}

void DBlurEffectWidget::changeEvent(QEvent *event)
{
    D_D(DBlurEffectWidget);

    if (!d->isBehindWindowBlendMode()) {
        return QWidget::changeEvent(event);
    }

    if (event->type() == QEvent::ParentAboutToChange) {
        d->removeFromBlurEffectWidgetHash();
    } else if (event->type() == QEvent::ParentChange) {
        d->addToBlurEffectWidgetHash();
    }

    QWidget::changeEvent(event);
}

DWIDGET_END_NAMESPACE
