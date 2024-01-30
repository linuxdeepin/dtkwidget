// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dblureffectwidget.h"
#include "private/dblureffectwidget_p.h"
#include "dplatformwindowhandle.h"

#include <DWindowManagerHelper>
#include <DGuiApplicationHelper>

#include <QPainter>
#include <QBackingStore>
#include <QPaintEvent>
#include <QDebug>

#include <qpa/qplatformbackingstore.h>
#include <private/qwidget_p.h>
#ifndef slots
#define slots Q_SLOTS
#endif

#define private public

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
#include <private/qwidgetrepaintmanager_p.h>
#else
#include <private/qwidgetbackingstore_p.h>
#endif

#undef private

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
           || q->isWindow();
}

bool DBlurEffectWidgetPrivate::isFull() const
{
    D_QC(DBlurEffectWidget);

    return full || (q->isWindow() && !(blurRectXRadius && blurRectYRadius) && maskPath.isEmpty());
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

    blurEffectWidgetHash.insert(topLevelWidget, q);
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

quint8 DBlurEffectWidgetPrivate::getMaskColorAlpha() const
{
    if (maskAlpha < 0)
        return isBehindWindowBlendMode() ? 102 : 204;

    return static_cast<quint8>(maskAlpha);
}

QColor DBlurEffectWidgetPrivate::getMaskColor(const QColor &baseColor) const
{
    QColor color = baseColor;
    DGuiApplicationHelper::ColorType ct = DGuiApplicationHelper::toColorType(color);

    if (DGuiApplicationHelper::DarkType == ct) {
        color = DGuiApplicationHelper::adjustColor(color, 0, 0, -10, 0, 0, 0, 0);
    } else {
        color = DGuiApplicationHelper::adjustColor(color, 0, 0, -5, 0, 0, 0, 0);
    }

    int maskAlpha = this->getMaskColorAlpha();

    if (!isBehindWindowBlendMode() || DWindowManagerHelper::instance()->hasBlurWindow()) {
        color.setAlpha(maskAlpha);
    } else {
        return ct == DGuiApplicationHelper::DarkType ? "#202020" : "#D2D2D2";
    }

    return color;
}

void DBlurEffectWidgetPrivate::resetSourceImage()
{
    // 设置了自定义的image时忽略此请求
    // 属于某个组时也忽略
    if (customSourceImage || group)
        return;

    sourceImage = QImage();
}

void DBlurEffectWidgetPrivate::setMaskColor(const QColor &color)
{
    maskColor = color;

    if (isBehindWindowBlendMode()) {
        maskColor.setAlpha(DWindowManagerHelper::instance()->hasBlurWindow() ? getMaskColorAlpha() : MASK_COLOR_ALPHA_DEFAULT);
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
  \class Dtk::Widget::DBlurEffectWidget
  \inmodule dtkwidget

  \brief 用于实现主窗口或控件背景的实时模糊效果.
  \brief The DBlurEffectWidget class provides widget that backgrounds are blurred and semitranslucent.
  
  分为主窗口模糊 DBlurEffectWidget::BehindWindowBlend 和控件模糊 DBlurEffectWidget::InWindowBlend DBlurEffectWidget::InWindowBlend
  这两种不同的模式，主窗口模糊效果依赖于窗口管理器的实现，目前仅支持 deepin-wm 和 kwin，
  可以使用DWindowManagerHelper::hasBlurWindow 判断当前运行环境中的窗口管理器是否支持
  模糊特效，使用 DPlatformWindowHandle::setWindowBlurAreaByWM 设置主窗口背景的模糊
  区域。如果在一个主窗口内同时使用了多个 DBlurEffectWidget 控件，则在更新主窗口模糊区域
  时会自动将所有控件的区域合并到 QVector<WMBlurArea> 或者 QList<QPainterPath>
  （如果有任意一个控件设置了 DBlurEffectWidget::radius 属性）中统一设置。
  控件模糊效果的原理和 DClipEffectWidget 类似，在控件绘制时先使用 QPlatformBackingStore::toImage()
  获取主窗口上控件对应区域内已经绘制的内容，此时获取的内容为此控件下层控件所绘制的内容，将
  获得的 QImage 对象使用软件算法进行模糊处理，然后再将模糊之后的图像绘制到控件上。由于实现
  原理限制，不适用于主窗口为OpenGL绘制模式的窗口（如：QOpenGLWindow ），且控件下面使用
  OpenGL绘制的内容也生成模糊的效果（如 QOpenGLWidget ）

  DBlurEffectWidget is QWidget that has blurry background. With different
  blend mode set, DBlurEffectWidget can do in-window-blend, which means the
  the widget is blended with the widgets between the top level window and the
  widget itself, and behind-window-blend, which means the widget is blended with
  the scene behind the window (with the help of WM).
  
  The effect has optional styles can choose from: DBlurEffectWidget::DarkColor, DBlurEffectWidget::LightColor, and
  DBlurEffectWidget::CustomColor. Usually the first two are sufficient, you can also choose
  CustomColor and set the color you want by setting DBlurEffectWidget::maskColor.
  
  \note DBlurEffectWidget with BehindWindowBlend mode will become opaque if
  WM supports no X11 composite protocol.
 */

/*!
  \property DBlurEffectWidget::radius
  \brief 模糊效果计算时的取样范围，模糊的原理为：将此像素点和周围像素点合成计算得到得到新的值，
  此属性表示像素点周围 radius 距离的所有像素点参与计算
  \note 可读可写
  \note 只在模式为 DBlurEffectWidget::InWindowBlend 时有效
 */

/*!
  \property DBlurEffectWidget::mode
  \brief 所采用的模糊算法，默认值为 \a GaussianBlur
  \note 可读可写
  \note 只在模式为 DBlurEffectWidget::InWindowBlend 时有效
 */

/*!
  \property DBlurEffectWidget::blendMode
  \brief 模糊的应用场景，默认会根据有没有父控件自动判断使用哪种模式
  \note 可读可写
  \note 父对象为空时设置模式为 DBlurEffectWidget::InWindowBlend 没有意义
 */

/*!
  \property DBlurEffectWidget::blurRectXRadius
  \brief 模糊区域在x轴方向上的圆角半径，默认值为0
  \note 可读可写
  \sa DBlurEffectWidget::blurRectYRadius DBlurEffectWidget::setMaskPath QPainterPath::addRoundedRect
 */

/*!
  \property DBlurEffectWidget::blurRectYRadius
  \brief 模糊区域在y轴方向上的圆角半径，默认值为0
  \note 可读可写
  \sa DBlurEffectWidget::blurRectXRadius DBlurEffectWidget::setMaskPath QPainterPath::addRoundedRect
 */

/*!
  \property DBlurEffectWidget::maskColor
  \brief 画在控件模糊背景之上的颜色，此颜色的alpha通道值会被 \a maskAlpha 属性影响
  修改此属性会自动将颜色模式设置为 CustomColor，设置后将不能再自动根据窗口管理器是否
  支持混成和模糊来自动使用最佳的颜色
  \note 可读可写
  \sa DBlurEffectWidget::blurRectXRadius DBlurEffectWidget::setMaskColor()
 */

/*!
  \property DBlurEffectWidget::maskAlpha
  \brief maskColor 的alpha通道值。当前窗口管理器支持混成（窗口背景透明）时默认值为102，否则为204
  \note 可读可写
  \sa DBlurEffectWidget::maskColor
 */

/*!
  \property DBlurEffectWidget::full
  \brief 如果值为true，将模糊此控件所在顶层窗口的整个背景，而无论控件的大小和位置，否则使用控件的位置和大小
  设置顶层窗口的模糊区域。需要注意的时，当控件本身就是顶层窗口且未设置 blurRectXRadius 和 blurRectYRadius
  属性的情况下，无论 full 属性的值为多少，都将和值为 true 时的行为保持一致。
  \note 可读可写
  \note 此属性不影响蒙版的绘制区域
  \note 只在模糊的混合模式为 BehindWindowBlend 或 DBlurEffectWidget::InWindowBlend 时生效
  \sa DBlurEffectWidget::blurRectXRadius DBlurEffectWidget::blurRectYRadius
  \sa DBlurEffectWidget::maskColor
  \sa DBlurEffectWidget::blendMode
 */

/*!
  \property DBlurEffectWidget::blurEnabled
  \brief 如果值为 true 则此控件的模糊设置生效，否则不生效
  \note 可读可写
 */

/*!
  \fn void DBlurEffectWidget::radiusChanged(int radius)
  \brief 信号会在 \a radius 属性的值改变时被发送
 */
/*!
  \fn void DBlurEffectWidget::modeChanged(BlurMode mode)
  \brief 信号会在 \a mode 属性的值改变时被发送.
 */
/*!
  \fn void DBlurEffectWidget::blendModeChanged(BlendMode blendMode)
  \brief 信号会在 \a blendMode 属性的值改变时被发送
 */
/*!
  \fn void DBlurEffectWidget::blurRectXRadiusChanged(int blurRectXRadius)
  \brief 信号会在 \a blurRectXRadius 属性的值改变时被发送
 */
/*!
  \fn void DBlurEffectWidget::blurRectYRadiusChanged(int blurRectYRadius)
  \brief 信号会在 \a blurRectYRadius 属性的值改变时被发送
 */
/*!
  \fn void DBlurEffectWidget::maskAlphaChanged(quint8 alpha)
  \brief 信号会在 \a alpha 属性的值改变时被发送
  \sa DBlurEffectWidget::maskAlpha
 */
/*!
  \fn void DBlurEffectWidget::maskColorChanged(QColor maskColor)
  \brief 信号会在 \a maskColor 属性的值改变时被发送
 */

/*!
  \enum DBlurEffectWidget::BlurMode
  DBlurEffectWidget::BlurMode 模糊算法
  
  \value GaussianBlur
  \l {https://zh.wikipedia.org/wiki/高斯模糊}{高斯模糊算法}
 */

/*!
  \enum DBlurEffectWidget::BlendMode
  DBlurEffectWidget::BlendMode 模糊模式
  \image html blur-effect.png
  
  \var DBlurEffectWidget::BlendMode DBlurEffectWidget::InWindowBlend
  以窗口内部控件作为模糊背景
  
  \var DBlurEffectWidget::BlendMode DBlurEffectWidget::BehindWindowBlend
  以外部的其它窗口作为模糊背景
  
  \var DBlurEffectWidget::BlendMode DBlurEffectWidget::InWidgetBlend
  同 DBlurEffectWidget::InWindowBlend，区别是不会自动更新用于模糊的源图片
  可手动调用 DBlurEffectWidget::updateBlurSourceImage 更新，以此来实现自行控制模糊
  源图片的更新时机
 */

/*!
  \enum Dtk::Widget::DBlurEffectWidget::MaskColorType
  DBlurEffectWidget::MaskColorType 内置的几种颜色模式。分为三种情况：
  \a A：模式为 DBlurEffectWidget::InWindowBlend 或当前窗口管理器支持混成且支持窗口背景模糊
  \a B：模式为 DBlurEffectWidget::BehindWindowBlend 或 DBlurEffectWidget::InWindowBlend 窗口管理器不支持混成
  \a C：模式为 DBlurEffectWidget::BehindWindowBlend 或 DBlurEffectWidget::InWindowBlend 窗口管理器不支持模糊
  \sa DBlurEffectWidget::maskAlpha
  
  \value DarkColor
  深色，三种情况下的值分别为：
  
  \a A：color{black,#000000}（alpha通道值为非定值）
  \a B：color{#373F47,#373F47}
  \a C：color{rgba(0\,0\,0\,0.8),#CC000000}
  
  \value LightColor
  浅色，三种情况下的值分别为：
  \a A：color{#FFFFFF,#FFFFFF}（alpha通道值为非定值）
  \a B：color{#FCFCFC,#FCFCFC}
  \a C：color{rgba(255\,255\,255\,0.8),#CCFFFFFF}
  
  \value AutoColor
  自动颜色，以当前应用的主题的背景色作为叠加色（alpha通道值为非定值）
  
  \value CustomColor
  自定义颜色，使用 DBlurEffectWidget::setMaskColor 设置的颜色
 */

/*!
  \brief DBlurEffectWidget::DBlurEffectWidget constructs an instance of DBlurEffectWidget.
  \brief 和普通控件使用方式一样，可以作为任何控件的子控件。默认会开启 Qt::WA_TranslucentBackground，
  必须设置控件为背景透明，否则控件所在区域的内容重绘时，在此控件下方的区域不会被下层控件重新绘制

  \a parent is passed to QWidget constructor.
  \a parent 当父对象为空时，默认使用 DBlurEffectWidget::BehindWindowBlend 模式，否则使用 DBlurEffectWidget::InWindowBlend 模式
 */
DBlurEffectWidget::DBlurEffectWidget(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DBlurEffectWidgetPrivate(this))
{
    setAttribute(Qt::WA_TranslucentBackground);
    setBackgroundRole(QPalette::Window);

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

    // clean group
    if (d->group) {
        d->group->removeWidget(this);
    }
}

/*!
  \brief This property holds the radius of the blur effect.
  
  \note This property has no effect with the DBlurEffectWidget::blendMode set
  to DBlurEffectWidget::BehindWindowBlend.
 */
int DBlurEffectWidget::radius() const
{
    D_DC(DBlurEffectWidget);

    return d->radius;
}

/*!
  \brief This property holds which blur algorithm to be used.
  
  Currently it only supports DBlurEffectWidget::GaussianBlur.
 */
DBlurEffectWidget::BlurMode DBlurEffectWidget::mode() const
{
    D_DC(DBlurEffectWidget);

    return d->mode;
}

/*!
  \brief This property holds which mode is used to blend the widget and its background scene.
 */
DBlurEffectWidget::BlendMode DBlurEffectWidget::blendMode() const
{
    D_DC(DBlurEffectWidget);

    return d->blendMode;
}

/*!
  \brief This property holds the xRadius of the effective background.
  
  The xRadius and yRadius specify the radius of the ellipses defining
  the corners of the effective background.
  
  \sa DBlurEffectWidget::blurRectYRadius
 */
int DBlurEffectWidget::blurRectXRadius() const
{
    D_DC(DBlurEffectWidget);

    return d->blurRectXRadius;
}

/*!
  \brief This property holds the yRadius of the effective background.
  
  The xRadius and yRadius specify the radius of the ellipses defining
  the corners of the effective background.
  
  \sa DBlurEffectWidget::blurRectXRadius
 */
int DBlurEffectWidget::blurRectYRadius() const
{
    D_DC(DBlurEffectWidget);

    return d->blurRectYRadius;
}

/*!
  \brief This property holds the background color of this widget.
  
  It returns predefined colors if the DBlurEffectWidget::maskColorType is set
  to DBlurEffectWidget::DarkColor or BlurEffectWidget::LightColor, returns
  the color set by DBlurEffectWidget::setMaskColor if
  DBlurEffectWidget::maskColorType is set to BlurEffectWidget::CustomColor.
 */
QColor DBlurEffectWidget::maskColor() const
{
    D_DC(DBlurEffectWidget);

    switch ((int)d->maskColorType) {
    case DarkColor:
        return d->getMaskColor(DGuiApplicationHelper::standardPalette(DGuiApplicationHelper::DarkType).window().color());
    case LightColor:
        return d->getMaskColor(DGuiApplicationHelper::standardPalette(DGuiApplicationHelper::LightType).window().color());
    case AutoColor: {
        QColor color = palette().color(backgroundRole());

        return d->getMaskColor(color);
    }
    }

    return d->maskColor;
}

quint8 DBlurEffectWidget::maskAlpha() const {
    D_DC(DBlurEffectWidget);

    return d->getMaskColorAlpha();
}

/*!
  \brief DBlurEffectWidget::setMaskPath set custom area as the effective background.
  \brief 设置模糊区域的路径，当未调用过此接口时，模糊区域为整个控件所占据的空间，此路径坐标原点为控件左上角

  \a path a QPainterPath to be used as the effectvie background.
  \warning 设置自定义路径之后，控件大小改变时不会再自动调整模糊区域大小
  \sa DBlurEffectWidget::blurRectXRadius DBlurEffectWidget::blurRectYRadius
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

/*!
  \brief DBlurEffectWidget::setSourceImage
  \a image
  \a autoScale
  \warning
 */
void DBlurEffectWidget::setSourceImage(const QImage &image, bool autoScale)
{
    D_D(DBlurEffectWidget);

    d->sourceImage = image;
    d->customSourceImage = !image.isNull();
    d->autoScaleSourceImage = autoScale && d->customSourceImage;

    if (autoScale && isVisible()) {
        d->sourceImage.setDevicePixelRatio(devicePixelRatioF());
        d->sourceImage = d->sourceImage.scaled((size() + QSize(d->radius * 1, d->radius * 2)) * devicePixelRatioF());
        d->sourceImage.setDevicePixelRatio(devicePixelRatioF());
    }
}

/*!
  \brief DBlurEffectWidget::isFull
  \return　true 窗口占据了整个模糊区域,反之为false
 */
bool DBlurEffectWidget::isFull() const
{
    D_DC(DBlurEffectWidget);

    return d->full;
}

/*!
  \brief DBlurEffectWidget::blurEnabled
  \return true 模糊区域可伸缩调整
 */
bool DBlurEffectWidget::blurEnabled() const
{
    D_DC(DBlurEffectWidget);

    return d->blurEnabled;
}

/*!
  \brief DBlurEffectWidget::setRadius
  \a radius　模糊区域的圆角大小　如果设定值和原值不一致会发送信号radiusChanged
 */
void DBlurEffectWidget::setRadius(int radius)
{
    D_D(DBlurEffectWidget);

    if (d->radius == radius) {
        return;
    }

    d->radius = radius;
    d->resetSourceImage();

    update();

    Q_EMIT radiusChanged(radius);
}

/*!
  \brief DBlurEffectWidget::setMode
  \a mode 设定模糊算法,默认为高斯模糊算法GaussianBlur
 */
void DBlurEffectWidget::setMode(DBlurEffectWidget::BlurMode mode)
{
    D_D(DBlurEffectWidget);

    if (d->mode == mode) {
        return;
    }

    d->mode = mode;

    Q_EMIT modeChanged(mode);
}

/*!
  \brief DBlurEffectWidget::setBlendMode
  \a blendMode 窗口混合模式，模式设定变化发送blendModeChanged信号
 */
void DBlurEffectWidget::setBlendMode(DBlurEffectWidget::BlendMode blendMode)
{
    D_D(DBlurEffectWidget);

    if (d->blendMode == blendMode) {
        return;
    }

    if (blendMode == BehindWindowBlend) {
        d->addToBlurEffectWidgetHash();

        // 移除针对顶层窗口的事件过滤器
        topLevelWidget()->removeEventFilter(this);
    } else {
        if (blendMode != BehindWindowBlend) {
            d->maskColor.setAlpha(d->getMaskColorAlpha());
        }

        if (d->blendMode == BehindWindowBlend) {
            d->removeFromBlurEffectWidgetHash();
        }

        if (isVisible()) {
            // 给顶层窗口添加事件过滤器
            topLevelWidget()->installEventFilter(this);
        }
    }

    // 前面还有用到 d->blendMode 读取上一次的值
    d->blendMode = blendMode;
    update();

    Q_EMIT blendModeChanged(blendMode);
}

/*!
  \brief DBlurEffectWidget::setBlurRectXRadius
  \a blurRectXRadius 模糊区域矩形的X圆角大小
 */
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

/*!
  \brief DBlurEffectWidget::setBlurRectYRadius
  \a blurRectYRadius 模糊区域矩形的Y圆角大小
 */
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

/*!
  \brief DBlurEffectWidget::setMaskAlpha
  \a alpha　设置Alpha通道,值变化发送maskAlphaChanged信号
 */
void DBlurEffectWidget::setMaskAlpha(quint8 alpha) {
    D_D(DBlurEffectWidget);

    if (alpha == d->maskAlpha) return;

    d->setMaskAlpha(alpha);

    Q_EMIT maskAlphaChanged(alpha);
}

/*!
  \brief DBlurEffectWidget::setMaskColor
  \a maskColor 设定mask的颜色
 */
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
  \brief 设置控件的颜色模式，默认值为 MaskColorType::CustomColor
  \a type
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

/*!
  \brief DBlurEffectWidget::setFull
  \a full 设置时候模糊区域占满整个窗口大小
 */
void DBlurEffectWidget::setFull(bool full)
{
    D_D(DBlurEffectWidget);

    if (d->full == full)
        return;

    d->full = full;
    d->updateWindowBlurArea();

    Q_EMIT fullChanged(full);
}

/*!
  \brief DBlurEffectWidget::setBlurEnabled
  \a blurEnabled 设定模糊区域是否可以伸缩
 */
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

inline QRect operator *(const QRect &rect, qreal scale)
{
    return QRect(rect.left() * scale, rect.top() * scale, rect.width() * scale, rect.height() * scale);
}

/*!
  \brief DBlurEffectWidget::updateBlurSourceImage
  \a ren 设定模糊区域的背景图片
 */
void DBlurEffectWidget::updateBlurSourceImage(const QRegion &ren)
{
    D_D(DBlurEffectWidget);

    // 自定义模式，或者属于某个组时不需要以下操作
    if (d->customSourceImage || d->group)
        return;

    const qreal device_pixel_ratio = devicePixelRatioF();
    const QPoint point_offset = mapTo(window(), QPoint(0, 0));

    if (d->sourceImage.isNull()) {
        const QRect &tmp_rect = rect().translated(point_offset).adjusted(-d->radius, -d->radius, d->radius, d->radius);

        d->sourceImage = window()->backingStore()->handle()->toImage().copy(tmp_rect * device_pixel_ratio);
        d->sourceImage = d->sourceImage.scaledToWidth(d->sourceImage.width() / device_pixel_ratio);
    } else {
        QPainter pa_image(&d->sourceImage);

        pa_image.setCompositionMode(QPainter::CompositionMode_Source);

        if (device_pixel_ratio > 1) {
            const QRect &tmp_rect = this->rect().translated(point_offset);
            QImage area = window()->backingStore()->handle()->toImage().copy(tmp_rect * device_pixel_ratio);
            area = area.scaledToWidth(area.width() / device_pixel_ratio);

            for (const QRect &rect : ren) {
                pa_image.drawImage(rect.topLeft() + QPoint(d->radius, d->radius), rect == area.rect() ? area : area.copy(rect));
            }
        } else {
            for (const QRect &rect : ren) {
                pa_image.drawImage(rect.topLeft() + QPoint(d->radius, d->radius),
                                   window()->backingStore()->handle()->toImage().copy(rect.translated(point_offset)));
            }
        }

        pa_image.end();
    }
}

DBlurEffectWidget::DBlurEffectWidget(DBlurEffectWidgetPrivate &dd, QWidget *parent)
    : QWidget(parent)
    , DObject(dd)
{

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
        // 此模式下是自行控制sourceImage的更新
        if (d->blendMode != InWidgetBlend) {
            updateBlurSourceImage(event->region());
        }

        if (d->customSourceImage || !d->sourceImage.isNull()) {
            int radius = d->radius;
            qreal device_pixel_ratio = devicePixelRatioF();
            QImage image;
            const QRect &paintRect = event->rect();

            if (d->customSourceImage) {
                image = d->sourceImage.copy(paintRect.adjusted(0, 0, 2 * radius, 2 * radius) * device_pixel_ratio);
                image.setDevicePixelRatio(device_pixel_ratio);
                pa.setOpacity(0.2);
            } else {// 非customSourceImage不考虑缩放产生的影响
                image = d->sourceImage.copy(paintRect.adjusted(0, 0, 2 * radius, 2 * radius));
            }

            QTransform old_transform = pa.transform();
            pa.translate(paintRect.topLeft() - QPoint(radius, radius));
            qt_blurImage(&pa, image, radius, false, false);
            pa.setTransform(old_transform);
            pa.setOpacity(1);
        } else if (d->group) { // 组模式
            d->group->paint(&pa, this);
        }
    }

    pa.fillRect(rect(), maskColor());
}

void DBlurEffectWidget::moveEvent(QMoveEvent *event)
{
    D_D(DBlurEffectWidget);

    if (isWindow()) {
        return QWidget::moveEvent(event);
    }

    if (d->blendMode == DBlurEffectWidget::InWindowBlend
            || d->blendMode == DBlurEffectWidget::BehindWindowBlend) {
        d->resetSourceImage();

        return QWidget::moveEvent(event);
    }

    d->updateWindowBlurArea();

    QWidget::moveEvent(event);
}

void DBlurEffectWidget::resizeEvent(QResizeEvent *event)
{
    D_D(DBlurEffectWidget);

    d->resetSourceImage();

    if (!d->isBehindWindowBlendMode()) {
        if (d->autoScaleSourceImage) {
            d->sourceImage = d->sourceImage.scaled((size() + QSize(d->radius * 1, d->radius * 2)) * devicePixelRatioF());
            d->sourceImage.setDevicePixelRatio(devicePixelRatioF());
        }

        return QWidget::resizeEvent(event);
    }

    d->updateWindowBlurArea();

    QWidget::resizeEvent(event);
}

void DBlurEffectWidget::showEvent(QShowEvent *event)
{
    D_D(DBlurEffectWidget);

    if (!d->isBehindWindowBlendMode()) {
        if (d->autoScaleSourceImage) {
            d->sourceImage = d->sourceImage.scaled((size() + QSize(d->radius * 1, d->radius * 2)) * devicePixelRatioF());
            d->sourceImage.setDevicePixelRatio(devicePixelRatioF());
        }

        // 给顶层窗口添加事件过滤器
        topLevelWidget()->installEventFilter(this);

        return QWidget::showEvent(event);
    }

    d->addToBlurEffectWidgetHash();

    QWidget::showEvent(event);
}

void DBlurEffectWidget::hideEvent(QHideEvent *event)
{
    D_D(DBlurEffectWidget);

    if (!d->isBehindWindowBlendMode()) {
        // 移除事件过滤器
        topLevelWidget()->removeEventFilter(this);

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

bool DBlurEffectWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() != QEvent::UpdateRequest) {
        return QWidget::eventFilter(watched, event);
    }

    // 截获顶层窗口的绘制请求事件，判断需要重绘的区域是否在模糊半径内
    // 是的话则重绘模糊控件，因为避免由于DBlurEffectWidget控件外部（但是在模糊半径内，所以需要将此区域的内容计算到模糊）的重绘
    if (QWidget *widget = qobject_cast<QWidget*>(watched)) {
        auto wd = QWidgetPrivate::get(widget);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        if (!wd->maybeRepaintManager()) {
#else
        if (!wd->maybeBackingStore()) {
#endif
            return QWidget::eventFilter(watched, event);
        }

        // 当前待绘制的区域
        QRegion dirty;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        for (const QWidget *w : wd->maybeRepaintManager()->dirtyWidgets) {
#else
        for (const QWidget *w : wd->maybeBackingStore()->dirtyWidgets) {
#endif
            dirty |= QWidgetPrivate::get(w)->dirty.translated(w->mapToGlobal(QPoint(0, 0)));
        }

        if (dirty.isEmpty()) {
            return QWidget::eventFilter(watched, event);
        }

        D_D(DBlurEffectWidget);
        const QPoint &offset = mapToGlobal(QPoint(0, 0));
        const QRect frame_rect = rect() + QMargins(d->radius, d->radius, d->radius, d->radius);
        QRegion radius_edge = QRegion(frame_rect) - QRegion(rect());

        // 如果更新内容区域包含控件外围的区域（主要时radius半径下的区域），应当更新模糊绘制
        if (!(dirty & radius_edge.translated(offset)).isEmpty()) {
            // 此区域已经脏了，应当重置source image
            d->resetSourceImage();

            if (d->blendMode == InWidgetBlend)
                Q_EMIT blurSourceImageDirtied();
            else
                update();
        }
    }

    return QWidget::eventFilter(watched, event);
}

class DBlurEffectGroupPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DBlurEffectGroupPrivate(DBlurEffectGroup *qq)
        : DObjectPrivate(qq)
    {

    }

    D_DECLARE_PUBLIC(DBlurEffectGroup)
    QHash<DBlurEffectWidget*, QPoint> effectWidgetMap;
    QPixmap blurPixmap;
};

DBlurEffectGroup::DBlurEffectGroup()
    : DObject(*new DBlurEffectGroupPrivate(this))
{

}

DBlurEffectGroup::~DBlurEffectGroup()
{
    D_DC(DBlurEffectGroup);

    for (DBlurEffectWidget *widget : d->effectWidgetMap.keys()) {
        widget->d_func()->group = nullptr;
        widget->update();
    }
}

void DBlurEffectGroup::setSourceImage(QImage image, int blurRadius)
{
    D_D(DBlurEffectGroup);

    if (image.isNull()) {
        d->blurPixmap = QPixmap();
        return;
    }


    if (blurRadius > 0) {
        QImage tmp(image.size(), image.format());
        QPainter pa(&tmp);
        qt_blurImage(&pa, image, blurRadius, false, false);
        pa.end();
        d->blurPixmap = QPixmap::fromImage(tmp);
    } else {
        d->blurPixmap = QPixmap::fromImage(image);
    }

    d->blurPixmap.setDevicePixelRatio(image.devicePixelRatio());

    // 重绘制模糊控件
    for (auto begin = d->effectWidgetMap.constBegin(); begin != d->effectWidgetMap.constEnd(); ++begin) {
        begin.key()->update();
    }
}

void DBlurEffectGroup::addWidget(DBlurEffectWidget *widget, const QPoint &offset)
{
    if (widget->d_func()->group && widget->d_func()->group != this) {
        widget->d_func()->group->removeWidget(widget);
    }

    widget->d_func()->group = this;
    D_D(DBlurEffectGroup);
    d->effectWidgetMap[widget] = offset;

    widget->update();
}

void DBlurEffectGroup::removeWidget(DBlurEffectWidget *widget)
{
    D_D(DBlurEffectGroup);

    if (d->effectWidgetMap.remove(widget)) {
        widget->d_func()->group = nullptr;
        widget->update();
    }
}

void DBlurEffectGroup::paint(QPainter *pa, DBlurEffectWidget *widget) const
{
    D_DC(DBlurEffectGroup);

    pa->drawPixmap(widget->rect(), d->blurPixmap, widget->geometry().translated(d->effectWidgetMap[widget]));
}

DWIDGET_END_NAMESPACE
