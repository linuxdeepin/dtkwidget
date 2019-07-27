/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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
#ifndef DSTYLEOPTION_H
#define DSTYLEOPTION_H

#include <dtkwidget_global.h>

#include <QStyleOption>

QT_BEGIN_NAMESPACE
class QGuiApplication;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DStyleOption
{
public:
    enum PrimitiveElement {
        PE_BACKGROUND = QStyle::PE_CustomBase + 1,
    };

    virtual void init(QWidget *widget);
};

class DStyleOptionSuggestButton : public QStyleOptionButton, public DStyleOption
{
public:
    enum ButtonFeature {
        SuggestButton = (CommandLinkButton << 1)
    };

    void init(QWidget *widget) override;
};

class DStyleOptionLineEdit : public DStyleOption
{
public:
    enum LineEditFeature {
        None        = 0x0,
        Alert       = 0x1,
        IconButton  = 0x2
    };
    Q_DECLARE_FLAGS(LineEditFeatures, LineEditFeature)

    void init(QWidget *widget) override;

    LineEditFeatures features = None;
    QRect iconButtonRect;
};

class DStyleOptionBackgroundGroup : public QStyleOption, public DStyleOption
{
public:
    enum BackgroundDirection {
        horizontal,
        vertical,
    };

    enum ItemBackgroundPosition {
        Invalid,
        Beginning,
        Middle,
        End,
        OnlyOne
    };

    using DStyleOption::DStyleOption;
    using QStyleOption::QStyleOption;
    void init(QWidget *widget) override;

    BackgroundDirection direction;
    ItemBackgroundPosition position;
    QRect backgroundRect;
};

class DPalettePrivate;
class DPalette : public QPalette
{
public:
    enum ColorType {
        ItemBackground,
        TextTitle,
        TextTips,
        TextWarning,
        TextLively,
        NColorTypes
    };

    DPalette();
    DPalette(const QPalette &palette);
    ~DPalette();

    static DPalette get(const QWidget *widget, const QPalette &base);
    static inline DPalette get(const QWidget *widget)
    { return get(widget, widget->palette()); }

    static void set(QWidget *widget, const DPalette &pa);
    static void setGeneric(const DPalette &pa);

    inline const QColor &color(ColorGroup cg, ColorType ct) const
    { return brush(cg, ct).color(); }
    const QBrush &brush(ColorGroup cg, ColorType ct) const;
    inline void setColor(ColorGroup cg, ColorType ct, const QColor &color)
    { setBrush(cg, ct, color); }
    inline void setColor(ColorType ct, const QColor &color)
    { setColor(All, ct, color); }
    inline void setBrush(ColorType ct, const QBrush &brush)
    { setBrush(All, ct, brush); }
    void setBrush(ColorGroup cg, ColorType ct, const QBrush &brush);

    inline const QColor &color(ColorType ct) const { return color(Current, ct); }
    inline const QBrush &brush(ColorType ct) const { return brush(Current, ct); }
    inline const QBrush &itemBackground() const { return brush(ItemBackground); }
    inline const QBrush &textTiele() const { return brush(TextTitle); }
    inline const QBrush &textTips() const { return brush(TextTips); }
    inline const QBrush &textWarning() const { return brush(TextWarning); }
    inline const QBrush &textLively() const { return brush(TextLively); }

private:
    QSharedPointer<DPalettePrivate> d;
};

class DFontSizeManagerPrivate;
class DFontSizeManager
{
public:
    enum SizeType {
        T1,
        T2,
        T3,
        T4,
        T5,
        T6,
        T7,
        T8,
        T9,
        T10,
        NSizeTypes
    };

    static DFontSizeManager *instance();
    void bind(QWidget *widget, SizeType type);
    void unbind(QWidget *widget);

    quint16 fontPixelSize(SizeType type) const;
    void setFontPixelSize(SizeType type, quint16 size);
    void setFontGenericPixelSize(quint16 size);
    const QFont get(SizeType type, const QFont &base = QFont()) const;

    inline const QFont t1(const QFont &base = QFont()) const
    { return get(T1, base); }
    inline const QFont t2(const QFont &base = QFont()) const
    { return get(T2, base); }
    inline const QFont t3(const QFont &base = QFont()) const
    { return get(T3, base); }
    inline const QFont t4(const QFont &base = QFont()) const
    { return get(T4, base); }
    inline const QFont t5(const QFont &base = QFont()) const
    { return get(T5, base); }
    inline const QFont t6(const QFont &base = QFont()) const
    { return get(T6, base); }
    inline const QFont t7(const QFont &base = QFont()) const
    { return get(T7, base); }
    inline const QFont t8(const QFont &base = QFont()) const
    { return get(T8, base); }
    inline const QFont t9(const QFont &base = QFont()) const
    { return get(T9, base); }
    inline const QFont t10(const QFont &base = QFont()) const
    { return get(T10, base); }

private:
    DFontSizeManager();

    QScopedPointer<DFontSizeManagerPrivate> d;
};

DWIDGET_END_NAMESPACE

#endif // DSTYLEOPTION_H
