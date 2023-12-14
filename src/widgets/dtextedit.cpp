// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dtextedit.h"

#include <QPainter>
#include <QEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QClipboard>
#include <QKeySequence>

#include <private/qtextedit_p.h>

#include <DStyle>
#include <DObjectPrivate>
#include <DGuiApplicationHelper>
DGUI_USE_NAMESPACE

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

    bool bSpeechToText = true;
    bool bTextToSpeech = true;
    bool bTextToTranslate = true;

public:
    D_DECLARE_PUBLIC(DTextEdit)
};

/*!
@~english
  @brief DTextEdit constructor, implementing a widget for editing and displaying both plain and rich text
  @param[in] parent is passed to QTextEdit
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
        int frame_radius = dstyle.pixelMetric(DStyle::PM_FrameRadius, nullptr, this) / 2;
        setViewportMargins(frame_radius, 0, frame_radius, 0);

        d->widgetTop->setFixedHeight(frame_radius);
        d->widgetButtom->setFixedHeight(frame_radius);
        d->widgetLeft->setFixedWidth(frame_radius);
        d->widgetRight->setFixedWidth(frame_radius);
    } else if (e->type() == QEvent::InputMethodQuery && testAttribute(Qt::WA_InputMethodEnabled)) {
        QInputMethodQueryEvent *query = static_cast<QInputMethodQueryEvent *>(e);
        Qt::InputMethodQueries queries = query->queries();
        for (uint i = 0; i < 32; ++i) {
            // cppcheck-suppress shiftTooManyBitsSigned
            Qt::InputMethodQuery property = (Qt::InputMethodQuery(uint(queries & (1<<i))));
            if (property == Qt::ImCursorRectangle) {
                QRect rc = cursorRect();
                DStyleHelper dstyle(style());
                int frame_radius = dstyle.pixelMetric(DStyle::PM_FrameRadius, nullptr, this);
                // FIX bug-79676 setViewportMargins 会导致光标位置异常，此处调整回来吧
                // FIX bug-79676 setViewportMargins cause cursor position abnormal, fix it plz
                rc.adjust(frame_radius, 0, frame_radius, 0);
                query->setValue(property, rc);
                query->accept();
                return true;
            } else if (property == Qt::ImAnchorRectangle) {
                QTextEditPrivate *d = reinterpret_cast<QTextEditPrivate *>(qGetPtrHelper(d_ptr));
                QRect anchorRect = d->control->inputMethodQuery(Qt::ImAnchorRectangle, true).toRect();

                DStyleHelper dstyle(style());
                int frame_radius = dstyle.pixelMetric(DStyle::PM_FrameRadius, nullptr, this);

                anchorRect.adjust(frame_radius, 0, frame_radius, 0);
                query->setValue(Qt::ImAnchorRectangle, anchorRect);
                query->accept();
                return true;
            }
        }
    } else if ((e->type() == QEvent::WindowActivate || e->type() == QEvent::WindowDeactivate) &&
               DGuiApplicationHelper::testAttribute(DGuiApplicationHelper::UseInactiveColorGroup)) {
        // only work for DTextEdit without setAttribute UseInactiveColorGroup false
        // TODO: remove me if QTextEdit fix it
        QTextEditPrivate *d = reinterpret_cast<QTextEditPrivate *>(qGetPtrHelper(d_ptr));
        d->control->setPalette(palette());
    }

    return QTextEdit::event(e);
}

void DTextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event == QKeySequence::SelectAll) {
        QApplication::clipboard()->setText(this->toPlainText(), QClipboard::Mode::Selection);
    }

    QTextEdit::keyPressEvent(event);
}

void DTextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    auto msg = QDBusMessage::createMethodCall("com.iflytek.aiassistant", "/",
                                   "org.freedesktop.DBus.Peer", "Ping");
    // 用之前 Ping 一下, 300ms 内没回复就认定是服务出问题，不再添加助手菜单项
    // ping before using it, consider no response within 300ms as a service issue, do not add menu anymore
    auto pingReply = QDBusConnection::sessionBus().call(msg, QDBus::Block, 300);
    auto errorType = QDBusConnection::sessionBus().lastError().type();
    if (errorType == QDBusError::Timeout || errorType == QDBusError::NoReply) {
        qWarning() << pingReply << "\nwill not add aiassistant actions!";
        return QTextEdit::contextMenuEvent(e);
    }

    QDBusInterface testSpeech("com.iflytek.aiassistant",
                               "/aiassistant/tts",
                               "com.iflytek.aiassistant.tts",
                               QDBusConnection::sessionBus());
    //测试朗读接口是否开启
    //test whether textToSpeech interface is enabled
    QDBusReply<bool> speechReply = testSpeech.call(QDBus::AutoDetect, "getTTSEnable");

    QDBusInterface testReading("com.iflytek.aiassistant",
                               "/aiassistant/tts",
                               "com.iflytek.aiassistant.tts",
                               QDBusConnection::sessionBus());
    //测试朗读是否在进行
    //test whether textToSpeech is ongoing
    QDBusReply<bool> readingReply = testReading.call(QDBus::AutoDetect, "isTTSInWorking");

    QDBusInterface testTranslate("com.iflytek.aiassistant",
                               "/aiassistant/trans",
                               "com.iflytek.aiassistant.trans",
                               QDBusConnection::sessionBus());
    //测试翻译接口是否开启
    //test whether translating interface is enabled
    QDBusReply<bool> translateReply = testTranslate.call(QDBus::AutoDetect, "getTransEnable");

    QDBusInterface testSpeechToText("com.iflytek.aiassistant",
                               "/aiassistant/iat",
                               "com.iflytek.aiassistant.iat",
                               QDBusConnection::sessionBus());
    //测试听写接口是否开启
    //test whether speechToText interface is enabled
    QDBusReply<bool> speechToTextReply = testSpeechToText.call(QDBus::AutoDetect, "getIatEnable");

    //test whether the service exists
    if ((!speechReply.value() && !translateReply.value() && !speechToTextReply.value())
            || (!textToSpeechIsEnabled() && !textToTranslateIsEnabled() && !speechToTextIsEnabled())) {
        QTextEdit::contextMenuEvent(e);
        return;
    }

    QMenu *menu = createStandardContextMenu();
    if (!menu)
        return QTextEdit::contextMenuEvent(e);

    menu->addSeparator();

    connect(menu, &QMenu::triggered, this, [this](QAction *pAction) {
        if (pAction->text().startsWith(qApp->translate("QWidgetTextControl", "Select All"))) {
            QApplication::clipboard()->setText(this->toPlainText(), QClipboard::Mode::Selection);
        }
    });

    if (speechReply.value() && textToSpeechIsEnabled()) {
        QAction *pAction = nullptr;
        if (readingReply.value()) {
            pAction = menu->addAction(QCoreApplication::translate("DTextEdit", "Stop reading"));
        } else {
            pAction = menu->addAction(QCoreApplication::translate("DTextEdit", "Text to Speech"));
        }

        //没有选中文本，则菜单置灰色
        //set the menu in color gray if no text is selected
        if (textCursor().selectedText().isEmpty()) {
            pAction->setEnabled(false);
        }

        connect(pAction, &QAction::triggered, this, [] {
            QDBusInterface speechInterface("com.iflytek.aiassistant",
                                 "/aiassistant/deepinmain",
                                 "com.iflytek.aiassistant.mainWindow",
                                 QDBusConnection::sessionBus());

            if (speechInterface.isValid()) {
                speechInterface.call(QDBus::BlockWithGui, "TextToSpeech");
                //此函在第一次调用时朗读，在朗读状态下再次调用为停止朗读
                //the first call means start textToSpeech, a second call means stop textToSpeech
            } else {
                qWarning() << "[DTextEdit] TextToSpeech ERROR";
            }
        });
    }

    if (translateReply.value() && textToTranslateIsEnabled()) {
        QAction *pAction_2 = menu->addAction(QCoreApplication::translate("DTextEdit", "Translate"));

        //没有选中文本，则菜单置灰色
        //set the menu in color gray if no text is selected
        if (textCursor().selectedText().isEmpty()) {
            pAction_2->setEnabled(false);
        }

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

    if (speechToTextReply.value() && speechToTextIsEnabled()) {
        QAction *pAction_3 = menu->addAction(QCoreApplication::translate("DTextEdit", "Speech To Text"));
        connect(pAction_3, &QAction::triggered, this, [] {
            QDBusInterface speechToTextInterface("com.iflytek.aiassistant",
                                 "/aiassistant/deepinmain",
                                 "com.iflytek.aiassistant.mainWindow",
                                 QDBusConnection::sessionBus());

            if (speechToTextInterface.isValid()) {
                speechToTextInterface.call(QDBus::BlockWithGui, "SpeechToText");//执行听写
            } else {
                qWarning() << "[DTextEdit] SpeechToText ERROR";
            }
        });
    }

    //FIXME: 由于Qt在UOS系统环境下不明原因的bug,使用menu->setAttribute(Qt::WA_DeleteOnClose) 销毁menu会在特定情况下出现崩溃的问题，这里采用一种变通的做法
    //FIXME: Due to an unknown bug in Qt in the UOS system,using menu ->setAttribute (Qt:: WA-DeleteOnClose) to destroy a menu may cause a crash issue in specific situations, thus using another way to do it
    connect(menu, &QMenu::aboutToHide, this, [=] {
        if (menu->activeAction()) {
            menu->deleteLater();
        } else {
            QTimer::singleShot(0, this, [=] {
                menu->deleteLater();
            });
        }
    });

    menu->popup(e->globalPos());
}

/*!
@~english
  @brief DLineEdit::speechToTextIsEnabled return whether speechToText is enabled
  @return true means speechToText is enabled, while false means not
 */
bool DTextEdit::speechToTextIsEnabled() const
{
    D_D(const DTextEdit);
    return d->bSpeechToText;
}

/*!
@~english
  @brief DLineEdit::setSpeechToTextEnabled whether speechToText should be enabled
  @param[in] enable true means yes, while flase means no
 */
void DTextEdit::setSpeechToTextEnabled(bool enable)
{
    D_D(DTextEdit);
    d->bSpeechToText = enable;
}

/*!
@~english
  @brief DTextEdit::textToSpeechIsEnabled return whether textToSpeech is enabled
  @return true means enabled, while false means not
 */
bool DTextEdit::textToSpeechIsEnabled() const
{
    D_D(const DTextEdit);
    return d->bTextToSpeech;
}

/*!
@~english
  @brief DTextEdit::setTextToSpeechEnabled set whether textToSpeech should be enabled
  @param[in] enable true means enabled, while flase means not
 */
void DTextEdit::setTextToSpeechEnabled(bool enable)
{
    D_D(DTextEdit);
    d->bTextToSpeech = enable;
}

/*!
@~english
  @brief DTextEdit::textToTranslateIsEnabled return whether textToTranslate is enabled
  @return true means enabled, while false means not
 */
bool DTextEdit::textToTranslateIsEnabled() const
{
    D_D(const DTextEdit);
    return d->bTextToTranslate;
}

/*!
@~english
  @brief DTextEdit::setTextToTranslateEnabled set whether textToTranslate is enabled
  @param[in] true means enabled, while false means not
 */
void DTextEdit::setTextToTranslateEnabled(bool enable)
{
    D_D(DTextEdit);
    d->bTextToTranslate = enable;
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
