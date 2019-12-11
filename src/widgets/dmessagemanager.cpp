#include "dmessagemanager.h"
#include <QDebug>
#include <QEvent>

#define D_MESSAGE_MANAGER_CONTENT "_d_message_manager_content"

DMessageManager::DMessageManager()               //私有静态构造函数
{
}

DMessageManager *DMessageManager::instance()  //公有静态函数
{
    static DMessageManager instance;             //局部静态变量
    return &instance;
}

void DMessageManager::sendMessage(QWidget *par, DFloatingMessage *floMsg)
{
    QWidget *content = par->findChild<QWidget *>(D_MESSAGE_MANAGER_CONTENT);

    if (!content) {
        content = new QWidget(par);
        content->setObjectName(D_MESSAGE_MANAGER_CONTENT);
        content->setAttribute(Qt::WA_AlwaysStackOnTop);
        content->installEventFilter(this);
        par->installEventFilter(this);
        QVBoxLayout *layout = new QVBoxLayout(content);
        layout->setSpacing(0);
        layout->setMargin(0);
        layout->setDirection(QBoxLayout::BottomToTop);
        content->show();
    }

    static_cast<QBoxLayout*>(content->layout())->addWidget(floMsg, 0, Qt::AlignHCenter);
}

void DMessageManager::sendMessage(QWidget *par, const QIcon &icon, const QString &message)
{
    QWidget *content = par->findChild<QWidget *>(D_MESSAGE_MANAGER_CONTENT);
    int text_message_count = 0;

    for (DFloatingMessage *message : content->findChildren<DFloatingMessage*>()) {
        if (message->messageType() == DFloatingMessage::TransientType) {
            ++text_message_count;
        }
    }

    // TransientType 类型的通知消息，最多只允许同时显示三个
    if (text_message_count >= 3)
        return;

    DFloatingMessage *floMsg = new DFloatingMessage(DFloatingMessage::TransientType);
    floMsg->setAttribute(Qt::WA_DeleteOnClose);
    floMsg->setIcon(icon);
    floMsg->setMessage(message);
    sendMessage(par, floMsg);
}

bool DMessageManager::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::LayoutRequest || event->type() == QEvent::Resize) {
        if (QWidget *widget = qobject_cast<QWidget *>(watched)) {
            QWidget *content;

            if (widget->objectName() == D_MESSAGE_MANAGER_CONTENT) {
                content = widget;
            } else {
                content = widget->findChild<QWidget*>(D_MESSAGE_MANAGER_CONTENT);
            }

            QWidget *par = content->parentWidget();

            // 限制通知消息的最大宽度
            for (DFloatingMessage *message : content->findChildren<DFloatingMessage*>()) {
                message->setMaximumWidth(par->width() * 0.8);
            }

            QRect geometry(QPoint(0, 0), content->sizeHint());
            geometry.moveCenter(par->rect().center());
            geometry.moveBottom(par->rect().bottom());
            content->setGeometry(geometry);
        }
    } else if (event->type() == QEvent::ChildRemoved) {
        // 如果是通知消息被删除的事件
        if (QWidget *widget = qobject_cast<QWidget*>(watched)) {
            if (widget->objectName() == D_MESSAGE_MANAGER_CONTENT
                    && widget->layout() && widget->layout()->count() == 0) {
                widget->parent()->removeEventFilter(this);
                widget->deleteLater();
            }
        }
    }

    return QObject::eventFilter(watched, event);
}
