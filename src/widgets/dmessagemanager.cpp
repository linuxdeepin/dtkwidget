#include "dmessagemanager.h"
#include <DFloatingMessage>
#include <QVBoxLayout>
#include <QEvent>

#define D_MESSAGE_MANAGER_CONTENT "_d_message_manager_content"

Q_DECLARE_METATYPE(QMargins)

// 仅仅为了接口兼容， 符号不会减少， 如果使用了这个接口，实际调用走的有namespace的
class Q_DECL_EXPORT DMessageManager: public QObject
{
    Q_OBJECT
private:
    DMessageManager();

public:
    static DMessageManager *instance();

    void sendMessage(QWidget *par, DTK_WIDGET_NAMESPACE::DFloatingMessage *floMsg);
    void sendMessage(QWidget *par, const QIcon &icon, const QString &message);
    bool setContentMargens(QWidget *par, const QMargins &margins);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

DMessageManager::DMessageManager()
{
}

DMessageManager *DMessageManager::instance()
{
    return reinterpret_cast<DMessageManager *>(DTK_WIDGET_NAMESPACE::DMessageManager::instance());
}

void DMessageManager::sendMessage(QWidget *par, DTK_WIDGET_NAMESPACE::DFloatingMessage *floMsg)
{
    DTK_WIDGET_NAMESPACE::DMessageManager::instance()->sendMessage(par, floMsg);
}

void DMessageManager::sendMessage(QWidget *par, const QIcon &icon, const QString &message)
{
    DTK_WIDGET_NAMESPACE::DMessageManager::instance()->sendMessage(par, icon, message);
}

bool DMessageManager::setContentMargens(QWidget *par, const QMargins &margins)
{
    return DTK_WIDGET_NAMESPACE::DMessageManager::instance()->setContentMargens(par, margins);
}

bool DMessageManager::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
    Q_UNUSED(event);
    return false;
}

DWIDGET_BEGIN_NAMESPACE
DMessageManager::DMessageManager()               //私有静态构造函数
{
}

/*!
  \brief DMessageManager::instance 构造一个单例对象
  \return 返回对象引用
 */
DMessageManager *DMessageManager::instance()  //公有静态函数
{
    static DMessageManager instance;             //局部静态变量
    return &instance;
}

/*!
  \brief DMessageManager::sendMessage 发送弹窗消息
  \a par 父控件
  \a floMsg 弹窗浮动消息（控件）
 */
void DMessageManager::sendMessage(QWidget *par, DFloatingMessage *floMsg)
{
    QWidget *content = par->findChild<QWidget *>(D_MESSAGE_MANAGER_CONTENT, Qt::FindDirectChildrenOnly);

    if (!content) {
        content = new QWidget(par);
        content->setObjectName(D_MESSAGE_MANAGER_CONTENT);
        content->setAttribute(Qt::WA_AlwaysStackOnTop);

        QMargins magins = par->property("_d_margins").value<QMargins>();
        if (par->property("_d_margins").isValid())
            content->setContentsMargins(magins);
        else
            content->setContentsMargins(QMargins(20, 0, 20, 0));

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

/*!
  \brief DMessageManager::sendMessage 发送弹窗消息
  \a par 父控件
  \a icon 消息图标
  \a message 消息文字
 */
void DMessageManager::sendMessage(QWidget *par, const QIcon &icon, const QString &message)
{
    QWidget *content = par->findChild<QWidget *>(D_MESSAGE_MANAGER_CONTENT, Qt::FindDirectChildrenOnly);
    int text_message_count = 0;

    for (DFloatingMessage *message : content->findChildren<DFloatingMessage*>(QString(), Qt::FindDirectChildrenOnly)) {
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

/*!
  \brief DMessageManager::setContentMargens 设置中间间隔值
  \a par 父控件
  \a margins四周的间隔值
  \return 是否设置成功
 */
bool DMessageManager::setContentMargens(QWidget *par, const QMargins &margins)
{
    QWidget *content = par->findChild<QWidget *>(D_MESSAGE_MANAGER_CONTENT, Qt::FindDirectChildrenOnly);
    if (content) {
        content->setContentsMargins(margins);
        return true;
    } else {
        par->setProperty("_d_margins", QVariant::fromValue(margins));
        return false;
    }
}

/*!
  \brief DMessageManager::eventFilter 消息过滤
  \a watched 查看对象
  \a event 消息事件
  \return 成功与否
 */
bool DMessageManager::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::LayoutRequest || event->type() == QEvent::Resize) {
        if (QWidget *widget = qobject_cast<QWidget *>(watched)) {
            QWidget *content;

            if (widget->objectName() == D_MESSAGE_MANAGER_CONTENT) {
                content = widget;
            } else {
                content = widget->findChild<QWidget*>(D_MESSAGE_MANAGER_CONTENT, Qt::FindDirectChildrenOnly);
            }

            QWidget *par = content->parentWidget();

            // 限制通知消息的最大宽度
            for (DFloatingMessage *message : content->findChildren<DFloatingMessage*>(QString(), Qt::FindDirectChildrenOnly)) {
                message->setMaximumWidth(par->rect().marginsRemoved(content->contentsMargins()).width());
                message->setMinimumHeight(message->sizeHint().height());
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

DWIDGET_END_NAMESPACE

#include "dmessagemanager.moc"
