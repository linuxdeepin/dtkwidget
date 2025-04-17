// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QMenu>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>
#include <QTimer>

#include "dlineedit.h"
#include "private/dlineedit_p.h"
#include "darrowrectangle.h"

#include <DToolTip>
#include <DPalette>
#include <DGuiApplicationHelper>

#define private public
#ifndef slots
#define slots Q_SLOTS
#endif
#include <private/qlineedit_p.h>
#undef private

DWIDGET_BEGIN_NAMESPACE

/*!
@~english
  \class Dtk::Widget::DLineEdit
  \inmodule dtkwidget
  \brief DLineEdit is an input box for aggregating QLineEdit.

  \list
  \li DLineEdit provides the function of inserting controls into the left and right ends of the input box
  \li DLineEdit provides an input box with warning color
  \li DLineEdit provides an input box with a text warning message
  \endlist
 */

/*!
@~english
    \property DLineEdit::alert

    \brief Warning Mode Properties.

    Used to turn on or judge whether it is in warning mode.
 */

/*!
@~english
  \brief Constructor of DLineEdit
  \a parent The parameter is sent to the QWidget constructor.
 */
DLineEdit::DLineEdit(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DLineEditPrivate(this))
{
    D_D(DLineEdit);
    d->init();
}

DLineEdit::~DLineEdit()
{

}

/*!
@~english
  \brief Returns the QLineEdit object.

  If DLineEdit does not meet the use requirements of the input box, 
  please use the object thrown by this function
  \return
 */
QLineEdit *DLineEdit::lineEdit() const
{
    D_DC(DLineEdit);
    return d->lineEdit;
}

DLineEdit::DLineEdit(DLineEditPrivate &q, QWidget *parent)
    : QWidget(parent)
    , DObject(q)
{
    d_func()->init();
}


void DLineEdit::setAlert(bool isAlert)
{
    Q_D(DLineEdit);
    //qDebug() << "setAlert..." << isAlert;
    d->control->setAlert(isAlert);
}

bool DLineEdit::isAlert() const
{
    D_DC(DLineEdit);
    return d->control->isAlert();
}

void DLineEdit::showAlertMessage(const QString &text, int duration)
{
    showAlertMessage(text, nullptr, duration);
}

/*!
@~english
  \brief Display warning message.

  Display the specified text message, and the warning message disappears after the specified time.
  \note When the time parameter is - 1, the warning message will always exist
  \a text Warning text
  \a duration The length of time displayed in milliseconds
  \a follower Tooltip follow
 */
void DLineEdit::showAlertMessage(const QString &text, QWidget *follower, int duration)
{
    D_D(DLineEdit);
    d->control->showAlertMessage(text, follower ? follower : this->lineEdit(), duration);
}

/*!
@~english
  \brief Specify alignment.

  Now only (bottom) left, (bottom) right, (bottom horizontal) center are supported, and left alignment is the default.
  \note When the parameter is other, the default left alignment
  \a alignment Message alignment
 */
void DLineEdit::setAlertAlignment(Qt::Alignment alignment)
{
    D_D(DLineEdit);
    d->control->setAlertAlignment(alignment);
}

Qt::Alignment DLineEdit::alertAlignment() const
{
    D_DC(DLineEdit);
    return d->control->alertAlignment();
}

void DLineEdit::setAlertMessageAlignment(Qt::Alignment alignment)
{
    D_D(DLineEdit);
    d->control->setMessageAlignment(alignment);
}

Qt::Alignment DLineEdit::alertMessageAlignment() const
{
    D_DC(DLineEdit);
    return d->control->messageAlignment();
}

/*!
@~english
  \brief Hide warning message box.

 */

void DLineEdit:: hideAlertMessage()
{
    Q_D(DLineEdit);
    if (d->control) {
        d->control->hideAlertMessage();
    }
//    if (d->frame) {
//        d->frame->hide();
//        if (d->follower) {
//            this->removeEventFilter(d->follower);
//            d->follower = nullptr;
//        }
//    }
}

/*!
@~english
  \brief Add a control to the left of the input box.

  Insert the control in QList to the left of the input box
  \note Call this function several times, and only the last call will take effect
  \a list Stores the list of controls
 */

void DLineEdit::setLeftWidgets(const QList<QWidget *> &list)
{
    Q_D(DLineEdit);

    if (d->leftWidget != nullptr) {
        d->leftWidget->hide();
        d->leftWidget->deleteLater();
        d->leftWidget = nullptr;
    }

    if (list.isEmpty())
        return;

    d->leftWidget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(d->leftWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    d->hLayout->insertWidget(0, d->leftWidget);

    QList<QWidget *>::const_iterator itor;

    for (itor = list.constBegin(); itor != list.constEnd(); itor++) {
        layout->addWidget(*itor);
    }

    d->leftWidget->adjustSize();
}

/*!
@~english
  \brief Add a control to the right of the input box.

  Insert the control in QList to the right of the input box
  \note Call this function several times, and only the last call will take effect
  \a list Stores the list of controls
 */

void DLineEdit::setRightWidgets(const QList<QWidget *> &list)
{
    Q_D(DLineEdit);

    if (d->rightWidget != nullptr) {
        d->rightWidget->hide();
        d->rightWidget->deleteLater();
        d->rightWidget = nullptr;
    }

    if (list.isEmpty())
        return;

    d->rightWidget = new QWidget;
    d->rightWidget->setAccessibleName("DLineEditRightWidget");
    QHBoxLayout *layout = new QHBoxLayout(d->rightWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    d->hLayout->addWidget(d->rightWidget);
    QList<QWidget *>::const_iterator itor;

    for (itor = list.constBegin(); itor != list.constEnd(); itor++) {
        layout->addWidget(*itor);
    }

    d->rightWidget->adjustSize();
}

/*!
@~english
  \brief Whether to hide the control on the left side of the input box.

  \a visible Whether to hide
 */
void DLineEdit::setLeftWidgetsVisible(bool visible)
{
    Q_D(DLineEdit);
    d->leftWidget->setVisible(visible);
}
/*!
@~english
  \brief Whether to hide the control on the right of the input box.

  \a visible Whether to hide
 */
void DLineEdit::setRightWidgetsVisible(bool visible)
{
    Q_D(DLineEdit);
    d->rightWidget->setVisible(visible);
}

/*!
@~english
  \brief Set whether the clear button is visible.

  \a enable True means the button is visible, false means the button is not visible
 */
void DLineEdit::setClearButtonEnabled(bool enable)
{
    Q_D(DLineEdit);
    d->lineEdit->setClearButtonEnabled(enable);

    if (enable)
        if (QToolButton *lineEditClearButton = d->lineEdit->findChild<QToolButton *>())
            lineEditClearButton->setAccessibleName("DLineEditClearButton");
}

/*!
@~english
  \brief Whether the return clear button is visible.

  \return True means the clear button is visible, false means the clear button is not visible
 */
bool DLineEdit::isClearButtonEnabled() const
{
    D_DC(DLineEdit);
    return  d->lineEdit->isClearButtonEnabled();
}

/*!
@~english
  \brief Set the displayed text.

  \a text Displayed text
 */
void DLineEdit::setText(const QString &text)
{
    D_D(DLineEdit);
    d->lineEdit->setText(text);
}

/*!
@~english
  \brief Returns the currently displayed text.

  \return Return the displayed text
 */
QString DLineEdit::text()
{
    D_DC(DLineEdit);
    return d->lineEdit->text();
}

/*!
@~english
  \brief Empty the edited content.
 */
void DLineEdit::clear()
{
    D_D(DLineEdit);
    return d->lineEdit->clear();
}

/*!
@~english
  \brief Returns the echo mode of the input box.

  \return Returns the mode of echo
 */
QLineEdit::EchoMode DLineEdit::echoMode() const
{
    D_DC(DLineEdit);
    return d->lineEdit->echoMode();
}

/*!
@~english
  \brief Set the mode of echo.

  \a mode Echoed mode
 */
void DLineEdit::setEchoMode(QLineEdit::EchoMode mode)
{
    D_D(DLineEdit);
    d->lineEdit->setEchoMode(mode);
}

/*!
@~english
  \brief Set the text menu policy of the line edit control.

  \a policy How to display the right-click menu
  Forwards the ContextMenuEvent message of the actual variable QLineEdit
  \sa QLineEdit::setContextMenuPolicy
 */
void DLineEdit::setContextMenuPolicy(Qt::ContextMenuPolicy policy)
{
    D_D(DLineEdit);
    d->lineEdit->setContextMenuPolicy(policy);
}

/*!
@~english
  \brief Return to whether the voice dictation menu item is displayed.

  \return True to display voice dictation menu items, false to not display
 */
bool DLineEdit::speechToTextIsEnabled() const
{
    D_D(const DLineEdit);
    return d->bSpeechToText;
}

void DLineEdit::setPlaceholderText(const QString& placeholderText)
{
    D_D(DLineEdit);
    d->lineEdit->setPlaceholderText(placeholderText);
}

/*!
@~english
  \brief Set whether to display voice dictation menu items.

  \a enable True is displayed, and the flash is not displayed
 */
void DLineEdit::setSpeechToTextEnabled(bool enable)
{
    D_D(DLineEdit);
    d->bSpeechToText = enable;
}

/*!
@~english
  \brief Return to whether to display voice reading menu item.

  \return True to display voice reading menu items, false to not display
 */
bool DLineEdit::textToSpeechIsEnabled() const
{
    D_D(const DLineEdit);
    return d->bTextToSpeech;
}

/*!
@~english
  \brief Set whether to display voice reading menu items.

  \a enable True is displayed, and the flash is not displayed
 */
void DLineEdit::setTextToSpeechEnabled(bool enable)
{
    D_D(DLineEdit);
    d->bTextToSpeech = enable;
}

/*!
@~english
  \brief Returns whether to display the text translation menu item.

  \return True to display the text translation menu item, false to not display
 */
bool DLineEdit::textToTranslateIsEnabled() const
{
    D_D(const DLineEdit);
    return d->bTextToTranslate;
}

/*!
@~english
  \brief Set whether to display the text translation menu item

  \a enable True to display, false to not display
 */
void DLineEdit::setTextToTranslateEnabled(bool enable)
{
    D_D(DLineEdit);
    d->bTextToTranslate = enable;
}

/*!
@~english
  \brief Returns whether the input text can be cut
  \return True means the text can be cut, false means it cannot be cut
 */
bool DLineEdit::cutEnabled() const
{
    D_DC(DLineEdit);
    return d->bEnableCut;
}

/*!
@~english
  \brief Set whether the input text can be cut
  \a enabled　True means the input text can be cut, false means it cannot be cut
 */
void DLineEdit::setCutEnabled(bool enable)
{
    D_D(DLineEdit);
    d->bEnableCut = enable;
}

/*!
@~english
  \brief Returns whether the input text can be copied
  \return True means that the text can be copied, and false means that it cannot be copied
 */
bool DLineEdit::copyEnabled() const
{
    D_DC(DLineEdit);
    return d->bEnableCopy;
}

/*!
@~english
  \brief Set whether the input text can be copied
  \a enabled　True means the input text can be copied, false means it cannot be copied
 */
void DLineEdit::setCopyEnabled(bool enable)
{
    D_D(DLineEdit);
    d->bEnableCopy = enable;
}

/*!
@~english
  \brief Event filter

  \a watched Listened child control pointer， \a event Events to be filtered \a event example.

  The filter does not perform any filtering action, but will monitor the focus status of the input box and send the signal focusChanged ()

  \return If filtering is successful, return true; otherwise, return false .
 */
bool DLineEdit::eventFilter(QObject *watched, QEvent *event)
{
    D_D(DLineEdit);

    if (event->type() == QEvent::FocusIn) {
        if (DGuiApplicationHelper::IsTableEnvironment && QGuiApplication::inputMethod())
            QGuiApplication::inputMethod()->show();
        Q_EMIT focusChanged(true);
    } else if (event->type() == QEvent::FocusOut) {
        Q_EMIT focusChanged(false);
    } else if (watched == lineEdit() && event->type() == QEvent::KeyPress) {
        QKeyEvent *pKeyEvent = static_cast<QKeyEvent *>(event);

        if (pKeyEvent == QKeySequence::Copy && !copyEnabled()) {
            return true;
        }
        if (pKeyEvent == QKeySequence::Cut && !cutEnabled()) {
            return true;
        }

        if (pKeyEvent == QKeySequence::SelectAll) {
            QApplication::clipboard()->setText(lineEdit()->text(), QClipboard::Mode::Selection);
        }
    } else if (watched == lineEdit() && event->type() == QEvent::ContextMenu && lineEdit()->contextMenuPolicy() == Qt::DefaultContextMenu) {
        QLineEdit *le = static_cast<QLineEdit *>(watched);
        if (!le->isEnabled() || le->echoMode() == QLineEdit::Password ||
                (!textToSpeechIsEnabled() && !textToTranslateIsEnabled() && !speechToTextIsEnabled())) {
            return QWidget::eventFilter(watched, event);
        }

        QLineEdit *pLineEdit = static_cast<QLineEdit*>(watched);
        QMenu *menu = pLineEdit->createStandardContextMenu();

        for (QAction *action : menu->actions()) {
            if (action->text().startsWith(QLineEdit::tr("&Copy")) && !copyEnabled()  ) {
                action->setEnabled(false);
            }
            if (action->text().startsWith(QLineEdit::tr("Cu&t")) && !cutEnabled()) {
                action->setEnabled(false);
            }
        }
        connect(menu, &QMenu::triggered, this, [pLineEdit](QAction *pAction) {
            if (pAction->text().startsWith(QLineEdit::tr("Select All"))) {
                QApplication::clipboard()->setText(pLineEdit->text(), QClipboard::Mode::Selection);
            }
        });

        auto msg = QDBusMessage::createMethodCall("com.iflytek.aiassistant", "/",
                                       "org.freedesktop.DBus.Peer", "Ping");
        // 用之前 Ping 一下, 300ms 内没回复就认定是服务出问题，不再添加助手菜单项
        // Fix：Bug-154857 此处不能使用 BlockWithGui 否则右键事件会被处理，事件传递到
        //  DSearchEdit 上会导致 edit 获得焦点然后菜单弹出后又失去焦点，有闪烁现象。。
        auto pingReply = QDBusConnection::sessionBus().call(msg, QDBus::Block, 300);
        auto errorType = QDBusConnection::sessionBus().lastError().type();
        if (errorType == QDBusError::Timeout || errorType == QDBusError::NoReply) {
            qWarning() << pingReply << "\nwill not add aiassistant actions!";
            menu->popup(static_cast<QContextMenuEvent*>(event)->globalPos());
            event->accept();
            return true;
        }

        QDBusInterface testSpeech("com.iflytek.aiassistant",
                                   "/aiassistant/tts",
                                   "com.iflytek.aiassistant.tts",
                                   QDBusConnection::sessionBus());
        // 测试朗读接口是否开启
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

        QDBusInterface testSpeechToText("com.iflytek.aiassistant",
                                   "/aiassistant/iat",
                                   "com.iflytek.aiassistant.iat",
                                   QDBusConnection::sessionBus());
        //测试听写接口是否开启
        QDBusReply<bool> speechToTextReply = testSpeechToText.call(QDBus::AutoDetect, "getIatEnable");

        //朗读,翻译,听写都没有开启，则弹出默认菜单
        if (!speechReply.value() && !translateReply.value() && !speechToTextReply.value()) {
            menu->popup(static_cast<QContextMenuEvent*>(event)->globalPos());
            event->accept();
            return true;
        }

        menu->addSeparator();

        if (speechReply.value() && textToSpeechIsEnabled()) {
            QAction *pAction_1 = nullptr;
            if (readingReply.value()) {
                pAction_1 = menu->addAction(QCoreApplication::translate("DLineEdit", "Stop reading"));
            } else {
                pAction_1 = menu->addAction(QCoreApplication::translate("DLineEdit", "Text to Speech"));
            }

            //没有选中文本，则菜单置灰色
            if (!pLineEdit->hasSelectedText()) {
                pAction_1->setEnabled(false);
            }

            connect(pAction_1, &QAction::triggered, this, [] {
                QDBusInterface speechInterface("com.iflytek.aiassistant",
                                                "/aiassistant/deepinmain",
                                                "com.iflytek.aiassistant.mainWindow",
                                                QDBusConnection::sessionBus());

                if (speechInterface.isValid()) {
                    speechInterface.call(QDBus::BlockWithGui, "TextToSpeech");//此函在第一次调用时朗读，在朗读状态下再次调用为停止朗读
                } else {
                    qWarning() << "[DLineEdit] TextToSpeech ERROR";
                }
            });
        }

        if (translateReply.value() && textToTranslateIsEnabled()) {
            QAction *pAction_2 = menu->addAction(QCoreApplication::translate("DLineEdit", "Translate"));

            //没有选中文本，则菜单置灰色
            if (!pLineEdit->hasSelectedText()) {
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
                    qWarning() << "[DLineEdit] Translation ERROR";
                }
            });
        }

        if (speechToTextReply.value() && speechToTextIsEnabled()) {
            QAction *pAction_3 = menu->addAction(QCoreApplication::translate("DLineEdit", "Speech To Text"));
            connect(pAction_3, &QAction::triggered, this, [] {
                QDBusInterface speechToTextInterface("com.iflytek.aiassistant",
                                     "/aiassistant/deepinmain",
                                     "com.iflytek.aiassistant.mainWindow",
                                     QDBusConnection::sessionBus());

                if (speechToTextInterface.isValid()) {
                    speechToTextInterface.call(QDBus::BlockWithGui, "SpeechToText");//执行听写
                } else {
                    qWarning() << "[DLineEdit] SpeechToText ERROR";
                }
            });
        }

        //FIXME: 由于Qt在UOS系统环境下不明原因的bug,使用menu->setAttribute(Qt::WA_DeleteOnClose) 销毁menu会在特定情况下出现崩溃的问题，这里采用一种变通的做法
        connect(menu, &QMenu::aboutToHide, this, [=] {
            if (menu->activeAction()) {
                menu->deleteLater();
            } else {
                QTimer::singleShot(0, this, [=] {
                    menu->deleteLater();
                });
            }
        });

        menu->popup(static_cast<QContextMenuEvent*>(event)->globalPos());
        event->accept();
        pLineEdit->setFocus();
        return true;
    }

//    if (d->frame)
//    {
//        if (watched == d->follower && event->type() == QEvent::Move) {
//            d->updateTooltipPos();
//        }
//    }

    return QWidget::eventFilter(watched, event);
}

bool DLineEdit::event(QEvent *e)
{
    //D_D(DLineEdit);

//    if (e->type() == QEvent::Move || e->type() == QEvent::Resize) {
//        if (d->frame)
//            d->updateTooltipPos();
//    }
    return QWidget::event(e);
}

DLineEditPrivate::DLineEditPrivate(DLineEdit *q)
    : DObjectPrivate(q)
    , leftWidget(nullptr)
    , rightWidget(nullptr)
    , lineEdit(nullptr)
    , hLayout(nullptr)
{

}

void DLineEditPrivate::updateTooltipPos()
{
    //control->updateTooltipPos();
}

void DLineEditPrivate::init()
{
    Q_Q(DLineEdit);

    hLayout = new QHBoxLayout(q);
    lineEdit = new QLineEdit(q);
    q->setFocusProxy(lineEdit); // fix DlineEdit setFocut but lineEdit can not edit(without focus rect)
    q->setFocusPolicy(lineEdit->focusPolicy());

    control = new DAlertControl(lineEdit, q);
    q->connect(control, &DAlertControl::alertChanged, q, &DLineEdit::alertChanged);

    hLayout->setContentsMargins(0, 0, 0, 0);
    // Set spacing to 10 for between widget.
    hLayout->setSpacing(10);
    hLayout->addWidget(lineEdit);

    lineEdit->installEventFilter(q);
    lineEdit->setAccessibleName("DLineEditChildLineEdit");
    q->setClearButtonEnabled(true);

    q->connect(lineEdit, &QLineEdit::textChanged, q, &DLineEdit::textChanged);
    q->connect(lineEdit, &QLineEdit::textEdited, q, &DLineEdit::textEdited);
    q->connect(lineEdit, &QLineEdit::cursorPositionChanged, q, &DLineEdit::cursorPositionChanged);
    q->connect(lineEdit, &QLineEdit::returnPressed, q, &DLineEdit::returnPressed);
    q->connect(lineEdit, &QLineEdit::editingFinished, q, &DLineEdit::editingFinished);
    q->connect(lineEdit, &QLineEdit::selectionChanged, q, &DLineEdit::selectionChanged);
}

DWIDGET_END_NAMESPACE
