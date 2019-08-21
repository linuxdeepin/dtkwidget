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

#include "dstyleoption.h"

#include "dlineedit.h"
#include "private/dlineedit_p.h"

#include <QGuiApplication>

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~english \class DStyleOption
 * \brief DStyleOption is the base class of deepin style option, use for dxcb plugins
 * \param widget
 */

/*!
 * \~chinese \class DStyleOption
 * \brief dcxb插件通过DStyleOption来控制控件的样式
 * \param widget
 */


/*!
 * \~english \class DStyleOptionSuggestButton
 * \brief DStyleOptionSuggestButton handle the style option of Dtk::Widget::DSuggestButton
 * \param widget
 */
/*!
 * \~english \enum DStyleOptionSuggestButton::ButtonFeature
 * \brief This enum extend types of features a Dtk::Widget::DSuggestButton can have
 */
/*!
 * \~english \var DStyleOptionSuggestButton::SuggestButton
 * \brief Default Button
 */

/*!
 * \~chinese \class DStyleOptionSuggestButton
 * \brief Dtk::Widget::DSuggestButton使用的样式配置
 * \param widget
 */
/*!
 * \~chinese \enum DStyleOptionSuggestButton::ButtonFeature
 * \brief Dtk::Widget::DSuggestButton扩展的样式
 */
/*!
 * \~chinese \var DStyleOptionSuggestButton::SuggestButton
 * \brief 默认样式
 */


/*!
 * \~english \class DStyleOptionLineEdit
 * \brief DStyleOptionLineEdit handle the style option of Dtk::Widget::DLineEdit
 * \param widget
 */
/*!
 * \~english \var DStyleOptionLineEdit::None
 * \brief Default none style
 */
/*!
 * \~english \var DStyleOptionLineEdit::Alert
 * \brief Alert style
 */
/*!
 * \~english \var DStyleOptionLineEdit::IconButton
 * \brief Edit with an icon
 */


/*!
 * \~chinese \class DStyleOptionLineEdit
 * \brief Dtk::Widget::DLineEdit使用的样式配置
 * \param widget
 */
/*!
 * \~chinese \enum DStyleOptionLineEdit::LineEditFeature
 * \brief Dtk::Widget::DSuggestButton扩展的样式
 */
/*!
 * \~chinese \var DStyleOptionLineEdit::None
 * \brief 不使用自定义样式，保持Qt默认样式。
 */
/*!
 * \~chinese \var DStyleOptionLineEdit::Alert
 * \brief 警告状态样式
 */
/*!
 * \~chinese \var DStyleOptionLineEdit::IconButton
 * \brief 包含图标按钮的编辑框样式
 */


/*!
 * \~english \brief Init style with widget.
 * \param widget
 */
/*!
 * \~chinese \brief 控件初始化样式时调用，可以用于重写控件样式。
 * \param widget
 */
void DStyleOption::init(QWidget *widget)
{
    init(static_cast<const QWidget*>(widget));
}

void DStyleOption::init(const QWidget *widget)
{
    dpalette = DPalette::get(widget);
}

/*!
 * \~english \brief DStyleOptionSuggestButton::init set style option for Dtk::Widget::DSuggestButton
 * \param widget
 * \sa Dtk::Widget::DSuggestButton
 */
/*!
 * \~chinese \brief 初始化Dtk::Widget::DSuggestButton样式，设置为 DStyleOptionSuggestButton::SuggestButton 。
 * \param widget
 * \sa Dtk::Widget::DSuggestButton
 */
void DStyleOptionButton::init(const QWidget *widget)
{
    DStyleOption::init(widget);
}

/*!
 * \~english \brief DStyleOptionSuggestButton::init set style option for Dtk::Widget::DLineEdit
 * \param widget
 * \sa Dtk::Widget::DLineEdit
 */
/*!
 * \~chinese \brief 根据Dtk::Widget::DLineEdit控件状态初始化样式
 * \param widget
 * \sa Dtk::Widget::DLineEdit
 */
void DStyleOptionLineEdit::init(const QWidget *widget)
{
    DStyleOption::init(widget);

    if (const DLineEdit *edit = qobject_cast<const DLineEdit*>(widget)) {
        if (edit->isAlert()) {
            features |= Alert;
        }

        if (edit->iconVisible()) {
            features |= IconButton;
            iconButtonRect = edit->d_func()->m_rightIcon->geometry();
        }
    }
}

void DStyleOptionBackgroundGroup::init(const QWidget *widget)
{
    DStyleOption::init(widget);
    rect = widget->geometry();
}

struct DPaletteData : public QSharedData
{
    QBrush br[DPalette::NColorGroups][DPalette::NColorTypes];
};

class DPalettePrivate
{
public:
    DPalettePrivate(const QSharedDataPointer<DPaletteData> &d)
        : data(d)
    {

    }

    QSharedDataPointer<DPaletteData> data;

    static QHash<const QWidget*, QSharedDataPointer<DPaletteData>> map;
    static QSharedDataPointer<DPaletteData> appPalette;
};

QHash<const QWidget*, QSharedDataPointer<DPaletteData>> DPalettePrivate::map;
QSharedDataPointer<DPaletteData> DPalettePrivate::appPalette(new DPaletteData());

DPalette::DPalette()
    : d(new DPalettePrivate(DPalettePrivate::appPalette))
{

}

DPalette::DPalette(const QPalette &palette)
    : QPalette(palette)
    , d(new DPalettePrivate(DPalettePrivate::appPalette))
{

}

DPalette::DPalette(const DPalette &palette)
    : QPalette(palette)
    , d(new DPalettePrivate(palette.d->data))
{

}

DPalette::~DPalette()
{

}

DPalette &DPalette::operator=(const DPalette &palette)
{
    d->data = palette.d->data;

    return *this;
}

DPalette DPalette::get(const QWidget *widget, const QPalette &base)
{
    DPalette pa = base;
    auto data = DPalettePrivate::map.value(widget);

    while (!data) {
        widget = widget->parentWidget();

        if (!widget) {
            break;
        }

        data = DPalettePrivate::map.value(widget);
    }

    if (data) {
        pa.d->data = data;
    }

    return pa;
}

void DPalette::set(QWidget *widget, const DPalette &pa)
{
    if (!DPalettePrivate::map.contains(widget)) {
        QObject::connect(widget, &QWidget::destroyed, [widget] {
            DPalettePrivate::map.remove(widget);
        });
    }

    DPalettePrivate::map[widget] = pa.d->data;
}

void DPalette::setGeneric(const DPalette &pa)
{
    DPalettePrivate::appPalette = pa.d->data;
}

const QBrush &DPalette::brush(QPalette::ColorGroup cg, DPalette::ColorType cr) const
{
    if (cr >= NColorTypes) {
        return QPalette::brush(cg, QPalette::NoRole);
    }

    if (cg == Current) {
        cg = currentColorGroup();
    } else if (cg >= NColorGroups) {
        cg = Active;
    }

    return d->data->br[cg][cr];
}

void DPalette::setBrush(QPalette::ColorGroup cg, DPalette::ColorType cr, const QBrush &brush)
{
    if (cg == All) {
        for (uint i = 0; i < NColorGroups; i++)
            setBrush(ColorGroup(i), cr, brush);
        return;
    }

    if (cr >= NColorTypes) {
        return QPalette::setBrush(cg, QPalette::NoRole, brush);
    }

    if (cg == Current) {
        cg = currentColorGroup();
    } else if (cg >= NColorGroups) {
        cg = Active;
    }

    d->data->br[cg][cr] = brush;
}

class DFontSizeManagerPrivate
{
public:
    QList<QWidget*> binderMap[DFontSizeManager::NSizeTypes];
    quint16 fontPixelSize[DFontSizeManager::NSizeTypes] = {40, 30, 24, 20, 17, 14, 13, 12, 11, 10};
    quint8 fontGenericSizeType = DFontSizeManager::T6;
    // 字号的差值
    quint16 fontPixelSizeDiff = 0;

    void updateWidgetFont(DFontSizeManager *manager, DFontSizeManager::SizeType type)
    {
        for (QWidget *w : binderMap[type]) {
            w->setFont(manager->get(type, w->font()));
        }
    }
};

DFontSizeManager *DFontSizeManager::instance()
{
    static DFontSizeManager manager;

    return &manager;
}

void DFontSizeManager::bind(QWidget *widget, DFontSizeManager::SizeType type)
{
    unbind(widget);

    d->binderMap[type].append(widget);
    widget->setFont(get(type, widget->font()));

    QObject::connect(widget, &QWidget::destroyed, [this, widget] {
        unbind(widget);
    });
}

void DFontSizeManager::unbind(QWidget *widget)
{
    for (int i = 0; i < NSizeTypes; ++i) {
        d->binderMap[i].removeOne(widget);
    }
}

quint16 DFontSizeManager::fontPixelSize(DFontSizeManager::SizeType type) const
{
    if (type >= NSizeTypes) {
        return 0;
    }

    return d->fontPixelSize[type] + d->fontPixelSizeDiff;
}

void DFontSizeManager::setFontPixelSize(DFontSizeManager::SizeType type, quint16 size)
{
    if (type >= NSizeTypes) {
        return;
    }

    if (d->fontPixelSize[type] == size) {
        return;
    }

    d->fontPixelSize[type] = size;
    d->updateWidgetFont(this, type);
}

void DFontSizeManager::setFontGenericPixelSize(quint16 size)
{
    qint16 diff = size - d->fontPixelSize[d->fontGenericSizeType];

    if (diff == d->fontPixelSizeDiff)
        return;

    d->fontPixelSizeDiff = diff;

    for (int i = 0; i < NSizeTypes; ++i) {
        d->updateWidgetFont(this, static_cast<DFontSizeManager::SizeType>(i));
    }
}

const QFont DFontSizeManager::get(DFontSizeManager::SizeType type, const QFont &base) const
{
    QFont font = base;

    font.setPixelSize(fontPixelSize(type));

    return font;
}

DFontSizeManager::DFontSizeManager()
    : d(new DFontSizeManagerPrivate())
{

}

DWIDGET_END_NAMESPACE
