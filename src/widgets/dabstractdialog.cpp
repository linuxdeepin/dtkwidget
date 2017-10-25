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

#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QScreen>
#include <QPainter>
#include <QWidget>
#include <QDebug>

#include "dialog_constants.h"
#include "dabstractdialog.h"
#include "private/dabstractdialogprivate_p.h"
#include "dthememanager.h"
#include "dplatformwindowhandle.h"
#include "dapplication.h"
#include "dblureffectwidget.h"

DWIDGET_BEGIN_NAMESPACE

DAbstractDialogPrivate::DAbstractDialogPrivate(DAbstractDialog *qq):
    DObjectPrivate(qq)
{

}

void DAbstractDialogPrivate::init()
{
    D_Q(DAbstractDialog);

    if (qApp->isDXcbPlatform()) {
        handle = new DPlatformWindowHandle(q, q);

        handle->setTranslucentBackground(true);
        handle->setEnableSystemMove(false);
        handle->setEnableSystemResize(false);

//        bgBlurWidget = new DBlurEffectWidget(q);
//        bgBlurWidget->lower();
//        bgBlurWidget->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
//        bgBlurWidget->setVisible(DPlatformWindowHandle::hasBlurWindow());

//        DPlatformWindowHandle::connectWindowManagerChangedSignal(q, [this] {
//            bgBlurWidget->setVisible(DPlatformWindowHandle::hasBlurWindow());
//        });
    }

    q->setWindowFlags(q->windowFlags() | Qt::FramelessWindowHint  | Qt::WindowCloseButtonHint);

    q->setAttribute(Qt::WA_TranslucentBackground);
    q->resize(DIALOG::DEFAULT_WIDTH, DIALOG::DEFAULT_HEIGHT);
    q->setMaximumWidth(480);
    q->setAttribute(Qt::WA_Resized, false);
    q->setBorderColor(QColor(0, 0, 0));
}

QRect DAbstractDialogPrivate::getParentGeometry() const
{
    D_QC(DAbstractDialog);

    if (q->parentWidget()) {
        return q->parentWidget()->window()->geometry();
    } else {
        QPoint pos = QCursor::pos();

        for (QScreen *screen : qApp->screens()) {
            if (screen->geometry().contains(pos)) {
                return screen->geometry();
            }
        }
    }

    return qApp->primaryScreen()->geometry();
}

/**
 * @brief DAbstractDialog::DAbstractDialog constructs a DAbstractDialog instance.
 * @param parent is the parent widget to be used.
 */
DAbstractDialog::DAbstractDialog(QWidget *parent) :
    QDialog(parent),
    DObject(*new DAbstractDialogPrivate(this))
{
    D_THEME_INIT_WIDGET(DAbstractDialog);

    d_func()->init();
}

/**
 * @brief DAbstractDialog::backgroundColor
 * @return the background color of the dialog.
 */
QColor DAbstractDialog::backgroundColor() const
{
    D_DC(DAbstractDialog);

    return d->backgroundColor;
}

/**
 * @brief DAbstractDialog::borderColor
 * @return the border color of the dialog.
 */
QColor DAbstractDialog::borderColor() const
{
    D_DC(DAbstractDialog);

    return d->borderColor;
}

/**
 * @brief DAbstractDialog::displayPosition
 * @return the display position of this dialog.
 */
DAbstractDialog::DisplayPosition DAbstractDialog::displayPosition() const
{
    D_DC(DAbstractDialog);

    return d->displayPosition;
}

DAbstractDialog::DisplayPostion DAbstractDialog::displayPostion() const
{
    return static_cast<DisplayPostion>(displayPosition());
}

/**
 * @brief DAbstractDialog::moveToCenter moves the dialog to the center of the screen or its parent widget.
 */
void DAbstractDialog::moveToCenter()
{
    D_DC(DAbstractDialog);

    moveToCenterByRect(d->getParentGeometry());
}

/**
 * @brief DAbstractDialog::moveToTopRight moves the dialog to the top right of the screen or its parent widget.
 */
void DAbstractDialog::moveToTopRight()
{
    D_DC(DAbstractDialog);

    moveToTopRightByRect(d->getParentGeometry());
}

/**
 * @brief DAbstractDialog::moveToTopRightByRect moves the dialog to the top right corner of the rect.
 * @param rect is the target rect.
 */
void DAbstractDialog::moveToTopRightByRect(const QRect &rect)
{
    int x = rect.x() + rect.width() - width();
    move(QPoint(x, 0));
}

/**
 * @brief DAbstractDialog::setBackgroundColor sets the background color of the dialog.
 * @param backgroundColor is the target background color.
 */
void DAbstractDialog::setBackgroundColor(QColor backgroundColor)
{
    D_D(DAbstractDialog);

    d->backgroundColor = backgroundColor;

    if (d->bgBlurWidget)
        d->bgBlurWidget->setMaskColor(backgroundColor);

    update();
}

/**
 * @brief DAbstractDialog::setBorderColor sets the border color of the dialog.
 * @param borderColor is the target border color.
 */
void DAbstractDialog::setBorderColor(QColor borderColor)
{
    D_D(DAbstractDialog);

    d->borderColor = borderColor;

    if (d->handle) {
        d->handle->setBorderColor(d->borderColor);
    } else {
        update();
    }
}

/**
 * @brief DAbstractDialog::setDisplayPosition sets the position of the dialog.
 * @param displayPosition is the target position.
 */
void DAbstractDialog::setDisplayPosition(DAbstractDialog::DisplayPosition displayPosition)
{
    D_D(DAbstractDialog);

    d->displayPosition = displayPosition;

    switch (displayPosition) {
    case DisplayCenter:
        moveToCenter();
        break;
    case DisplayTopRight:
        moveToTopRight();
        break;
    default:
        break;
    }
}

void DAbstractDialog::setDisplayPostion(DAbstractDialog::DisplayPostion displayPosition)
{
    setDisplayPosition(static_cast<DisplayPosition>(displayPosition));
}

/**
 * @brief DAbstractDialog::moveToCenterByRect moves the dialog to the center of the rect.
 * @param rect is the target rect.
 */
void DAbstractDialog::moveToCenterByRect(const QRect &rect)
{
    QRect qr = geometry();
    qr.moveCenter(rect.center());
    move(qr.topLeft());
}

void DAbstractDialog::mousePressEvent(QMouseEvent *event)
{
    D_DC(DAbstractDialog);

    if (d->handle) {
        return QDialog::mousePressEvent(event);
    }

    if (event->button() == Qt::LeftButton) {
        D_D(DAbstractDialog);

        d->dragPosition = event->globalPos() - frameGeometry().topLeft();
        d->mousePressed = true;
    }

    QDialog::mousePressEvent(event);
}

void DAbstractDialog::mouseReleaseEvent(QMouseEvent *event)
{
    D_D(DAbstractDialog);

    if (d->handle) {
        return QDialog::mouseReleaseEvent(event);
    }

    d->mousePressed = false;

    QDialog::mouseReleaseEvent(event);
}

void DAbstractDialog::mouseMoveEvent(QMouseEvent *event)
{
    D_D(DAbstractDialog);

    if (d->handle) {
        d->handle->setEnableSystemMove(true);

        return QDialog::mouseMoveEvent(event);
    }

    if (d->mousePressed) {
        move(event->globalPos() - d->dragPosition);
        d->mouseMoved = true;
    }

    QDialog::mouseMoveEvent(event);
}

void DAbstractDialog::paintEvent(QPaintEvent *event)
{
    D_DC(DAbstractDialog);

    QPainter painter(this);

    if (d->handle) {
        painter.fillRect(event->rect(), d->backgroundColor);
    } else {
        painter.setPen(QPen(d->borderColor, DIALOG::BORDER_SHADOW_WIDTH));
        painter.setBrush(d->backgroundColor);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QRectF r(DIALOG::BORDER_SHADOW_WIDTH / 2.0, DIALOG::BORDER_SHADOW_WIDTH / 2.0,
                 width() - DIALOG::BORDER_SHADOW_WIDTH, height() - DIALOG::BORDER_SHADOW_WIDTH);
        painter.drawRoundedRect(r, DIALOG::BORDER_RADIUS, DIALOG::BORDER_RADIUS);
    }

    QDialog::paintEvent(event);
}

void DAbstractDialog::resizeEvent(QResizeEvent *event)
{
    if (event->size().width() >= maximumWidth()) {
        bool resized = testAttribute(Qt::WA_Resized);

        setFixedWidth(maximumWidth());

        if (!resized)
            setAttribute(Qt::WA_Resized, false);
    }
    QDialog::resizeEvent(event);

    D_DC(DAbstractDialog);

    if (!d->mouseMoved) {
        setDisplayPosition(displayPosition());
    }

    if (d->bgBlurWidget)
        d->bgBlurWidget->resize(event->size());

    Q_EMIT sizeChanged(event->size());
}

DAbstractDialog::DAbstractDialog(DAbstractDialogPrivate &dd, QWidget *parent):
    QDialog(parent),
    DObject(dd)
{
    dd.init();
}

DWIDGET_END_NAMESPACE
