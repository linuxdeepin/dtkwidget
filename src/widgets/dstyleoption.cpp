// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#define private public
#include <QFont>
#undef private

#include "dstyleoption.h"

#include "dlineedit.h"
#include "private/dlineedit_p.h"
#include "dpalettehelper.h"

#include <QGuiApplication>
#include <qmath.h>
#include <private/qfont_p.h>

#include <cmath>

DWIDGET_BEGIN_NAMESPACE

/*!
  \class Dtk::Widget::DStyleOption
  \inmodule dtkwidget

  \brief dcxb插件通过DStyleOption来控制控件的样式.
  \brief DStyleOption is the base class of deepin style option, use for dxcb plugins.

  \a widget
 */

/*!
  \class Dtk::Widget::DStyleOptionButton
  \inmodule dtkwidget

  \brief Dtk 样式统一的按钮样式配置.
 */

/*!
  \enum Dtk::Widget::DStyleOptionButton::ButtonFeature
  \brief This enum extend types of features a Dtk Button can have.
  \brief Dtk 按钮扩展的样式.

  \value SuggestButton
  \brief Default Button.
  \brief 默认样式.

  \omitvalue WarningButton
  \omitvalue FloatingButton
  \omitvalue TitleBarButton
  \omitvalue CircleButton
 */

/*!
  \class Dtk::Widget::DStyleOptionLineEdit
  \inmodule dtkwidget

  \brief Dtk::Widget::DLineEdit使用的样式配置.
  \brief DStyleOptionLineEdit handle the style option of Dtk::Widget::DLineEdit.

  \a widget
 */

/*!
  \enum Dtk::Widget::DStyleOptionLineEdit::LineEditFeature
  \brief Dtk 行编辑控件的自定义样式.

  \value None
  \brief 不使用自定义样式，保持Qt默认样式.
  \brief Default none style.

  \value Alert
  \brief 警告状态样式.
  \brief Alert style.

  \value IconButton
  \brief 包含图标按钮的编辑框样式.
  \brief Edit with an icon.
 */

/*!
  \brief Init style with widget.
  \brief 控件初始化样式时调用，可以用于重写控件样式.

  \a widget
 */
void DStyleOption::init(QWidget *widget)
{
    init(static_cast<const QWidget*>(widget));
}

void DStyleOption::init(const QWidget *widget)
{
    dpalette = DPaletteHelper::instance()->palette(widget);
}

/*!
  \brief DStyleOptionSuggestButton::init set style option for Dtk::Widget::DSuggestButton.
  \brief 初始化Dtk::Widget::DSuggestButton样式，设置为 DStyleOptionSuggestButton::SuggestButton.

  \a widget
  \sa Dtk::Widget::DSuggestButton
 */
DStyleOptionButton::DStyleOptionButton()
    : QStyleOptionButton ()
    , DStyleOption ()
{
}

/*!
 * @brief DStyleOptionButton::DStyleOptionButton Custom copy constructor
 * @param other Source object
 */
DStyleOptionButton::DStyleOptionButton(const DStyleOptionButton &other)
    : QStyleOptionButton (other)
    , DStyleOption (other)
{
    // `dciIcon` broke abi, so we need to distinguish weather `other` has dciIcon.
    if (other.features & DStyleOptionButton::HasDciIcon)
        dciIcon = other.dciIcon;
}

/*!
 * @brief DStyleOptionButton::operator = Custom assignment constructor
 * @param other Source obejct
 * @return
 */
DStyleOptionButton &DStyleOptionButton::operator=(const DStyleOptionButton &other)
{
    // Set the value of the parent class member variable.
    this->QStyleOptionButton::operator=(other);
    this->DStyleOption::operator=(other);

    if (other.features & DStyleOptionButton::HasDciIcon)
        dciIcon = other.dciIcon;
    return *this;
}

void DStyleOptionButton::init(const QWidget *widget)
{
    DStyleOption::init(widget);
}

/*!
  \brief DStyleOptionSuggestButton::init set style option for Dtk::Widget::DLineEdit.
  \brief 根据Dtk::Widget::DLineEdit控件状态初始化样式.

  \a widget
  \sa Dtk::Widget::DLineEdit
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
    QObject guarder;

    void updateWidgetFont(DFontSizeManager *manager, DFontSizeManager::SizeType type)
    {
        for (QWidget *w : binderMap[type]) {
            w->setFont(manager->get(type, w->font()));
        }
    }

};

/*!
  \class Dtk::Widget::DFontSizeManager
  \inmodule dtkwidget
  \brief 字体大小设置的一个类,系统默认只设置T6.
 */

/*!
  \brief 创建一个字体管理的实例.

  \return 返回一个创建的 static DFontSizeManager 实例
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
  \enum Dtk::Widget::DFontSizeManager::SizeType
  DFontSizeManager::SizeType 定义了 DFontSizeManager 的系统字体的定义的大小; 而系统只会设置 T6 为系统默认的字体.

  \value T1
  系统级别为 T1 的字体大小, 默认是40 px
  \value T2
  系统级别为 T2 的字体大小, 默认是30 px
  \value T3
  系统级别为 T3 的字体大小, 默认是24 px
  \value T4
  系统级别为 T4 的字体大小, 默认是20 px
  \value T5
  系统级别为 T5 的字体大小, 默认是17 px
  \value T6
  系统级别为 T6 的字体大小, 默认是14 px
  \value T7
  系统级别为 T7 的字体大小, 默认是13 px
  \value T8
  系统级别为 T8 的字体大小, 默认是12 px
  \value T9
  系统级别为 T9 的字体大小, 默认是11 px
  \value T10
  系统级别为 T10 的字体大小, 默认是10 px

  \omitvalue NSizeTypes
 */

/*!
  \brief 将字体的大小枚举 SizeType 和控件 widget 进行绑定, 其控件的字体大小(随绑定的枚举的)对应值的改变而改变;
         系统自定义的绑定枚举值 T6, 若 T6 = 14px,  则其他枚举 T1 - T10 的数值,依次为:40, 30, 24, 20, 17, 14(T6), 13, 12, 11, 10;
         系统自定义的绑定枚举值 T6 改为 T6 = 20px, 则其他枚举 T1 - T10 的数值,依次为:46, 36, 30, 26, 23, 20(T6), 19, 18, 17, 16;
         即: 其对应的无论 T6 为何值, 其两个相邻的 T 值的差是定值: T(n) - T(n-1) == 定值
         而系统是只设置 T6 这以枚举, 作为基准.

  \a widget 将要绑定字体大小枚举数值的控件
  \a type 字体的枚举类型, 每一个枚举数值对应着一个字体像素大小
 */
void DFontSizeManager::bind(QWidget *widget, DFontSizeManager::SizeType type, int weight)
{
    unbind(widget);

    d->binderMap[type].append(widget);
    widget->setFont(get(type, weight, widget->font()));

    if (!widget->property("_d_dtk_fontSizeBind").toBool()){
        QObject::connect(widget, &QWidget::destroyed, &(d->guarder), [this, widget] {
            unbind(widget);
        });
        widget->setProperty("_d_dtk_fontSizeBind", true);
    }
}

/*!
  \brief 将字体大小枚举值和 widget 的绑定解除, 不跟随 "枚举对应的像素值" 而改变本控件的字体大小.

  \a widget 与 "字体枚举所对应的像素值" 的 (解除绑定的控件) 对象
 */
void DFontSizeManager::unbind(QWidget *widget)
{
    for (int i = 0; i < NSizeTypes; ++i) {
        d->binderMap[i].removeOne(widget);
    }
}

/*!
  \brief 获取字体像素的大小.

  \a type 字体枚举类型
  \return 返回字体像素的大小
 */
quint16 DFontSizeManager::fontPixelSize(DFontSizeManager::SizeType type) const
{
    if (type >= NSizeTypes) {
        return 0;
    }

    return d->fontPixelSize[type] + d->fontPixelSizeDiff;
}

/*!
  \brief 设置字体像素大小.

  \a type 字体枚举类型
  \a size 字体大小
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
  \brief 设置字体的通用像素大小.

  \a size 预设计的字体像素的大小
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
  \brief 获取字体.

  \a type 字体的大小枚举
  \a base 将改变大小的字体
  \return 返回设置字体大小后的字体
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
  \brief 构造函数.
 */
DFontSizeManager::DFontSizeManager()
    : d(new DFontSizeManagerPrivate())
{

}

DWIDGET_END_NAMESPACE
