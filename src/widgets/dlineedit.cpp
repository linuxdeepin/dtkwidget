/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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
  \class Dtk::Widget::DLineEdit
  \inmodule dtkwidget
  \brief DLineEdit一个聚合 QLineEdit 的输入框.

  \list
  \li DLineEdit提供了向输入框左右两端插入控件的函数
  \li DLineEdit提供了带警告颜色的输入框
  \li DLineEdit提供了带文本警告消息的输入框
  \endlist
 */

/*!
    \property DLineEdit::alert

    \brief 警告模式属性.

    用于开启或者判断是否处于警告模式.
 */

/*!
  \brief DLineEdit的构造函数
  \a parent 参数被发送到 QWidget 构造函数。
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
  \brief 返回 QLineEdit 对象.

  若 DLineEdit 不满足输入框的使用需求，请用此函数抛出的对象
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
  \brief 显示警告消息.

  显示指定的文本消息，超过指定时间后警告消息消失.
  \note 时间参数为-1时，警告消息将一直存在
  \a text 警告的文本
  \a duration 显示的时间长度，单位毫秒
  \a follower tooltip跟随
 */
void DLineEdit::showAlertMessage(const QString &text, QWidget *follower, int duration)
{
    D_D(DLineEdit);
    d->control->showAlertMessage(text, follower ? follower : this, duration);
}

/*!
  \brief 指定对齐方式.

  现只支持(下)左，(下)右，(下水平)居中， 默认左对齐.
  \note 参数为其他时，默认左对齐
  \a alignment 消息对齐方式
 */
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
  \brief 隐藏警告消息框.

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
  \brief 向输入框左侧添加控件.

  将 QList 里的控件插入到输入框的左侧
  \note 多次调用，只有最后一次调用生效
  \a list 存储控件的列表
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
  \brief 向输入框右侧添加控件.

  将 QList 里的控件插入到输入框的右侧
  \note 多次调用，只有最后一次调用生效
  \a list 存储控件的列表
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
  \brief 是否隐藏输入框左侧控件.

  \a visible 是否隐藏
 */
void DLineEdit::setLeftWidgetsVisible(bool visible)
{
    Q_D(DLineEdit);
    d->leftWidget->setVisible(visible);
}
/*!
  \brief 是否隐藏输入框右侧控件.

  \a visible 是否隐藏
 */
void DLineEdit::setRightWidgetsVisible(bool visible)
{
    Q_D(DLineEdit);
    d->rightWidget->setVisible(visible);
}

/*!
  \brief 设置清除按钮是否可见.

  \a enable true 按钮可见 false 按钮不可见
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
  \brief 返回清除按钮是否可见.

  \return true 清除按钮可见 false 清除按钮不可见
 */
bool DLineEdit::isClearButtonEnabled() const
{
    D_DC(DLineEdit);
    return  d->lineEdit->isClearButtonEnabled();
}

/*!
  \brief 设置显示的文本.

  \a text 显示的文本
 */
void DLineEdit::setText(const QString &text)
{
    D_D(DLineEdit);
    d->lineEdit->setText(text);
}

/*!
  \brief 返回当前显示的文本.

  \return 返回显示的文本
 */
QString DLineEdit::text()
{
    D_DC(DLineEdit);
    return d->lineEdit->text();
}

/*!
  \brief 清空编辑的内容.
 */
void DLineEdit::clear()
{
    D_D(DLineEdit);
    return d->lineEdit->clear();
}

/*!
  \brief 返回输入框的回显模式.

  \return 返回回显的模式
 */
QLineEdit::EchoMode DLineEdit::echoMode() const
{
    D_DC(DLineEdit);
    return d->lineEdit->echoMode();
}

/*!
  \brief 设置回显的模式.

  \a mode 回显的模式
 */
void DLineEdit::setEchoMode(QLineEdit::EchoMode mode)
{
    D_D(DLineEdit);
    d->lineEdit->setEchoMode(mode);
}

/*!
  \brief 设置行编辑控件的文本菜单策略.

  \a policy 显示右键菜单的方式
  转发实际变量 QLineEdit 的 ContextMenuEvent 消息
  \sa QLineEdit::setContextMenuPolicy
 */
void DLineEdit::setContextMenuPolicy(Qt::ContextMenuPolicy policy)
{
    D_D(DLineEdit);
    d->lineEdit->setContextMenuPolicy(policy);
}

/*!
  \brief 返回是否显示语音听写菜单项.

  \return true 显示语音听写菜单项 false不显示
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
  \brief 设置是否显示语音听写菜单项.

  \a enable true显示 flase不显示
 */
void DLineEdit::setSpeechToTextEnabled(bool enable)
{
    D_D(DLineEdit);
    d->bSpeechToText = enable;
}

/*!
  \brief 返回是否显示语音朗读菜单项.

  \return true 显示语音朗读菜单项 false不显示
 */
bool DLineEdit::textToSpeechIsEnabled() const
{
    D_D(const DLineEdit);
    return d->bTextToSpeech;
}

/*!
  \brief 设置是否显示语音朗读菜单项.

  \a enable true显示 flase不显示
 */
void DLineEdit::setTextToSpeechEnabled(bool enable)
{
    D_D(DLineEdit);
    d->bTextToSpeech = enable;
}

/*!
  \brief 返回是否显示文本翻译菜单项.

  \return true 显示文本翻译菜单项 false 不显示
 */
bool DLineEdit::textToTranslateIsEnabled() const
{
    D_D(const DLineEdit);
    return d->bTextToTranslate;
}

/*!
  \brief 设置是否显示文本翻译菜单项

  \a enable true显示 flase不显示
 */
void DLineEdit::setTextToTranslateEnabled(bool enable)
{
    D_D(DLineEdit);
    d->bTextToTranslate = enable;
}

/*!
  \brief DLineEdit::cutEnabled
  \return true文本可剪切　false不可剪切
 */
bool DLineEdit::cutEnabled() const
{
    D_DC(DLineEdit);
    return d->bEnableCut;
}

/*!
  \brief DLineEdit::setCutEnabled 设置输入文本是否可拷贝
  \a enabled　true输入文本可剪切　false不可剪切
 */
void DLineEdit::setCutEnabled(bool enable)
{
    D_D(DLineEdit);
    d->bEnableCut = enable;
}

/*!
  \brief DLineEdit::copyEnabled
  \return true文本可拷贝　false不可拷贝
 */
bool DLineEdit::copyEnabled() const
{
    D_DC(DLineEdit);
    return d->bEnableCopy;
}

/*!
  \brief DLineEdit::setCopyEnabled 设置输入文本是否可拷贝
  \a enabled　true输入文本可拷贝　false不可拷贝
 */
void DLineEdit::setCopyEnabled(bool enable)
{
    D_D(DLineEdit);
    d->bEnableCopy = enable;
}

/*!
  \brief 事件过滤器

  \a watched 被监听的子控件指针， \a event 待过滤的事件 \a event 实例.

  该过滤器不做任何过滤动作，但会监控输入框的焦点状态，并发送信号 focusChanged()。

  \return 成功过滤返回 true，否则返回 false .
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

        QDBusInterface testSpeechToText("com.iflytek.aiassistant",
                                   "/aiassistant/iat",
                                   "com.iflytek.aiassistant.iat",
                                   QDBusConnection::sessionBus());
        //测试听写接口是否开启
        QDBusReply<bool> speechToTextReply = testSpeechToText.call(QDBus::AutoDetect, "getIatEnable");

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
