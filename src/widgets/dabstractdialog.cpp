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
#include <QLabel>
#include <QDebug>

#include "danchors.h"
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
    } else {
        q->setWindowFlags(q->windowFlags() | Qt::FramelessWindowHint);
    }

    windowTitle = new QLabel(q);
    windowTitle->setAlignment(Qt::AlignCenter);
    DAnchorsBase::setAnchor(windowTitle, Qt::AnchorHorizontalCenter, q, Qt::AnchorHorizontalCenter);
    q->connect(q,&QWidget::windowTitleChanged, windowTitle,[=](const QString &title){
        windowTitle->setText(title);
    });

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

/*!
    \~chinese \class DAbstractDialog
    \~chinese \brief 可以使用 DAbstractDialog 类创建符合 DDE 风格的对话框窗口。

    \~chinese \ingroup dialog-classes
    \~chinese \ingroup abstractwidgets

    \~chinese 对话框是一个用于供用户进行短暂周期的任务交互的顶层窗体。 DDialogs 对话框可以是模态的或非模态的。
    对话框可以提供一个 \ref return "返回值", 并且对话框可以拥有 \ref default "默认按钮"。

    \~chinese 请注意 DAbstractDialog （以及其他继承自 QDialog 的对话框）对构造时传入的 parent 父组件的行为和其他
    Qt 组件或 DTK 组件不同。一个对话框总是一个顶层控件（top-level widget），但如果它有一个父组件
    则对话框的默认位置将会位于其父组件的正中央，并共用其父控件的任务栏入口。

    \~chinese \section modal 模态对话框

    \~chinese 一个 \b{模态} （modal）对话框可以阻止对模态对话框之外的原可见窗体的操作，如请求用户输入
    文件名的对话框或是对应用程序本身进行设置的对话框就常是模态对话框。模态对话框可以是
    \ref Qt::ApplicationModal "应用模态" 的（默认），也可以是 \ref Qt::WindowModal "窗体模态" 的。

    \~chinese 当应用模态对话框出现后，用户必须完成与对话框的交互并关闭对话框才能继续与应用的其他窗体进行交互。
    窗体模态对话框则仅仅阻止用户与这个对话框的父窗体进行交互而不影响这个父窗体之外的其他窗体。

    \~chinese 显示模态对话框的常见做法是调用 exec() 。当对话框被用户关闭时， exec() 会提供一个有用的
    \ref return "返回值" 。通常，需要关闭一个对话框并使其返回期望的值时，我们可以把默认按钮，
    如 \uicontrol OK 确认按钮连接到 accept() 槽，并把 \uicontrol Cancel 取消按钮连接到
    reject() 槽。当然，也可以调用 done() 槽并传递 \c Accepted 或是 \c Rejected 达到相同目的。

    \~chinese 另一个可行方案是调用 setModal(true) 或 setWindowModality(), 然后调用 show() 。而区别于 exec()
    的是， show() 将立即交回控制权给调用者。对于类似显示进度条的应用场景，如需限定用户只能与对话框交互（
    比如，允许取消一个执行时间过长的操作），此时 setModal(true) 就很有用处。

    \~chinese \section modeless 非模态对话框

    \~chinese 一个 \b{非模态}（modeless）对话框表示其对话框本身的操作和该应用的其他窗口互相独立互不影响。
    例如文字处理程序中的“查找文字”功能通常是非模态的，以便用户同时操作对话框和应用程序窗口。

    \~chinese 非模态对话框通过调用 show() 使其显示, 其会将控制权立即返回给调用者。

    \~chinese 如果在隐藏对话框后调用 \ref QWidget::show() "show()" ，对话框将显示在其原本所在的位置。若要记住用户
    调整对话框位置后的位置，需要在 \ref QWidget::closeEvent() "closeEvent()" 中保存窗体位置并在显示对话框
    前移动对话框到所保存的位置。

    \~chinese \section default 默认按钮

    \~chinese 对话框的 \e 默认 按钮是指当用户直接按下回车（Enter / Return）键时所会被按下的按钮。用以表明用户接受了对话框
    的某些操作并想要关闭对话框。对于 QDialog 可以使用 QPushButton::setDefault(), QPushButton::isDefault()
    和 QPushButton::autoDefault() 来设置或调整一个对话框的默认按钮。

    \~chinese \section escapekey 退出（Escape）键

    \~chinese 如果用户在对话框出现后按下了 Esc 键, 将会触发 QDialog::reject() 并导致窗体被关闭。 \ref QCloseEvent "关闭事件"
    不可以是 \ref QEvent::ignore() "忽略事件" .

    \~chinese \section return 返回值 (模态对话框)

    \~chinese 模态对话框通常伴随返回值一同使用。例如用来标识判断用户是按下了 \uicontrol OK 确认按钮 还是 \uicontrol Cancel 取消按钮。
    对话框可以通过调用 accept() 或 reject() 槽来关闭， exec() 则会根据实际情况返回 \c Accepted 或 \c Rejected 。
    如果对话框没有被销毁，也可以通过 result() 得到对话框的返回值。

    \~chinese 若要修改对话框的关闭行为，你可以重新实现 accept(), reject() 或 done() 。而 \ref QWidget::closeEvent() "closeEvent()"
    仅当你需要记住对话框位置或是重载标准的关闭行为时才应使用。

    \~chinese \sa DDialog, QDialog
*/

/*!
    \~english \class DAbstractDialog
    \~english \brief Use DAbstractDialog class to create dialog window matched DDE style.

    \~english \ingroup dialog-classes
    \~english \ingroup abstractwidgets

    \~english \sa DAbstractDialog, QDialog
*/

/**
 * \~english \brief DAbstractDialog::DAbstractDialog constructs a DAbstractDialog instance.
 * \~english \param parent is the parent widget to be used.
 *
 * \~chinese \brief 构造一个 DAbstractDialog 实例
 */
DAbstractDialog::DAbstractDialog(QWidget *parent) :
    QDialog(parent),
    DObject(*new DAbstractDialogPrivate(this))
{
    DThemeManager::registerWidget(this);

    d_func()->init();
}

/**
 * \~english \brief DAbstractDialog::backgroundColor
 * \~english \return the background color of the dialog.
 *
 * \~chinese \brief 获取对话框的背景色
 */
QColor DAbstractDialog::backgroundColor() const
{
    D_DC(DAbstractDialog);

    return d->backgroundColor;
}

/**
 * \~english \brief DAbstractDialog::borderColor
 * \~english \return the border color of the dialog.
 *
 * \~chinese \brief 获取对话框的边框颜色
 */
QColor DAbstractDialog::borderColor() const
{
    D_DC(DAbstractDialog);

    return d->borderColor;
}

/*!
 * \enum DAbstractDialog::DisplayPosition
 *
 * \~english \brief The DisplayPosition enum contains the position options that can be specified by all dialogs.
 * \~chinese \brief DAbstractDialog::DisplayPosition 表示对话框的显示位置。
 *
 * \var DAbstractDialog::DisplayPosition DAbstractDialog::Center
 * \~english display this dialog in the center of the screen
 * \~chinese 在屏幕中央显示对话框。
 *
 * \var DAbstractDialog::DisplayPosition DAbstractDialog::TopRight
 * \~english display this dialog in the top right of the screen
 * \~chinese 在屏幕右上角显示对话框。
 */

/**
 * \~english \brief DAbstractDialog::displayPosition
 * \~english \return the display position of this dialog.
 *
 * \~chinese \brief 获取对话框显示位置
 */
DAbstractDialog::DisplayPosition DAbstractDialog::displayPosition() const
{
    D_DC(DAbstractDialog);

    return d->displayPosition;
}

void DAbstractDialog::move(const QPoint &pos)
{
    QDialog::move(pos);

    D_D(DAbstractDialog);

    d->mouseMoved = true;
}

void DAbstractDialog::setGeometry(const QRect &rect)
{
    QDialog::setGeometry(rect);

    D_D(DAbstractDialog);

    d->mouseMoved = true;
}

/**
 * \~english \brief DAbstractDialog::moveToCenter moves the dialog to the center of the screen or its parent widget.
 *
 * \~chinese \brief 将对话框移动至屏幕中央或其父控件的中央。
 */
void DAbstractDialog::moveToCenter()
{
    D_DC(DAbstractDialog);

    moveToCenterByRect(d->getParentGeometry());
}

/**
 * \~english \brief DAbstractDialog::moveToTopRight moves the dialog to the top right of the screen or its parent widget.
 *
 * \~chinese \brief 将对话框移动至屏幕右上角或其父控件的右上角。
 */
void DAbstractDialog::moveToTopRight()
{
    D_DC(DAbstractDialog);

    moveToTopRightByRect(d->getParentGeometry());
}

/**
 * \~english \brief DAbstractDialog::moveToTopRightByRect moves the dialog to the top right corner of the rect.
 * \~english \param rect is the target rect.
 *
 * \~chinese \brief 移动对话框到给定 \a rect 区域的右上角。
 * \~chinese \param rect 是移动所需要参照的 QRect 位置。
 */
void DAbstractDialog::moveToTopRightByRect(const QRect &rect)
{
    int x = rect.x() + rect.width() - width();
    QDialog::move(QPoint(x, 0));
}

/**
 * \~english \brief DAbstractDialog::setBackgroundColor sets the background color of the dialog.
 * \~english \param backgroundColor is the target background color.
 *
 * \~chinese \brief 设置对话框的背景色
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
 * \~english \brief DAbstractDialog::setBorderColor sets the border color of the dialog.
 * \~english \param borderColor is the target border color.
 *
 * \~chinese \brief 设置对话框边框的颜色
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
 * \~english \brief DAbstractDialog::setDisplayPosition sets the position of the dialog.
 * \~english \param displayPosition is the target position.
 *
 * \~chinese \brief 设置对话框的显示位置。
 * \~chinese \param displayPosition 要显示到的位置
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

/**
 * \~english \brief DAbstractDialog::moveToCenterByRect moves the dialog to the center of the rect.
 * \~english \param rect is the target rect.
 *
 * \~chinese \brief 移动对话框到给定 \a rect 区域的中央。
 * \~chinese \param rect 是移动对话框要参照的 QRect 区域
 */
void DAbstractDialog::moveToCenterByRect(const QRect &rect)
{
    QRect qr = geometry();
    qr.moveCenter(rect.center());
    QDialog::move(qr.topLeft());
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

/*!\reimp */
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

/*!\reimp */
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

    if (d->bgBlurWidget)
        d->bgBlurWidget->resize(event->size());

    Q_EMIT sizeChanged(event->size());
}

void DAbstractDialog::showEvent(QShowEvent *event)
{
    D_DC(DAbstractDialog);

    // 不要在resizeEvent中改变窗口geometry，可能会导致以下问题。在双屏开启不同缩放比时：
    // 屏幕A缩放为1，屏幕B缩放为2。窗口初始在屏幕B上，此时大小为 100x100，则其缩放
    // 后的真实大小为200x200，由于窗口管理器会对未设置过位置的窗口自动布局，接下来
    // 窗口可能会因为位置改变而被移动到屏幕A上，则此对话框的大小将变化为 200x200，
    // 触发了resize事件，紧接着又会尝试将窗口位置更新到鼠标所在屏幕的中心，如果鼠标
    // 所在屏幕为B，则在移动窗口位置时又会根据B的缩放比重新设置窗口大小，窗口将变为：
    // 400x400，将形成一个循环。。。
    if (!d->mouseMoved) {
        setDisplayPosition(displayPosition());
    }

    QDialog::showEvent(event);
}

DAbstractDialog::DAbstractDialog(DAbstractDialogPrivate &dd, QWidget *parent):
    QDialog(parent),
    DObject(dd)
{
    dd.init();
}

DWIDGET_END_NAMESPACE
