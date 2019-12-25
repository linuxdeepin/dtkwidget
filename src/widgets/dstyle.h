/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef DSTYLE_H
#define DSTYLE_H

#include <dtkwidget_global.h>
#include <DPalette>

#include <QCommonStyle>
#include <QPainter>
#include <QIconEngine>
#include <QStyleOption>

QT_BEGIN_NAMESPACE
class QTextLayout;
QT_END_NAMESPACE

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

namespace DDrawUtils
{
enum Corner {
    TopLeftCorner = 0x00001,
    TopRightCorner = 0x00002,
    BottomLeftCorner = 0x00004,
    BottomRightCorner = 0x00008
};
Q_DECLARE_FLAGS(Corners, Corner)

void drawShadow(QPainter *pa, const QRect &rect, qreal xRadius, qreal yRadius, const QColor &sc, qreal radius, const QPoint &offset);
void drawShadow(QPainter *pa, const QRect &rect, const QPainterPath &path, const QColor &sc, int radius, const QPoint &offset);
void drawRoundedRect(QPainter *pa, const QRect &rect, qreal xRadius, qreal yRadius, Corners corners, Qt::SizeMode mode = Qt::AbsoluteSize);
void drawFork(QPainter *pa, const QRectF &rect, const QColor &color, int width = 2);
void drawMark(QPainter *pa, const QRectF &rect, const QColor &boxInside, const QColor &boxOutside, const int penWidth, const int outLineLeng = 2);
void drawBorder(QPainter *pa, const QRectF &rect, const QBrush &brush, int borderWidth, int radius);
void drawArrow(QPainter *pa, const QRectF &rect, const QColor &color, Qt::ArrowType arrow, int width = 2);
void drawPlus(QPainter *painter, const QRectF &rect, const QColor &color , qreal width);
void drawSubtract(QPainter *painter, const QRectF &rect, const QColor &color, qreal width);

void drawForkElement(QPainter *pa, const QRectF &rect);
void drawArrowElement(Qt::ArrowType arrow, QPainter *pa, const QRectF &rect);
void drawDecreaseElement(QPainter *pa, const QRectF &rect);
void drawIncreaseElement(QPainter *pa, const QRectF &rect);
void drawMarkElement(QPainter *pa, const QRectF &rect);
void drawSelectElement(QPainter *pa, const QRectF &rect);
void drawEditElement(QPainter *pa, const QRectF &rect);
void drawExpandElement(QPainter *pa, const QRectF &rect);
void drawReduceElement(QPainter *pa, const QRectF &rect);
void drawLockElement(QPainter *pa, const QRectF &rect);
void drawUnlockElement(QPainter *pa, const QRectF &rect);
void drawMediaVolumeElement(QPainter *pa, const QRectF &rect);
void drawMediaVolumeFullElement(QPainter *pa, const QRectF &rect);
void drawMediaVolumeMutedElement(QPainter *pa, const QRectF &rect);
void drawMediaVolumeLeftElement(QPainter *pa, const QRectF &rect);
void drawMediaVolumeRightElement(QPainter *pa, const QRectF &rect);
void drawArrowEnter(QPainter *pa, const QRectF &rect);
void drawArrowLeave(QPainter *pa, const QRectF &rect);
void drawArrowNext(QPainter *pa, const QRectF &rect);
void drawArrowPrev(QPainter *pa, const QRectF &rect);
void drawShowPassword(QPainter *pa, const QRectF &rect);
void drawHidePassword(QPainter *pa, const QRectF &rect);
void drawCloseButton(QPainter *pa, const QRectF &rect);
void drawIndicatorMajuscule(QPainter *pa, const QRectF &rect);
void drawIndicatorUnchecked(QPainter *pa, const QRectF &rect);
void drawIndicatorChecked(QPainter *pa, const QRectF &rect);
void drawDeleteButton(QPainter *pa, const QRectF &rect);
void drawAddButton(QPainter *pa, const QRectF &rect);

void drawTitleBarMenuButton(QPainter *pa, const QRectF &rect);
void drawTitleBarMinButton(QPainter *pa, const QRectF &rect);
void drawTitleBarMaxButton(QPainter *pa, const QRectF &rect);
void drawTitleBarCloseButton(QPainter *pa, const QRectF &rect);
void drawTitleBarNormalButton(QPainter *pa, const QRectF &rect);
void drawArrowUp(QPainter *pa, const QRectF &rect);
void drawArrowDown(QPainter *pa, const QRectF &rect);
void drawArrowLeft(QPainter *pa, const QRectF &rect);
void drawArrowRight(QPainter *pa, const QRectF &rect);
void drawArrowBack(QPainter *pa, const QRectF &rect);
void drawArrowForward(QPainter *pa, const QRectF &rect);
void drawLineEditClearButton(QPainter *pa, const QRectF &rect);

Q_DECLARE_OPERATORS_FOR_FLAGS(Corners)
}

class DViewItemAction;
class DStyle : public QCommonStyle
{
    Q_OBJECT

public:
    enum PrimitiveElement {
        PE_ItemBackground = QStyle::PE_CustomBase + 1,          //列表项的背景色
        PE_IconButtonPanel,
        PE_IconButtonIcon,
        PE_Icon,
        PE_SwitchButtonGroove,
        PE_SwitchButtonHandle,
        PE_FloatingWidget,
        PE_CustomBase = QStyle::PE_CustomBase + 0xf00000
    };

    enum ControlElement {
        CE_IconButton = QStyle::CE_CustomBase + 1,
        CE_SwitchButton,
        CE_FloatingWidget,
        CE_ButtonBoxButton,
        CE_ButtonBoxButtonBevel,
        CE_ButtonBoxButtonLabel,
        CE_TextButton,
        CE_CustomBase = QStyle::CE_CustomBase + 0xf00000
    };

    enum PixelMetric {
        PM_FocusBorderWidth = QStyle::PM_CustomBase + 1,        //控件焦点状态的边框宽度
        PM_FocusBorderSpacing,                                  //控件内容和border之间的间隔
        PM_FrameRadius,                                         //控件的圆角大小
        PM_ShadowRadius,                                        //控件阴影效果的半径
        PM_ShadowHOffset,                                       //阴影在水平方向的偏移
        PM_ShadowVOffset,                                       //阴影在竖直方向的偏移
        PM_FrameMargins,                                        //控件的margins区域，控件内容 = 控件大小 - FrameMargins
        PM_IconButtonIconSize,
        PM_TopLevelWindowRadius,                                //窗口的圆角大小
        PM_SwitchButtonHandleWidth,
        PM_SwithcButtonHandleHeight,
        PM_FloatingWidgetRadius,                                //(基类)的圆角半径:控件内容-Radius < 控件内容 < 控件显示大小
        PM_FloatingWidgetShadowRadius,                          //(基类)的阴影Radius区域:控件内容 < 控件内容+阴影margins < 控件内容+阴影margins+阴影Radius = 控件显示大小
        PM_FloatingWidgetShadowMargins,                         //(基类)阴影的宽度 = 控件显示大小 - 阴影Radius - 控件内容
        PM_FloatingWidgetShadowHOffset,                         //(基类)的阴影水平偏移
        PM_FloatingWidgetShadowVOffset,                         //(基类)的阴影竖直偏移
        PM_ContentsMargins,                                     //内容的边距（一般只用于左右边距）
        PM_ContentsSpacing,                                     //内容的间距（可用于列表项中每一项的距离）
        PM_ButtonMinimizedSize,                                 //按钮控件的最小大小
        PM_IndicatorItemChekedSize,                            //checked状态（对勾）
        PM_CustomBase = QStyle::PM_CustomBase + 0xf00000
    };

    enum SubElement {
        SE_IconButtonIcon = QStyle::SE_CustomBase + 1,
        SE_SwitchButtonGroove,
        SE_SwitchButtonHandle,
        SE_FloatingWidget,
        SE_ButtonBoxButtonContents,
        SE_ButtonBoxButtonFocusRect,
        SE_CustomBase = QStyle::SE_CustomBase + 0xf00000
    };

    enum ContentsType {
        CT_IconButton = QStyle::CT_CustomBase + 1,
        CT_SwitchButton,
        CT_FloatingWidget,
        CT_ButtonBoxButton,
        CT_CustomBase = QStyle::CT_CustomBase + 0xf00000
    };

    enum StyleState {
        SS_NormalState = 0x00000000,
        SS_HoverState = 0x00000001,
        SS_PressState = 0x00000002,
        SS_StateCustomBase = 0x000000f0,

        StyleState_Mask = 0x000000ff,
        SS_CheckedFlag = 0x00000100,
        SS_SelectedFlag = 0x00000200,
        SS_FocusFlag = 0x00000400,
        SS_FlagCustomBase = 0xf00000
    };
    Q_DECLARE_FLAGS(StateFlags, StyleState)

    enum StandardPixmap {
        SP_ForkElement = QStyle::SP_CustomBase + 1,
        SP_DecreaseElement,                         //减少（-）
        SP_IncreaseElement,                         //增加（+）
        SP_MarkElement,                             //对勾
        SP_SelectElement,                           //选择（...）
        SP_EditElement,                             //编辑
        SP_ExpandElement,                           //展开
        SP_ReduceElement,                           //收缩
        SP_LockElement,                             //锁定
        SP_UnlockElement,                           //解锁
        SP_MediaVolumeLowElement,                   //音量
        SP_MediaVolumeHighElement,                  //满音量
        SP_MediaVolumeMutedElement,                 //静音
        SP_MediaVolumeLeftElement,                  //左声道
        SP_MediaVolumeRightElement,                 //右声道
        SP_ArrowEnter,                              //进入
        SP_ArrowLeave,                              //离开
        SP_ArrowNext,                               //下一页
        SP_ArrowPrev,                               //上一页
        SP_ShowPassword,                            //显示密码
        SP_HidePassword,                            //因此密码
        SP_CloseButton,                             //关闭按钮（X）
        SP_IndicatorMajuscule,                      //大写标识
        SP_IndicatorSearch,                         //搜索标识（放大镜）
        SP_IndicatorUnchecked,                      //搜索标识（对应对勾的选中状态）
        SP_IndicatorChecked,                        //搜索标识（对勾）
        SP_DeleteButton,                            //删除按钮
        SP_AddButton,                               //新增按钮
        SP_CustomBase = QStyle::SP_CustomBase + 0xf00000
    };

    static QColor adjustColor(const QColor &base,
                              qint8 hueFloat = 0, qint8 saturationFloat = 0, qint8 lightnessFloat = 0,
                              qint8 redFloat = 0, qint8 greenFloat = 0, qint8 blueFloat = 0, qint8 alphaFloat = 0);
    static QColor blendColor(const QColor &substrate, const QColor &superstratum);
    static QPair<QIcon::Mode, QIcon::State> toIconModeState(const QStyleOption *option);

    static void setTooltipTextFormat(Qt::TextFormat format);
    static Qt::TextFormat tooltipTextFormat();
    static DStyle::StyleState getState(const QStyleOption *option);
    static void setFocusRectVisible(QWidget *widget, bool visible);
    static void setFrameRadius(QWidget *widget, int radius);
    DStyle();

    static void drawPrimitive(const QStyle *style, DStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w = nullptr);
    static void drawControl(const QStyle *style, DStyle::ControlElement ce, const QStyleOption *opt, QPainter *p, const QWidget *w = nullptr);
    static int pixelMetric(const QStyle *style, DStyle::PixelMetric m, const QStyleOption *opt = nullptr, const QWidget *widget = nullptr);
    static QRect subElementRect(const QStyle *style, DStyle::SubElement r, const QStyleOption *opt, const QWidget *widget = nullptr);
    static QSize sizeFromContents(const QStyle *style, DStyle::ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget = nullptr);
    static QIcon standardIcon(const QStyle *style, StandardPixmap st, const QStyleOption *opt = nullptr, const QWidget *widget = 0);

    inline void drawPrimitive(DStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w = nullptr) const
    { proxy()->drawPrimitive(static_cast<QStyle::PrimitiveElement>(pe), opt, p, w); }
    inline void drawControl(DStyle::ControlElement ce, const QStyleOption *opt, QPainter *p, const QWidget *w = nullptr) const
    { proxy()->drawControl(static_cast<QStyle::ControlElement>(ce), opt, p, w); }
    inline int pixelMetric(DStyle::PixelMetric m, const QStyleOption *opt = nullptr, const QWidget *widget = nullptr) const
    { return proxy()->pixelMetric(static_cast<QStyle::PixelMetric>(m), opt, widget); }
    inline QRect subElementRect(DStyle::SubElement r, const QStyleOption *opt, const QWidget *widget = nullptr) const
    { return proxy()->subElementRect(static_cast<QStyle::SubElement>(r), opt, widget); }
    inline QSize sizeFromContents(DStyle::ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget = nullptr) const
    { return proxy()->sizeFromContents(static_cast<QStyle::ContentsType>(ct), opt, contentsSize, widget); }
    inline QIcon standardIcon(DStyle::StandardPixmap st, const QStyleOption *opt = nullptr, const QWidget *widget = nullptr) const
    { return proxy()->standardIcon(static_cast<QStyle::StandardPixmap>(st), opt, widget); }

    void drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w = nullptr) const override;
    void drawControl(QStyle::ControlElement ce, const QStyleOption *opt, QPainter *p, const QWidget *w = nullptr) const override;
    int pixelMetric(QStyle::PixelMetric m, const QStyleOption *opt = nullptr, const QWidget *widget = nullptr) const override;
    int styleHint(StyleHint sh, const QStyleOption *opt, const QWidget *w, QStyleHintReturn *shret) const override;
    QRect subElementRect(QStyle::SubElement r, const QStyleOption *opt, const QWidget *widget = nullptr) const override;
    QSize sizeFromContents(QStyle::ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget = nullptr) const override;
    QIcon standardIcon(QStyle::StandardPixmap st, const QStyleOption *opt = nullptr, const QWidget *widget = nullptr) const override;

    QPalette standardPalette() const override;
    QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *opt) const override;

    // 获取一个加工后的画笔
    QBrush generatedBrush(const QStyleOption *option, const QBrush &base,
                          QPalette::ColorGroup cg = QPalette::Normal,
                          QPalette::ColorRole role = QPalette::NoRole) const;
    QBrush generatedBrush(StyleState state, const QStyleOption *option, const QBrush &base,
                          QPalette::ColorGroup cg = QPalette::Normal,
                          QPalette::ColorRole role = QPalette::NoRole) const;
    virtual QBrush generatedBrush(StateFlags flags, const QBrush &base,
                                  QPalette::ColorGroup cg = QPalette::Normal,
                                  QPalette::ColorRole role = QPalette::NoRole,
                                  const QStyleOption *option = nullptr) const;

    QBrush generatedBrush(const QStyleOption *option, const QBrush &base,
                          DPalette::ColorGroup cg = DPalette::Normal,
                          DPalette::ColorType type = DPalette::ItemBackground) const;
    QBrush generatedBrush(StyleState state, const QStyleOption *option, const QBrush &base,
                          DPalette::ColorGroup cg = DPalette::Normal,
                          DPalette::ColorType type = DPalette::ItemBackground) const;
    virtual QBrush generatedBrush(StateFlags flags, const QBrush &base,
                                  DPalette::ColorGroup cg = DPalette::Normal,
                                  DPalette::ColorType type = DPalette::ItemBackground,
                                  const QStyleOption *option = nullptr) const;

    using QCommonStyle::drawPrimitive;
    using QCommonStyle::drawControl;
    using QCommonStyle::pixelMetric;
    using QCommonStyle::subElementRect;
    using QCommonStyle::sizeFromContents;
    using QCommonStyle::standardIcon;

#if QT_CONFIG(itemviews)
    static QSizeF viewItemTextLayout(QTextLayout &textLayout, int lineWidth);
    static QSize viewItemSize(const QStyle *style, const QStyleOptionViewItem *option, int role);
    static void viewItemLayout(const QStyle *style, const QStyleOptionViewItem *opt, QRect *pixmapRect,
                               QRect *textRect, QRect *checkRect, bool sizehint);
    virtual void viewItemLayout(const QStyleOptionViewItem *opt, QRect *pixmapRect,
                                QRect *textRect, QRect *checkRect, bool sizehint) const;

    static QRect viewItemDrawText(const QStyle *style, QPainter *p, const QStyleOptionViewItem *option, const QRect &rect);
    virtual QRect viewItemDrawText(QPainter *p, const QStyleOptionViewItem *option, const QRect &rect) const;
#endif
};

class DStyleHelper
{
public:
    inline DStyleHelper(const QStyle *style = nullptr) {
        setStyle(style);
    }

    inline void setStyle(const QStyle *style) {
        m_style = style;
        m_dstyle = qobject_cast<const DStyle*>(style);
    }

    inline const QStyle *style() const
    { return m_style; }
    inline const DStyle *dstyle() const
    { return m_dstyle; }

    inline QBrush generatedBrush(const QStyleOption *option, const QBrush &base,
                                 QPalette::ColorGroup cg = QPalette::Normal,
                                 QPalette::ColorRole role = QPalette::NoRole) const
    { return m_dstyle ? m_dstyle->generatedBrush(option, base, cg, role) : base; }
    inline QBrush generatedBrush(const QStyleOption *option, const QBrush &base,
                                 QPalette::ColorGroup cg = QPalette::Normal,
                                 DPalette::ColorType type = DPalette::NoType) const
    { return m_dstyle ? m_dstyle->generatedBrush(option, base, cg, type) : base; }
    inline QColor getColor(const QStyleOption *option, QPalette::ColorRole role) const
    { return generatedBrush(option, option->palette.brush(role), option->palette.currentColorGroup(), role).color(); }
    inline QColor getColor(const QStyleOption *option, const DPalette &palette, DPalette::ColorType type) const
    { return generatedBrush(option, palette.brush(type), palette.currentColorGroup(), type).color(); }
    template<class T>
    inline QColor getColor(const T *option, DPalette::ColorType type) const
    { return getColor(option, option->dpalette, type); }

    inline void drawPrimitive(DStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w = nullptr) const
    { m_dstyle ? m_dstyle->drawPrimitive(pe, opt, p, w) : DStyle::drawPrimitive(m_style, pe, opt, p, w); }
    inline void drawControl(DStyle::ControlElement ce, const QStyleOption *opt, QPainter *p, const QWidget *w = nullptr) const
    { m_dstyle ? m_dstyle->drawControl(ce, opt, p, w) : DStyle::drawControl(m_style, ce, opt, p, w); }
    inline int pixelMetric(DStyle::PixelMetric m, const QStyleOption *opt = nullptr, const QWidget *widget = nullptr) const
    { return m_dstyle ? m_dstyle->pixelMetric(m, opt, widget) : DStyle::pixelMetric(m_style, m, opt, widget); }
    inline QRect subElementRect(DStyle::SubElement r, const QStyleOption *opt, const QWidget *widget = nullptr) const
    { return m_dstyle ? m_dstyle->subElementRect(r, opt, widget) : DStyle::subElementRect(m_style, r, opt, widget); }
    inline QSize sizeFromContents(DStyle::ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget = nullptr) const
    { return m_dstyle ? m_dstyle->sizeFromContents(ct, opt, contentsSize, widget) : DStyle::sizeFromContents(m_style, ct, opt, contentsSize, widget); }
    inline QIcon standardIcon(DStyle::StandardPixmap standardIcon, const QStyleOption *opt, const QWidget *widget = nullptr) const
    { return m_dstyle ? m_dstyle->standardIcon(standardIcon, opt, widget) : DStyle::standardIcon(m_style, standardIcon, opt, widget); }

private:
    const QStyle *m_style;
    const DStyle *m_dstyle;
};

class DStylePainter : public QPainter
{
public:
    inline DStylePainter() : QPainter(), widget(nullptr), wstyle(nullptr) {}
    inline explicit DStylePainter(QWidget *w) { begin(w, w); }
    inline DStylePainter(QPaintDevice *pd, QWidget *w) { begin(pd, w); }
    inline bool begin(QWidget *w) { return begin(w, w); }
    inline bool begin(QPaintDevice *pd, QWidget *w) {
        Q_ASSERT_X(w, "DStylePainter::DStylePainter", "Widget must be non-zero");
        widget = w;
        wstyle = w->style();
        dstyle.setStyle(wstyle);
        return QPainter::begin(pd);
    };
    inline void drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption &opt);
    inline void drawPrimitive(DStyle::PrimitiveElement pe, const QStyleOption &opt);
    inline void drawControl(QStyle::ControlElement ce, const QStyleOption &opt);
    inline void drawControl(DStyle::ControlElement ce, const QStyleOption &opt);
    inline void drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex &opt);
    inline void drawItemText(const QRect &r, int flags, const QPalette &pal, bool enabled,
                             const QString &text, QPalette::ColorRole textRole = QPalette::NoRole);
    inline void drawItemPixmap(const QRect &r, int flags, const QPixmap &pixmap);
    inline QStyle *style() const { return wstyle; }

private:
    QWidget *widget;
    QStyle *wstyle;
    DStyleHelper dstyle;
    Q_DISABLE_COPY(DStylePainter)
};

void DStylePainter::drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption &opt)
{
    wstyle->drawPrimitive(pe, &opt, this, widget);
}

void DStylePainter::drawPrimitive(DStyle::PrimitiveElement pe, const QStyleOption &opt)
{
    dstyle.drawPrimitive(pe, &opt, this, widget);
}

void DStylePainter::drawControl(QStyle::ControlElement ce, const QStyleOption &opt)
{
    wstyle->drawControl(ce, &opt, this, widget);
}

void DStylePainter::drawControl(DStyle::ControlElement ce, const QStyleOption &opt)
{
    dstyle.drawControl(ce, &opt, this, widget);
}

void DStylePainter::drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex &opt)
{
    wstyle->drawComplexControl(cc, &opt, this, widget);
}

void DStylePainter::drawItemText(const QRect &r, int flags, const QPalette &pal, bool enabled,
                                 const QString &text, QPalette::ColorRole textRole)
{
    wstyle->drawItemText(this, r, flags, pal, enabled, text, textRole);
}

void DStylePainter::drawItemPixmap(const QRect &r, int flags, const QPixmap &pixmap)
{
    wstyle->drawItemPixmap(this, r, flags, pixmap);
}

class DStyledIconEngine : public QIconEngine
{
public:
    static void drawIcon(const QIcon &icon, QPainter *pa, const QRectF &rect);

    typedef std::function<void(QPainter *, const QRectF &rect)> DrawFun;
    DStyledIconEngine(DrawFun drawFun, const QString &iconName = QString());

    void bindDrawFun(DrawFun drawFun);
    void setIconName(const QString &name);

    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;
    void paint(QPainter *painter, const QPalette &palette, const QRectF &rect);
    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;

    QIconEngine *clone() const override;
    void setFrontRole(const QWidget* widget, QPalette::ColorRole role);

protected:
    void virtual_hook(int id, void *data) override;

    DrawFun m_drawFun = nullptr;
    QString m_iconName;
    QPalette::ColorRole m_painterRole;
    const QWidget *m_widget;
};

DWIDGET_END_NAMESPACE

#endif // DSTYLE_H
