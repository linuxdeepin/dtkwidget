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

#define private public
#include <QFont>
#undef private

#include "dstyleoption.h"

#include "dlineedit.h"
#include "private/dlineedit_p.h"
#include "dapplicationhelper.h"

#include <QGuiApplication>
#include <qmath.h>
#include <private/qfont_p.h>

#include <cmath>

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
    dpalette = DApplicationHelper::instance()->palette(widget);
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
    }
}

void DStyleOptionBackgroundGroup::init(const QWidget *widget)
{
    DStyleOption::init(widget);
    if (widget)
        rect = widget->geometry();
}

class DFontSizeManagerPrivate
{
public:
    DFontSizeManagerPrivate() {
        fontPixelSizeDiff = DFontSizeManager::fontPixelSize(qGuiApp->font()) - fontPixelSize[DFontSizeManager::T6];
    }

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

/*!
 * \~chinese \class DFontSizeManager
 * \~chinese \brief 字体大小设置的一个类,系统默认只设置T6
 * \chiinese \image html font.png
 */

/*!
 * \~chinese \brief 创建一个字体管理的实例
 * \~chinese \return 返回一个创建的 static DFontSizeManager 实例
 */
DFontSizeManager *DFontSizeManager::instance()
{
    static DFontSizeManager manager;

    return &manager;
}

void DFontSizeManager::bind(QWidget *widget, DFontSizeManager::SizeType type)
{
    bind(widget, type, widget->font().weight());
}

/*!
 * \~chinese \enum DFontSizeManager::SizeType
 * \~chinese DFontSizeManager::SizeType 定义了 DFontSizeManager 的系统字体的定义的大小; 而系统只会设置 T6 为系统默认的字体
 * \~chinese \var DFontSizeManager:SizeType DFontSizeManager::T1
 * \~chinese 系统级别为 T1 的字体大小, 默认是40 px
 * \~chinese \var DFontSizeManager:SizeType DFontSizeManager::T2
 * \~chinese 系统级别为 T2 的字体大小, 默认是30 px
 * \~chinese \var DFontSizeManager:SizeType DFontSizeManager::T3
 * \~chinese 系统级别为 T3 的字体大小, 默认是24 px
 * \~chinese \var DFontSizeManager:SizeType DFontSizeManager::T4
 * \~chinese 系统级别为 T4 的字体大小, 默认是20 px
 * \~chinese \var DFontSizeManager:SizeType DFontSizeManager::T5
 * \~chinese 系统级别为 T5 的字体大小, 默认是17 px
 * \~chinese \var DFontSizeManager:SizeType DFontSizeManager::T6
 * \~chinese 系统级别为 T6 的字体大小, 默认是14 px
 * \~chinese \var DFontSizeManager:SizeType DFontSizeManager::T7
 * \~chinese 系统级别为 T7 的字体大小, 默认是13 px
 * \~chinese \var DFontSizeManager:SizeType DFontSizeManager::T8
 * \~chinese 系统级别为 T8 的字体大小, 默认是12 px
 * \~chinese \var DFontSizeManager:SizeType DFontSizeManager::T9
 * \~chinese 系统级别为 T9 的字体大小, 默认是11 px
 * \~chinese \var DFontSizeManager:SizeType DFontSizeManager::T10
 * \~chinese 系统级别为 T10 的字体大小, 默认是10 px
 */

/*!
 * \~chinese \brief 将字体的大小枚举 SizeType 和控件 widget 进行绑定, 其控件的字体大小(随绑定的枚举的)对应值的改变而改变;
 * \~chinese        系统自定义的绑定枚举值 T6, 若 T6 = 14px,  则其他枚举 T1 - T10 的数值,依次为:40, 30, 24, 20, 17, 14(T6), 13, 12, 11, 10;
 * \~chinese        系统自定义的绑定枚举值 T6 改为 T6 = 20px, 则其他枚举 T1 - T10 的数值,依次为:46, 36, 30, 26, 23, 20(T6), 19, 18, 17, 16;
 * \~chinese        即: 其对应的无论 T6 为何值, 其两个相邻的 T 值的差是定值: T(n) - T(n-1) == 定值
 * \~chinese        而系统是只设置 T6 这以枚举, 作为基准
 * \~chinese \param[in] widget 将要绑定字体大小枚举数值的控件
 * \~chinese \param[int] type 字体的枚举类型, 每一个枚举数值对应着一个字体像素大小
 */
void DFontSizeManager::bind(QWidget *widget, DFontSizeManager::SizeType type, int weight)
{
    unbind(widget);

    d->binderMap[type].append(widget);
    widget->setFont(get(type, weight, widget->font()));

    QObject::connect(widget, &QWidget::destroyed, [this, widget] {
        unbind(widget);
    });
}

/*!
 * \~chinese \brief 将字体大小枚举值和 widget 的绑定解除, 不跟随 "枚举对应的像素值" 而改变本控件的字体大小
 * \~chinese \param[in] widget 与 "字体枚举所对应的像素值" 的 (解除绑定的控件) 对象
 */
void DFontSizeManager::unbind(QWidget *widget)
{
    for (int i = 0; i < NSizeTypes; ++i) {
        d->binderMap[i].removeOne(widget);
    }
}

/*!
 * \~chinese \brief 获取字体像素的大小
 * \~chinese \param[in] type 字体枚举类型
 * \~chinese \return 返回字体像素的大小
 */
quint16 DFontSizeManager::fontPixelSize(DFontSizeManager::SizeType type) const
{
    if (type >= NSizeTypes) {
        return 0;
    }

    return d->fontPixelSize[type] + d->fontPixelSizeDiff;
}

/*!
 * \~chinese \brief 设置字体像素大小
 * \~chinese \param[in] type 字体枚举类型
 * \~chinese \param[in] size 字体大小
 */
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

/*!
 * \~chinese \brief 设置字体的通用像素大小
 * \~chinese \param[in] size 预设计的字体像素的大小
 */
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
    return get(type, base.weight(), base);
}

/*!
 * \~chinese \brief 获取字体
 * \~chinese \param[in] type 字体的大小枚举
 * \~chinese \param[in] base 将改变大小的字体
 * \~chinese \return 返回设置字体大小后的字体
 */
const QFont DFontSizeManager::get(DFontSizeManager::SizeType type, int weight, const QFont &base) const
{
    QFont font = base;

    font.setPixelSize(fontPixelSize(type));
    font.setWeight(weight);

    return font;
}

int DFontSizeManager::fontPixelSize(const QFont &font)
{
    int px = font.pixelSize();

    if (px == -1) {
        px = qRound(std::floor(((font.pointSizeF() * font.d->dpi) / 72) * 100 + 0.5) / 100);
    }

    return px;
}

/*!
 * \~chinese \brief 构造函数
 */
DFontSizeManager::DFontSizeManager()
    : d(new DFontSizeManagerPrivate())
{

}

DWIDGET_END_NAMESPACE
