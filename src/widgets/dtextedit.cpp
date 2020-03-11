#include "dtextedit.h"

#include <QPainter>
#include <QEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>

#include <DStyle>
#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DTextEditPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DTextEditPrivate(DTextEdit* qq);
    ~DTextEditPrivate();

public:
    QWidget* widgetTop;
    QWidget* widgetButtom;
    QWidget* widgetLeft;
    QWidget* widgetRight;

public:
    D_DECLARE_PUBLIC(DTextEdit)
};

/*!
 * \~chinese \brief DTextEdit::DTextEdit 实现一个用于编辑和显示纯文本和富文本的控件
 * \~chinese \param parent
 */
DTextEdit::DTextEdit(QWidget *parent)
    : QTextEdit(parent)
    , DObject(*new DTextEditPrivate(this))
{
    Q_D(DTextEdit);

    viewport()->setAutoFillBackground(false);
    setFrameShape(QFrame::NoFrame);

    d->widgetTop->setFixedWidth(1);
    d->widgetButtom->setFixedWidth(1);
    d->widgetLeft->setFixedHeight(1);
    d->widgetRight->setFixedHeight(1);

    addScrollBarWidget(d->widgetTop, Qt::AlignTop);
    addScrollBarWidget(d->widgetButtom, Qt::AlignBottom);
    addScrollBarWidget(d->widgetLeft, Qt::AlignLeft);
    addScrollBarWidget(d->widgetRight, Qt::AlignRight);
}

DTextEdit::DTextEdit(const QString &text, QWidget *parent)
    : DTextEdit(parent)
{
    setText(text);
}

bool DTextEdit::event(QEvent *e)
{
    Q_D(DTextEdit);

    if (e->type() == QEvent::Paint) {
        QPainter p(this);
        p.setRenderHints(QPainter::Antialiasing);

        QStyleOptionFrame panel;
        initStyleOption(&panel);
        style()->drawPrimitive(QStyle::PE_PanelLineEdit, &panel, &p, this);

        return true;
    } else if (e->type() == QEvent::Polish) {
        DStyleHelper dstyle(style());
        int frame_radius = dstyle.pixelMetric(DStyle::PM_FrameRadius, nullptr, this);
        setViewportMargins(frame_radius, 0, frame_radius, 0);

        d->widgetTop->setFixedHeight(frame_radius);
        d->widgetButtom->setFixedHeight(frame_radius);
        d->widgetLeft->setFixedWidth(frame_radius);
        d->widgetRight->setFixedWidth(frame_radius);
    }

    return QTextEdit::event(e);
}

void DTextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    QDBusInterface testSpeech("com.iflytek.aiassistant",
                               "/aiassistant/tts",
                               "com.iflytek.aiassistant.tts",
                               QDBusConnection::sessionBus());
    //测试朗读接口是否开启
    QDBusReply<bool> speechReply = testSpeech.call(QDBus::AutoDetect, "getTTSEnable");

    QDBusInterface testReading("com.iflytek.aiassistant",
                               "/aiassistant/tts",
                               "com.iflytek.aiassistant.tts",
                               QDBusConnection::sessionBus());
    //测试朗读是否在进行
    QDBusReply<bool> readingReply = testReading.call(QDBus::AutoDetect, "isTTSInWorking");

    QDBusInterface testTranslate("com.iflytek.aiassistant",
                               "/aiassistant/trans",
                               "com.iflytek.aiassistant.trans",
                               QDBusConnection::sessionBus());
    //测试翻译接口是否开启
    QDBusReply<bool> translateReply = testTranslate.call(QDBus::AutoDetect, "getTransEnable");

    //测试服务是否存在
    if (!speechReply.value() && !translateReply.value()) {
        QTextEdit::contextMenuEvent(e);
        return;
    }

    QMenu *menu = createStandardContextMenu();
    menu->addSeparator();

    if (speechReply.value()) {
        QAction *pAction = nullptr;
        if (readingReply.value()) {
            pAction = menu->addAction(tr("Stop reading"));
        } else {
            pAction = menu->addAction(tr("Text to Speech"));
        }

        connect(pAction, &QAction::triggered, this, [] {
            QDBusInterface speechInterface("com.iflytek.aiassistant",
                                 "/aiassistant/deepinmain",
                                 "com.iflytek.aiassistant.mainWindow",
                                 QDBusConnection::sessionBus());

            if (speechInterface.isValid()) {
                speechInterface.call(QDBus::BlockWithGui, "TextToSpeech");//此函在第一次调用时朗读，在朗读状态下再次调用为停止朗读
            } else {
                qWarning() << "[DTextEdit] TextToSpeech ERROR";
            }
        });
    }

    if (translateReply.value()) {
        QAction *pAction_2 = menu->addAction(tr("Translation"));
        connect(pAction_2, &QAction::triggered, this, [] {
            QDBusInterface translationInterface("com.iflytek.aiassistant",
                                 "/aiassistant/deepinmain",
                                 "com.iflytek.aiassistant.mainWindow",
                                 QDBusConnection::sessionBus());

            if (translationInterface.isValid()) {
                translationInterface.call(QDBus::BlockWithGui, "TextToTranslate");//执行翻译
            } else {
                qWarning() << "[DTextEdit] Translation ERROR";
            }
        });
    }

    menu->setAttribute(Qt::WA_DeleteOnClose);
    menu->popup(e->globalPos());
}

DTextEditPrivate::DTextEditPrivate(DTextEdit *qq)
    : DObjectPrivate(qq)
{
    widgetTop = new QWidget(qq);
    widgetButtom = new QWidget(qq);
    widgetLeft = new QWidget(qq);
    widgetRight = new QWidget(qq);
}

DTextEditPrivate::~DTextEditPrivate()
{
}

DWIDGET_END_NAMESPACE
