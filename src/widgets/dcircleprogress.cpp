// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dcircleprogress.h"
#include "private/dcircleprogress_p.h"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE

DCircleProgressPrivate::DCircleProgressPrivate(DCircleProgress *q)
    : DObjectPrivate(q)
{
    m_topLabel.setAttribute(Qt::WA_TranslucentBackground);
    m_topLabel.setAlignment(Qt::AlignCenter);
    m_topLabel.setObjectName("TopLabel");
    m_topLabel.setAccessibleName("DCircleProgressTopLabel");
    m_bottomLabel.setAttribute(Qt::WA_TranslucentBackground);
    m_bottomLabel.setAlignment(Qt::AlignCenter);
    m_bottomLabel.setObjectName("BottomLabel");
    m_bottomLabel.setAccessibleName("DCircleProgressBottomLabel");
    m_bottomLabel.hide();

    QBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch();
    mainLayout->addWidget(&m_topLabel);
    mainLayout->addWidget(&m_bottomLabel);
    mainLayout->addStretch();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    q->setLayout(mainLayout);
}

void DCircleProgressPrivate::paint(QPainter *painter)
{
    Q_Q(const DCircleProgress);

    painter->setRenderHints(QPainter::Antialiasing);

    QPen pen(m_chunkColor);
    pen.setWidth(m_lineWidth);
    painter->setPen(pen);
    const QRect widgetRect = q->rect();
    QRect outerCircleRect = widgetRect;
    outerCircleRect.setWidth(outerCircleRect.width() - (m_lineWidth - 1) * 2);
    outerCircleRect.setHeight(outerCircleRect.height() - (m_lineWidth - 1) * 2);
    if (outerCircleRect.width() < outerCircleRect.height())
        outerCircleRect.setHeight(outerCircleRect.width());
    else
        outerCircleRect.setWidth(outerCircleRect.height());
    outerCircleRect.setTop((widgetRect.height() - outerCircleRect.height()) / 2);
    outerCircleRect.setLeft((widgetRect.width() - outerCircleRect.width()) / 2);

    const double percent = double(m_currentValue) / (m_maximumValue - m_minmumValue);
    const int splitPos = -percent * 16 * 360;
    painter->drawArc(outerCircleRect, 90 * 16, splitPos);
    pen.setColor(m_backgroundColor);
    painter->setPen(pen);
    painter->drawArc(outerCircleRect, 90 * 16, 16 * 360 + splitPos);
}

/*!
@~english
    @class Dtk::Widget::DCircleProgress
    @ingroup dtkwidget
    @brief Use DCircleProgress to create a circle progress display widget.
    DCircleProgress provide a circle progress widget with configurable foreground color chunkColor
    and background color backgroundColor. You can also set text for display status or for other purpose.
*/

DCircleProgress::DCircleProgress(QWidget *parent)
    : QWidget(parent),
      DObject(*new DCircleProgressPrivate(this))
{

}

/*!
@~english
  @brief Progress value of the progressbar.
  Maximum value is 100, minimum value is 0.
  @sa setValue()
 */
int DCircleProgress::value() const
{
    Q_D(const DCircleProgress);

    return d->m_currentValue;
}

/*!
@~english
  @brief Set the progress 
  @param[in] value Value of the progressbar.
  Maximum value is 100, minimum value is 0.
  @sa value()
 */
void DCircleProgress::setValue(int value)
{
    Q_D(DCircleProgress);
    if (d->m_currentValue == value){
        return;
    }
    d->m_currentValue = value;
    emit valueChanged(value);
    update();
}

/*!
@~english
  @brief Text display near the circle progressbar.
 */
const QString DCircleProgress::text() const
{
    Q_D(const DCircleProgress);

    return d->m_topLabel.text();
}

/*!
@~english
  @brief Set the text display near the circle progressbar.
  @param[in] text The text near the circle progressbar.
 */
void DCircleProgress::setText(const QString &text)
{
    Q_D(DCircleProgress);

    return d->m_topLabel.setText(text);
}

/*!
@~english
  @brief Background color of the circle progressbar.
  @sa setBackgroundColor()
 */
const QColor DCircleProgress::backgroundColor() const
{
    Q_D(const DCircleProgress);

    return d->m_backgroundColor;
}

/*!
@~english
  @brief Set the background color of the circle progressbar.
  @param[in] color Color of the circle progress
  @sa backgroundColor()
 */
void DCircleProgress::setBackgroundColor(const QColor &color)
{
    Q_D(DCircleProgress);

    d->m_backgroundColor = color;
}

/*!
@~english
  @brief Foreground color of the progressbar to indicate progress.
  @sa setChunkColor()
 */
const QColor DCircleProgress::chunkColor() const
{
    Q_D(const DCircleProgress);

    return d->m_chunkColor;
}

/*!
@~english
  @brief Set the foreground color of the progressbar to indicate progress.
  @param[in] color Color of the progressbar
  @sa chunkColor()
 */
void DCircleProgress::setChunkColor(const QColor &color)
{
    Q_D(DCircleProgress);

    d->m_chunkColor = color;
}

/*!
@~english
  @brief circle line width of the circle progressbar.
  @sa setLineWidth()
 */
int DCircleProgress::lineWidth() const
{
    Q_D(const DCircleProgress);

    return d->m_lineWidth;
}

/*!
@~english
  @brief Set the circle line width of the circle progressbar.
  @param[in] width Width of the circle progressbar.
  @sa setLineWidth()
 */
void DCircleProgress::setLineWidth(const int width)
{
    Q_D(DCircleProgress);

    d->m_lineWidth = width;
}

/*!
@~english
  @brief DCircleProgress::topLabel
  @return 返回环形进度条顶部文字Label
 */
QLabel *DCircleProgress::topLabel()
{
    Q_D(DCircleProgress);

    return &d->m_topLabel;
}

/*!
@~english
  @brief DCircleProgress::bottomLabel
  @return 返回环形进度条底部文字Label
 */
QLabel *DCircleProgress::bottomLabel()
{
    Q_D(DCircleProgress);

    return &d->m_bottomLabel;
}

void DCircleProgress::paintEvent(QPaintEvent *e)
{
    Q_D(DCircleProgress);

    QPainter p(this);
    d->paint(&p);

    QWidget::paintEvent(e);
}

void DCircleProgress::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e)

    Q_EMIT clicked();
}
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
void DCircleProgress::enterEvent(QEnterEvent *e)
#else
void DCircleProgress::enterEvent(QEvent *e)
#endif
{
    Q_EMIT mouseEntered();

    QWidget::enterEvent(e);
}

void DCircleProgress::leaveEvent(QEvent *e)
{
    Q_EMIT mouseLeaved();

    QWidget::leaveEvent(e);
}

/*!
  @fn void Dtk::Widget::DCircleProgress::clicked()
  This signal is emitted when the user clicks the DCircleProgress widget.
*/

/*!
  @fn void Dtk::Widget::DCircleProgress::mouseEntered()
  This signal is emitted when the user's mouse pointer entered the DCircleProgress widget.
*/

/*!
  @fn void Dtk::Widget::DCircleProgress::mouseLeaved()
  This signal is emitted when the user's mouse pointer leaved the DCircleProgress widget.
*/
