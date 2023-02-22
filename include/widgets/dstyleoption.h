// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSTYLEOPTION_H
#define DSTYLEOPTION_H

#include <dtkwidget_global.h>
#include <DPalette>

#include <DDciIcon>
#include <QStyleOption>

QT_BEGIN_NAMESPACE
class QGuiApplication;
QT_END_NAMESPACE

DGUI_USE_NAMESPACE

DTK_BEGIN_NAMESPACE

enum ItemDataRole {
    MarginsRole = Qt::UserRole + 1,
    LeftActionListRole,
    TopActionListRole,
    RightActionListRole,
    BottomActionListRole,
    TextActionListRole,
    ViewItemFontLevelRole,
    ViewItemBackgroundRole,
    ViewItemForegroundRole,
    ViewItemShowToolTipRole,
    UserRole = Qt::UserRole << 2
};

DTK_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE

class DStyleOption
{
public:
    enum OptionType {
        SO_HighlightButton = QStyleOption::SO_CustomBase + 1,
        SO_CustomBase = QStyleOption::SO_CustomBase << 2
    };

    virtual void init(QWidget *widget);
    virtual void init(const QWidget *widget);
    virtual ~DStyleOption() {}

    DPalette dpalette;
};

class DStyleOptionButton : public QStyleOptionButton, public DStyleOption
{
public:
    explicit DStyleOptionButton();
    DStyleOptionButton(const DStyleOptionButton &other);
    DStyleOptionButton &operator=(const DStyleOptionButton &other);
    enum ButtonFeature {
        SuggestButton = (CommandLinkButton << 1),
        WarningButton = (SuggestButton << 1),
        FloatingButton = (WarningButton << 1),
        TitleBarButton = (FloatingButton << 1),
        CircleButton = (TitleBarButton << 1),
        HasDciIcon = (CircleButton << 1)
    };

    void init(const QWidget *widget) override;
    DDciIcon dciIcon;
};

class DStyleOptionButtonBoxButton : public DStyleOptionButton
{
public:
    enum ButtonPosition {
        Invalid,
        Beginning,
        Middle,
        End,
        OnlyOne
    };

    Qt::Orientation orientation;
    ButtonPosition position;
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

    void init(const QWidget *widget) override;

    LineEditFeatures features = None;
    QRect iconButtonRect;
};

class DStyleOptionBackgroundGroup : public QStyleOption, public DStyleOption
{
public:
    enum ItemBackgroundPosition {
        Invalid,
        Beginning,
        Middle,
        End,
        OnlyOne
    };

    using DStyleOption::DStyleOption;
    using QStyleOption::QStyleOption;
    void init(const QWidget *widget) override;

    Qt::Orientations directions;
    ItemBackgroundPosition position;
};

class DStyleOptionIcon : public QStyleOption, public DStyleOption
{
public:
    QIcon icon;
};

typedef DStyleOptionIcon DStyleOptionIconV1;

class DStyleOptionIconV2 : public DStyleOptionIconV1
{
public:
    enum IconType { SI_QIcon, SI_DciIcon };

    IconType iconType;
    QSize iconSize;
    Qt::Alignment iconAlignment;

    DDciIcon dciIcon;
    DDciIcon::Theme dciTheme;
    DDciIcon::Mode dciMode;
};

class DStyleOptionViewItem : public QStyleOptionViewItem, public DStyleOption
{
public:
    enum ViewItemFeature {

    };
};

class DStyleOptionFloatingWidget : public QStyleOption, public DStyleOption
{
public:
    using DStyleOption::init;
    bool noBackground;
    int frameRadius = -1;
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
    void bind(QWidget *widget, SizeType type, int weight);
    void unbind(QWidget *widget);

    quint16 fontPixelSize(SizeType type) const;
    void setFontPixelSize(SizeType type, quint16 size);
    void setFontGenericPixelSize(quint16 size);
    const QFont get(SizeType type, const QFont &base = QFont()) const;
    const QFont get(SizeType type, int weight, const QFont &base = QFont()) const;

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

    static int fontPixelSize(const QFont &font);

private:
    DFontSizeManager();

    QScopedPointer<DFontSizeManagerPrivate> d;
};

DWIDGET_END_NAMESPACE

#endif // DSTYLEOPTION_H
