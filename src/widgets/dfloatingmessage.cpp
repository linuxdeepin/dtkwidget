// SPDX-FileCopyrightText: 2019 - 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfloatingmessage.h"
#include "private/dfloatingmessage_p.h"
#include "ddialogclosebutton.h"
#include "dsizemode.h"

#include <QHBoxLayout>
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QWindow>

class MessageLabel : public QLabel
{
public:
    QSize sizeHint() const override
    {
        return fontMetrics().size(Qt::TextSingleLine, text());
    }
};
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
    widget = new QWidget();
    q->DFloatingWidget::setWidget(widget);
    hBoxLayout = new QHBoxLayout(widget);
    iconButton = new DIconButton(nullptr);
    labMessage = new MessageLabel();
    labMessage->setWordWrap(true);

    iconButton->setFlat(true);
    iconButton->setFocusPolicy(Qt::NoFocus);
    iconButton->setAttribute(Qt::WA_TransparentForMouseEvents);
    iconButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    iconButton->setIconSize(DSizeModeHelper::element(QSize(20, 20), QSize(30, 30)));

    hBoxLayout->addWidget(iconButton);

    if (ENABLE_ANIMATIONS && ENABLE_ANIMATION_MESSAGE)
        hBoxLayout->addSpacing(10);

    hBoxLayout->addWidget(labMessage);

    if (notifyType == DFloatingMessage::MessageType::TransientType) {  //临时消息
        timer = new QTimer(q);
        timer->setInterval(4000);
        timer->setSingleShot(true);
        if (ENABLE_ANIMATIONS && ENABLE_ANIMATION_MESSAGE)
            q->connect(timer, &QTimer::timeout, q, [q]() {
                q->close();
                Q_EMIT q->messageClosed();
            });
        else
            q->connect(timer, &QTimer::timeout, q, &DFloatingMessage::close);
    } else {  //常驻消息
        content  = nullptr;
        closeButton = new DDialogCloseButton(q);
        // FIX bug-20506 close button too small
        closeButton->setIconSize(DSizeModeHelper::element(QSize(20, 20), QSize(32, 32)));

        hBoxLayout->addWidget(closeButton);

        q->connect(closeButton, &DIconButton::clicked, q, [q]() {
            if (q->windowHandle()) {
                q->windowHandle()->close();
            }
            if(ENABLE_ANIMATIONS && ENABLE_ANIMATION_MESSAGE) {
                Q_EMIT q->messageClosed();
            }
            Q_EMIT q->closeButtonClicked();
        });
    }

    if (!ENABLE_ANIMATIONS || !ENABLE_ANIMATION_MESSAGE)
        return;

    auto effect = new QGraphicsDropShadowEffect(q);
    effect->setColor(QColor(0, 0, 0, 0.1 * 255));
    effect->setBlurRadius(20);
    effect->setXOffset(0);
    effect->setYOffset(2);
    q->setGraphicsEffect(effect);
}

/*!
  \class Dtk::Widget::DFloatingMessage
  \inmodule dtkwidget
  \brief 一个浮动消息的类.

  类似于 ios 的通知栏消息, 然后有每一个新的消息对象, 就添加到一个竖直布局里面,
  然后一个消息的其布局模式是: icon (图标) + Message (消息) + QWidget (存放如控件按钮,可选) + closeButton (关闭按钮,可选)
  多个消息均是以竖直布局存放在一个 DMessageManager (消息管理类) 里面的
  
  \raw HTML
  <pre style="font-family: FreeMono, Consolas, Menlo, 'Noto Mono', 'Courier New', Courier, monospace;line-height: 100%;">
  =============================================================================================
  =                                                                                           =
  =        ==========       =======================         =========        ==========       =
  =        = icon   =       =        message      =         =QWidget=        =  quit  =       =
  =        ==========       =======================         =========        ==========       =
  =          (必选)                 (必选)                    (永驻:可选)       (永驻:可选)       =
  =============================================================================================
  </pre>
  \endraw
  消息通知一共有两种类型,临时停留几秒和手动清除的永驻消息
 */

/*!
  \enum Dtk::Widget::DFloatingMessage::MessageType
  DFloatingMessage::MessageType 定义了 DFloatingMessage 通知类型
  
  \value TransientType
  临时的消息
  
  \value ResidentType
  常驻的消息
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
  \brief 设置控件图标 icon
  \a ico 是最终的效果图标
 */
void DFloatingMessage::setIcon(const QIcon &ico)
{
    D_D(DFloatingMessage);

    d->iconButton->setIcon(ico);
}

void DFloatingMessage::setIcon(const DDciIcon &icon)
{
    D_D(DFloatingMessage);
    d->iconButton->setIcon(icon);
}

/*!
  \brief 设置显示的文本消息(文字)
  \a str 消息文本的具体文字内容
 */
void DFloatingMessage::setMessage(const QString &str)
{
    D_D(DFloatingMessage);

    d->labMessage->setText(str);
}

/*!
  \brief 消息里面的,水平布局里面的第三个位置, 用来存放一些自定义控件, 比如一个确定按钮
  \a w 插入的具体控件, 若之前此处已经有其他的控件, 先删除挪出空位子, 在将 w 控件放到这个上面
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
  \brief 若是为临时消息,则设置时间(毫秒)后会自动消失
  \a msec 消息显示的毫秒数, 默认是4000毫秒
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
  \brief 显示事件,里面设置计时器开始工作
  \param[in] event 系统传过来的显示消息事件,没有感兴趣的消息需要处理,直接所有事件继续传递给下一个对象
 */
void DFloatingMessage::showEvent(QShowEvent *event)
{
    D_D(DFloatingMessage);

    if (d->timer)
        d->timer->start();

    DFloatingWidget::showEvent(event);
}

void DFloatingMessage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::StyleChange) {
        D_D(DFloatingMessage);
        d->iconButton->setIconSize(DSizeModeHelper::element(QSize(20, 20), QSize(30, 30)));
        if (d->closeButton)
            d->closeButton->setIconSize(DSizeModeHelper::element(QSize(20, 20), QSize(32, 32)));
    }
    return DFloatingWidget::changeEvent(event);
}

DWIDGET_END_NAMESPACE

