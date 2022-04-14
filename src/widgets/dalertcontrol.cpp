/*
 * Copyright (C) 2020 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     ck <chenke@uniontech.com>
 *
 * Maintainer: ck <chenke@uniontech.com>
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

#include "DStyle"
#include "dalertcontrol.h"
#include "private/dalertcontrol_p.h"

#include <QTimer>
#include <QLayout>
#include <QEvent>

DWIDGET_BEGIN_NAMESPACE

DAlertControlPrivate::DAlertControlPrivate(DAlertControl *q)
 : DObjectPrivate(q)
{
    alertColor = q->defaultAlertColor();
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, q, &DAlertControl::hideAlertMessage);
}

void DAlertControlPrivate::updateTooltipPos()
{
    if (!target || !target->parentWidget() || !frame || !frame->parentWidget()) {
        qWarning("target or frame is nullptr.");
        return;
    }
    QWidget *p = target->parentWidget();

    int w = DStyle::pixelMetric(p->style(), DStyle::PM_FloatingWidgetShadowMargins) / 2;
    QPoint point = QPoint(target->x() - w, target->y() + target->height() - w);
    frame->move(p->mapTo(qobject_cast<QWidget *>(frame->parentWidget()), point));
    int tipWidth= frame->parentWidget()->width() - 20;
    tooltip->setMaximumWidth(tipWidth);
    frame->setMinimumHeight(tooltip->heightForWidth(tipWidth) + frame->layout()->spacing() *2);
    frame->adjustSize();

    int tw = target->width();
    int fw = frame->width();
    int leftPending = tw - fw;
    QPoint objPoint;
    switch (alignment) {
    case Qt::AlignLeft:
        objPoint = p->mapTo(qobject_cast<QWidget *>(frame->parentWidget()), point);
        if (objPoint.x() + frame->width() > (frame->parentWidget()->width())) {
            int shift = (objPoint.x() + frame->width()) - frame->parentWidget()->width();
            objPoint.setX(objPoint.x() - shift);
        }
        break;
    case Qt::AlignRight:
        point += QPoint(leftPending, 0);
        objPoint = p->mapTo(qobject_cast<QWidget *>(frame->parentWidget()), point);
        if (objPoint.x() < 0) {
            objPoint.setX(0);
        }
        break;
    case Qt::AlignHCenter:
    case Qt::AlignCenter:
        point += QPoint(leftPending/2, 0);
        objPoint = p->mapTo(qobject_cast<QWidget *>(frame->parentWidget()), point);
        break;
    default:
        return;
    }

    frame->move(objPoint);
}

DAlertControl::DAlertControl(QWidget *target, QObject *parent)
    : QObject(parent)
    , DObject(*new DAlertControlPrivate(this))
{
    D_D(DAlertControl);
    d->target = target;
}

DAlertControl::~DAlertControl()
{
    hideAlertMessage();
}

/*!
  \brief DAlertControl::setAlert设置是否开启警告模式
  警告模式，开启警告模式，target将显示警告颜色
  \a isAlert 是否开启警告模式
 */
void DAlertControl::setAlert(bool isAlert)
{
    D_D(DAlertControl);
    if (isAlert == d->isAlert || !d->target) {
        return;
    }

    d->isAlert = isAlert;

    DPalette p = d->target->palette();

    if (isAlert) {
        p.setColor(QPalette::Button, alertColor());
        d->target->setPalette(p);
    } else {
        d->target->setPalette(QPalette());
    }
    d->target->update();

    Q_EMIT alertChanged(isAlert);
}

/*!
  \brief DAlertControl::alert返回当前是否处于警告模式
 */
bool DAlertControl::isAlert() const
{
    D_DC(DAlertControl);

    return d->isAlert;
}

/*!
  \brief DAlertControl::defaultAlertColor返回默认告警颜色
  \note 默认颜色和原 DLineEdit 一致
 */
QColor DAlertControl::defaultAlertColor() const
{
    return QColor(241, 57, 50, qRound(0.15 * 255));
}

/*!
  \brief DAlertControl::setAlertColor 设置告警颜色
  \a c 告警颜色
 */
void DAlertControl::setAlertColor(QColor c)
{
    D_D(DAlertControl);
    if (c == d->alertColor) {
        return;
    }

    d->alertColor = c;
    if (d->target) {
        d->target->update();
    }
}

/*!
  \brief DAlertControl::alertColor 返回当前告警颜色
 */
QColor DAlertControl::alertColor() const
{
    D_DC(DAlertControl);
    return d->alertColor;
}

/*!
  \brief DAlertControl::setMessageAlignment指定对齐方式
  现只支持左，右，居中， 默认左对齐.
  \note 参数为其他时，默认左对齐
  \a alignment 消息对齐方式
 */
void DAlertControl::setMessageAlignment(Qt::Alignment alignment)
{
    D_D(DAlertControl);
    d->alignment = alignment;
}

/*!
  \brief DAlertControl::messageAlignment 返回当前告警 tooltips 对齐方式
 */
Qt::Alignment DAlertControl::messageAlignment() const
{
    D_DC(DAlertControl);
    return d->alignment;
}

/*!
  \brief DAlertControl::showAlertMessage显示警告消息
  显示指定的文本消息，超过指定时间后警告消息消失.
  \note 时间参数为-1时，警告消息将一直存在
  \a text 警告的文本
  \a duration 显示的时间长度，单位毫秒
 */
void DAlertControl::showAlertMessage(const QString &text, int duration)
{
    showAlertMessage(text, nullptr, duration);
}

/*!
  \brief DAlertControl::showAlertMessage显示警告消息.

  显示指定的文本消息，超过指定时间后警告消息消失.
  \note 时间参数为-1时，警告消息将一直存在
  \a text 警告的文本
  \a follow 指定文本消息跟随的对象
  \a duration 显示的时间长度，单位毫秒
 */
void DAlertControl::showAlertMessage(const QString &text, QWidget *follower, int duration)
{
    D_D(DAlertControl);
    if (!d->target)
        return;

    if (!d->tooltip) {
        d->tooltip = new DToolTip(text);
        d->tooltip->setObjectName("AlertTooltip");
        d->tooltip->setAccessibleName("DAlertControlAlertToolTip");
        d->tooltip->setForegroundRole(DPalette::TextWarning);
        d->tooltip->setWordWrap(true);

        d->frame = new DFloatingWidget;
        d->frame->setAccessibleName("DAlertControlFloatingWidget");
        d->frame->setFramRadius(DStyle::pixelMetric(d->target->style(), DStyle::PM_FrameRadius));
        d->frame->setBackgroundRole(QPalette::ToolTipBase);
        d->frame->setWidget(d->tooltip);
    }

    if (follower) {
        d->frame->setParent(follower->topLevelWidget());
        d->follower = follower;
        d->follower->installEventFilter(this);
    } else {
        d->frame->setParent(d->target->topLevelWidget());
        d->follower = d->target;
        d->follower->installEventFilter(this);
    }
    d->follower->topLevelWidget()->installEventFilter(this);

    d->tooltip->setText(text);
    if (d->frame->parent()) {
        d->updateTooltipPos();
        // 如果 target 都隐藏了,显示警告消息毫无意义，只会让人困惑
        d->frame->setVisible(d->target->isVisibleTo(d->target->topLevelWidget()));
        d->frame->adjustSize();
        d->frame->raise();
    }
    if (duration < 0) {
        d->timer.stop();
        return;
    }

    d->timer.start(duration);
}

/*!
  \brief DAlertControl:: hideAlertMessage隐藏警告消息框
 */
void DAlertControl::hideAlertMessage()
{
    Q_D(DAlertControl);

    if (d->frame) {
        d->frame->hide();
        if (d->follower) {
            //this->removeEventFilter(d->follower);
            d->follower->removeEventFilter(this);
            d->follower = nullptr;
        }
    }
}

DAlertControl::DAlertControl(DAlertControlPrivate &d, QObject *parent)
    : QObject(parent)
    , DObject(d)
{

}

bool DAlertControl::eventFilter(QObject *watched, QEvent *event)
{
    Q_D(DAlertControl);
    if (watched == d->follower) {
        if (event->type() == QEvent::Move || event->type() == QEvent::Resize)
            d->updateTooltipPos();

        if (event->type() == QEvent::Hide || event->type() == QEvent::HideToParent)
            hideAlertMessage();
    }

    if (d->follower && watched == d->follower->topLevelWidget()) {
        if (event->type() == QEvent::HoverMove || event->type() == QEvent::UpdateRequest)
            d->updateTooltipPos();

        if (d->timer.isActive())
            d->frame->setVisible(!d->follower->visibleRegion().isNull());
    }
    return QObject::eventFilter(watched, event);
}

DWIDGET_END_NAMESPACE
