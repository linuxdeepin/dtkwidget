/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     wp <wangpeng_cm@deepin.com>
 *
 * Maintainer: wp <wangpeng_cm@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dslider.h"
#include "private/dslider_p.h"

#include <QPainter>
#include <QEvent>
#include <QtMath>
#include <QDebug>
#include <DStyle>

#include <DPaletteHelper>
#include <DFontSizeManager>

DWIDGET_BEGIN_NAMESPACE

class SliderStrip : public QWidget
{
public:
    SliderStrip(Qt::Orientation orie, QWidget *parent = nullptr)
        : QWidget(parent)
        , orient(orie)
    {
        DFontSizeManager::instance()->bind(this, DFontSizeManager::T7);
    }
    QSize sizeHint() const override;

    void setScaleInfo(QStringList scaleInfo, QSlider::TickPosition tickPosition);
    void setMarkList(QList<int> list, QSlider::TickPosition tickPosition);
    QList<int> getList();
    QStringList getScaleInfo();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *e) override;
private:
    void drawSCale();
private:
    Qt::Orientation orient;
    QSlider::TickPosition tick;
    QStringList scaleInfo;
    QList<int> list;
};

/*!
  \class Dtk::Widget::DSlider
  \inmodule dtkwidget
  \brief DSlider一个聚合 QSlider 的滑块.

  DSlider提供了在滑块两侧设置图标函数
  DSlider提供了滑块的刻度及刻度标识
 */

/*!
  \fn void DSlider::valueChanged(int value)
  \brief 信号会在 slider \a value 属性的值改变时被发送.
 */
/*!
  \fn void DSlider::sliderMoved(int position)
  \brief 信号会在 slider 拖动时被发送.

  \a position 为 slider 被拖动的指针的位置。
 */
/*!
  \fn void DSlider::sliderReleased()
  \brief 信号会在 slider 被松开时被发送.
 */
/*!
  \fn void DSlider::rangeChanged(int min, int max)
  \brief 信号会在 range 属性的值改变时被发送.

  \a min 为 range 的最小值， \a max 为 range 的最大值。
 */
/*!
  \fn void DSlider::actionTriggered(int action)
  \brief 信号会在 slider \a action 触发时被发送
 */
/*!
  \fn void DSlider::iconClicked(SliderIcons icon, bool checked)
  \brief 信号会在左右 iconbutton 被点击时被发送.

  \a icon 表示按钮被点击的位置，\a checked 表示按钮是否被选中。
 */

/*!
  \brief DSlider的构造函数.

  \a parent 参数被发送到 QWidget 构造函数。

  \sa Qt::Orientation
 */
DSlider::DSlider(Qt::Orientation orientation, QWidget *parent)
    : QWidget(parent)
    , DObject(*new DSliderPrivate(this))
{
    Q_D(DSlider);
    d->orientation = orientation;
    d->init();
}

DSlider::DSlider(DSliderPrivate &q, QWidget *parent)
    : QWidget(parent)
    , DObject(q)
{

}

bool DSlider::event(QEvent *e)
{
    D_D(DSlider);
    if (d->tipvalue && (e->type() == QEvent::Resize || e->type() == QEvent::LayoutRequest)) {
        d->updtateTool(value());
    }
    return QWidget::event(e);
}

/*!
  \brief 事件过滤器函数.
  \reimp

  此函数目前仅处理了鼠标滚轮事件
  \a watched 被监听的子控件，\a e 对应的事件指针。
 */
bool DSlider::eventFilter(QObject *watched, QEvent *e)
{
    Q_D(DSlider);

    if ((watched == d->slider) && (e->type() == QEvent::Wheel)) {
        return !d->mouseWheelEnabled;
    }
    return QWidget::eventFilter(watched, e);
}

/*!
  \brief 滑块方向

  \return 返回当前滑块的方向。

  \sa QSlider::orientation()
 */
Qt::Orientation DSlider::orientation() const
{
    D_DC(DSlider);
    return d->slider->orientation();
}
/*!
  \brief 返回 QSlider 对象.

  若 DSlider 不满足输入框的使用需求，请用此函数抛出的对象。
  \return QSlider 对象。
 */
QSlider *DSlider::slider()
{
    Q_D(DSlider);
    return d->slider;
}

/*!
  \brief 设置滑块左侧图标.
  \a left 左图标
 */
void DSlider::setLeftIcon(const QIcon &left)
{
    D_D(DSlider);

    if (d->leftIcon == nullptr) {
        d->leftIcon = new DIconButton(nullptr);
        d->leftIcon->setFlat(true);
        d->leftIcon->setFocusPolicy(Qt::NoFocus);
        connect(d->leftIcon, &DIconButton::clicked, this, [this](bool checked) {
            this->iconClicked(LeftIcon, checked);
        });

        if (orientation() == Qt::Horizontal) {
            d->layout->addWidget(d->leftIcon, 1, 0, Qt::AlignVCenter);
        } else {
            d->layout->addWidget(d->leftIcon, 0, 1, Qt::AlignHCenter);
        }

        if (d->iconSize.isValid())
            d->leftIcon->setIconSize(d->iconSize);
    }
    d->leftIcon->setIcon(left);
}

/*!
  \brief 设置滑块右图标.
  \a right 右图标
 */
void DSlider::setRightIcon(const QIcon &right)
{
    D_D(DSlider);

    if (d->rightIcon == nullptr) {
        d->rightIcon = new DIconButton(nullptr);
        d->rightIcon->setFlat(true);
        d->rightIcon->setFocusPolicy(Qt::NoFocus);
        if (orientation() == Qt::Horizontal) {
            d->layout->addWidget(d->rightIcon, 1, 2, Qt::AlignVCenter);
        } else {
            d->layout->addWidget(d->rightIcon, 2, 1, Qt::AlignHCenter);
        }
        connect(d->rightIcon, &DIconButton::clicked, this, [this](bool checked) {
            this->iconClicked(RightIcon, checked);
        });

        if (d->iconSize.isValid())
            d->rightIcon->setIconSize(d->iconSize);
    }
    d->rightIcon->setIcon(right);
}

/*!
  \brief 设置滑块图标大小.
  \a size 图标大小
 */
void DSlider::setIconSize(const QSize &size)
{
    D_D(DSlider);

    d->iconSize = size;

    if (d->leftIcon != nullptr) {
        d->leftIcon->setIconSize(size);
    }

    if (d->rightIcon != nullptr) {
        d->rightIcon->setIconSize(size);
    }
}

/*!
  \brief 设置滑动范围的最小值.

  \a min 滑动最小值。

  \sa QSlider::setMinimum() DSlider::minimum()
 */
void DSlider::setMinimum(int min)
{
    D_D(DSlider);
    d->slider->setMinimum(min);
}

/*!
  \brief 滑动范围的最小值.

  \return 返回滑动范围的最小值。

  \sa QSlider::minimum() DSlider::setMinimum()
 */
int DSlider::minimum() const
{
    D_DC(DSlider);
    return d->slider->minimum();
}

/*!
  \brief 设置滑块当前值

  \a value 滑块的当前值。

  \sa QSlider::setValue()
 */
void DSlider::setValue(int value)
{
    D_D(DSlider);
    d->slider->setValue(value);
}

/*!
  \brief DSlider::value
  \sa QSlider::value()
 */
int DSlider::value() const
{
    D_DC(DSlider);
    return d->slider->value();
}

/*!
  \brief 设置页面单步的大小

  使用按键 PageUp 或者 PageDown 时，滑块
  滑动的单步大小。

  \a pageStep 单步大小.

  \sa QSlider::setPageStep()
 */
void DSlider::setPageStep(int pageStep)
{
    D_D(DSlider);
    d->slider->setPageStep(pageStep);
}

/*!
  \brief 返回页面单步大小

  \return 页面单步大小的值。

  \sa QSlider::pageStep() DSlider::setPageStep()
 */
int DSlider::pageStep() const
{
    D_DC(DSlider);
    return d->slider->pageStep();
}

/*!
  \brief 设置滑动范围的最大值

  \a max 滑动范围的最大值。

  \sa QSlider::setMaximum() DSlider::maximum()
 */
void DSlider::setMaximum(int max)
{
    D_D(DSlider);
    d->slider->setMaximum(max);
}

/*!
  \brief 返回滑动范围的最大值

  \return 滑动范围的最大值

  \sa QSlider::maximum
 */
int DSlider::maximum() const
{
    D_DC(DSlider);
    return d->slider->maximum();
}

/*!
  \brief 设置滑块左侧的刻度值.

  根据 QStringList 数量，绘制刻度的个数，绘制刻度标识：
  滑块为水平,刻度在滑块上方；滑块为垂直,刻度在滑块左侧。

  \a info 刻度标识
 */
void DSlider::setLeftTicks(const QStringList &info)
{
    D_D(DSlider);

    if (info.isEmpty()) {
        if (d->left) {
            if (d->left->getList().empty()) {
                d->left->deleteLater();
                d->left = nullptr;
            }
        }

        return;
    }

    if (d->left == nullptr) {
        d->left = new SliderStrip(orientation());
        d->left->setScaleInfo(info, QSlider::TicksLeft);

        if (orientation() == Qt::Horizontal) {
            d->layout->addWidget(d->left, 0, 1, Qt::AlignTop);
        } else {
            d->layout->addWidget(d->left, 1, 0, Qt::AlignRight);
        }
    }
    d->left->setScaleInfo(info, QSlider::TicksLeft);
}

/*!
  \brief 设置滑块右侧的刻度值.

  根据 QStringList 数量，绘制刻度的个数，绘制刻度标识：
  滑块为水平,刻度在滑块下方；滑块为垂直,刻度在滑块右侧。

  \a info 刻度标识
 */
void DSlider::setRightTicks(const QStringList &info)
{
    D_D(DSlider);

    if (info.isEmpty()) {
        if (d->right) {
            if (d->right->getList().isEmpty()){
                d->right->deleteLater();
                d->right = nullptr;
            }
        }

        return;
    }

    if (d->right == nullptr) {
        d->right = new SliderStrip(orientation());
        d->right->setScaleInfo(info, QSlider::TicksRight);

        if (orientation() == Qt::Horizontal) {
            d->layout->addWidget(d->right, 2, 1, Qt::AlignTop);
        } else {
            d->layout->addWidget(d->right, 1, 2, Qt::AlignLeft);
        }
    }
    d->right->setScaleInfo(info, QSlider::TicksRight);
}

/*!
  \brief 设置滑块上方的刻度值

  \a info 刻度标识.

  \sa DSlider::setLeftTicks()
 */
void DSlider::setAboveTicks(const QStringList &info)
{
    setLeftTicks(info);
}

/*!
  \brief 设置滑块下方的刻度值

  \a info 刻度标识.

  \sa DSlider::setRightTicks()
 */
void DSlider::setBelowTicks(const QStringList &info)
{
    setRightTicks(info);
}

/*!
  \brief 设置显示双边的刻度线（不显示刻度值）.

  举例用途：比如调节音量的 DSlider ，需要在 value = 100 的地方标记一个刻度，而不需要显示其他的刻度值（并且实际音量值是可以超过 100 的）
  其他：设置指定数值的刻度线(setMarkPositions)和设置刻度线+刻度值(setBelowTicks)是两个相互独立的，且互不干扰，若是同时使用，也会同时绘画各自的线；
  另外两个的先后顺序也并没有关系.

  \a list 双边刻度线的值.
  
  \code 示例代码
  DSlider* slider = new DSlider(Qt::Horizontal, wTemp);
  QStringList list1;
  list1 << "0" << "25" << "50" << "75" << "100";
  slider->setBelowTicks(list1);
  QStringList list2;
  list2 << "0" << "25" << "50" << "75" << "100"";
  slider->setAboveTicks(list2);
  QList<int> list3;
  list3 << 0 << 30 << 80;
  slider->setMarkPositions(list3);
  slider->setMinimum(0);
  slider->setMaximum(100);
  \endcode
 */
void DSlider::setMarkPositions(QList<int> list)
{
    D_D(DSlider);

    if (list.isEmpty()) {
        if (d->left) {
            if (d->left->getScaleInfo().isEmpty()) {
                d->left->deleteLater();
                d->left = nullptr;
            }
        }

        if (d->right) {
            if (d->right->getScaleInfo().isEmpty()) {
                d->right->deleteLater();
                d->right = nullptr;
            }
        }

        return;
    }

    if (d->left == nullptr) {
        d->left = new SliderStrip(orientation());

        if (orientation() == Qt::Horizontal) {
            d->layout->addWidget(d->left, 0, 1, Qt::AlignTop);
        } else {
            d->layout->addWidget(d->left, 1, 0, Qt::AlignRight);
        }
    }

    if (d->right == nullptr) {
        d->right = new SliderStrip(orientation());

        if (orientation() == Qt::Horizontal) {
            d->layout->addWidget(d->right, 2, 1, Qt::AlignTop);
        } else {
            d->layout->addWidget(d->right, 1, 2, Qt::AlignLeft);
        }
    }

    d->left->setMarkList(list, QSlider::TicksLeft);
    d->right->setMarkList(list, QSlider::TicksRight);
}

/*!
  \brief 设置鼠标滚轮是否开启.

  开启鼠标滚轮后，用户可以通过鼠标滚轮来控制滑块的滑动。

  \a enabled 是否开启鼠标滚轮
 */
void DSlider::setMouseWheelEnabled(bool enabled)
{
    D_D(DSlider);
    d->mouseWheelEnabled = enabled;
}

void DSliderPrivate::updtateTool(int value)
{
    Q_Q(DSlider);

    if (!tipvalue)
        return;

    int min = q->minimum();
    int max = q->maximum();
    QStyleOptionSlider opt;
    const QRectF rectHandle = q->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, slider); //滑块

    double x = ((value - min) * 1.0 / (max -min)) * (slider->rect().width() - rectHandle.width());
    double y = slider->y() + slider->height();

    //x是以实际滑槽为起始坐标，而非DSlider为参考坐标（实际滑槽长度 == Slider长度 - 滑块长度）
    //气泡位置
    if (slider->invertedAppearance()) {
        tipvalue->move(slider->width() - x - (tipvalue->width() + rectHandle.width()) / 2.0 , y);
    } else {
        tipvalue->move(x + (rectHandle.width() - tipvalue->width()) / 2.0, y);
    }

    int shadowMarge = DStyle::pixelMetric(q->style(), DStyle::PM_FloatingWidgetShadowMargins) - 1;

    //气泡贴边
    if (tipvalue->x() < 0) {
         tipvalue->move(0 - shadowMarge, y);
    }

    if (tipvalue->x() + tipvalue->width() > slider->width()) {
        tipvalue->move(slider->width() - tipvalue->width() + shadowMarge, y);
    }

    tipvalue->raise();
    tipvalue->adjustSize();
}

/*!
  \brief 用于创建气泡，气泡将跟随滑块移动.

  \a value 非空开启气泡 \a value 空关闭气泡(销毁)
 */
void DSlider::setTipValue(const QString &value)
{
    D_D(DSlider);

    if (d->tipvalue == nullptr) {
        d->label = new DLabel(value);
        d->tipvalue = new DFloatingWidget;
        d->tipvalue->setWidget(d->label);
        d->tipvalue->setParent(this);
        d->label->setForegroundRole(DPalette::HighlightedText);
        d->label->setAlignment(Qt::AlignCenter);
        d->tipvalue->setBackgroundRole(QPalette::Highlight);
        d->tipvalue->setFramRadius(DStyle::pixelMetric(d->label->style(), DStyle::PM_FrameRadius));
        d->tipvalue->adjustSize();
        d->tipvalue->show();
    }

    if (value.isEmpty()) {
        d->tipvalue->deleteLater();
        d->tipvalue = nullptr;
        return;
    }

    d->label->setText(value);
    d->updtateTool(slider()->value());
}

/*!
  \brief 返回滑块的记号位置.

  获取滑块刻度当前朝向。

  \return 滑块刻度的朝向
  \sa QSlider::TickPosition
 */
QSlider::TickPosition DSlider::tickPosition() const
{
    D_DC(DSlider);

    if (d->left && d->right) {
        return QSlider::TicksBothSides;
    }

    if (d->left) {
        return QSlider::TicksLeft;
    }

    if (d->right) {
        return QSlider::TicksRight;
    }

    return QSlider::NoTicks;
}

/*!
  \brief 滑动条的大小策略

  这个函数会返回该滑动条推荐的大小，如果
  滑动条没有布局，这个大小将会是一个无效值，如果
  存在布局，将返回该布局下的推荐大小。

  \sa  QSlider::sizeHint
 */
QSize DSlider::sizeHint() const
{
    D_DC(DSlider);
    QSize size = QWidget::sizeHint();
    if (d->tipvalue && d->right == nullptr)
        size.setHeight(size.height() + d->tipvalue->height() * 2);
    return size;
}

/*!
  \brief 设置滑块是否显示.

  \a b 为 true 时滑块显示，否则滑块隐藏。
  默认地，滑块为显示状态。
 */
void DSlider::setHandleVisible(bool b)
{
    D_D(DSlider);

    if (d->handleVisible == b)
        return;

    d->handleVisible = b;
    update();
}

/*!
  \brief 获取滑块是否显示的状态.

  \return 返回滑块是否显示的状态
 */
bool DSlider::handleVisible() const
{
    D_DC(DSlider);
    return d->handleVisible;
}

/*!
  \brief 该函数用于设置滑槽是否禁用活动色填充已经滑过的滑槽.

  默认普通 DSlider 滑过的滑槽是活动色填充， 调用过 setXXXTicks 的 DSlider 则默认禁用活动色填充
  \a enabled true 无活动色，可用于音量平衡等不需要显示滑过的，false 滑过的位置(如左侧)是高亮色显示，如调节亮度等(默认)
  默认地，改属性为 false 。
 */
void DSlider::setEnabledAcrossStyle(bool enabled)
{
    slider()->setProperty("_d_dtk_sldier_across", enabled);
}

DSliderPrivate::DSliderPrivate(DSlider *q)
    : DObjectPrivate(q)
    , slider(nullptr)
    , handleVisible(true)
    , leftIcon(nullptr)
    , rightIcon(nullptr)
    , left(nullptr)
    , right(nullptr)
    , layout(nullptr)
    , tipvalue(nullptr)
    , label(nullptr)
    , mouseWheelEnabled(false)
{

}

void DSliderPrivate::init()
{
    D_Q(DSlider);

    slider = new SpecialSlider(orientation, q);
    q->connect(slider, &QSlider::valueChanged, q, &DSlider::valueChanged);
    q->connect(slider, &QSlider::sliderPressed, q, &DSlider::sliderPressed);
    q->connect(slider, &QSlider::sliderMoved, q, &DSlider::sliderMoved);
    q->connect(slider, &QSlider::sliderReleased, q, &DSlider::sliderReleased);
    q->connect(slider, &QSlider::rangeChanged, q, &DSlider::rangeChanged);
    q->connect(slider, &QSlider::actionTriggered, q, &DSlider::actionTriggered);

    layout = new QGridLayout(q);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(slider, 1, 1);

    if (q->orientation() == Qt::Horizontal) {
        q->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    } else {
        q->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    }
    slider->setTickPosition(QSlider::NoTicks);
    slider->installEventFilter(q);
    slider->setAccessibleName("DSliderSpecialSlider");
}

QSize SliderStrip::sizeHint() const
{
    QSize size = QWidget::sizeHint();
    int fontSize = fontMetrics().height();

    if (orient == Qt::Horizontal) {
        size.setHeight(fontSize * 1.5);
    } else {
        int max = 0;

        for (int i = 0; i < scaleInfo.count(); i++) {
            max = qMax(max, fontMetrics().width(scaleInfo[i]));
        }
        size.setWidth(max + fontSize / 2);
    }

    return size;
}

/*!
  \internal
  \brief SliderStrip::setScaleInfo 设置显示刻度线和刻度值
  \a scaleInfo 显示的刻度值
  \a tickPosition 显示的方向枚举值
 */
void SliderStrip::setScaleInfo(QStringList scaleInfo, QSlider::TickPosition tickPosition)
{
    this->scaleInfo = scaleInfo;
    this->tick = tickPosition;
}

/*!
  \internal
  \brief SliderStrip::setMarkList 设置显示刻度线（不显示刻度值）
  \a list 显示的刻度线的list
  \a tickPosition 显示的方向枚举值
 */
void SliderStrip::setMarkList(QList<int> list, QSlider::TickPosition tickPosition)
{
    this->list = list;
    this->tick = tickPosition;
}

/*!
  \internal
  \brief SliderStrip::getList 返回刻度线的 list
  \return 刻度线的 list
 */
QList<int> SliderStrip::getList()
{
    return this->list;
}

/*!
  \internal
  \brief SliderStrip::getScaleInfo 返回刻度值的 list
  \return 度值的 list
 */
QStringList SliderStrip::getScaleInfo()
{
    return this->scaleInfo;
}

/*!
  \internal
  \brief SliderStrip::paintEvent
  \sa QWidget::paintEvent()
 */
void SliderStrip::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter pa(this);
    auto dpa = DPaletteHelper::instance()->palette(this);
    QPen penLine = QPen(dpa.color(DPalette::ObviousBackground), 1);
    QPen penNumber = QPen(dpa.textTips(), 1);

    int tickSize = fontMetrics().height() / 2;
    int offsetSize = style()->pixelMetric(QStyle::PM_SliderLength, nullptr, this) / 2;
    int margin = DStyle::pixelMetric(style(), DStyle::PM_FocusBorderSpacing) + \
                    DStyle::pixelMetric(style(), DStyle::PM_FocusBorderSpacing);
    qreal startX = 0, startY = 0, endX = 0, endY = 0;
    int width = this->width(), height = this->height();    //slider宽高
    int paragraph = scaleInfo.count();    //刻度个数
    qreal average = 0;    //每一段的距离
    qreal textPos = 0;    //字体位置

    QSlider* slider = static_cast<DSlider *>(this->parent())->slider();
    for (int i = 0; i < list.count(); i++) {
        qreal percentage = (list.at(i) - slider->minimum()) * 1.0 / (slider->maximum() - slider->minimum());
        pa.setPen(penLine);

        if (orient == Qt::Horizontal) {
            qreal sliderX = percentage * width;
            if (tick == QSlider::TicksAbove)
                pa.drawLine(QPointF(sliderX + offsetSize, height), QPointF(sliderX + offsetSize,  height - tickSize));
            if (tick == QSlider::TicksBelow)
                pa.drawLine(QPointF(sliderX + offsetSize, 0), QPointF(sliderX + offsetSize, tickSize));
        } else {
            qreal sliderX = percentage * height;
            if (tick == QSlider::TicksLeft)
                pa.drawLine(QPointF(width - tickSize, height - (sliderX + offsetSize)), QPointF(width, height - (sliderX + offsetSize)));
            if (tick == QSlider::TicksRight)
                pa.drawLine(QPointF(0, height - (sliderX + offsetSize)), QPointF(tickSize, height - (sliderX + offsetSize)));
        }
    }

    if (scaleInfo.isEmpty())
        return;

    if (orient == Qt::Horizontal) {
        width -= 2 * offsetSize + margin * 2;
        startX += offsetSize + margin;
        endX = startX;
        average = width / static_cast<qreal>(paragraph - 1);

        if (tick == QSlider::TicksBelow) {
            endY = tickSize;
            textPos = endY;
        } else if (tick == QSlider::TicksAbove) {
            startY = height;
            endY = startY - tickSize;
        }

        pa.setPen(penLine);
        pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
        pa.setPen(penNumber);
        pa.drawText(QRectF(endX, textPos, width, height - tickSize), Qt::AlignLeft, scaleInfo[0]);

        for (int i = 1; i < paragraph - 1; i++) {
            startX += average;
            endX = startX;
            pa.setPen(penLine);
            pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
            pa.setPen(penNumber);
            pa.drawText(QRectF(endX - width / 2, textPos, width, height - tickSize), Qt::AlignHCenter, scaleInfo[i]);
        }

        if (paragraph > 1) {
            startX += average;
            endX = startX;
            pa.setPen(penLine);
            pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
            pa.setPen(penNumber);
            pa.drawText(QRectF(endX - width, textPos, width, height - tickSize), Qt::AlignRight, scaleInfo[paragraph - 1]);
        }
    } else {
        startY = offsetSize + margin;
        height -= offsetSize * 2 + margin * 2;
        endY = startY;
        average = height / static_cast<qreal>(paragraph - 1);
        Qt::Alignment text_flags = Qt::AlignVCenter;

        if (tick == QSlider::TicksLeft) {
            startX = width;
            endX = startX - tickSize;
            textPos = 0;
            text_flags |= Qt::AlignRight;
        } else if (tick == QSlider::TicksRight) {
            endX = tickSize;
            textPos = endX;
            text_flags |= Qt::AlignLeft;
        }

        pa.setPen(penLine);
        pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
        pa.setPen(penNumber);
        pa.drawText(QRectF(textPos, endY - average / 2 + offsetSize / 2, width - tickSize, average), text_flags, scaleInfo[0]);

        for (int i = 1; i < paragraph - 1; i++) {
            startY += average;
            endY = startY;
            pa.setPen(penLine);
            pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
            pa.setPen(penNumber);
            pa.drawText(QRectF(textPos, endY - average / 2, width - tickSize, average), text_flags, scaleInfo[i]);
        }

        if (paragraph > 1) {
            startY += average;
            endY = startY;
            pa.setPen(penLine);
            pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
            pa.setPen(penNumber);
            pa.drawText(QRectF(textPos, endY - average / 2 - offsetSize / 2, width - tickSize, average), text_flags, scaleInfo[paragraph - 1]);
        }
    }
}

/*!
  \internal
  \brief SliderStrip::event
  \sa QWidget::event()
 */
bool SliderStrip::event(QEvent *e)
{
    if (e->type() == QEvent::FontChange || e->type() == QEvent::ApplicationFontChange) {
        updateGeometry();
    }

    return QWidget::event(e);
}

DWIDGET_END_NAMESPACE
