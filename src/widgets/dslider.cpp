// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dslider.h"
#include "private/dslider_p.h"

#include <QPainter>
#include <QEvent>
#include <QKeyEvent>
#include <QtMath>
#include <QDebug>
#include <DStyle>

#include <DPaletteHelper>
#include <DFontSizeManager>
#include <DGuiApplicationHelper>

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
  @~english
  \class Dtk::Widget::DSlider
  \inmodule dtkwidget
  \brief DSlider A slider that aggregates QSLIDER.

  DSlider Provides icon functions on both sides of the slider
  DSlider Provide the size and scale identification of the slider
 */

/*!
  @~english
  \fn void DSlider::valueChanged(int value)
  \brief The signal will be sent when the value of the Slider Value property is changed.
 */
/*!
  @~english
  \fn void DSlider::sliderMoved(int position)
  \brief The signal will be sent when the slider drags.
  \param[in] position The position of the pointer dragged by Slider.
 */
/*!
  @~english
  \fn void DSlider::sliderReleased()
  \brief The signal will be sent when the slider is loosened.
 */
/*!
  @~english
  \fn void DSlider::rangeChanged(int min, int max)
  \brief The signal will be sent when the value of the Range attribute is changed
  \param[in] min The minimum value of Range
  \param[in] max is the maximum value of Range
 */
/*!
  @~english
  \fn void DSlider::actionTriggered(int action)
  \brief The signal is sent when the slider action triggers
 */
/*!
  @~english
  \fn void DSlider::iconClicked(SliderIcons icon, bool checked)
  \brief The signal will be sent when Iconbutton is clicked.
  \param[in] icon Express the position where the button is clicked, 
  and Checked indicates whether the button is selected.
 */

/*!
  @~english
  \brief DSLider's constructor.
  \param[in] orientation Slip direction.
  \param[in] parent The parameters are sent to the QWIDGET constructor.
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
  @~english
  \brief Event filter function
  \details This function currently only handle the mouse roller incident
  \param[in] watched Subsidined control
  \param[in] e Corresponding event pointer
 */
bool DSlider::eventFilter(QObject *watched, QEvent *e)
{
    Q_D(DSlider);

    if ((watched == d->slider) && (e->type() == QEvent::Wheel)) {
        return !d->mouseWheelEnabled;
    }

    if (e->type() == QEvent::MouseButtonRelease) {
        d->needFocus = watched == d->slider;
    }

    if (d->needFocus && (e->type() == QEvent::KeyPress)) {
        if (auto keyEvent = dynamic_cast<QKeyEvent *>(e)) {
            if ((keyEvent->key() == Qt::Key_Up
              || keyEvent->key() == Qt::Key_Down
              || keyEvent->key() == Qt::Key_Left
              || keyEvent->key() == Qt::Key_Right))
            {
                d->needFocus = false;
                d->slider->setFocus();
            }
        }
    }

    return QWidget::eventFilter(watched, e);
}

/*!
  @~english
  \brief orientation of slider
  \return Return to the direction of the current slider
  \sa QSlider::orientation()
 */
Qt::Orientation DSlider::orientation() const
{
    D_DC(DSlider);
    return d->slider->orientation();
}
/*!
  @~english
  \brief return QSLider object.
  \details If DSLider does not meet the needs of the input box, use this function to throw it.
  \return QSlider Object.
 */
QSlider *DSlider::slider()
{
    Q_D(DSlider);
    return d->slider;
}

/*!
  @~english
  \brief Set the left icon on the left side of the slider.
  \param[in] left Left icon
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
  @~english
  \brief Set the right icon of slider.
  \param[in] right Right icon
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
  @~english
  \brief Set the slider icon size.
  \param[in] size Icon size
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
  @~english
  \brief Set the minimum value of sliding range.
  \param[in] min Slide the minimum value.
  \sa QSlider::setMinimum() DSlider::minimum()
 */
void DSlider::setMinimum(int min)
{
    D_D(DSlider);
    d->slider->setMinimum(min);
}

/*!
  @~english
  \brief The minimum value of sliding range.
  \return Return to the minimum value of sliding range.
  \sa QSlider::minimum() DSlider::setMinimum()
 */
int DSlider::minimum() const
{
    D_DC(DSlider);
    return d->slider->minimum();
}

/*!
  @~english
  \brief Set the current value of slider
  \param[in] value The current value of the slider.
  \sa QSlider::setValue()
 */
void DSlider::setValue(int value)
{
    D_D(DSlider);
    d->slider->setValue(value);
}

/*!
  @~english
  \brief DSlider::value
  \sa QSlider::value()
 */
int DSlider::value() const
{
    D_DC(DSlider);
    return d->slider->value();
}

/*!
  @~english
  \brief Set the size of the page single step.
  \details use the kernel Pageup or PageDown, the slider slides the single step size.
  \param[in] pageStep One-step size.
  \sa QSlider::setPageStep()
 */
void DSlider::setPageStep(int pageStep)
{
    D_D(DSlider);
    d->slider->setPageStep(pageStep);
}

/*!
  @~english
  \brief Back to the page single step size
  \return The one-step value of the page.
  \sa QSlider::pageStep() DSlider::setPageStep()
 */
int DSlider::pageStep() const
{
    D_DC(DSlider);
    return d->slider->pageStep();
}

/*!
  @~english
  \brief Set the maximum value of sliding range
  \param[in] max The maximum value of sliding range.
  \sa QSlider::setMaximum() DSlider::maximum()
 */
void DSlider::setMaximum(int max)
{
    D_D(DSlider);
    d->slider->setMaximum(max);
}

/*!
  @~english
  \brief Back to the maximum value of sliding range
  \return The maximum value of sliding range
  \sa QSlider::maximum
 */
int DSlider::maximum() const
{
    D_DC(DSlider);
    return d->slider->maximum();
}

/*!
  @~english
  \brief Set the scale on the left side of the slider. 
  \details According to the quantity of QStringList，draw number of scales，Draw the scale identification:
  The slider is horizontal, and the scale is above the slider; 
  the slider is vertical and the scale is on the left side of the slider.
  \param[in] info Scale identification
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
  @~english
  \brief Set the scale value of the right side of the slider. 
  \details According to the number of QStringList, the number of drawing scale, draw scale identification:
  The slider is horizontal, and the scale is below the slider; 
  the slider is vertical and the scale is on the right side of the slider.
  \param[in] info Scale identification
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
  @~english
  \brief Set the scale value above the slider
  \param[in] info Scale logo.
  \sa DSlider::setLeftTicks()
 */
void DSlider::setAboveTicks(const QStringList &info)
{
    setLeftTicks(info);
}

/*!
  @~english
  \brief Set the scale value below the slider
  \param[in] info Scale logo.
  \sa DSlider::setRightTicks()
 */
void DSlider::setBelowTicks(const QStringList &info)
{
    setRightTicks(info);
}

/*!
  @~english
  \brief Set the bilateral scales (no scale value).

  \details For example: The dslider of the sound volume requires a scale in the place of value = 100 without displaying other scale values (and the actual volume value can exceed 100)
  Other: Set the specified value line (SetmarkPositions) and setting scale line+scale value (Setbelowticks) are two independent and do not interfere with each other. If they are used at the same time, they will also draw their own lines at the same time;
  The other two sequential order does not matter.

  \param[in] list The value of the bilateral scale line.
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
  @~english
  \brief Set whether the mouse roller is turned on.
  \details After turning on the mouse wheel, the user can control the sliding slide through the mouse roller.
  \param[in] enabled Whether to turn on the mouse wheel
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
  @~english
  \brief For the creation of bubbles, the bubbles will follow the slider.
  \param[in] value Non-empty: open bubble
  \param[in] value empty: Turn off bubbles (destroyed)
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
  @~english
  \brief the slider's mark position. 
  \details Get the slider extension currently facing.
  \return The orientation of the slider
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
  @~english
  \brief The size strategy of sliding bars

  \details This function will return the recommended size recommended by the sliding bar, if
  There is no layout of sliding bars, this size will be an invalid value, if
  There is a layout that will return to the recommendation size under the layout.

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
  @~english
  \brief Set the slider whether to display it.
  \param[in] b Show the slider for True, otherwise the slider will be hidden.
  By default, the slider is the display state.
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
  @~english
  \brief Get the state of whether the slider is displayed.
  \return Whether the back slider is displayed
 */
bool DSlider::handleVisible() const
{
    D_DC(DSlider);
    return d->handleVisible;
}

/*!
  @~english
  \brief This function is used to set the sliding color that has been slid over.
  \details The sliding skating of ordinary DSLider is filled with active color. DSLider that calls SetxXXTICKS defaults to active color filling by default
  \param[in] enabled true Lustless，It can be used for the volume balance, etc. It does not need to be displayed, the position of FALSE slipping (such as the left) is a high -bright color display, such as adjusting brightness, etc. (default)
  By default, the attribute is False.
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
    , needFocus(false)
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
    qApp->installEventFilter(q);
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
            max = qMax(max, fontMetrics().horizontalAdvance(scaleInfo[i]));
        }
        size.setWidth(max + fontSize / 2);
    }

    return size;
}

/*!
  @~english
  \brief SliderStrip::setScaleInfo Set the display scales and scale values
  \param[in] scaleInfo Display scale value
  \param[in] tickPosition Display direction enumeration value
 */
void SliderStrip::setScaleInfo(QStringList scaleInfo, QSlider::TickPosition tickPosition)
{
    this->scaleInfo = scaleInfo;
    this->tick = tickPosition;
}

/*!
  @~english
  \brief SliderStrip::setMarkList Set the display scales (no display scale value)
  \param[in] list List of the displayed scale
  \param[in] tickPosition Display direction enumeration value
 */
void SliderStrip::setMarkList(QList<int> list, QSlider::TickPosition tickPosition)
{
    this->list = list;
    this->tick = tickPosition;
}

/*!
  @~english
  \brief SliderStrip::getList Return to List of the scale line
  \return List of scale line
 */
QList<int> SliderStrip::getList()
{
    return this->list;
}

/*!
  @~english
  \brief SliderStrip::getScaleInfo Return to the List of Dialogue
  \return List
 */
QStringList SliderStrip::getScaleInfo()
{
    return this->scaleInfo;
}

/*!
  @~english
  \brief SliderStrip::paintEvent
  \sa QWidget::paintEvent()
 */
void SliderStrip::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter pa(this);
    auto dpa = DPaletteHelper::instance()->palette(this);
    QPen penLine = QPen(DGuiApplicationHelper::adjustColor(dpa.color(DPalette::ObviousBackground), 0, 0, 0, 0, 0, 0, +20), 1);
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

    auto elidedText = [this](const QString &text, int width) {
        return fontMetrics().elidedText(text, Qt::ElideRight, width);
    };

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
        pa.drawText(QRectF(endX, textPos, width, height - tickSize), Qt::AlignLeft, elidedText(scaleInfo[0], width / paragraph));

        for (int i = 1; i < paragraph - 1; i++) {
            startX += average;
            endX = startX;
            pa.setPen(penLine);
            pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
            pa.setPen(penNumber);
            pa.drawText(QRectF(endX - width / 2, textPos, width, height - tickSize), Qt::AlignHCenter, elidedText(scaleInfo[i], width / paragraph));
        }

        if (paragraph > 1) {
            startX += average;
            endX = startX;
            pa.setPen(penLine);
            pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
            pa.setPen(penNumber);
            pa.drawText(QRectF(endX - width, textPos, width, height - tickSize), Qt::AlignRight, elidedText(scaleInfo[paragraph - 1], width / paragraph));
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
        pa.drawText(QRectF(textPos, endY - average / 2 + offsetSize / 2, width - tickSize, average), text_flags, elidedText(scaleInfo[0], width - tickSize));

        for (int i = 1; i < paragraph - 1; i++) {
            startY += average;
            endY = startY;
            pa.setPen(penLine);
            pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
            pa.setPen(penNumber);
            pa.drawText(QRectF(textPos, endY - average / 2, width - tickSize, average), text_flags, elidedText(scaleInfo[i], width - tickSize));
        }

        if (paragraph > 1) {
            startY += average;
            endY = startY;
            pa.setPen(penLine);
            pa.drawLine(QPointF(startX, startY), QPointF(endX, endY));
            pa.setPen(penNumber);
            pa.drawText(QRectF(textPos, endY - average / 2 - offsetSize / 2, width - tickSize, average), text_flags, elidedText(scaleInfo[paragraph - 1], width - tickSize));
        }
    }
}

/*!
  @~english
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
