/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QTimer>
#include <QStyle>
#include <QStyleOptionSlider>

#include "dslider.h"
#include "dthememanager.h"

DWIDGET_BEGIN_NAMESPACE

static const int CustomDrawingLeftPadding = 10;
static const int CustomDrawingRightPadding = 10;
static const int CustomDrawingScaleHeight = 6;

// TODO: optimize for better vertical slider support
class DSliderPrivate
{
    DSliderPrivate(DSlider *q):
        m_hoverShowValue(false),
        m_handleHovering(false),
        m_hoverTimout(false),
        m_hoverShowValueInterval(-1),
        q_ptr(q)
    {
        m_hoverTimer.setSingleShot(true);
    }

    int m_handleType = DSlider::RoundHandle;

    QString m_leftTip;
    QString m_rightTip;
    QList<int> m_scales;

    QColor m_tipColor = Qt::black;
    QColor m_scaleColor = Qt::black;
    QColor m_hoverValueColor;
    bool m_hoverShowValue;
    bool m_handleHovering;
    bool m_hoverTimout;
    bool mousePressed = false;
    int clickOffset = 0;
    int m_hoverShowValueInterval;
    QTimer m_hoverTimer;

    int getScalePosition(int value);
    int pixelPosToRangeValue(int pos) const;

    inline int pick(const QPoint &pt) const
    {
        Q_Q(const DSlider);
        return q->orientation() == Qt::Horizontal ? pt.x() : pt.y();
    }

    DSlider *q_ptr;
    Q_DECLARE_PUBLIC(DSlider)
};


/*!
 * \~english \class DSlider
 * \~english \brief The DSlider class provides an extended QSlider widget.
 *
 * \~english DSlider has optional vernier type handle type, scale marks that can be placed at
 * \~english any position and corresponding scale tips.
 *
 * \~english DSlider also supports showing texts, value of the slider, when
 * \~english user hovers on the slider handle.
 *
 * \~english \note This class relies on dstyle Qt style plugin to work properly.
 *
 * \~english \note This class is not working very well while orientation is set to vertical.
 */

/*!
 * \~chinese \class DSlider
 * \~chinese \brief DSlider 是基于 QSlider 之上扩展的控件。
 *
 * \~chinese 当用户将鼠标悬停在滑块手柄上时，DSlider 还支持显示文本，滑块的值。
 *
 * \~chinese \note 这个类依赖于 dstyle Qt 样式插件才能正常工作。
 *
 * \~chinese \note 当方向设置垂直时，不能很好的工作。
 */

/*!
 * \~chinese \enum HandleType
 * \~chinese \brief DSlider可以使用的句柄类型。
 * \~chinese \var HandleType SharpHandler 箭头滑块
 * \~chinese \var HandleType RoundHandle 圆形滑块
 */

/*!
 * \~english \brief DSlider::DSlider constructs an instance of DSlider.
 * \~english \param parent is passed to QSlider constructor.
 */

/*!
 * \~chinese \brief DSlider 构造函数
 * \~chinese \param parent
 */
DSlider::DSlider(QWidget *parent) :
    QSlider(parent),
    d_ptr(new DSliderPrivate(this))
{
    init();
}

/*!
 * \~english \brief DSlider::DSlider constructs an instance of DSlider.
 * \~english \param orientation is passed to QSlider constructor.
 * \~english \param parent is passed to QSldier constructor.
 */

/*!
 * \~chinese \brief DSlider 的重载构造函数
 * \~chinese \param orientation 初始化方向
 * \~chinese \param parent
 */
DSlider::DSlider(Qt::Orientation orientation, QWidget *parent) :
    QSlider(orientation, parent),
    d_ptr(new DSliderPrivate(this))
{
    init();
}

DSlider::~DSlider()
{
    delete d_ptr;
}

/*!
 * \~english \property DSlider::handleType
 * \~english \brief This property holds the handle type used by this slider.
 *
 * \~english Default value is DSlider::RoundHandle.
 */

/*!
 * \~chinese \property DSlider::handleType
 * \~chinese \brief 滑块句柄类型
 *
 * \~chinese 默认值是 DSlider::RoundHandle.
 */
int DSlider::handleType() const
{
    Q_D(const DSlider);

    return d->m_handleType;
}

/*!
 * \~chinese \brief 设置滑块句柄类型
 */
void DSlider::setHandleType(HandleType handleType)
{
    Q_D(DSlider);

    d->m_handleType = handleType;
}

/*!
 * \~chinese \brief 显示在滑块右侧的文本
 */
QString DSlider::rightTip() const
{
    Q_D(const DSlider);

    return d->m_rightTip;
}

/*!
 * \~chinese \brief 设置滑块右侧的文本
 */
void DSlider::setRightTip(const QString &rightTip)
{
    Q_D(DSlider);

    if (d->m_rightTip == rightTip) {
        return;
    }

    if (d->m_rightTip.isEmpty() || rightTip.isEmpty()) {
        d->m_rightTip = rightTip;
        updateGeometry();
    } else {
        d->m_rightTip = rightTip;
    }

    repaint();
}

/*!
 * \~english \property DSlider::leftTip
 * \~english \brief This property holds the text to be display on the left side of the slider.
 *
 * \~english DSlider::leftTip and DSlider::rightTip are used to help the user understand
 * \~english how the slider works, for example: leftTip set to Low and rightTip set to High
 * \~english tells the user that drag the handle to left lower the value and vice versa.
 */

/*!
 * \~chinese \brief 显示在滑块左侧的文本
 */
QString DSlider::leftTip() const
{
    Q_D(const DSlider);

    return d->m_leftTip;
}

/*!
 * \~chinese \brief 设置滑块左侧的文本
 */
void DSlider::setLeftTip(const QString &leftTip)
{
    Q_D(DSlider);

    if (d->m_leftTip == leftTip) {
        return;
    }

    if (d->m_leftTip.isEmpty() || leftTip.isEmpty()) {
        d->m_leftTip = leftTip;
        updateGeometry();
    } else {
        d->m_leftTip = leftTip;
    }

    repaint();
}

/*!
 * \~english \property DSlider::scaleColor
 * \~english \brief This property holds the color to paint the scale marks.
 */

/*!
 * \~chinese \brief 刻度线颜色
 */
QColor DSlider::scaleColor() const
{
    Q_D(const DSlider);

    return d->m_scaleColor;
}

/*!
 * \~chinese \brief 设置刻度线颜色
 */
void DSlider::setScaleColor(const QColor &scaleColor)
{
    Q_D(DSlider);

    d->m_scaleColor = scaleColor;

    repaint();
}

/*!
 * \~english \property DSlider::tipColor
 * \~english \brief This property holds the color to paint the scale tips.
 */
QColor DSlider::tipColor() const
{
    Q_D(const DSlider);

    return d->m_tipColor;
}

void DSlider::setTipColor(const QColor &tipColor)
{
    Q_D(DSlider);

    d->m_tipColor = tipColor;

    repaint();
}

/*!
 * \~english \brief DSlider::addScale adds a scale mark on the slider.
 * \~english \param value is the position that the scale mark to be added.
 *
 * \~english The scale added can be removed using DSlider::removeScale passing the
 * \~english value as the parameter.
 */

/*!
 * \~chinese \brief 添加刻度标记
 * \~chinese \param value 是要添加刻度标记的位置
 *
 * \~chinese 可以使用 DSlider::removeScale 删除刻度标记。
 */
void DSlider::addScale(int value)
{
    Q_D(DSlider);

    d->m_scales.append(value);

    if (d->m_scales.count() == 1) {
        updateGeometry();
    }

    repaint();
}

/*!
 * \~english \brief DSlider::removeScale removes a scale mark on the slider.
 * \~english \param value is the value used to add the scale mark.
 */

/*!
 * \~english \brief 删除滑块上的刻度标记。
 */
void DSlider::removeScale(int value)
{
    Q_D(DSlider);

    d->m_scales.removeOne(value);

    if (d->m_scales.isEmpty()) {
        updateGeometry();
    }

    repaint();
}

void DSlider::mousePressEvent(QMouseEvent *event)
{
    QAbstractSlider::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        if (orientation() == Qt::Vertical) {
            setValue(minimum() + ((maximum() - minimum()) * (height() - event->y())) / height()) ;
        } else {
            // FIXME
            // the value 10 is specified in DSlider.theme, it's ugly here, but I don't have any
            // good idea for now, maybe someone can help.
            setValue(minimum() + ((maximum() - minimum()) * (event->x() - 10)) / (width() - 10 - 10)) ;
        }

        event->accept();

        Q_D(DSlider);

        QStyleOptionSlider opt;
        initStyleOption(&opt);
        setRepeatAction(SliderNoAction);
        QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

        d->clickOffset = d->pick(event->pos() - sr.topLeft());
        d->mousePressed = true;

        Q_EMIT sliderPressed();
    }
}

void DSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QAbstractSlider::mouseReleaseEvent(event);

    if(event->button() == Qt::LeftButton) {
        d_func()->mousePressed = false;

        Q_EMIT sliderReleased();
    }
}

void DSlider::paintEvent(QPaintEvent *event)
{
    Q_D(DSlider);

    QPainter painter;
    painter.begin(this);

    // draw tips
    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);

    QPen pen = painter.pen();
    pen.setColor(d->m_tipColor);
    painter.setPen(pen);

    QRect tmp = rect().adjusted(CustomDrawingLeftPadding - 5, 0, -CustomDrawingRightPadding + 5, 0);

    QTextOption leftBottomOption;
    leftBottomOption.setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    painter.drawText(tmp, d->m_leftTip, leftBottomOption);

    QTextOption rightBottomOption;
    rightBottomOption.setAlignment(Qt::AlignRight | Qt::AlignBottom);
    painter.drawText(tmp, d->m_rightTip, rightBottomOption);

    // draw scales
    pen.setColor(d->m_scaleColor);
    painter.setPen(pen);

    Q_FOREACH(int scale, d->m_scales) {
        int x = d->getScalePosition(scale);
        int y = height() - 8;
        painter.drawLine(x, y, x, y - CustomDrawingScaleHeight);
    }

    if (d->m_handleHovering && !d->m_hoverTimout) {
        QString str = QString::number(value());
        int x = d->getScalePosition(value()) - painter.fontMetrics().width(str) / 2.0;
        painter.setPen(d->m_hoverValueColor);
        painter.drawText(x, 10, str);
    }

    painter.end();

    QSlider::paintEvent(event);
}

void DSlider::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(DSlider);

    if(d->mousePressed) {
        int newPosition = d->pixelPosToRangeValue(d->pick(event->pos()) - d->clickOffset);
        setSliderPosition(newPosition);

        Q_EMIT sliderMoved(newPosition);
    }

    if (!d->m_hoverShowValue) {
        return;
    }

    QPoint pos = event->pos();
    QRect rect(d->getScalePosition(value()) - CustomDrawingLeftPadding, 10, 20, 20);
    if (d->m_handleHovering) {
        d->m_handleHovering = rect.contains(pos);
    } else {
        d->m_handleHovering = rect.contains(pos);
        if (d->m_handleHovering) {
            d->m_hoverTimout = false;
            if (d->m_hoverShowValueInterval > 0) {
                d->m_hoverTimer.start(d->m_hoverShowValueInterval);
            }
        }
    }

    update();
}

void DSlider::hoverTimout()
{
    Q_D(DSlider);

    d->m_hoverTimout = true;
    update();
}

void DSlider::init()
{
    Q_D(DSlider);

    DThemeManager::registerWidget(this);

    setMouseTracking(true);
    connect(&d->m_hoverTimer, &QTimer::timeout, this, &DSlider::hoverTimout);
}

QSize DSlider::sizeHint() const
{
    Q_D(const DSlider);

    QSize size = QSlider::sizeHint();
    if (!d->m_leftTip.isEmpty() || !d->m_rightTip.isEmpty() || !d->m_scales.isEmpty()) {
        size.setHeight(size.height() + 25);
    } else {
        if (d->m_hoverShowValue) {
            size.setHeight(size.height() + 25);
        } else {
            size.setHeight(size.height() + 3);
        }
    }

    return size;
}

/*!
 * \~english \property DSlider::hoverShowValue
 * \~english \brief This property holds whether the value should be displayed when user
 * \~english hovers on the slider handle.
 */

/*!
 * \~chinese \brief 是否显示鼠标悬停在滑块上的值。
 */
bool DSlider::hoverShowValue() const
{
    Q_D(const DSlider);

    return d->m_hoverShowValue;
}

/*!
 * \~english \property DSlider::hoverValueColor
 * \~english \brief This property holds the color to be used to display the texts.
 */

/*!
 * \~chinese \brief 鼠标悬停在滑块上提示颜色。
 */
QColor DSlider::hoverValueColor() const
{
    Q_D(const DSlider);

    return d->m_hoverValueColor;
}

/*!
 * \~english \property DSlider::hoverShowValueInterval
 * \~english \brief This property holds the interval of how long the value will be displayed
 * \~english if the user keeps hovering on the slider handle.
 */

/*!
 * \~chinese \brief 鼠标悬停在滑块显示值的时间间隔。
 */
int DSlider::hoverShowValueInterval() const
{
    Q_D(const DSlider);

    return d->m_hoverShowValueInterval;
}

/*!
 * \~chinese \brief 设置显示值的时间间隔。
 */
void DSlider::setHoverShowValue(bool hoverShowValue)
{
    Q_D(DSlider);

    if (d->m_hoverShowValue == hoverShowValue) {
        return;
    }

    d->m_hoverShowValue = hoverShowValue;
    d->m_handleHovering &= hoverShowValue;

    updateGeometry();
    repaint();
}

void DSlider::setHoverValueColor(QColor hoverValueColor)
{
    Q_D(DSlider);

    d->m_hoverValueColor = hoverValueColor;
}

void DSlider::setHoverShowValueInterval(int hoverShowValueInterval)
{
    Q_D(DSlider);

    d->m_hoverShowValueInterval = hoverShowValueInterval;
}

DSlider::DSlider(DSliderPrivate &d): d_ptr(&d)
{
    init();
}

int DSliderPrivate::getScalePosition(int value)
{
    Q_Q(DSlider);

    float valueRange = q->maximum() - q->minimum();
    float posRange = q->width() - CustomDrawingLeftPadding - CustomDrawingRightPadding;
    return CustomDrawingLeftPadding + (value - q->minimum()) * posRange / valueRange;
}

int DSliderPrivate::pixelPosToRangeValue(int pos) const
{
    Q_Q(const DSlider);

    QStyleOptionSlider opt;
    q->initStyleOption(&opt);
    QRect gr = q->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, q);
    QRect sr = q->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, q);
    int sliderMin, sliderMax, sliderLength;

    if (q->orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    } else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    return QStyle::sliderValueFromPosition(q->minimum(), q->maximum(), pos - sliderMin,
                                           sliderMax - sliderMin, opt.upsideDown);
}

DWIDGET_END_NAMESPACE
