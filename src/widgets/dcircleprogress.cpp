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
    mainLayout->setMargin(0);

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
    \class Dtk::Widget::DCircleProgress
    \inmodule dtkwidget

    \brief 可以使用 DCircleProgress 类快速创建环形进度条控件.
    \brief Use DCircleProgress to create a circle progress display widget.

    DCircleProgress 提供了一个可控制颜色并可以包含文字显示的环形进度条控件。可以快速创建底色为 backgroundColor
    ，进度颜色为 chunkColor 的环形进度条。
    DCircleProgress provide a circle progress widget with configurable foreground color chunkColor
    and background color backgroundColor. You can also set text for display status or for other purpose.
*/

DCircleProgress::DCircleProgress(QWidget *parent)
    : QWidget(parent),
      DObject(*new DCircleProgressPrivate(this))
{

}

/*!
  \brief Progress value of the progressbar.
  \brief 环形进度条的进度值。
  
  Maximum value is 100, minimum value is 0.
  最大值为 100 ，最小值为 0 。
  
  \sa setValue()
 */
int DCircleProgress::value() const
{
    Q_D(const DCircleProgress);

    return d->m_currentValue;
}

/*!
  \brief Set the progress \a value of the progressbar.
  \brief 设置环形进度条的进度值。
  
  Maximum \a value is 100, minimum \a value is 0.
  最大值为 100 ，最小值为 0 。将根据该值和最大值（100）的比例关系绘制进度。
  
  \sa value()
 */
void DCircleProgress::setValue(int value)
{
    Q_D(DCircleProgress);

    d->m_currentValue = value;
    update();
}

/*!
  \brief Text display near the circle progressbar.
  \brief 环形进度条旁边的文字。
 */
const QString DCircleProgress::text() const
{
    Q_D(const DCircleProgress);

    return d->m_topLabel.text();
}

/*!
  \brief Set the \a text display near the circle progressbar.
  \brief 设置环形进度条旁边的文字。
 */
void DCircleProgress::setText(const QString &text)
{
    Q_D(DCircleProgress);

    return d->m_topLabel.setText(text);
}

/*!
  \brief Background color of the circle progressbar.
  \brief 环形进度条的背景色。
  
  \sa setBackgroundColor()
 */
const QColor DCircleProgress::backgroundColor() const
{
    Q_D(const DCircleProgress);

    return d->m_backgroundColor;
}

/*!
  \brief Set the background \a color of the circle progressbar.
  \brief 设置环形进度条的背景色。
  
  \sa backgroundColor()
 */
void DCircleProgress::setBackgroundColor(const QColor &color)
{
    Q_D(DCircleProgress);

    d->m_backgroundColor = color;
}

/*!
  \brief Foreground color of the progressbar to indicate progress.
  \brief 环形进度条的前景色，用以标识进度。
  
  \sa setChunkColor()
 */
const QColor DCircleProgress::chunkColor() const
{
    Q_D(const DCircleProgress);

    return d->m_chunkColor;
}

/*!
  \brief Set the foreground \a color of the progressbar to indicate progress.
  \brief 设置用以标识进度环形进度条的前景色。
  
  \sa chunkColor()
 */
void DCircleProgress::setChunkColor(const QColor &color)
{
    Q_D(DCircleProgress);

    d->m_chunkColor = color;
}

/*!
  \brief circle line width of the circle progressbar.
  \brief 环形进度条的环形宽度。
  
  \sa setLineWidth()
 */
int DCircleProgress::lineWidth() const
{
    Q_D(const DCircleProgress);

    return d->m_lineWidth;
}

/*!
  \brief Set the circle line \a width of the circle progressbar.
  \brief 设置环形进度条的环形宽度。
  
  \sa setLineWidth()
 */
void DCircleProgress::setLineWidth(const int width)
{
    Q_D(DCircleProgress);

    d->m_lineWidth = width;
}

/*!
  \brief DCircleProgress::topLabel
  \return 返回环形进度条顶部文字Label
 */
QLabel *DCircleProgress::topLabel()
{
    Q_D(DCircleProgress);

    return &d->m_topLabel;
}

/*!
  \brief DCircleProgress::bottomLabel
  \return 返回环形进度条底部文字Label
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

void DCircleProgress::enterEvent(QEvent *e)
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
  \fn void Dtk::Widget::DCircleProgress::clicked()

  This signal is emitted when the user clicks the DCircleProgress widget.

  该信号将会在用户点击 DCircleProgress 控件时发出。
*/

/*!
  \fn void Dtk::Widget::DCircleProgress::mouseEntered()

  This signal is emitted when the user's mouse pointer entered the DCircleProgress widget.

  该信号将会在用户的鼠标指针移入 DCircleProgress 控件时发出。
*/

/*!
  \fn void Dtk::Widget::DCircleProgress::mouseLeaved()

  This signal is emitted when the user's mouse pointer leaved the DCircleProgress widget.

  该信号将会在用户的鼠标指针离开 DCircleProgress 控件时发出。
*/
