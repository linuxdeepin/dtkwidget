/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zhangwengeng <zhangwengeng_cm@deepin.com>
 *
 * Maintainer: zhangwengeng <zhangwengeng_cm@deepin.com>
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

#include "dfloatingmessage.h"
#include "private/dfloatingmessage_p.h"
#include "ddialogclosebutton.h"

#include <QHBoxLayout>
#include <QTimer>
#include <QLabel>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

DFloatingMessagePrivate::DFloatingMessagePrivate(DFloatingMessage *qq)
    : DFloatingWidgetPrivate(qq)
{

}

DFloatingMessagePrivate::~DFloatingMessagePrivate()
{

}

void DFloatingMessagePrivate::init()
{
    D_Q(DFloatingMessage);

    timer = nullptr;
    content = nullptr;
    closeButton = nullptr;
    notifyType = notifyType;
    widget = new QWidget();
    q->DFloatingWidget::setWidget(widget);
    hBoxLayout = new QHBoxLayout(widget);
    iconButton = new DIconButton(nullptr);
    labMessage = new QLabel();
    labMessage->setWordWrap(true);

    iconButton->setFlat(true);
    iconButton->setFocusPolicy(Qt::NoFocus);
    iconButton->setAttribute(Qt::WA_TransparentForMouseEvents);
    iconButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    iconButton->setIconSize(QSize(30, 30));

    hBoxLayout->addWidget(iconButton);
    hBoxLayout->addWidget(labMessage);

    if (notifyType == DFloatingMessage::MessageType::TransientType) {  //临时消息
        timer = new QTimer(q);
        timer->setInterval(4000);
        timer->setSingleShot(true);
        q->connect(timer, &QTimer::timeout, q, &DFloatingMessage::close);
    } else {  //常驻消息
        content  = nullptr;
        closeButton = new DDialogCloseButton(q);

        hBoxLayout->addWidget(closeButton);
        q->connect(closeButton, &DIconButton::clicked, q, &DFloatingMessage::closeButtonClicked);
        q->connect(closeButton, &DIconButton::clicked, q, &DFloatingMessage::close);
    }
}

/*!
 * \~chinese \class DFloatingMessage
 * \~chinese \brief 是一个浮动消息的类, 类似于 ios 的通知栏消息, 然后有每一个新的消息对象, 就添加到一个竖直布局里面,
 * \~chinese 然后一个消息的其布局模式是: icon (图标) + Message (消息) + QWidget (存放如控件按钮,可选) + closeButton (关闭按钮,可选)
 * \~chinese 多个消息均是以竖直布局存放在一个 DMessageManager (消息管理类) 里面的
 *
 * \~chinese \htmlonly
 * <pre style="font-family: FreeMono, Consolas, Menlo, 'Noto Mono', 'Courier New', Courier, monospace;line-height: 100%;">
 * =============================================================================================
 * =                                                                                           =
 * =        ==========       =======================         =========        ==========       =
 * =        = icon   =       =        message      =         =QWidget=        =  quit  =       =
 * =        ==========       =======================         =========        ==========       =
 * =          (必选)                 (必选)                    (永驻:可选)       (永驻:可选)       =
 * =============================================================================================
 * </pre>
 * \endhtmlonly
 * \~chinese 消息通知一共有两种类型,临时停留几秒和手动清除的永驻消息
 */

/*!
 *
 * \~chinese \enum DFloatingMessage::MessageType
 * \~chinese DFloatingMessage::MessageType 定义了 DFloatingMessage 通知类型
 *
 * \~chinese \var DFloatingMessage:MessageType DFloatingMessage::TransientType
 * \~chinese 临时的消息
 *
 * \~chinese \var DDFloatingMessage:MessageType DFloatingMessage::ResidentType
 * \~chinese 常驻的消息
 */

DFloatingMessage::DFloatingMessage(MessageType notifyType, QWidget *parent)
    : DFloatingWidget(*new DFloatingMessagePrivate(this), parent)
{
    D_D(DFloatingMessage);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    d->notifyType = notifyType;
    d->init();
}

DFloatingMessage::MessageType DFloatingMessage::messageType() const
{
    D_DC(DFloatingMessage);

    return d->notifyType;
}

/*!
 * \~chinese \brief 设置控件图标 icon
 * \~chinese \param[in] ico 是最终的效果图标
 */
void DFloatingMessage::setIcon(const QIcon &ico)
{
    D_D(DFloatingMessage);

    d->iconButton->setIcon(ico);
}

/*!
 * \~chinese \brief 设置显示的文本消息(文字)
 * \~chinese \param[in] str 消息文本的具体文字内容
 */
void DFloatingMessage::setMessage(const QString &str)
{
    D_D(DFloatingMessage);

    d->labMessage->setText(str);
}

/*!
 * \~chinese \brief 消息里面的,水平布局里面的第三个位置, 用来存放一些自定义控件, 比如一个确定按钮
 * \~chinese \param[in] w 插入的具体控件, 若之前此处已经有其他的控件, 先删除挪出空位子, 在将 w 控件放到这个上面
 */
void DFloatingMessage::setWidget(QWidget *w)
{
    D_D(DFloatingMessage);

    if (d->content)
        delete d->hBoxLayout->takeAt(2);

    d->content = w;
    d->hBoxLayout->insertWidget(2, d->content);
}

/*!
 * \~chinese \brief 若是为临时消息,则设置时间(毫秒)后会自动消失
 * \~chinese \param[in] msec 消息显示的毫秒数, 默认是4000毫秒
 */
void DFloatingMessage::setDuration(int msec)
{
    D_D(DFloatingMessage);

    if (d->timer == nullptr)
        return;

    d->timer->setInterval(msec);
}

QSize DFloatingMessage::sizeHint() const
{
    int max_width = maximumWidth();
    QSize size_hint = DFloatingWidget::sizeHint();

    if (max_width < size_hint.width())
        return QSize(max_width, heightForWidth(max_width));

    return DFloatingWidget::sizeHint();
}

/*!
 * \~chinese \brief 显示事件,里面设置计时器开始工作
 * \~chinese \param[in] event 系统传过来的显示消息事件,没有感兴趣的消息需要处理,直接所有事件继续传递给下一个对象
 */
void DFloatingMessage::showEvent(QShowEvent *event)
{
    D_D(DFloatingMessage);

    if (d->timer)
        d->timer->start();

    DFloatingWidget::showEvent(event);
}

DWIDGET_END_NAMESPACE

