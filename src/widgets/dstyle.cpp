// SPDX-FileCopyrightText: 2019 - 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dstyle.h"
#include "ddciicon.h"
#include "ddciiconpalette.h"
#include "dguiapplicationhelper.h"
#include "dstyleoption.h"
#include "dtooltip.h"
#include "dsizemode.h"

#include <DGuiApplicationHelper>
#include <DIconTheme>
#include <DConfig>

#include <QStyleOption>
#include <QTextLayout>
#include <QTextLine>
#include <QPixmapCache>
#include <QGuiApplication>
#include <QAbstractItemView>
#include <QPainterPath>

#include <qmath.h>
#include <private/qfixed_p.h>
#include <private/qtextengine_p.h>
#include <private/qicon_p.h>

#include <math.h>

QT_BEGIN_NAMESPACE
//extern Q_WIDGETS_EXPORT void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed = 0);
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

DCORE_USE_NAMESPACE
DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE


/*!
  \brief 该函数用于调整给定颜色.

  此函数用于调整给定颜色 \a base 的色彩混合度，通过参数 \a hueFloat , \a saturationFloat
  \a lightnessFloat , \a redFloat , \a greenFloat , \a blueFloat, 以及 \a alphaFloat
  来控制。其中 \a hueFloat 表示色调浮动值， \a saturationFloat 表示饱和度浮动值， \a lightnessFloat 表示
  亮度浮动值， \a redFloat 表示红色色系的浮动值， \a greenFloat 表示绿色色系的浮动值， \a blueFloat 表示蓝色
  色系的浮动值， \a alphaFloat 表示透明度的浮动值。

  \sa Dtk::Gui::DGuiApplicationHelper::adjustColor()
 */
QColor DStyle::adjustColor(const QColor &base,
                           qint8 hueFloat, qint8 saturationFloat, qint8 lightnessFloat,
                           qint8 redFloat, qint8 greenFloat, qint8 blueFloat, qint8 alphaFloat)
{
    return DGuiApplicationHelper::adjustColor(base, hueFloat, saturationFloat, lightnessFloat, redFloat, greenFloat, blueFloat, alphaFloat);
}

/*!
  \brief 该函数用于混合两种给定颜色.

  此函数用于混合给定的 \a substrate 以及 \a superstratum 颜色值，并返回 \return 一个
  以此两种颜色为基础的全新颜色。其中， \a substrate 表示底色,  \a superstratum 表示上层
  颜色。

  \sa Dtk::Gui::DGuiApplicationHelper::blendColor()
 */
QColor DStyle::blendColor(const QColor &substrate, const QColor &superstratum)
{
    return DGuiApplicationHelper::blendColor(substrate, superstratum);
}

/*!
  \brief 根据给定选项值返回图标的模式和状态.

  通过参数 \a option 中的 QIcon::State 属性和 QIcon::Mode 属性，
  返回 \return 当前图标模式对应的状态值。
 */
QPair<QIcon::Mode, QIcon::State> DStyle::toIconModeState(const QStyleOption *option)
{
    QIcon::Mode mode = QIcon::Normal;
    QIcon::State state = option->state & State_On ? QIcon::On : QIcon::Off;

    if (option->state & QStyle::State_Enabled) {
        if (option->state & (State_Sunken | State_Selected)) {
            mode = QIcon::Selected;
        } else if (option->state & State_MouseOver) {
            mode = QIcon::Active;
        }
    } else {
        mode = QIcon::Disabled;
    }

    return qMakePair(mode, state);
}

DDciIcon::Mode DStyle::toDciIconMode(const QStyleOption *option)
{
    DDciIcon::Mode mode = DDciIcon::Normal;

    if (option->state & QStyle::State_Enabled) {
        if (option->state & (State_Sunken | State_Selected)) {
            mode = DDciIcon::Pressed;
        } else if (option->state & State_MouseOver) {
            mode = DDciIcon::Hover;
        }
    } else {
        mode = DDciIcon::Disabled;
    }

    return mode;
}

static DDciIconPalette makeIconPalette(const QPalette &pal)
{
    DDciIconPalette iconPalette;
    iconPalette.setForeground(pal.color(QPalette::WindowText));
    iconPalette.setBackground(pal.color(QPalette::Window));
    iconPalette.setHighlight(pal.color(QPalette::Highlight));
    iconPalette.setHighlightForeground(pal.color(QPalette::HighlightedText));
    return iconPalette;
}

/*!
  \brief 设置 tooltip 的文本格式.

  通过给定 \a format , 将 DStyle 内部中的 ToolTip
  文本格式设置为 \a format 指定的样式。

  \sa Qt::TextFormat
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
void DStyle::setTooltipTextFormat(Qt::TextFormat format)
{
    DToolTip::setToolTipTextFormat(format);
}
#endif

/*!
  \brief 获取 tooltip 文本格式.

  \return 返回 DStyle 内部使用的 ToolTip 文本格式。

  \sa Qt::TextFormat
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
Qt::TextFormat DStyle::tooltipTextFormat()
{
    return DToolTip::toolTipTextFormat();
}
#endif

void DStyle::setFocusRectVisible(QWidget *widget, bool visible)
{
    widget->setProperty("_d_dtk_noFocusRect", !visible);
}

void DStyle::setFrameRadius(QWidget *widget, int radius)
{
    widget->setProperty("_d_dtk_frameRadius", radius);
}

void DStyle::setUncheckedItemIndicatorVisible(QWidget *widget, bool visible)
{
    widget->setProperty("_d_dtk_UncheckedItemIndicator", visible);
}

void DStyle::setRedPointVisible(QObject *object, bool visible)
{
    object->setProperty("_d_menu_item_redpoint", visible);
}

void DStyle::setShortcutUnderlineVisible(bool visible)
{
    qApp->setProperty("_d_menu_underlineshortcut", visible);
}

static inline bool hasConfig(const QString &key, bool fallback = false)
{
    static DConfig config("org.deepin.dtk.preference");
    return config.value(key, fallback).toBool();
}

static inline bool hasProperty(const char *key, std::function<bool()> fallback)
{
    const QVariant &prop = qApp->property(key);
    if (prop.isValid())
        return prop.toBool();

    return fallback();
}

static inline bool hasEnv(const char *key, std::function<bool()> fallback)
{
    if (qEnvironmentVariableIsSet(key))
        return true;

    return fallback();
}

bool DStyle::shortcutUnderlineVisible()
{
    return hasEnv("D_MENU_UNDERLINESHORTCUT", []()->bool {
        return hasProperty("_d_menu_underlineshortcut", []()->bool {
            return hasConfig("underlineShortcut");
        });
    });
}

void DStyle::setMenuKeyboardSearchDisabled(bool disabled)
{
    qApp->setProperty("_d_menu_keyboardsearch_disabled", disabled);
}

bool DStyle::isMenuKeyboardSearchDisabled()
{
    return hasEnv("D_MENU_DISABLE_KEYBOARDSEARCH", []()->bool {
        return hasProperty("_d_menu_keyboardsearch_disabled", []()->bool {
            return hasConfig("keyboardsearchDisabled");
        });
    });
}

namespace DDrawUtils {
static QImage dropShadow(const QPixmap &px, qreal radius, const QColor &color)
{
    if (px.isNull())
        return QImage();

    QImage tmp(px.size() + QSize(radius * 2, radius * 2), QImage::Format_ARGB32_Premultiplied);
    tmp.fill(0);
    QPainter tmpPainter(&tmp);
    tmpPainter.setOpacity(0.3); // design requirement
    tmpPainter.setRenderHint(QPainter::Antialiasing);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.drawPixmap(QPoint(radius, radius), px);
    tmpPainter.end();

    // blur the alpha channel
    QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
    blurred.fill(0);
    QPainter blurPainter(&blurred);
    qt_blurImage(&blurPainter, tmp, radius, false, true);
    blurPainter.end();

    if (color == QColor(Qt::black))
        return blurred;

    tmp = blurred;

    // blacken the image...
    tmpPainter.begin(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    tmpPainter.fillRect(tmp.rect(), color);
    tmpPainter.end();

    return tmp;
}

static QList<QRect> sudokuByRect(const QRect &rect, QMargins borders)
{
    QList<QRect> list;

//    qreal border_width = borders.left() + borders.right();

//    if ( border_width > rect.width()) {
//        borders.setLeft(borders.left() / border_width * rect.width());
//        borders.setRight(rect.width() - borders.left());
//    }

//    qreal border_height = borders.top() + borders.bottom();

//    if (border_height > rect.height()) {
//        borders.setTop(borders.top()/ border_height * rect.height());
//        borders.setBottom(rect.height() - borders.top());
//    }

    const QRect &contentsRect = rect - borders;

    list << QRect(0, 0, borders.left(), borders.top());
    list << QRect(list.at(0).topRight(), QSize(contentsRect.width(), borders.top())).translated(1, 0);
    list << QRect(list.at(1).topRight(), QSize(borders.right(), borders.top())).translated(1, 0);
    list << QRect(list.at(0).bottomLeft(), QSize(borders.left(), contentsRect.height())).translated(0, 1);
    list << contentsRect;
    list << QRect(contentsRect.topRight(), QSize(borders.right(), contentsRect.height())).translated(1, 0);
    list << QRect(list.at(3).bottomLeft(), QSize(borders.left(), borders.bottom())).translated(0, 1);
    list << QRect(contentsRect.bottomLeft(), QSize(contentsRect.width(), borders.bottom())).translated(0, 1);
    list << QRect(contentsRect.bottomRight(), QSize(borders.left(), borders.bottom())).translated(1, 1);

    return list;
}

static QImage borderImage(const QPixmap &px, const QMargins &borders, const QSize &size, QImage::Format format)
{
    QImage image(size, format);
    QPainter pa(&image);

    const QList<QRect> sudoku_src = sudokuByRect(px.rect(), borders);
    const QList<QRect> sudoku_tar = sudokuByRect(QRect(QPoint(0, 0), size), borders);

    pa.setCompositionMode(QPainter::CompositionMode_Source);  //设置组合模式

    for (int i = 0; i < 9; ++i) {
        pa.drawPixmap(sudoku_tar[i], px, sudoku_src[i]);
    }

    pa.end();

    return image;
}

void drawShadow(QPainter *pa, const QRect &rect, qreal xRadius, qreal yRadius, const QColor &sc, qreal radius, const QPoint &offset)
{
    QPixmap shadow;
    qreal scale = pa->paintEngine()->paintDevice()->devicePixelRatioF();
    QRect shadow_rect = rect;

    shadow_rect.setTopLeft(shadow_rect.topLeft() + offset);

    xRadius *= scale;
    yRadius *= scale;
    radius *= scale;

    const QString &key = QString("dtk-shadow-%1x%2-%3-%4").arg(xRadius).arg(yRadius).arg(sc.name()).arg(radius);

    if (!QPixmapCache::find(key, &shadow)) {
        QImage shadow_base(QSize(xRadius * 3, yRadius * 3), QImage::Format_ARGB32_Premultiplied);
        shadow_base.fill(0);
        QPainter pa(&shadow_base);

        pa.setBrush(sc);
        pa.setPen(Qt::NoPen);
        pa.setRenderHint(QPainter::Antialiasing);
        pa.drawRoundedRect(shadow_base.rect(), xRadius, yRadius);
        pa.end();

        shadow_base = dropShadow(QPixmap::fromImage(shadow_base), radius, sc);
        shadow = QPixmap::fromImage(shadow_base);
        QPixmapCache::insert(key, shadow);
    }

    const QMargins margins(xRadius + radius, yRadius + radius, xRadius + radius, yRadius + radius);
    QImage new_shadow = borderImage(shadow, margins, shadow_rect.size() * scale, QImage::Format_ARGB32_Premultiplied);
//    QPainter pa_shadow(&new_shadow);
//    pa_shadow.setCompositionMode(QPainter::CompositionMode_Clear);
//    pa_shadow.setPen(Qt::NoPen);
//    pa_shadow.setBrush(Qt::transparent);
//    pa_shadow.setRenderHint(QPainter::Antialiasing);
//    pa_shadow.drawRoundedRect((new_shadow.rect() - QMargins(radius, radius, radius, radius)).translated(-offset), xRadius, yRadius);
//    pa_shadow.end();
    new_shadow.setDevicePixelRatio(scale);
    pa->drawImage(shadow_rect.topLeft(), new_shadow);
}

void drawShadow(QPainter *pa, const QRect &rect, const QPainterPath &path, const QColor &sc, int radius, const QPoint &offset)
{
    QPixmap shadow;
    qreal scale = pa->paintEngine()->paintDevice()->devicePixelRatioF();
    QRect shadow_rect = rect;

    shadow_rect.setTopLeft(rect.topLeft() + offset);
    radius *= scale;

    QImage shadow_base(shadow_rect.size() * scale, QImage::Format_ARGB32_Premultiplied);
    shadow_base.fill(0);
    shadow_base.setDevicePixelRatio(scale);

    QPainter paTmp(&shadow_base);
    paTmp.setRenderHint(QPainter::Antialiasing, true);
    paTmp.setBrush(sc);
    paTmp.setPen(Qt::NoPen);
    paTmp.drawPath(path);
    paTmp.end();
    shadow_base = dropShadow(QPixmap::fromImage(shadow_base), radius, sc);
    shadow = QPixmap::fromImage(shadow_base);
    shadow.setDevicePixelRatio(scale);

    pa->drawPixmap(shadow_rect, shadow);
}

void drawFork(QPainter *pa, const QRectF &rect, const QColor &color, int width)
{
    QPen pen;
    pen.setWidth(width);
    pen.setColor(color);

    pa->setRenderHint(QPainter::Antialiasing, true);
    pa->setPen(pen);
    pa->setBrush(Qt::NoBrush);

    drawForkElement(pa, rect);
}

void drawRoundedRect(QPainter *pa, const QRect &rect, qreal xRadius, qreal yRadius, Corners corners, Qt::SizeMode mode)
{
    QRectF r = rect.normalized();

    if (r.isNull())
        return;

    if (mode == Qt::AbsoluteSize) {
        qreal w = r.width() / 2;
        qreal h = r.height() / 2;

        if (w == 0) {
            xRadius = 0;
        } else {
            xRadius = 100 * qMin(xRadius, w) / w;
        }
        if (h == 0) {
            yRadius = 0;
        } else {
            yRadius = 100 * qMin(yRadius, h) / h;
        }
    } else {
        if (xRadius > 100)                          // fix ranges
            xRadius = 100;

        if (yRadius > 100)
            yRadius = 100;
    }

    if (xRadius <= 0 || yRadius <= 0) {             // add normal rectangle
        pa->drawRect(r);
        return;
    }

    QPainterPath path;
    qreal x = r.x();
    qreal y = r.y();
    qreal w = r.width();
    qreal h = r.height();
    qreal rxx2 = w * xRadius / 100;
    qreal ryy2 = h * yRadius / 100;

    path.arcMoveTo(x, y, rxx2, ryy2, 180);

    if (corners & TopLeftCorner) {
        path.arcTo(x, y, rxx2, ryy2, 180, -90);
    } else {
        path.lineTo(r.topLeft());
    }

    if (corners & TopRightCorner) {
        path.arcTo(x + w - rxx2, y, rxx2, ryy2, 90, -90);
    } else {
        path.lineTo(r.topRight());
    }

    if (corners & BottomRightCorner) {
        path.arcTo(x + w - rxx2, y + h - ryy2, rxx2, ryy2, 0, -90);
    } else {
        path.lineTo(r.bottomRight());
    }

    if (corners & BottomLeftCorner) {
        path.arcTo(x, y + h - ryy2, rxx2, ryy2, 270, -90);
    } else {
        path.lineTo(r.bottomLeft());
    }

    path.closeSubpath();
    pa->drawPath(path);
}

void drawMark(QPainter *pa, const QRectF &rect, const QColor &boxInside, const QColor &boxOutside, const int penWidth, const int outLineLeng)
{
    QPen pen(boxInside);
    pen.setWidth(penWidth);
    pa->setPen(pen);
    pen.setJoinStyle(Qt::RoundJoin);
    pa->setRenderHint(QPainter::Antialiasing, true);

    drawMarkElement(pa, rect);

    if (outLineLeng == 0)
        return;

    double xWide = (rect.width() / 2.0);
    int yHigh = rect.height();
    double length = sqrt(pow(xWide, 2) + pow(yHigh, 2));
    double x = rect.right() + (outLineLeng / length) * xWide;
    double y = rect.y() - (outLineLeng / length) * yHigh;

    pen.setColor(boxOutside);
    pa->setPen(pen);
    pa->drawLine(QPointF(rect.topRight()), QPointF(x, y));
}

void drawBorder(QPainter *pa, const QRectF &rect, const QBrush &brush, int borderWidth, int radius)
{
    pa->setPen(QPen(brush, borderWidth, Qt::SolidLine));
    pa->setBrush(Qt::NoBrush);
    pa->setRenderHint(QPainter::Antialiasing);
    pa->drawRoundedRect(rect.adjusted(1, 1, -1, -1), radius, radius) ;
}

void drawArrow(QPainter *pa, const QRectF &rect, const QColor &color, Qt::ArrowType arrow, int width)
{
    QPen pen;
    pen.setWidth(width);
    pen.setColor(color);
    pa->setRenderHint(QPainter::Antialiasing, true);
    pa->setPen(pen);
    pa->setBrush(Qt::NoBrush);

    drawArrowElement(arrow, pa, rect);
}

void drawPlus(QPainter *painter, const QRectF &rect, const QColor &color, qreal width)
{
    QPen pen = color;
    pen.setWidthF(width);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    drawIncreaseElement(painter, rect);
}

void drawSubtract(QPainter *painter, const QRectF &rect, const QColor &color, qreal width)
{
    QPen pen = color;
    pen.setWidthF(width);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    drawDecreaseElement(painter, rect);
}

void drawForkElement(QPainter *pa, const QRectF &rect)
{
    pa->drawLine(rect.topLeft(), rect.bottomRight());
    pa->drawLine(rect.bottomLeft(), rect.topRight());
}

void drawDecreaseElement(QPainter *pa, const QRectF &rect)
{
    qreal centerY = rect.center().y();

    pa->drawLine(QPointF(rect.left(), centerY), QPointF(rect.right(), centerY));
}

void drawIncreaseElement(QPainter *pa, const QRectF &rect)
{
    qreal centerX = rect.center().x();
    qreal centerY = rect.center().y();

    pa->drawLine(QPointF(rect.x(), centerY), QPointF(rect.right(), centerY));
    pa->drawLine(QPointF(centerX, rect.y()), QPointF(centerX, rect.bottom()));
}

void drawMarkElement(QPainter *pa, const QRectF &rect)
{
    pa->drawLine(rect.x(), rect.center().y(), rect.center().x(), rect.bottom());
    pa->drawLine(rect.center().x(), rect.bottom(), rect.right(), rect.y());
}

void drawSelectElement(QPainter *pa, const QRectF &rect)
{
    qreal radius = rect.width() / 15.0;
    QPointF pointff = rect.center();

    pa->setBrush(QBrush(pa->pen().color()));
    pa->drawEllipse(pointff, radius, radius);
    pa->drawEllipse(pointff - QPointF(radius * 3.5, 0), radius, radius);
    pa->drawEllipse(pointff + QPointF(radius * 3.5, 0), radius, radius);
}

void drawExpandElement(QPainter *pa, const QRectF &rect)
{
    drawArrowUp(pa, rect);
}

void drawReduceElement(QPainter *pa, const QRectF &rect)
{
    drawArrowDown(pa, rect);
}

void drawLockElement(QPainter *pa, const QRectF &rect)
{
    qreal width = rect.width() / 5.0;
    qreal height = rect.height() / 5.0;
    qreal y = rect.y();
    qreal x = rect.x();

    QRectF topRect(x + width, y, width * 3, height * 3);
    QRectF bottomRect(x, y + height * 2, rect.width(), rect.height() - height * 2);
    QPainterPath path;

    path.arcMoveTo(topRect, 0);
    path.arcTo(topRect, 0, 180);

    path.addRect(bottomRect);
    pa->drawPath(path);
}

void drawUnlockElement(QPainter *pa, const QRectF &rect)
{
    qreal width = rect.width() / 5.0;
    qreal height = rect.height() / 5.0;
    qreal y = rect.y();
    qreal x = rect.x();

    QRectF topRect(x + width * 3, y, rect.width() - width * 3, height * 3);
    QRectF bottomRect(x + width, y + height * 2, rect.width() - width * 2, rect.height() - height * 2);
    QPainterPath path;

    path.arcMoveTo(topRect, 0);
    path.arcTo(topRect, 0, 180);
    path.addRect(bottomRect);
    pa->drawPath(path);
}

void drawArrowEnter(QPainter *pa, const QRectF &rect)
{
    drawArrowRight(pa, rect);
}

void drawArrowLeave(QPainter *pa, const QRectF &rect)
{
    drawArrowLeft(pa, rect);
}

void drawArrowNext(QPainter *pa, const QRectF &rect)
{
    QRectF content_rect(rect.x() + rect.width() / 2, rect.y(), rect.width() / 2, rect.height());
    qreal y = rect.center().y();

    drawArrowElement(Qt::RightArrow, pa, content_rect);
    pa->drawLine(rect.x(), y, rect.bottom(), y);
}

void drawArrowPrev(QPainter *pa, const QRectF &rect)
{
    QRectF content_rect(rect.x(), rect.y(), rect.width() / 2, rect.height());
    qreal y = rect.center().y();

    drawArrowElement(Qt::LeftArrow, pa, content_rect);
    pa->drawLine(rect.x(), y, rect.bottom(), y);
}

void drawCloseButton(QPainter *pa, const QRectF &rect)
{
    pa->setRenderHint(QPainter::Antialiasing);
    QPen pen = pa->pen();
    QPen new_pen = pa->pen();
    QColor border_color = pen.color();
    border_color.setAlphaF(0.1);
    new_pen.setColor(border_color);
    pa->setPen(new_pen);
    qreal pen_extent = pen.widthF() / 2;
    pa->drawEllipse(rect.adjusted(pen_extent, pen_extent, -pen_extent, -pen_extent));
    QRectF content_rect(0, 0, rect.width() / 3, rect.height() / 3);
    content_rect.moveCenter(rect.center());
    pa->setPen(pen);
    drawForkElement(pa, content_rect);
}

void drawDeleteButton(QPainter *pa, const QRectF &rect)
{
    const QPen pen = pa->pen();
    pa->setPen(Qt::NoPen);

    QPainterPath path;
    path.addEllipse(rect);

    QRectF hRect(rect.x(), rect.y(), rect.width() / 2, 1);

    hRect.moveCenter(rect.center());

    path.addRect(hRect);

    pa->fillPath(path, QColor("#ff6a6a"));
}

void drawAddButton(QPainter *pa, const QRectF &rect)
{
    const QPen pen = pa->pen();
    pa->setPen(Qt::NoPen);

    QPainterPath path;
    path.addEllipse(rect);

    QRectF hRect(rect.x(), rect.y(), rect.width() / 2, 1);
    QRectF vRect(rect.x(), rect.y(), 1, rect.height() / 2);

    hRect.moveCenter(rect.center());
    vRect.moveCenter(rect.center());

    path.addRect(hRect);
    path.addRect(vRect);

    QRectF center(0, 0, 1, 1);
    center.moveCenter(rect.center());
    path.addEllipse(center);

    pa->fillPath(path, QColor("#48bf00"));
}

void drawTitleBarIcon(QPainter *pa, const QRectF &rect, const QString &iconName)
{
    const QPen pen = pa->pen();
    pa->setPen(Qt::NoPen);
    pa->drawRect(rect);
    QRect content_rect(0, 0, rect.width() / 5, rect.height() / 5);
    content_rect.moveCenter(rect.center().toPoint());
    pa->setPen(pen);

    const DDciIcon &icon = DDciIcon::fromTheme(iconName);
    auto devicePixelRatio = pa->device() ? pa->device()->devicePixelRatioF()
                                         : qApp->devicePixelRatio();
    auto appTheme = DGuiApplicationHelper::toColorType(pa->pen().color());
    DDciIcon::Theme theme = appTheme == DGuiApplicationHelper::LightType ? DDciIcon::Light : DDciIcon::Dark;
    DDciIconPalette palette(pa->pen().color());
    icon.paint(pa, rect.toRect(), devicePixelRatio, theme, DDciIcon::Normal, Qt::AlignCenter, palette);
}

void drawTitleBarMenuButton(QPainter *pa, const QRectF &rect)
{
    drawTitleBarIcon(pa, rect, QLatin1String("window_menu"));
}

void drawTitleBarMinButton(QPainter *pa, const QRectF &rect)
{
    drawTitleBarIcon(pa, rect, QLatin1String("window_minimize"));
}

void drawTitleBarMaxButton(QPainter *pa, const QRectF &rect)
{
    drawTitleBarIcon(pa, rect, QLatin1String("window_maximize"));
}

void drawTitleBarCloseButton(QPainter *pa, const QRectF &rect)
{
    drawTitleBarIcon(pa, rect, QLatin1String("window_close"));
}

void drawTitleBarNormalButton(QPainter *pa, const QRectF &rect)
{
    drawTitleBarIcon(pa, rect, QLatin1String("window_normal"));
}

void drawTitleQuitFullButton(QPainter *pa, const QRectF &rect)
{
    const QPen pen = pa->pen();
    pa->setPen(Qt::NoPen);
    pa->drawRect(rect);
    QRectF content_rect(0, 0, rect.width() / 5, rect.height() / 5);
    content_rect.moveCenter(rect.center());
    pa->setPen(pen);
    pa->setRenderHint(QPainter::Antialiasing, pa->device()->devicePixelRatioF() > 1.0);
    qreal x = content_rect.x();
    qreal y = content_rect.y();
    qreal w = content_rect.width();
    qreal h = content_rect.height();
    qreal mean = w / 11;

    QRectF right(x + mean * 6, y, mean * 5, mean * 5);
    pa->drawLine(QPointF(x + mean * 7, y), QPointF(x + mean * 7, y + mean * 5));
    pa->drawLine(QPointF(x + mean * 7, y + mean * 5), QPointF(x + w, y + mean * 5));
    pa->drawPoint(right.center());

    QRectF left(x, y + mean * 6, mean * 5, mean * 5);
    pa->drawLine(QPointF(x, y + mean * 7), QPointF(x + mean * 5, y + mean * 7));
    pa->drawLine(QPointF(x + mean * 5, y + mean * 7), QPointF(x + mean * 5, y + h));
    pa->drawPoint(left.center());
}

void drawArrowUp(QPainter *pa, const QRectF &rect)
{
    QRectF ar(0, 0, rect.width(), rect.height() / 2);
    ar.moveCenter(rect.center());
    drawArrowElement(Qt::UpArrow, pa, ar);
}

void drawArrowDown(QPainter *pa, const QRectF &rect)
{
    QRectF ar(0, 0, rect.width(), rect.height() / 2);
    ar.moveCenter(rect.center());
    drawArrowElement(Qt::DownArrow, pa, ar);
}

void drawArrowLeft(QPainter *pa, const QRectF &rect)
{
    QRectF ar(0, 0, rect.width() / 2, rect.height());
    ar.moveCenter(rect.center());
    drawArrowElement(Qt::LeftArrow, pa, ar);
}

void drawArrowRight(QPainter *pa, const QRectF &rect)
{
    QRectF ar(0, 0, rect.width() / 2, rect.height());
    ar.moveCenter(rect.center());
    drawArrowElement(Qt::RightArrow, pa, ar);
}

void drawArrowBack(QPainter *pa, const QRectF &rect)
{
    drawArrowLeft(pa, rect);
}

void drawArrowForward(QPainter *pa, const QRectF &rect)
{
    drawArrowRight(pa, rect);
}

void drawLineEditClearButton(QPainter *pa, const QRectF &rect)
{
    drawCloseButton(pa, rect);
}

void drawIndicatorUnchecked(QPainter *pa, const QRectF &rect)
{
    pa->drawEllipse(rect);
}

void drawIndicatorChecked(QPainter *pa, const QRectF &rect)
{
    QRectF mark(0, 0, rect.width() / 2, rect.height() / 2);
    mark.moveCenter(rect.center());
    QStyleOption opt;
    QColor brush = opt.palette.color(DPalette::Highlight);
    pa->setPen(Qt::NoPen);
    pa->setBrush(brush);

    pa->drawEllipse(rect);
    drawMark(pa, mark, opt.palette.color(DPalette::Window), opt.palette.color(DPalette::Window), 2, 0);
}

void drawArrowElement(Qt::ArrowType arrow, QPainter *pa, const QRectF &rect)
{
    const QPointF center = rect.center();

    switch (arrow) {
    case Qt::UpArrow: {
        pa->drawLine(QPointF(center.x(), rect.y()), rect.bottomLeft());
        pa->drawLine(QPointF(center.x(), rect.y()), rect.bottomRight());
        break;
    }
    case Qt::LeftArrow: {
        pa->drawLine(QPointF(rect.x(), center.y()), rect.bottomRight());
        pa->drawLine(QPointF(rect.x(), center.y()), rect.topRight());
        break;
    }
    case Qt::DownArrow: {
        pa->drawLine(QPointF(center.x(), rect.bottom()), rect.topLeft());
        pa->drawLine(QPointF(center.x(), rect.bottom()), rect.topRight());
        break;
    }
    case Qt::RightArrow: {
        pa->drawLine(QPointF(rect.right(), center.y()), rect.topLeft());
        pa->drawLine(QPointF(rect.right(), center.y()), rect.bottomLeft());
        break;
    }
    default:
        break;
    }
}

}

/*!
  \class Dtk::Widget::DStyle
  \inmodule dtkwidget
  \brief DStyle 提供了一个 Dtk 控件的基础类.

  类似于 QCommonStyle 中的实现， DStyle 中只实现了
  一些基础控件的绘制和一个通用接口的实现，如果想要实现
  一个自定义的风格主题，可以通过尝试继承该类以实现自己的
  功能。不过，仅使用 DStyle 并不会将控件的风格保持和 Dtk
  控件一致，这是由于 Dtk 的实际控件风格在 Chameleon 风格
  插件中实现。因此如果需要在 Dtk 中继承 Chameleon 风格，并
  添加自定义风格的绘制，可以尝试使用 QProxyStyle 类。

  \sa QCommonStyle QProxyStyle
 */

/*!
  \enum Dtk::Widget::DStyle::PrimitiveElement
  \sa DStyle::PrimitiveElement QStyle::PrimitiveElement
  
  \value PE_ItemBackground
  item 的背景颜色
  
  \value PE_IconButtonPanel
  DIconButton 面板
  
  \value PE_IconButtonIcon
  DIconButton 不同状态下的背景颜色
  
  \value PE_Icon
  DIconButton
  
  \value PE_SwitchButtonGroove
  DSwitchButton 面板
  
  \value PE_SwitchButtonHandle
  DSwitchButton 滑块面板
  
  \value PE_FloatingWidget
  DFloatingWidget 面板

  \omitvalue PE_CustomBase
*/

/*!
  \enum Dtk::Widget::DStyle::StyleState
  记录控件状态的值
  
  \value SS_NormalState
  正常状态
  
  \value SS_HoverState
  悬停状态
  
  \value SS_PressState
  按下状态
  
  \value SS_StateCustomBase
  自定义子元素的基值
  
  \value StyleState_Mask
  状态掩码
  
  \value SS_CheckedFlag
  选定标志
  
  \value SS_SelectedFlag
  选中状态
  
  \value SS_FocusFlag
  焦点状态
  
  \value SS_FlagCustomBase
  自定义子元素的基值
*/

/*!
  \enum Dtk::Widget::DStyle::PixelMetric
  \sa DStyle::PixelMetric QStyle::PixelMetric
  
  \value PM_FocusBorderWidth
  控件焦点状态的边框宽度
  
  \value PM_FocusBorderSpacing
  控件内容和border之间的间隔
  
  \value PM_FrameRadius
  控件的圆角大小
  
  \value PM_ShadowRadius
  控件阴影效果的半径
  
  \value PM_ShadowHOffset
  阴影在水平方向的偏移
  
  \value PM_ShadowVOffset
  阴影在竖直方向的偏移
  
  \value PM_FrameMargins
  控件的margins区域，控件内容 = 控件大小 - FrameMargins
  
  \value PM_IconButtonIconSize
  控件大小
  
  \value PM_TopLevelWindowRadius
  窗口的圆角大小
  
  \value PM_SwitchButtonHandleWidth
  控件开关的宽度
  
  \value PM_SwithcButtonHandleHeight
  控件开关的高度
  
  \value PM_FloatingWidgetRadius
  (基类)的圆角半径:控件内容-Radius < 控件内容 < 控件显示大小
  
  \value PM_FloatingWidgetShadowRadius
  (基类)的阴影Radius区域:控件内容 < 控件内容+阴影margins < 控件内容+阴影margins+阴影Radius = 控件显示大小
  
  \value PM_FloatingWidgetShadowMargins
  (基类)阴影的宽度 = 控件显示大小 - 阴影Radius - 控件内容
  
  \value PM_FloatingWidgetShadowHOffset
  (基类)的阴影水平偏移
  
  \value PM_ContentsMargins
  (基类)内容的边距（一般只用于左右边距）
  
  \value PM_ContentsSpacing
  内容的间距（可用于列表项中每一项的距离）
  
  \value PM_ButtonMinimizedSize
  按钮控件的最小大小

  \omitvalue PM_CustomBase
  \omitvalue PM_FloatingWidgetShadowVOffset
*/

/*!
  \enum Dtk::Widget::DStyle::StandardPixmap
  \sa DStyle::StandardPixmap QStyle::StandardPixmap
  
  \value SP_DecreaseElement
  减少（-）
  \value SP_IncreaseElement
  增加（+）
  \value SP_MarkElement
  对勾
  \value SP_SelectElement
  选择（...）
  \value SP_EditElement
  编辑
  \value SP_ExpandElement
  展开
  \value SP_ReduceElement
  收缩
  \value SP_LockElement
  锁定
  \value SP_UnlockElement
  解锁
  \value SP_MediaVolumeLowElement
  音量
  \value SP_MediaVolumeHighElement
  满音量
  \value SP_MediaVolumeMutedElement
  静音
  \value SP_MediaVolumeLeftElement
  左声道
  \value SP_MediaVolumeRightElement
  右声道
  \value SP_ArrowEnter
  进入
  \value SP_ArrowLeave
  离开
  \value SP_ArrowNext
  下一页
  \value SP_ArrowPrev
  上一页
  \value SP_ShowPassword
  显示密码
  \value SP_HidePassword
  隐藏密码
  \value SP_CloseButton
  关闭按钮（X）
  \value SP_IndicatorMajuscule
  大写标识
  \value SP_IndicatorSearch
  搜索标识（放大镜）
  \value SP_IndicatorUnchecked
  搜索标识（对应对勾的选中状态）
  \value SP_IndicatorChecked
  搜索标识（对勾）
  
  \value SP_DeleteButton
  删除按钮
  
  \value SP_AddButton
  新增按钮

  \omitvalue SP_ForkElement
  \omitvalue SP_DecreaseElement
  \omitvalue SP_TitleMoreButton
  \omitvalue SP_CustomBase
  \omitvalue SP_TitleQuitFullButton
*/

/*!
  \brief DStyle 的默认构造函数.
 */
DStyle::DStyle()
{

}

/*!
  \brief 使用给定的 \a style 风格， \a pe 原始元素， \a opt 风格选项， \a p 画笔，以及 \a w 窗口，
  绘制风格中的原始元素对象.
  \overload

  类似于 QStyle::drawPrimitive 函数，这里绘制的是 Dtk 控件中的基本原始元素，与 Qt 的接口不同，这个
  接口是一个静态函数，通过传递一个 \a style 参数指定的风格实例，将其内部的绘制操作，都通过 style 中的
  接口实现。\a pe 参数除了包括 QStyle::PrimitiveElement 中的原始元素之外，也包含了 DStyle::PrimitiveElement
  枚举中的自定义类型。

  \a opt 参数指定绘制时的各种状态值，不同控件经过不同操作，其状态也会不一样；当 \a pe 为 PrimitiveElement::PE_IconButtonPanel 时
  表示需要绘制一个 IconButton 的面板，因此可以通过传递一个 DStyleOptionButton 的 \a opt 实例，并在该实例中指定绘制状态，如 Button 的
  features ， state 等等信息。 \a w 参数在绘制某些原始元素时，可以传空，如 PE_ItemBackground 。但大多数情况下，需要指定 \a w 为被绘制
  的控件实例。

  \sa QStyle::drawPrimitive()
 */
void DStyle::drawPrimitive(const QStyle *style, DStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w)
{
    DStyleHelper dstyle(style);

    switch (pe) {
    case PE_ItemBackground: {
        if (const DStyleOptionBackgroundGroup *vopt = qstyleoption_cast<const DStyleOptionBackgroundGroup *>(opt)) {
            QColor color = vopt->dpalette.color(DPalette::ItemBackground);

            if (dstyle.dstyle()) {
                color = dstyle.dstyle()->generatedBrush(vopt, color, vopt->dpalette.currentColorGroup(), DPalette::ItemBackground).color();
            }

            if (color.alpha() == 0) {
                return;
            }

            int frame_radius = dstyle.pixelMetric(PM_FrameRadius, opt, w);
            p->setBrush(color);
            p->setPen(Qt::NoPen);
            p->setRenderHint(QPainter::Antialiasing);

            if (vopt->directions != Qt::Horizontal && vopt->directions != Qt::Vertical) {
                p->drawRoundedRect(vopt->rect, frame_radius, frame_radius);
                break;
            }

            switch (vopt->position) {
            case DStyleOptionBackgroundGroup::OnlyOne:
                p->drawRoundedRect(vopt->rect, frame_radius, frame_radius);
                break;
            case DStyleOptionBackgroundGroup::Beginning: {
                if (vopt->directions == Qt::Horizontal) {
                    DDrawUtils::drawRoundedRect(p, vopt->rect, frame_radius, frame_radius,
                                                DDrawUtils::TopLeftCorner | DDrawUtils::BottomLeftCorner);
                } else {
                    DDrawUtils::drawRoundedRect(p, vopt->rect, frame_radius, frame_radius,
                                                DDrawUtils::TopLeftCorner | DDrawUtils::TopRightCorner);
                }

                break;
            }
            case DStyleOptionBackgroundGroup::End:
                if (vopt->directions == Qt::Horizontal) {
                    DDrawUtils::drawRoundedRect(p, vopt->rect, frame_radius, frame_radius,
                                                DDrawUtils::TopRightCorner | DDrawUtils::BottomRightCorner);
                } else {
                    DDrawUtils::drawRoundedRect(p, vopt->rect, frame_radius, frame_radius,
                                                DDrawUtils::BottomLeftCorner | DDrawUtils::BottomRightCorner);
                }

                break;
            case DStyleOptionBackgroundGroup::Middle:
                p->setRenderHint(QPainter::Antialiasing, false);
                p->drawRect(vopt->rect);
                break;
            default:
                break;
            }

            return;
        }
        break;
    }
    case PE_IconButtonPanel: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);

            if (btn->features & DStyleOptionButton::FloatingButton) {
                int frame_margins = dstyle.pixelMetric(DStyle::PM_FloatingButtonFrameMargin, opt, w);
                const QMargins margins(frame_margins, frame_margins, frame_margins, frame_margins);
                QRect shadow_rect = opt->rect + margins;
                const QRect content_rect = opt->rect - margins;
                QColor color = dstyle.getColor(opt, QPalette::Button);

                qreal frame_radius = content_rect.width() / 2.0;
                int shadow_radius = 5;
                int shadow_xoffset = 0;
                int shadow_yoffset = 2;

                shadow_rect.setTopLeft(shadow_rect.topLeft() + QPoint(shadow_xoffset, shadow_yoffset));
                shadow_rect.setWidth(qMin(shadow_rect.width(), shadow_rect.height()));
                shadow_rect.setHeight(qMin(shadow_rect.width(), shadow_rect.height()));
                shadow_rect.moveCenter(opt->rect.center() + QPoint(shadow_xoffset / 2.0, shadow_yoffset / 2.0));

                p->setRenderHint(QPainter::Antialiasing);
                DDrawUtils::drawShadow(p, shadow_rect, frame_radius, frame_radius,
                                       DStyle::adjustColor(color, 0, 0, +30), shadow_radius, QPoint(0, 0));

                p->setPen(Qt::NoPen);
                p->setBrush(color);
                p->drawEllipse(content_rect);
            } else if (btn->features & DStyleOptionButton::CircleButton) {
                QRect content_rect = opt->rect;
                QColor color = dstyle.getColor(opt, QPalette::Button);

                p->setPen(Qt::NoPen);
                p->setBrush(color);
                p->setRenderHint(QPainter::Antialiasing);
                p->drawEllipse(content_rect.adjusted(3, 3, -3, -3));
            } else {
                style->drawControl(CE_PushButtonBevel, opt, p, w);
            }
        }
        break;
    }
    case PE_IconButtonIcon: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);
            bool hasDciIcon = (btn->features & DStyleOptionButton::HasDciIcon);
            DStyleOptionIconV2 icon_option;

            icon_option.QStyleOption::operator =(*opt);
            if (hasDciIcon) {
                icon_option.dciIcon = btn->dciIcon;
                icon_option.iconType = DStyleOptionIconV2::SI_DciIcon;
                icon_option.dciMode = toDciIconMode(opt);
                icon_option.dciTheme = (DGuiApplicationHelper::toColorType(btn->palette)
                                        == DGuiApplicationHelper::LightType) ? DDciIcon::Light : DDciIcon::Dark;
            } else {
                icon_option.icon = btn->icon;
                icon_option.iconType = DStyleOptionIconV2::SI_QIcon;
            }

            icon_option.iconSize = btn->iconSize;
            icon_option.dpalette = btn->dpalette;
            icon_option.iconAlignment = Qt::AlignCenter;

            QPalette pa = opt->palette;

            if (btn->features & DStyleOptionButton::TitleBarButton) {
                if (!(opt->state & (State_MouseOver | State_Sunken)) || !(opt->state & State_Enabled)) {
                    pa.setBrush(QPalette::Window, Qt::transparent);
                } else {
                    QColor color;
                    DGuiApplicationHelper *guiAppHelp = DGuiApplicationHelper::instance();
                    if (guiAppHelp->themeType() == DGuiApplicationHelper::ColorType::DarkType)
                        color = QColor(255, 255, 255, 255 * 0.05);
                    else
                        color = QColor(0, 0, 0, 255 * 0.05);
                    pa.setBrush(QPalette::Window, color);
                }

                if (opt->state & State_Sunken) {
                    pa.setBrush(QPalette::WindowText, opt->palette.highlight());
                } else {
                    pa.setBrush(QPalette::WindowText, opt->palette.buttonText());
                }
            } else {
                pa.setBrush(QPalette::Window, dstyle.generatedBrush(opt, pa.button(), pa.currentColorGroup(), QPalette::Button));

                if (opt->state & QStyle::State_On) {
                    pa.setBrush(QPalette::WindowText, dstyle.generatedBrush(opt, pa.highlightedText(), pa.currentColorGroup(), QPalette::HighlightedText));
                } else {
                    pa.setBrush(QPalette::WindowText, dstyle.generatedBrush(opt, pa.buttonText(), pa.currentColorGroup(), QPalette::ButtonText));
                }
            }

            icon_option.palette = pa;
            icon_option.rect.setSize(btn->iconSize);
            icon_option.rect.moveCenter(QRect(opt->rect).center());

            dstyle.drawPrimitive(PE_Icon, &icon_option, p, w);
        }
        break;
    }
    case PE_Icon: {
        if (const DStyleOptionIconV2 *icon_opt = qstyleoption_cast<const DStyleOptionIconV2 *>(opt)) {
            switch (icon_opt->iconType) {
            case DStyleOptionIconV2::SI_QIcon: {
                auto *data = const_cast<DStyleOptionIconV2 *>(icon_opt)->icon.data_ptr();
                if (!data)
                    return;

                if (DStyledIconEngine *engine = dynamic_cast<DStyledIconEngine *>(data->engine)) {
                    engine->paint(p, opt->palette, opt->rect);
                } else {
                    auto icon_mode_state = toIconModeState(opt);
                    p->setBrush(opt->palette.window());
                    p->setPen(QPen(opt->palette.windowText(), 1));
                    icon_opt->icon.paint(p, opt->rect, icon_opt->iconAlignment, icon_mode_state.first, icon_mode_state.second);
                }
            }
                break;

            case DStyleOptionIconV2::SI_DciIcon: {
                DDciIcon icon = icon_opt->dciIcon;
                if (icon.isNull())
                    break;

                p->save();
                p->setBrush(Qt::NoBrush);
                const DDciIconPalette &iconPalette = makeIconPalette(opt->palette);
                int iconSizeForRect = qMax(icon_opt->iconSize.width(), icon_opt->iconSize.height());
                const QRect iconRect{icon_opt->rect.topLeft(), QSize(iconSizeForRect, iconSizeForRect)};
                icon_opt->dciIcon.paint(p, iconRect, p->device() ? p->device()->devicePixelRatioF()
                                                                 : qApp->devicePixelRatio(), icon_opt->dciTheme,
                                        icon_opt->dciMode, icon_opt->iconAlignment, iconPalette);
                p->restore();
            }
                break;
            }
        }
        break;
    }
    case PE_SwitchButtonGroove: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            QRect rectGroove = btn->rect;
            int frame_radius = dstyle.pixelMetric(DStyle::PM_FrameRadius, opt, w);

            p->setRenderHint(QPainter::Antialiasing);
            p->setPen(Qt::NoPen);
            p->setBrush(dstyle.getColor(opt, QPalette::Button));
            p->drawRoundedRect(rectGroove, frame_radius, frame_radius);
        }
        break;
    }
    case PE_SwitchButtonHandle: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            QRect rectHandle = btn->rect;
            int frame_radius = dstyle.pixelMetric(DStyle::PM_FrameRadius, opt, w);
            p->setRenderHint(QPainter::Antialiasing);
            p->setPen(Qt::NoPen);

            if (btn->state & State_On) {
                p->setBrush(dstyle.getColor(opt, QPalette::Highlight));
            } else {
                p->setBrush(dstyle.getColor(opt, QPalette::ButtonText));
            }

            p->drawRoundedRect(rectHandle, frame_radius, frame_radius);
        }
        break;
    }
    case PE_FloatingWidget: {
        if (const DStyleOptionFloatingWidget *btn = qstyleoption_cast<const DStyleOptionFloatingWidget *>(opt)) {
            DStyleHelper dstyle(style);
            int shadowRadius = dstyle.pixelMetric(PM_FloatingWidgetShadowRadius, opt, w);    //18
            int frameRadius = dstyle.pixelMetric(PM_FloatingWidgetRadius, opt, w);           //18
            int offsetX = dstyle.pixelMetric(PM_FloatingWidgetShadowHOffset, opt, w);        //0
            int offsetY = dstyle.pixelMetric(PM_FloatingWidgetShadowVOffset, opt, w);        //6
            int shadowMargins = dstyle.pixelMetric(PM_FloatingWidgetShadowMargins, opt, w) * 2;

            //绘画 矩形(图标icon+text+btn+icon)和外面一小圈frameRadius/2的 合在一起的矩形
            p->setRenderHint(QPainter::Antialiasing);
            QMargins shadow_margin(shadowMargins, shadowMargins, shadowMargins, shadowMargins);

            //先绘画阴影
            DDrawUtils::drawShadow(p, btn->rect + shadow_margin, frameRadius, frameRadius, QColor(0, 0, 0, 0.25 * 255), shadowRadius, QPoint(offsetX, offsetY));
            //再绘画上面的待显示区域

            if(ENABLE_ANIMATIONS && ENABLE_ANIMATION_MESSAGE) {
                p->setPen(Qt::NoPen);
            } else {
                p->setPen(QPen(btn->dpalette.frameShadowBorder(), 1));
            }

            p->setPen(Qt::NoPen);
            p->setBrush(btn->noBackground ? Qt::NoBrush : p->background());
            p->drawRoundedRect(opt->rect, frameRadius, frameRadius);

            if(ENABLE_ANIMATIONS && ENABLE_ANIMATION_MESSAGE) {
                p->setBrush(Qt::NoBrush);
                QPen pen;
                pen.setWidth(1);
                pen.setColor(DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::DarkType
                                 ? QColor(255, 255, 255, int(0.1 * 255))
                                 : QColor(0, 0, 0, int(0.12 * 255)));
                p->setPen(pen);
                p->drawRoundedRect(opt->rect, frameRadius, frameRadius);
            }
        }
        break;
    }
    default:
        break;
    }
}

/*!
  \brief 使用指定的 \a style 风格主题、 \a opt 风格选项、 \a p 画笔，绘制特定的 \a ce 控制元素.

  控制元素 \a ce 包括 Qt 的控件和 Dtk 自研控件两种，特定的控制元素，需要将 \a opt 转换成特定的风格选项。
  通常情况下，使用 qstyleoption_cast 进行转换。

  \a w 为需要绘制的控件实例，某些情况下可以传空.

  \sa QStyle::drawControl() DStyle::ControlElement
 */
void DStyle::drawControl(const QStyle *style, DStyle::ControlElement ce, const QStyleOption *opt, QPainter *p, const QWidget *w)
{
    switch (ce) {
    case CE_IconButton: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);

            if (!(btn->features & DStyleOptionButton::Flat)) {
                dstyle.drawPrimitive(PE_IconButtonPanel, opt, p, w);
            }

            if ((!btn->text.isEmpty() && (btn->features & DStyleOptionButton::FloatingButton))) {
                QStyleOptionButton subopt = *btn;
                subopt.rect = style->subElementRect(SE_PushButtonContents, opt, w);

                if (!(btn->features & QStyleOptionButton::Flat))
                    subopt.palette.setBrush(QPalette::ButtonText, subopt.palette.highlightedText());

                style->drawControl(CE_PushButtonLabel, &subopt, p, w);
            } else {
                DStyleOptionButton new_opt = *btn;
                new_opt.rect = dstyle.subElementRect(SE_IconButtonIcon, opt, w);
                dstyle.drawPrimitive(PE_IconButtonIcon, &new_opt, p, w);
            }

            // 有新信息时添加小红点
            if (w && w->property("_d_menu_item_redpoint").toBool()){
                DPalette pa = DGuiApplicationHelper::instance()->standardPalette(DGuiApplicationHelper::LightType);
                // 按图标大小50x50时，小红点大小6x6，距离右边和上面8个像素的比例绘制
                const int redPointRadius = 3;
                int redPointPadding = (8 * w->size().width() / 50) + redPointRadius;
                p->setPen(pa.color(DPalette::TextWarning));
                p->setBrush(pa.color(DPalette::TextWarning));
                p->setRenderHint(QPainter::Antialiasing);
                p->drawEllipse(QPointF(w->size().width()-redPointPadding, redPointPadding),
                               redPointRadius, redPointRadius);
            }

            if (btn->state & State_HasFocus) {
                if (btn->features & DStyleOptionButton::FloatingButton) {
                    int border_width = dstyle.pixelMetric(PM_FocusBorderWidth, opt, w);
                    QColor color = dstyle.getColor(opt, QPalette::Highlight);

                    p->setPen(QPen(color, border_width, Qt::SolidLine));
                    p->setBrush(Qt::NoBrush);
                    p->setRenderHint(QPainter::Antialiasing);
                    p->drawEllipse(QRectF(opt->rect).adjusted(1, 1, -1, -1));
                } else if (btn->features & DStyleOptionButton::CircleButton) {
                    DStyleOptionButton fropt = *btn;
                    QColor color = dstyle.getColor(opt, QPalette::Highlight);

                    p->save();
                    p->setRenderHint(QPainter::Antialiasing);
                    p->setPen(Qt::NoPen);
                    p->setBrush(color);
                    QPainterPath path;

                    //添加外边框路径
                    path.addEllipse(fropt.rect);

                    fropt.rect.adjust(2, 2, -2, -2);
                    //添加内边框路径
                    path.addEllipse(fropt.rect);

                    p->fillPath(path, color);
                    p->restore();
                } else if (btn->features & DStyleOptionButton::TitleBarButton) {
                    QStyleOption option = *opt;
                    option.rect.adjust(6, 6, -6, -6);
                    style->drawPrimitive(PE_FrameFocusRect, &option, p, w);
                } else {
                    style->drawPrimitive(PE_FrameFocusRect, opt, p, w);
                }
            }
        }
        break;
    }
    case CE_SwitchButton: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);
            DStyleOptionButton option = *btn;
            option.dpalette = btn->dpalette;
            option.rect = dstyle.subElementRect(SE_SwitchButtonGroove, opt, w);

            if(!ENABLE_ANIMATIONS || !ENABLE_ANIMATION_SWITCHBUTTON) {
                dstyle.drawPrimitive(PE_SwitchButtonGroove, &option, p, w);
                option.rect = dstyle.subElementRect(SE_SwitchButtonHandle, opt, w);
                dstyle.drawPrimitive(PE_SwitchButtonHandle, &option, p, w);
            }

            if (btn->state & State_HasFocus) {
                QStyleOptionFocusRect fropt;
                fropt.QStyleOption::operator=(*btn);
                fropt.rect = dstyle.subElementRect(SE_SwitchButtonGroove, btn, w);
                QColor color = dstyle.getColor(opt, QPalette::Highlight);
                int frame_radius = dstyle.pixelMetric(DStyle::PM_FrameRadius, opt, w);

                p->save();
                p->setRenderHint(QPainter::Antialiasing);
                p->setPen(Qt::NoPen);
                p->setBrush(color);
                QPainterPath path;

                fropt.rect.adjust(-4, -4, 4, 4);
                //添加外边框路径
                path.addRoundedRect(fropt.rect, frame_radius + 3, frame_radius + 3);

                fropt.rect.adjust(2, 2, -2, -2);
                //添加内边框路径
                path.addRoundedRect(fropt.rect, frame_radius + 1, frame_radius + 1);

                p->drawPath(path);
                p->restore();
            }
        }
        break;
    }
    case CE_FloatingWidget: {
        if (const DStyleOptionFloatingWidget *btn = qstyleoption_cast<const DStyleOptionFloatingWidget *>(opt)) {
            DStyleHelper dstyle(style);
            DStyleOptionFloatingWidget option = *btn;
            option.dpalette = btn->dpalette;
            option.rect = dstyle.subElementRect(SE_FloatingWidget, opt, w);
            dstyle.drawPrimitive(PE_FloatingWidget, &option, p, w);
        }
        break;
    }
    case CE_ButtonBoxButton: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);

            if (!ENABLE_ANIMATIONS || !ENABLE_ANIMATION_BUTTONBOX)
                dstyle.drawControl(CE_ButtonBoxButtonBevel, btn, p, w);

            DStyleOptionButton subopt = *btn;
            if (btn->features & DStyleOptionButton::HasDciIcon)
                subopt.dciIcon = btn->dciIcon;
            subopt.rect = dstyle.subElementRect(SE_ButtonBoxButtonContents, btn, w);
            dstyle.drawControl(CE_ButtonBoxButtonLabel, &subopt, p, w);
            if ((btn->state & State_HasFocus)) {
                if (const DStyleOptionButtonBoxButton *boxbtn = qstyleoption_cast<const DStyleOptionButtonBoxButton *>(opt)) {
                    DStyleOptionButtonBoxButton fropt;
                    fropt = *boxbtn;
                    fropt.rect = dstyle.subElementRect(SE_ButtonBoxButtonFocusRect, btn, w);

                    if (ENABLE_ANIMATIONS && ENABLE_ANIMATION_BUTTONBOX)
                        fropt.position = DStyleOptionButtonBoxButton::OnlyOne;

                    style->drawPrimitive(PE_FrameFocusRect, &fropt, p, w);
                }
            }
        }
        break;
    }
    case CE_ButtonBoxButtonBevel: {
        if (const DStyleOptionButtonBoxButton *btn = qstyleoption_cast<const DStyleOptionButtonBoxButton *>(opt)) {
            bool checked = btn->state & State_On;

            DStyleHelper dstyle(style);
            const QColor &background = dstyle.getColor(opt, checked ? QPalette::Highlight : QPalette::Button);
            p->setBrush(background);
            p->setPen(QPen(background, 1));
            p->setRenderHint(QPainter::Antialiasing); // 统一使用抗锯齿，避免中间按钮未使用抗锯齿高度不对齐
            int radius = dstyle.pixelMetric(PM_FrameRadius, opt, w);
            int margins = dstyle.pixelMetric(PM_FrameMargins, opt, w);
            DStyleOptionButtonBoxButton::ButtonPosition pos = btn->position;

            if (btn->state & State_HasFocus) {
                pos = DStyleOptionButtonBoxButton::OnlyOne;
            }

            switch (pos) {
            case DStyleOptionButtonBoxButton::Beginning: {
                QRect rect;

                if (btn->orientation == Qt::Horizontal) {
                    rect = opt->rect.adjusted(margins, margins, 0, -margins);
                    DDrawUtils::drawRoundedRect(p, rect, radius, radius, DDrawUtils::TopLeftCorner | DDrawUtils::BottomLeftCorner);
                } else {
                    rect = opt->rect.adjusted(margins, margins, -margins, 0);
                    DDrawUtils::drawRoundedRect(p, rect, radius, radius, DDrawUtils::TopLeftCorner | DDrawUtils::TopRightCorner);
                }

                break;
            }
            case DStyleOptionButtonBoxButton::Middle: {
                QRect rect;

                if (btn->orientation == Qt::Horizontal)
                    rect = opt->rect.adjusted(0, margins, 0, -margins);
                else
                    rect = opt->rect.adjusted(margins, 0, -margins, 0);

                p->drawRect(rect);
                break;
            }
            case DStyleOptionButtonBoxButton::End: {
                QRect rect;

                if (btn->orientation == Qt::Horizontal) {
                    rect = opt->rect.adjusted(0, margins, -margins, -margins);
                    DDrawUtils::drawRoundedRect(p, rect, radius, radius, DDrawUtils::TopRightCorner | DDrawUtils::BottomRightCorner);
                } else {
                    rect = opt->rect.adjusted(margins, 0, -margins, -margins);
                    DDrawUtils::drawRoundedRect(p, rect, radius, radius, DDrawUtils::BottomLeftCorner | DDrawUtils::BottomRightCorner);
                }

                break;
            }
            case DStyleOptionButtonBoxButton::OnlyOne: {
                QRect rect = opt->rect.adjusted(margins, margins, -margins, -margins);
                p->setRenderHint(QPainter::Antialiasing);
                p->drawRoundedRect(rect, radius, radius);
                break;
            }
            default:
                break;
            }
        }
        break;
    }
    case CE_ButtonBoxButtonLabel: {
        if (ENABLE_ANIMATIONS && ENABLE_ANIMATION_BUTTONBOX) {
            if (opt->state & StateFlag::State_MouseOver) {
                constexpr qreal hoverScale = 1.2; // hover 时放大1.2倍
                p->scale(hoverScale, hoverScale);
                p->setRenderHint(QPainter::SmoothPixmapTransform);
                p->translate((1 - hoverScale) * (opt->rect.x() + opt->rect.width() / 2) / hoverScale
                             , (1 - hoverScale) * (opt->rect.y() + opt->rect.height() / 2) / hoverScale);
            }
        }

        style->drawControl(CE_PushButtonLabel, opt, p, w);
        break;
    }
    case CE_TextButton: {
        if (const QStyleOptionButton *option = qstyleoption_cast<const QStyleOptionButton *>(opt)) {
            DStyleHelper dstyle(style);
            p->setPen(dstyle.getColor(option, QPalette::Highlight));
            p->drawText(opt->rect, Qt::AlignCenter, option->text);

            if (option->state & QStyle::State_HasFocus) {
                style->drawPrimitive(QStyle::PE_FrameFocusRect, option, p, w);
            }
        }
        break;
    }
    default:
        break;
    }
}

/*!
  \brief 返回特定的 \a style 风格下， \a m 像素标准的值.

  像素标准 \a m 的值可以时 QStyle::PixelMetric 也可以是 DStyle::PixelMetric 。
  某些\a m 值下， \a opt 参数和 \a widget 参数可以为空。

  \return 返回 \a m 对应的像素标准。

  \sa QStyle::pixelMetric()
 */
int DStyle::pixelMetric(const QStyle *style, DStyle::PixelMetric m, const QStyleOption *opt, const QWidget *widget)
{
    DStyleHelper dstyle(style);

    switch (m) {
    case PM_FocusBorderWidth: Q_FALLTHROUGH();
    case PM_FocusBorderSpacing:
        return 2;
    case PM_FrameRadius:
        if (widget) {
            const QVariant &radius_value = widget->property("_d_dtk_frameRadius");
            bool ok = false;
            int radius = radius_value.toInt(&ok);
            if (ok && radius >= 0) {
                return radius;
            }
        }
        return DSizeModeHelper::element(6, 8);
    case PM_TopLevelWindowRadius:
        return 18;
    case PM_ShadowRadius:
        return 0;
    case PM_ShadowHOffset:
        return 0;
    case PM_ShadowVOffset:
        return 1;
    case PM_FrameMargins: {
        int shadow_radius = dstyle.pixelMetric(PM_ShadowRadius, opt, widget);
        int shadow_xoffset = dstyle.pixelMetric(PM_ShadowHOffset, opt, widget);
        int shadow_yoffset = dstyle.pixelMetric(PM_ShadowVOffset, opt, widget);
        int margins = shadow_radius + qMax(shadow_xoffset, shadow_yoffset);

        return margins;
    }
    case PM_IconButtonIconSize: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            if (btn->features & DStyleOptionButton::FloatingButton) {
                return 20;
            }
        }
        return 12;
    }
    case PM_SwitchButtonHandleWidth:
        return DSizeModeHelper::element(24, 30);
    case PM_SwithcButtonHandleHeight:
        return DSizeModeHelper::element(20, 24);
    case PM_FloatingWidgetRadius: {
        if (const DStyleOptionFloatingWidget *wid = qstyleoption_cast<const DStyleOptionFloatingWidget *>(opt)) {
            if (wid->frameRadius != -1)
                return wid->frameRadius;
        }
        return dstyle.pixelMetric(PM_TopLevelWindowRadius, opt, widget);
    }
    case PM_FloatingWidgetShadowRadius:
        return DSizeModeHelper::element(4, 8);
    case PM_FloatingWidgetShadowHOffset:
        return 0;
    case PM_FloatingWidgetShadowVOffset:
        return DSizeModeHelper::element(2, 4);
    case PM_FloatingWidgetShadowMargins: {
        int shadow_radius = dstyle.pixelMetric(PM_FloatingWidgetShadowRadius, opt, widget);
        int shadow_hoffset = dstyle.pixelMetric(PM_FloatingWidgetShadowHOffset, opt, widget);
        int shadow_voffset = dstyle.pixelMetric(PM_FloatingWidgetShadowVOffset, opt, widget);

        return (shadow_radius +  qMax(shadow_hoffset, shadow_voffset)) / 2;
    }
    case PM_ContentsMargins:
    case PM_ContentsSpacing:
        return 10;
    case PM_ButtonMinimizedSize:
        return DSizeModeHelper::element(24, 36);
    case PM_ToolTipLabelWidth:
        return 300;
    default:
        break;
    }

    return -1;
}

/*!
  \brief 通过给定的 \a style 风格,  \a opt 风格选项和 \a widget 控件，返回
  指定的 \a r 子元素区域矩形。

  \return 返回子元素 \a r 对应的屏幕坐标区域矩形。

  \sa QStyle::subElementRect()
 */
QRect DStyle::subElementRect(const QStyle *style, DStyle::SubElement r, const QStyleOption *opt, const QWidget *widget)
{
    Q_UNUSED(widget)

    switch (r) {
    case SE_IconButtonIcon:
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            if (btn->features & DStyleOptionButton::FloatingButton) {
                QRect icon_rect(opt->rect);
                icon_rect.setWidth(opt->rect.width() * 0.75);
                icon_rect.setHeight(opt->rect.height() * 0.75);
                icon_rect.moveCenter(opt->rect.center());

                return icon_rect;
            }
        }

        return opt->rect;
    case SE_SwitchButtonGroove: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            return btn->rect;
        }
        break;
    }
    case SE_SwitchButtonHandle: {
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            int handleWidth = btn->rect.width() / 2.0;
            int handleHeight = btn->rect.height();
            //这里的borderWidth为2,间隙宽度为2, 所以为4
            QRect rectHandle(4, 4, handleWidth, handleHeight);

            if (btn->state & QStyle::State_On) {
                rectHandle.moveRight(opt->rect.right());
            } else {
                rectHandle.moveLeft(opt->rect.left());
            }

            return rectHandle;
        }
        break;
    }
    case SE_FloatingWidget: {
        if (const DStyleOptionFloatingWidget *btn = qstyleoption_cast<const DStyleOptionFloatingWidget *>(opt)) {
            DStyleHelper dstyle(style);
            QRect rect = btn->rect;
            int shadowMarge = dstyle.pixelMetric(PM_FloatingWidgetShadowMargins, opt, widget);
            QRect rectBtn = rect.adjusted(shadowMarge, shadowMarge, -shadowMarge, -shadowMarge);

            return rectBtn;
        }
        break;
    }
    case SE_ButtonBoxButtonContents:
        return style->subElementRect(SE_PushButtonContents, opt, widget);
    case SE_ButtonBoxButtonFocusRect:
        return style->subElementRect(SE_PushButtonFocusRect, opt, widget);
    default:
        break;
    }

    return QRect();
}

/*!
  \brief 返回子内容区域的 \a ct 的大小.

  参数 \a style 为指定的风格类型， \a ct 为子元素内容类型，
  \a opt 为风格选项，不同的控件风格选项不同。 \a contentsSize 提供的原始内容大小 \a widget 参数
  为控件实例。

  \sa QStyle::sizeFromContents()
 */
QSize DStyle::sizeFromContents(const QStyle *style, DStyle::ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget)
{
    Q_UNUSED(widget)

    switch (ct) {
    case CT_IconButton:
        if (const DStyleOptionButton *btn = qstyleoption_cast<const DStyleOptionButton *>(opt)) {
            if (btn->features & DStyleOptionButton::FloatingButton) {
                DStyleHelper dstyle(style);
                int frame_margin = dstyle.pixelMetric(DStyle::PM_FloatingButtonFrameMargin, opt, widget);
                QSize marginSize(2 * frame_margin, 2 * frame_margin);
                return DSizeModeHelper::element(QSize(36, 36) + marginSize, QSize(48, 48) + marginSize);
            }

            if (btn->features & DStyleOptionButton::Flat) {
                return contentsSize.expandedTo(btn->iconSize);
            }

            QSize size = style->sizeFromContents(CT_PushButton, opt, btn->iconSize, widget);

            if (btn->features & DStyleOptionButton::CircleButton) {
                //这里需要预留绘制focusRect的区域,borderWidth为2,间隙宽度为1,所以 (2+1)*2=6
                size += QSize(6, 6);
            }

            return size;
        }
        Q_FALLTHROUGH();
    case CT_SwitchButton: {
        DStyleHelper dstyle(style);
        int w = dstyle.pixelMetric(PM_SwitchButtonHandleWidth, opt, widget);
        int h = dstyle.pixelMetric(PM_SwithcButtonHandleHeight, opt, widget);
        QSize size(qMax(contentsSize.width(), w * 5 / 3), qMax(contentsSize.height(), h));
        //按照设计要求: 在保持现有控件的尺寸下,这里需要预留绘制focusRect的区域,borderWidth为2,间隙宽度为2,所以 (2+2)*2=8
        size += QSize(8, 8);

        return size;
    }
    case CT_FloatingWidget: {
        DStyleHelper dstyle(style);
        int margins = dstyle.pixelMetric(PM_FloatingWidgetShadowMargins, opt, widget);
        int window_radius = dstyle.pixelMetric(PM_FloatingWidgetRadius, opt, widget);
        QSize size(2 * margins + qMax(2 * window_radius, contentsSize.width() + window_radius),
                   2 * margins + qMax(2 * window_radius,  contentsSize.height()));
        return size;
    }
    case CT_ButtonBoxButton: {
        QSize size = style->sizeFromContents(CT_PushButton, opt, contentsSize, widget);

        if (const DStyleOptionButtonBoxButton *btn = qstyleoption_cast<const DStyleOptionButtonBoxButton *>(opt)) {
            if (btn->text.isEmpty()) {
                // 只有图标时高度至少和宽度一致
                size.setHeight(qMax(size.width(), size.height()));
            }

            int frame_margin = DStyleHelper(style).pixelMetric(PM_FrameMargins, opt, widget);
            switch (btn->position) {
            case DStyleOptionButtonBoxButton::Beginning:
            case DStyleOptionButtonBoxButton::End:
                size.rwidth() -= frame_margin;
                break;
            case DStyleOptionButtonBoxButton::Middle:
                size.rwidth() -= 2 * frame_margin;
                break;
            default:
                break;
            }
        }

        return size;
    }
    default:
        break;
    }

    return contentsSize;
}

/*!
  \brief DStyle::standardIcon

  \a style 风格实例 \a st 基本 Pixmap 对象枚举 \a opt 风格选项 \a widget 控件实例

  \sa QStyle::standardIcon()
 */
QIcon DStyle::standardIcon(const QStyle *style, DStyle::StandardPixmap st, const QStyleOption *opt, const QWidget *widget)
{
    Q_UNUSED(opt)
    Q_UNUSED(style)
    Q_UNUSED(widget)

#define CASE_ICON(Value) \
case static_cast<uint32_t>(SP_##Value): { \
        DStyledIconEngine *icon_engine = new DStyledIconEngine(DDrawUtils::draw##Value, QStringLiteral(#Value)); \
        return QIcon(icon_engine);}

    switch (st) {
        CASE_ICON(SelectElement)

    case SP_IndicatorUnchecked:
        return DIconTheme::findQIcon("unselected_indicator");
    case SP_IndicatorChecked: {
        bool checked = opt && (opt->state & QStyle::State_Selected);
        const QIcon &sci = DIconTheme::findQIcon("selected_checked_indicator");
        bool useNewIcon = checked && !sci.isNull();
        const QIcon &icon = useNewIcon ? sci : DIconTheme::findQIcon("selected_indicator");
        DStyledIconEngine *icon_engine = new DStyledIconEngine(std::bind(DStyledIconEngine::drawIcon, icon, std::placeholders::_1, std::placeholders::_2), QStringLiteral("IndicatorChecked"));
        icon_engine->setFrontRole(widget, useNewIcon ? DPalette::HighlightedText : DPalette::Highlight );
        return QIcon(icon_engine);
    }
    case SP_DeleteButton:
        return DIconTheme::findQIcon("list_delete");
    case SP_AddButton:
        return DIconTheme::findQIcon("list_add");
    case SP_ForkElement:
        return DIconTheme::findQIcon("fork_indicator");
    case SP_CloseButton:
        return DIconTheme::findQIcon("window-close_round");
    case SP_DecreaseElement:
        return DIconTheme::findQIcon("button_reduce");
    case SP_IncreaseElement:
        return DIconTheme::findQIcon("button_add");
    case SP_MarkElement:
        return DIconTheme::findQIcon("mark_indicator");
    case SP_UnlockElement:
        return DIconTheme::findQIcon("unlock_indicator");
    case SP_LockElement:
        return DIconTheme::findQIcon("lock_indicator");
    case SP_ExpandElement:
        return DIconTheme::findQIcon("go-up");
    case SP_ReduceElement:
        return DIconTheme::findQIcon("go-down");
    case SP_ArrowEnter:
        return style->standardIcon(SP_ArrowForward);
    case SP_ArrowNext:
        return DIconTheme::findQIcon("next_indicator");
    case SP_ArrowLeave:
        return style->standardIcon(SP_ArrowBack);
    case SP_ArrowPrev:
        return DIconTheme::findQIcon("prev_indicator");
    case SP_EditElement:
        return DIconTheme::findQIcon("edit");
    case SP_MediaVolumeLowElement:
        return DIconTheme::findQIcon("audio-volume-low");
    case SP_MediaVolumeHighElement:
        return DIconTheme::findQIcon("audio-volume-medium");
    case SP_MediaVolumeMutedElement:
        return DIconTheme::findQIcon("audio-volume-muted");
    case SP_MediaVolumeLeftElement:
        return DIconTheme::findQIcon("audio-volume-left");
    case SP_MediaVolumeRightElement:
        return DIconTheme::findQIcon("audio-volume-right");
    case SP_IndicatorMajuscule:
        return DIconTheme::findQIcon("caps_lock");
    case SP_ShowPassword:
        return DIconTheme::findQIcon("password_show");
    case SP_HidePassword:
        return DIconTheme::findQIcon("password_hide");
    case SP_IndicatorSearch:
        return DIconTheme::findQIcon("search_indicator");
    case SP_TitleMoreButton:
        return DIconTheme::findQIcon("titlebar_more");
    case SP_Title_SS_LeftButton:
        return DIconTheme::findQIcon("splitscreen_left");
    case SP_Title_SS_RightButton:
        return DIconTheme::findQIcon("splitscreen_right");
    case SP_Title_SS_ShowNormalButton:
        return DIconTheme::findQIcon("splitscreen_shownormal");
    case SP_Title_SS_ShowMaximizeButton:
        return DIconTheme::findQIcon("splitscreen_showmaximize");
    default:
        break;
    }

    return QIcon();
}

/*!
  \brief DStyle::styleHint

  \a sh 风格策略枚举 \a opt 风格选项 \a w 控件实例 \a shret 传出参数.

  \sa QStyle::styleHint()
 */
int DStyle::styleHint(QStyle::StyleHint sh, const QStyleOption *opt, const QWidget *w, QStyleHintReturn *shret) const
{
    switch (sh) {
    case SH_UnderlineShortcut: {
        return shortcutUnderlineVisible();
    }
    case SH_Menu_KeyboardSearch: {
        return !isMenuKeyboardSearchDisabled();
    }
    case SH_ScrollBar_MiddleClickAbsolutePosition:
    case SH_FontDialog_SelectAssociatedText:
    case SH_Menu_Scrollable:
    case SH_Menu_SloppySubMenus:
    case SH_ComboBox_ListMouseTracking:
    case SH_Menu_MouseTracking:
    case SH_MenuBar_MouseTracking:
    case SH_Menu_FillScreenWithScroll:
    case SH_ItemView_ChangeHighlightOnFocus:
    case SH_TabBar_PreferNoArrows:
    case SH_ComboBox_Popup:
    case SH_Slider_StopMouseOverSlider:
    case SH_SpinBox_AnimateButton:
    case SH_SpinControls_DisableOnBounds:
    case SH_Menu_FadeOutOnHide:
    case SH_ItemView_ShowDecorationSelected:
    case SH_ScrollBar_Transient:
    case SH_TitleBar_ShowToolTipsOnButtons:
        return true;
    case SH_SpinBox_ButtonsInsideFrame:
    case SH_ScrollBar_LeftClickAbsolutePosition:
    case SH_Slider_SnapToValue:
    case SH_Menu_AllowActiveAndDisabled:
    case SH_BlinkCursorWhenTextSelected:
    case SH_ItemView_PaintAlternatingRowColorsForEmptyArea:
    case SH_ComboBox_AllowWheelScrolling:
        return false;
    case SH_Header_ArrowAlignment:
        return Qt::AlignVCenter | Qt::AlignRight;
    case SH_Menu_SubMenuPopupDelay:
        return 100;
    case SH_ToolTipLabel_Opacity:
        return 255;
    case SH_RequestSoftwareInputPanel:
        return RSIP_OnMouseClick;
    case SH_ItemView_ScrollMode:
        return QAbstractItemView::ScrollPerPixel;
    case SH_Widget_Animation_Duration:
        return 300;
    case SH_Button_FocusPolicy:
        return Qt::TabFocus;
    case SH_ScrollView_FrameOnlyAroundContents:
        return false;
    case SH_LineEdit_PasswordCharacter:
        return 0x26AB;
    default:
        break;
    }

    return QCommonStyle::styleHint(sh, opt, w, shret);
}

/*!
  \brief 获取标准调色板
  \return 调色板
 */
QPalette DStyle::standardPalette() const
{
    QPalette pa = DGuiApplicationHelper::instance()->standardPalette(DGuiApplicationHelper::LightType);
    // 将无效的颜色fallback到QCommonStyle提供的palette，且在resolve操作中将detach pa对象
    // 防止在QApplication initSystemPalette的setSystemPalette中获取到一个和 QGuiApplicationPrivate::platformTheme()->palette()
    // 一样的QPalette对象，这样将导致QApplicationPrivate::setPalette_helper中将 app_pal 和新的QPalette对比时认为他们没有变化
    return pa.resolve(QCommonStyle::standardPalette());
}

QPixmap DStyle::generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *opt) const
{
    Q_UNUSED(opt)
    switch (iconMode) {
        case QIcon::Active: {
            QImage image = pixmap.toImage();
            QPainter pa(&image);

            if (!pa.isActive())
                return QCommonStyle::generatedIconPixmap(iconMode, pixmap, opt);

            pa.setCompositionMode(QPainter::CompositionMode_SourceAtop);

            if (DGuiApplicationHelper::toColorType(opt->palette) == DGuiApplicationHelper::DarkType) {
                pa.fillRect(image.rect(), QColor(255, 255, 255, 0.1 * 255));
            } else {
                pa.fillRect(image.rect(), QColor(0, 0, 0, 0.1 * 255));
            }

            return QPixmap::fromImage(image);
        }
        case QIcon::Disabled: {
            QImage image = pixmap.toImage();

            // TODO: fix wayland parent window alpha changed
            // 暂时再此处规避一下, 后续如果是qt的bug移除此段代码
            if (!image.hasAlphaChannel()) {
                image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            }
            QPainter pa(&image);

            if (!pa.isActive())
                return QCommonStyle::generatedIconPixmap(iconMode, pixmap, opt);

            pa.setCompositionMode(QPainter::CompositionMode_DestinationIn);
            pa.fillRect(image.rect(), QColor(0, 0, 0, int(255 * 0.4)));

            return QPixmap::fromImage(image);
        }
        case QIcon::Selected: {
            return pixmap;
        }
        default:
            return QCommonStyle::generatedIconPixmap(iconMode, pixmap, opt);
    }
}

DStyle::StyleState DStyle::getState(const QStyleOption *option)
{
    DStyle::StyleState state = DStyle::SS_NormalState;

    if (!option->state.testFlag(DStyle::State_Enabled)) {
        return DStyle::SS_NormalState;
    }
    // 设置setAttribute(Qt::WA_Hover, false)后
    // 按钮按下时依旧会存在State_MouseOver状态
    // 错误的获取了SS_HoverState后的调色板
    QWidget *widget = qobject_cast<QWidget *>(option->styleObject);
    if (option->state & State_MouseOver && widget
            && !widget->testAttribute(Qt::WA_Hover)) {
        return DStyle::SS_NormalState;
    }

    if (option->state.testFlag(DStyle::State_Sunken)) {
        state = DStyle::SS_PressState;
    } else if (option->state.testFlag(DStyle::State_MouseOver)) {
        state = DStyle::SS_HoverState;
    }

    return state;
}

static DStyle::StateFlags getFlags(const QStyleOption *option)
{
    DStyle::StateFlags flags{0};

    if (option->state.testFlag(DStyle::State_On)) {
        flags |= DStyle::SS_CheckedFlag;
    }

    if (option->state.testFlag(DStyle::State_Selected)) {
        flags |= DStyle::SS_SelectedFlag;
    }

    if (option->state.testFlag(DStyle::State_HasFocus)) {
        flags |= DStyle::SS_FocusFlag;
    }

    return flags;
}

/*!
  \brief DStyle::drawPrimitive
  \sa QStyle::drawPrimitive()
 */
void DStyle::drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    switch (pe) {
    case PE_IndicatorArrowUp:
        p->setPen(QPen(opt->palette.windowText(), 1));
        return DDrawUtils::drawArrowUp(p, opt->rect);
    case PE_IndicatorArrowDown:
        p->setPen(QPen(opt->palette.windowText(), 1));
        return DDrawUtils::drawArrowDown(p, opt->rect);
    case PE_IndicatorArrowRight:
        p->setPen(QPen(opt->palette.windowText(), 1));
        return DDrawUtils::drawArrowRight(p, opt->rect);
    case PE_IndicatorArrowLeft:
        p->setPen(QPen(opt->palette.windowText(), 1));
        return DDrawUtils::drawArrowLeft(p, opt->rect);
    case PE_IndicatorHeaderArrow:
        if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(opt)) {
            p->setPen(QPen(opt->palette.windowText(), 1));
            // sort up draw down icon, since both windows, mac and even wikipedia did this...
            if (header->sortIndicator & QStyleOptionHeader::SortUp) {
                return proxy()->drawPrimitive(PE_IndicatorArrowDown, opt, p, w);
            } else if (header->sortIndicator & QStyleOptionHeader::SortDown) {
                return proxy()->drawPrimitive(PE_IndicatorArrowUp, opt, p, w);
            }
        }
        return;
    default:
        break;
    }

    if (Q_UNLIKELY(pe < QStyle::PE_CustomBase)) {
        return QCommonStyle::drawPrimitive(pe, opt, p, w);
    }

    drawPrimitive(this, static_cast<PrimitiveElement>(pe), opt, p, w);
}

/*!
  \brief DStyle::drawControl
  \sa QStyle::drawControl()
 */
void DStyle::drawControl(QStyle::ControlElement ce, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    if (Q_UNLIKELY(ce < QStyle::CE_CustomBase)) {
        return QCommonStyle::drawControl(ce, opt, p, w);
    }

    drawControl(this, static_cast<ControlElement>(ce), opt, p, w);
}

/*!
  \brief DStyle::pixelMetric
  \sa QStyle::pixelMetric()
 */
int DStyle::pixelMetric(QStyle::PixelMetric m, const QStyleOption *opt, const QWidget *widget) const
{
    switch (m) {
    case PM_ButtonDefaultIndicator:
    case PM_ButtonShiftHorizontal:
    case PM_ButtonShiftVertical:
    case PM_FocusFrameVMargin:
    case PM_FocusFrameHMargin:
    case PM_MenuBarPanelWidth:
    case PM_MenuDesktopFrameWidth:
        return 0;
    case PM_ButtonMargin:
        return 8;
    case PM_LayoutLeftMargin:
    case PM_LayoutRightMargin:
    case PM_LayoutTopMargin:
    case PM_LayoutBottomMargin:
        return DSizeModeHelper::element(pixelMetric(PM_FrameRadius, opt, widget), pixelMetric(PM_FrameRadius, opt, widget));
    case PM_DefaultFrameWidth:
        return 1;
    case PM_LayoutHorizontalSpacing:
    case PM_LayoutVerticalSpacing:
        return DSizeModeHelper::element(2, 5);
    case PM_MenuBarItemSpacing:
        return 6;
    case PM_IndicatorWidth:
    case PM_IndicatorHeight:
        return 16; // 解决 select_indicator 缩放 1.25 后截断的问题，大小改成16x16
    case PM_ExclusiveIndicatorHeight:
    case PM_ExclusiveIndicatorWidth:
        return 16; // fix-56991 解决radiobutton 内圈偏了， 外圈16, 内圈14
    case PM_ScrollBarSliderMin:
        return 36;
    case PM_SliderLength:
    case PM_ScrollBarExtent:
        return DSizeModeHelper::element(16, 20);
    case PM_SliderControlThickness:
        return DSizeModeHelper::element(20, 24);
    case PM_MenuBarHMargin:
        return 10;
    case PM_MenuBarVMargin:
        return 6;
    case PM_SliderTickmarkOffset:
        return 14;
    case PM_MenuHMargin:
        return 0;
    case PM_MenuVMargin:
        return pixelMetric(PM_TopLevelWindowRadius, opt, widget);
    case PM_SmallIconSize:
        return 14;
    case PM_ButtonIconSize:
        return 16;
    case PM_ListViewIconSize:
    case PM_LargeIconSize:
        return 24;
    case PM_IconViewIconSize:
        return 32;
    case PM_ScrollView_ScrollBarOverlap:
        return true;
    case PM_ToolBarIconSize:
        return 16;
    case PM_MenuButtonIndicator:
        return DSizeModeHelper::element(8, QCommonStyle::pixelMetric(m, opt, widget));
    case PM_FloatingButtonFrameMargin:
        return 3;
    default:
        break;
    }

#if (QT_VERSION >= QT_VERSION_CHECK(6, 2, 0))
    #define LineEditIconSize PM_LineEditIconSize
#else
    #define LineEditIconSize 96
#endif

    if (Q_UNLIKELY(LineEditIconSize == m)) {
        return DSizeModeHelper::element(20, 20);
    }

    if (Q_UNLIKELY(m < QStyle::PM_CustomBase)) {
        return QCommonStyle::pixelMetric(m, opt, widget);
    }

    return pixelMetric(this, static_cast<PixelMetric>(m), opt, widget);
}

/*!
  \brief DStyle::subElementRect
  \sa QStyle::subElementRect()
 */
QRect DStyle::subElementRect(QStyle::SubElement r, const QStyleOption *opt, const QWidget *widget) const
{
    if (Q_UNLIKELY(r < QStyle::SE_CustomBase)) {
        return QCommonStyle::subElementRect(r, opt, widget);
    }

    return subElementRect(this, static_cast<DStyle::SubElement>(r), opt, widget);
}

/*!
  \brief DStyle::sizeFromContents
  \sa QStyle::sizeFromContents()
 */
QSize DStyle::sizeFromContents(QStyle::ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget) const
{
    if (Q_UNLIKELY(ct < QStyle::CT_CustomBase)) {
        return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget);
    }

    return sizeFromContents(this, static_cast<DStyle::ContentsType>(ct), opt, contentsSize, widget);
}

/*!
  \brief DStyle::standardIcon
  \sa QStyle::standardIcon()
 */
QIcon DStyle::standardIcon(QStyle::StandardPixmap st, const QStyleOption *opt, const QWidget *widget) const
{
    switch (static_cast<uint32_t>(st)) {
        CASE_ICON(TitleBarMenuButton)
        CASE_ICON(TitleBarMinButton)
        CASE_ICON(TitleBarMaxButton)
        CASE_ICON(TitleBarCloseButton)
        CASE_ICON(TitleBarNormalButton)
        CASE_ICON(TitleQuitFullButton)
    case SP_LineEditClearButton:
        return DIconTheme::findQIcon("button_edit-clear");
    case SP_CommandLink:
            return DIconTheme::findQIcon(QLatin1String("go-next"),
                                    DIconTheme::findQIcon(QLatin1String("forward")));
    default:
        break;
    }

    if (st < QStyle::SP_CustomBase) {
        return QCommonStyle::standardIcon(st, opt, widget);
    }

    return standardIcon(this, static_cast<DStyle::StandardPixmap>(st), opt, widget);
}

/*!
  \fn QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const

  \brief 获取一个加工后的画刷

  \a base 画刷. \a option 原始风格选项. \a cg 调色版中的色组. \a role 调色板中的颜色角色值

  \sa QStyleOption
  \sa QPalette::ColorGroup QPalette::ColorRole
  \return 加工后的画刷
*/
QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    return generatedBrush(getState(option), option, base, cg, role);
}

/*!
  \fn QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const

  \brief 获取一个加工后的画刷

  \a base 画刷. \a state DStyle中的风格状态. \a option 风格选项. \a cg 调色板中的色组. \a role 调色板中颜色角色值.

  \sa DStyle::StyleState
  \sa QStyleOption
  \sa QPalette::ColorGroup QPalette::ColorRole
  \return 加工后的画刷
 */
QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role) const
{
    if (auto proxy = qobject_cast<const DStyle *>(this->proxy())) {
        return proxy->generatedBrush(getFlags(option) | state, base, cg, role, option);
    }

    return generatedBrush(getFlags(option) | state, base, cg, role, option);
}

/*!
  \fn QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role, const QStyleOption *option) const

  \brief 获取一个加工后的画刷

  \a base 画刷 \a flags DStyle中的风格状态. \a cg 调色板中的色组. \a role 调色板中颜色角色值. \a option 风格选项.

  \sa DStyle::StateFlags
  \sa QPalette::ColorRole QPalette::ColorGroup QStyleOption
  \return 加工后的画刷
 */
QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, QPalette::ColorRole role, const QStyleOption *option) const
{
    Q_UNUSED(cg)

    QColor colorNew = base.color();

    if (!colorNew.isValid())
        return base;

    if ((flags & StyleState_Mask)  == SS_HoverState) {
        switch (role) {
        case QPalette::Button:
        case QPalette::Light:
        case QPalette::Dark: {
            DGuiApplicationHelper::ColorType type = DGuiApplicationHelper::toColorType(option->palette);
            colorNew = adjustColor(colorNew, 0, 0, type == DGuiApplicationHelper::DarkType ? 10 : -10, 0, 0, 0, 0);
            break;
        }
        case QPalette::Highlight:
            colorNew = adjustColor(colorNew, 0, 0, +20);
            break;
        case QPalette::ButtonText: {
            DGuiApplicationHelper::ColorType type = DGuiApplicationHelper::toColorType(option->palette);
            colorNew = adjustColor(colorNew, 0, 0, type == DGuiApplicationHelper::DarkType ? 20 : -50);
            break;
        }
        case QPalette::HighlightedText:
            colorNew = adjustColor(colorNew, 0, 0, 20);
            break;
        default:
            break;
        }

        return colorNew;
    } else if ((flags & StyleState_Mask) == SS_PressState) {
        QColor hightColor = option->palette.highlight().color();
        hightColor.setAlphaF(0.1);

        switch (role) {
        case QPalette::Button:
        case QPalette::Light: {
            colorNew = adjustColor(colorNew, 0, 0, -20, 0, 0, +20, 0);
            colorNew = blendColor(colorNew, hightColor);
            break;
        }
        case QPalette::Dark: {
            colorNew = adjustColor(colorNew, 0, 0, -15, 0, 0, +20, 0);
            colorNew = blendColor(colorNew, hightColor);
            break;
        }
        case QPalette::Highlight:
            colorNew = adjustColor(colorNew, 0, 0, -10);
            break;
        case QPalette::ButtonText:
            return option->palette.highlight();
        case QPalette::HighlightedText:
            colorNew = adjustColor(colorNew, 0, 0, 0, 0, 0, 0, -40);
            break;
        default:
            break;
        }

        return colorNew;
    }

    return base;
}

/*!
  \fn QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type) const
  \brief 获取一个加工后的画刷.
  \overload

  \a base 画刷 \a type DStyle调色板中的颜色角色值. \a cg Dtk调色板中的色组. \a option 风格选项.

  \sa Dtk::Gui::DPalette::ColorType
  \sa QPalette::ColorGroup  QStyleOption
  \return 加工后的画刷
 */
QBrush DStyle::generatedBrush(const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type) const
{
    return generatedBrush(getState(option), option, base, cg, type);
}

/*!
  \fn QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type) const

  \brief 获取一个加工后的画刷.
  \overload

  \a state DStyle::StyleState
  \a option QStyleOption
  \a base 画刷
  \a cg QPalette::ColorGroup
  \a type DPalette::ColorType
  \return 加工后的画刷
 */
QBrush DStyle::generatedBrush(DStyle::StyleState state, const QStyleOption *option, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type) const
{
    if (auto proxy = qobject_cast<const DStyle *>(this->proxy())) {
        return proxy->generatedBrush(getFlags(option) | state, base, cg, type, option);
    }

    return generatedBrush(getFlags(option) | state, base, cg, type, option);
}

/*!
  \fn QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type, const QStyleOption *option) const

  \brief 获取一个加工后的画刷
  \overload

  \a flags DStyle::StateFlags
  \a base 画刷
  \a cg QPalette::ColorGroup
  \a type DPalette::ColorType
  \a option QStyleOption()
  \return 加工后的画刷
 */
QBrush DStyle::generatedBrush(StateFlags flags, const QBrush &base, QPalette::ColorGroup cg, DPalette::ColorType type, const QStyleOption *option) const
{
    Q_UNUSED(cg)
    Q_UNUSED(option)

    QColor colorNew = base.color();

    if (!colorNew.isValid())
        return base;

    if ((flags & StyleState_Mask)  == SS_HoverState) {
        switch (type) {
        case DPalette::LightLively:
            colorNew = adjustColor(colorNew, 0, 0, +30, 0, 0, 0, 0);
            break;
        case DPalette::DarkLively:
            colorNew = adjustColor(colorNew, 0, 0, +10, 0, 0, 0, 0);
            break;
        case DPalette::ItemBackground: {
            DGuiApplicationHelper::ColorType ct = DGuiApplicationHelper::toColorType(colorNew);
            colorNew = ct == DGuiApplicationHelper::LightType ? adjustColor(colorNew, 0, 0, -10, 0, 0, 0, +10)
                       : adjustColor(colorNew, 0, 0, +10, 0, 0, 0, +10);
            break;
        }
        case DPalette::TextWarning: {
            colorNew = adjustColor(colorNew, 0, 0, -10);
            break;
        }
        default:
            break;
        }

        return colorNew;
    } else if ((flags & StyleState_Mask) == SS_PressState) {
        switch (type) {
        case DPalette::LightLively:
            colorNew = adjustColor(colorNew, 0, 0, -30, 0, 0, 0, 0);
            break;
        case DPalette::DarkLively:
            colorNew = adjustColor(colorNew, 0, 0, -20, 0, 0, 0, 0);
            break;
        case DPalette::TextWarning: {
            colorNew = adjustColor(colorNew, 0, 0, -30);
            break;
        }
        default:
            break;
        }

        return colorNew;
    } else if ((flags & StyleState_Mask) == SS_NormalState) {
        switch (type) {
        case DPalette::LightLively:
            colorNew = adjustColor(colorNew, 0, 0, +40, 0, 0, 0, 0);
            break;
        case DPalette::DarkLively:
            colorNew = adjustColor(colorNew, 0, 0, +20, 0, 0, 0, 0);
            break;
        default:
            break;
        }

        return colorNew;
    }

    return base;
}

#if QT_CONFIG(itemviews)
/*!
  \brief 视图项文本布局
  \a textLayout 文本布局
  \a lineWidth 宽度
  \return 文本内容所占大小
 */
QSizeF DStyle::viewItemTextLayout(QTextLayout &textLayout, int lineWidth)
{
    qreal height = 0;
    qreal widthUsed = 0;
    textLayout.beginLayout();
    while (true) {
        QTextLine line = textLayout.createLine();
        if (!line.isValid())
            break;
        line.setLineWidth(lineWidth);
        line.setPosition(QPointF(0, height));
        height += line.height();
        widthUsed = qMax(widthUsed, line.naturalTextWidth());
    }
    textLayout.endLayout();
    return QSizeF(widthUsed, height);
}

/*!
  \brief DStyle::viewItemSize 视图项大小.

  \a style
  \a option
  \a role
  \sa QStyleOptionViewItem QStyle
  \sa Qt::ItemDataRole
  \return 文本内容所占大小
 */
QSize DStyle::viewItemSize(const QStyle *style, const QStyleOptionViewItem *option, int role)
{
    const QWidget *widget = option->widget;
    switch (role) {
    case Qt::CheckStateRole:
        if (option->features & QStyleOptionViewItem::HasCheckIndicator)
            return QSize(style->pixelMetric(QStyle::PM_IndicatorWidth, option, widget),
                         style->pixelMetric(QStyle::PM_IndicatorHeight, option, widget));
        break;
    case Qt::DisplayRole:
        if (option->features & QStyleOptionViewItem::HasDisplay) {
            QTextOption textOption;
            textOption.setWrapMode(QTextOption::WordWrap);
            QTextLayout textLayout(option->text, option->font);
            textLayout.setTextOption(textOption);
            const bool wrapText = option->features & QStyleOptionViewItem::WrapText;
            int spacing = DStyleHelper(style).pixelMetric(DStyle::PM_ContentsSpacing, option, widget);
            QRect bounds = option->rect;
            switch (option->decorationPosition) {
            case QStyleOptionViewItem::Left:
            case QStyleOptionViewItem::Right: {
                if (wrapText && bounds.isValid()) {
                    int width = bounds.width();
                    if (option->features & QStyleOptionViewItem::HasDecoration)
                        width -= option->decorationSize.width() + spacing;
                    bounds.setWidth(width);
                } else
                    bounds.setWidth(QFIXED_MAX);
                break;
            }
            case QStyleOptionViewItem::Top:
            case QStyleOptionViewItem::Bottom:
                if (wrapText)
                    bounds.setWidth(bounds.isValid() ? bounds.width() - spacing : option->decorationSize.width());
                else
                    bounds.setWidth(QFIXED_MAX);
                break;
            default:
                break;
            }

            if (wrapText && option->features & QStyleOptionViewItem::HasCheckIndicator)
                bounds.setWidth(bounds.width() - style->pixelMetric(QStyle::PM_IndicatorWidth) - spacing);

            const int lineWidth = bounds.width();
            const QSizeF size = viewItemTextLayout(textLayout, lineWidth);
            return QSize(qCeil(size.width()), qCeil(size.height()));
        }
        break;
    case Qt::DecorationRole:
        if (option->features & QStyleOptionViewItem::HasDecoration) {
            return option->decorationSize;
        }
        break;
    default:
        break;
    }

    return QSize(0, 0);
}

/*!
  \brief DStyle::viewItemLayout视图项组合(文字 + 图标 + 点击区域).

  \a pixmapRect 图标位置大小
  \a textRect 文字区域位置大小
  \a checkRect 点击区域位置大小
  \a sizehint 是否开启
  \a style 指定的风格实例.
  \a opt ViewItem相关的风格选项.

  \sa QStyle QStyleOptionViewItem
 */
void DStyle::viewItemLayout(const QStyle *style, const QStyleOptionViewItem *opt,  QRect *pixmapRect, QRect *textRect, QRect *checkRect, bool sizehint)
{
    Q_ASSERT(checkRect && pixmapRect && textRect);
    *pixmapRect = QRect(QPoint(0, 0), viewItemSize(style, opt, Qt::DecorationRole));
    *textRect = QRect(QPoint(0, 0), viewItemSize(style, opt, Qt::DisplayRole));
    *checkRect = QRect(QPoint(0, 0), viewItemSize(style, opt, Qt::CheckStateRole));

    const DStyleHelper dstyle(style);
    const QWidget *widget = opt->widget;
    const bool hasCheck = checkRect->isValid();
    const bool hasPixmap = pixmapRect->isValid();
    const bool hasText = textRect->isValid();
    const int spacing = dstyle.pixelMetric(DStyle::PM_ContentsSpacing, opt, widget);
    const int x = opt->rect.left();
    const int y = opt->rect.top();
    int w, h;

    if (textRect->height() == 0 && (!hasPixmap || !sizehint)) {
        //if there is no text, we still want to have a decent height for the item sizeHint and the editor size
        textRect->setHeight(opt->fontMetrics.height());
    }

    QSize pm(0, 0);
    if (hasPixmap) {
        pm = pixmapRect->size();
    }
    if (sizehint) {
        h = qMax(checkRect->height(), qMax(textRect->height(), pm.height()));
        if (opt->decorationPosition == QStyleOptionViewItem::Left
                || opt->decorationPosition == QStyleOptionViewItem::Right) {
            w = textRect->width() + pm.width();

            if (hasText) {
                w += spacing; // add spacing
            }
        } else {
            w = qMax(textRect->width(), pm.width());
        }

        int cw = 0;
        QRect check;
        if (hasCheck) {
            cw = checkRect->width();
            w += cw + spacing;

            if (opt->direction == Qt::RightToLeft) {
                check.setRect(x + w - cw, y, cw, h);
            } else {
                check.setRect(x, y, cw, h);
            }
        }

        QRect display;
        QRect decoration;
        switch (opt->decorationPosition) {
        case QStyleOptionViewItem::Top: {
            h = textRect->height();

            if (opt->direction == Qt::RightToLeft) {
                decoration.setRect(x, y, w - cw, pm.height());
                display.setRect(x, y + pm.height() + spacing, w - cw, h);
            } else {
                decoration.setRect(x + cw, y, w - cw, pm.height());
                display.setRect(x + cw, y + pm.height() + spacing, w - cw, h);
            }
            break;
        }
        case QStyleOptionViewItem::Bottom: {
            h = textRect->height() + pm.height();

            if (opt->direction == Qt::RightToLeft) {
                display.setRect(x, y, w - cw, textRect->height());
                decoration.setRect(x, y + textRect->height() + spacing, w - cw, h - textRect->height());
            } else {
                display.setRect(x + cw, y, w - cw, textRect->height());
                decoration.setRect(x + cw, y + textRect->height() + spacing, w - cw, h - textRect->height());
            }
            break;
        }
        case QStyleOptionViewItem::Left: {
            if (opt->direction == Qt::LeftToRight) {
                decoration.setRect(x + cw, y, pm.width(), h);
                display.setRect(decoration.right() + spacing, y, w - pm.width() - cw, h);
            } else {
                display.setRect(x, y, w - pm.width() - cw, h);
                decoration.setRect(display.right() + spacing, y, pm.width(), h);
            }
            break;
        }
        case QStyleOptionViewItem::Right: {
            if (opt->direction == Qt::LeftToRight) {
                display.setRect(x + cw, y, w - pm.width() - cw, h);
                decoration.setRect(display.right() + spacing, y, pm.width(), h);
            } else {
                decoration.setRect(x, y, pm.width(), h);
                display.setRect(decoration.right() + spacing, y, w - pm.width() - cw, h);
            }
            break;
        }
        default:
            qWarning("doLayout: decoration position is invalid");
            decoration = *pixmapRect;
            break;
        }

        *checkRect = check;
        *pixmapRect = decoration;
        *textRect = display;
    } else {
        w = opt->rect.width();
        h = opt->rect.height();

        *pixmapRect = QStyle::alignedRect(opt->direction, opt->decorationAlignment,
                                          pixmapRect->size(), opt->rect);

        QRect display = opt->rect;

        switch (opt->decorationPosition) {
        case QStyleOptionViewItem::Top: {
            int residue_height = opt->rect.bottom() - pixmapRect->bottom();

            // 空间不够时，要改变图标的位置来腾出空间
            if (textRect->height() > residue_height) {
                pixmapRect->moveTop(qMax(0, pixmapRect->top() - textRect->height() + residue_height));
            }

            display.setTop(pixmapRect->bottom() + spacing);
            break;
        }
        case QStyleOptionViewItem::Bottom: {
            int residue_height = pixmapRect->top() - opt->rect.top();

            // 空间不够时，要改变图标的位置来腾出空间
            if (textRect->height() > residue_height) {
                pixmapRect->moveBottom(qMin(opt->rect.bottom(), pixmapRect->bottom() + textRect->height() - residue_height));
            }

            display.setBottom(pixmapRect->top() - spacing);
            break;
        }
        case QStyleOptionViewItem::Left:
        case QStyleOptionViewItem::Right:
            if (opt->decorationPosition == QStyleOptionViewItem::Left
                    && opt->direction == Qt::LeftToRight) {
                int residue_width = pixmapRect->left() - opt->rect.left();

                // 空间不够时，要改变图标的位置来腾出空间
                if (textRect->width() > residue_width) {
                    pixmapRect->moveLeft(qMax(opt->rect.left(), pixmapRect->left() - textRect->width() + residue_width));
                }

                display.setLeft(pixmapRect->right() + spacing);

                // 居中时不需要增加spacing距离
                if (opt->displayAlignment & Qt::AlignHCenter) {
                    display.setLeft(pixmapRect->right());
                }
            } else {
                int residue_width = opt->rect.right() - pixmapRect->left();

                // 空间不够时，要改变图标的位置来腾出空间
                if (textRect->width() > residue_width) {
                    pixmapRect->moveRight(qMin(opt->rect.right(), pixmapRect->right() + textRect->width() - residue_width));
                }

                display.setRight(pixmapRect->left() - spacing);
            }
        default:
            break;
        }

        if (opt->features & QStyleOptionViewItem::HasCheckIndicator) {
            *checkRect = QStyle::alignedRect(opt->direction, Qt::AlignRight | Qt::AlignVCenter, checkRect->size(), display);
        }

//        *textRect = QStyle::alignedRect(opt->direction, opt->displayAlignment, textRect->size(), display);

        if (opt->features & QStyleOptionViewItem::HasCheckIndicator)
            display.setRight(checkRect->left() - spacing);

        *textRect = display;
    }
}

/*!
  \brief DStyle::viewItemLayout

  \a opt ViewItem的风格选项. \a pixmapRect 传出参数，pixmap的位置矩形. \a textRect 传出参数，文本的位置矩形，\a checkRect 传出参数，选中区域的位置矩形.
  \a sizehint 是否使用大小策略.

  \sa DStyle::viewItemLayout()
 */
void DStyle::viewItemLayout(const QStyleOptionViewItem *opt, QRect *pixmapRect, QRect *textRect, QRect *checkRect, bool sizehint) const
{
    viewItemLayout(this, opt, pixmapRect, textRect, checkRect, sizehint);
}

/*!
  \brief DStyle::viewItemDrawText视图项文字

  \a p 画家. \a rect 文字大小. \a style 指定的风格实例. \a option ViewItem的风格选项.

  \sa QStyleOptionViewItem QStyle
  \return 布局大小
 */
QRect DStyle::viewItemDrawText(const QStyle *style, QPainter *p, const QStyleOptionViewItem *option, const QRect &rect)
{
    Q_UNUSED(style)
    QModelIndex index = option->index;
    const QWidget *view = option->widget;
    QRect textRect = rect;
    const bool wrapText = option->features & QStyleOptionViewItem::WrapText;
    QTextOption textOption;
    textOption.setWrapMode(wrapText ? QTextOption::WordWrap : QTextOption::ManualWrap);
    textOption.setTextDirection(option->direction);
    textOption.setAlignment(QStyle::visualAlignment(option->direction, option->displayAlignment));
    QTextLayout textLayout(option->text, option->font);
    textLayout.setTextOption(textOption);

    viewItemTextLayout(textLayout, textRect.width());

    QString elidedText;
    qreal height = 0;
    qreal width = 0;
    int elidedIndex = -1;
    const int lineCount = textLayout.lineCount();
    for (int j = 0; j < lineCount; ++j) {
        const QTextLine line = textLayout.lineAt(j);
        if (j + 1 <= lineCount - 1) {
            const QTextLine nextLine = textLayout.lineAt(j + 1);
            if ((nextLine.y() + nextLine.height()) > textRect.height()) {
                int start = line.textStart();
                int length = line.textLength() + nextLine.textLength();
                const QStackTextEngine engine(textLayout.text().mid(start, length), option->font);
                elidedText = engine.elidedText(option->textElideMode, textRect.width());
                height += line.height();
                width = textRect.width();
                elidedIndex = j;
                break;
            }
        }
        if (line.naturalTextWidth() > textRect.width()) {
            int start = line.textStart();
            int length = line.textLength();
            const QStackTextEngine engine(textLayout.text().mid(start, length), option->font);
            elidedText = engine.elidedText(option->textElideMode, textRect.width());
            height += line.height();
            width = textRect.width();
            elidedIndex = j;
            break;
        }
        width = qMax<qreal>(width, line.width());
        height += line.height();
    }

    const QRect layoutRect = QStyle::alignedRect(option->direction, option->displayAlignment,
                                                 QSize(int(width), int(height)), textRect);
    const QPointF position = layoutRect.topLeft();
    for (int i = 0; i < lineCount; ++i) {
        const QTextLine line = textLayout.lineAt(i);
        if (i == elidedIndex) {
            qreal x = position.x() + line.x();
            qreal y = position.y() + line.y() + line.ascent();
            p->save();
            p->setFont(option->font);
            p->drawText(QPointF(x, y), elidedText);
            p->restore();
            break;
        }
        line.draw(p, position);
    }

    // Update ToolTip
    const DToolTip::ToolTipShowMode &showMode = DToolTip::toolTipShowMode(view);
    if (showMode != DToolTip::Default) {
        const bool showToolTip = (showMode == DToolTip::AlwaysShow) ||
                ((showMode == DToolTip::ShowWhenElided) && (elidedIndex != -1));
        QVariant vShowToolTip = index.data(ItemDataRole::ViewItemShowToolTipRole);
        bool needUpdate = false;
        if (vShowToolTip.isValid()) {
            bool oldShowStatus = vShowToolTip.toBool();
            if (showToolTip != oldShowStatus) {
                needUpdate = true;
            }
        } else {
            needUpdate = true;
        }
        if (needUpdate) {
            QString toolTipString = index.data(Qt::DisplayRole).toString();
            QString toolTip;
            if (showToolTip) {
                QTextOption toolTipOption;
                toolTipOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
                toolTipOption.setTextDirection(option->direction);
                toolTipOption.setAlignment(QStyle::visualAlignment(option->direction, option->displayAlignment));
                toolTip = DToolTip::wrapToolTipText(toolTipString, toolTipOption);
            }
            QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
            model->setData(index, toolTip, Qt::ToolTipRole);
            model->setData(index, showToolTip, ItemDataRole::ViewItemShowToolTipRole);
        }
    }
    return layoutRect;
}

/*!
  \brief DStyle::viewItemDrawText

  \a p 画笔实例, \a option ViewItem的风格选项，\a rect 原始矩形.
  \return 返回基于原始矩形的文本绘制矩形.

  \sa DStyle::viewItemDrawText()
 */
QRect DStyle::viewItemDrawText(QPainter *p, const QStyleOptionViewItem *option, const QRect &rect) const
{
    return viewItemDrawText(this, p, option, rect);
}
#endif

/*!
  \class Dtk::Widget::DStyledIconEngine
  \inmodule dtkwidget
  \brief DStyledIconEngine一个修改的 QIconEngine 类.
*/

void DStyledIconEngine::drawIcon(const QIcon &icon, QPainter *pa, const QRectF &rect)
{
    icon.paint(pa, rect.toRect());
}

/*!
  \brief DStyledIconEngine::DStyledIconEngine
  \a drawFun
  \a iconName
 */
DStyledIconEngine::DStyledIconEngine(DrawFun drawFun, const QString &iconName)
    : QIconEngine()
    , m_drawFun(drawFun)
    , m_iconName(iconName)
{
    m_painterRole = DPalette::NoRole;
    m_widget = nullptr;
}

/*!
  \brief DStyledIconEngine::bindDrawFun活页夹
  \a drawFun
 */
void DStyledIconEngine::bindDrawFun(DrawFun drawFun)
{
    m_drawFun = drawFun;
}

/*!
  \brief DStyledIconEngine::setIconName设置icon名称
  \a name 名称
 */
void DStyledIconEngine::setIconName(const QString &name)
{
    m_iconName = name;
}

/*!
  \brief DStyledIconEngine::pixmap

  \a size pixmap的大小， \a mode 图标模式的类型， \a state 图标的状态.

  \sa QIconEngine::pixmap()
 */
QPixmap DStyledIconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
    QImage image(size, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter pa(&image);
    paint(&pa, QRect(QPoint(0, 0), size), mode, state);
    pa.end();

    return QPixmap::fromImage(image);
}

/*!
  \brief DStyledIconEngine::paint 重载paint
  \a painter 画家
  \a palette 调色板
  \a rect 绘制的矩形大小.
  \sa QIconEngine::paint()
 */
void DStyledIconEngine::paint(QPainter *painter, const QPalette &palette, const QRectF &rect)
{
    if (!m_drawFun)
        return;

    painter->setBrush(palette.window());
    painter->setPen(QPen(palette.windowText(), painter->pen().widthF()));

    m_drawFun(painter, rect);
}

/*!
  \brief DStyledIconEngine::paint

  \a painter 画笔实例， \a rect 绘制的位置矩形，\a mode 图标模式，\a state 图标状态.

  \sa QIconEngine::paint()
 */
void DStyledIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
{
    Q_UNUSED(state)

    if (m_painterRole != QPalette::NoRole) {
        QPalette::ColorGroup cg = (mode == QIcon::Disabled) ? QPalette::Disabled : QPalette::Current;

        if (m_widget) {
            painter->setPen(m_widget->palette().brush(cg, m_painterRole).color());
            painter->setBrush(m_widget->palette().brush(cg, m_painterRole));
        } else {
            painter->setPen(qApp->palette().brush(cg, m_painterRole).color());
            painter->setBrush(qApp->palette().brush(cg, m_painterRole));
        }
    }

    m_drawFun(painter, rect);
}

/*!
  \brief DStyledIconEngine::clone

  \return 返回一个该引擎的克隆.

  \sa QIconEngine::clone()
 */
QIconEngine *DStyledIconEngine::clone() const
{
    return new DStyledIconEngine(m_drawFun, m_iconName);
}

void DStyledIconEngine::setFrontRole(const QWidget *widget, QPalette::ColorRole role)
{
    m_painterRole = role;
    m_widget = widget;
}

void DStyledIconEngine::virtual_hook(int id, void *data)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if (id == IconNameHook) {
        *reinterpret_cast<QString *>(data) = m_iconName;
    } else
#endif
    if (id == IsNullHook) {
        *reinterpret_cast<bool *>(data) = !m_drawFun;
    }

    return QIconEngine::virtual_hook(id, data);
}

DWIDGET_END_NAMESPACE
