// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QRegularExpressionValidator>
#include <QGuiApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QDebug>
#include <QLabel>
#include <DFrame>
#include <DStyle>

#include "dthememanager.h"
#include "dipv4lineedit.h"
#include "private/dipv4lineedit_p.h"

DWIDGET_BEGIN_NAMESPACE

#define RX_PATTERN_IP "^(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)?$"

DIpv4LineEditPrivate::DIpv4LineEditPrivate(DIpv4LineEdit *parent)
        : DObjectPrivate(parent)
{

}

void DIpv4LineEditPrivate::init()
{
    D_Q(DIpv4LineEdit);

    // 和子edit保持一致不显示菜单
    q->setContextMenuPolicy(Qt::NoContextMenu);
    // 带圆角的自绘制背景的frame，挡住背后的edit文字
    DFrame *frame = new DFrame(q);
    frame->setAccessibleName("DIpv4LineEditFrame");
    frame->setFrameRounded(true);
    frame->setFrameShape(QFrame::NoFrame);
    editMainWidget = frame;

    editMainWidget->setBackgroundRole(QPalette::Button);
    editMainWidget->setAutoFillBackground(true);

    QHBoxLayout *hbox_layout = new QHBoxLayout;

    hbox_layout->setSpacing(0);
    hbox_layout->setMargin(0);
    hbox_layout->addWidget(getEdit());
    hbox_layout->addWidget(getLabel());
    hbox_layout->addWidget(getEdit());
    hbox_layout->addWidget(getLabel());
    hbox_layout->addWidget(getEdit());
    hbox_layout->addWidget(getLabel());
    hbox_layout->addWidget(getEdit());

    editMainWidget->setLayout(hbox_layout);

    q->QLineEdit::setReadOnly(true);
    q->setFocusProxy(editList.first());
    q->setValidator(new QRegularExpressionValidator(QRegularExpression("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)?\\.){0,3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)?"), q));

    QObject::connect(q, SIGNAL(textChanged(QString)), q, SLOT(_q_setIpLineEditText(QString)), Qt::QueuedConnection);
}

QLabel *DIpv4LineEditPrivate::getLabel()
{
    QLabel *label = new QLabel(".");

    label->setObjectName("DIpv4LineEdit_Label");
    label->setCursor(Qt::IBeamCursor);

    return label;
}

QLineEdit *DIpv4LineEditPrivate::getEdit()
{
    D_Q(DIpv4LineEdit);

    QLineEdit *edit = new QLineEdit;

    edit->setObjectName("DIpv4LineEdit_Edit");
    edit->setValidator(new QRegularExpressionValidator(QRegularExpression(RX_PATTERN_IP), edit));
    edit->setAlignment(Qt::AlignHCenter);
    edit->setAttribute(Qt::WA_InputMethodEnabled, false);
    edit->setContextMenuPolicy(Qt::NoContextMenu);
    edit->installEventFilter(q);
    // 禁止绘制子edit的焦点框
    edit->setProperty("_d_dtk_noFocusRect", true);

    editList << edit;

    QObject::connect(edit, SIGNAL(textEdited(QString)), q, SLOT(_q_updateLineEditText()));

    return edit;
}

void DIpv4LineEditPrivate::setFocus(bool focus)
{
    D_Q(DIpv4LineEdit);

    if (this->focus == focus)
        return;

    this->focus = focus;

    // 焦点改变时边框重绘一下
    q->update();
    Q_EMIT q->focusChanged(focus);
}

void DIpv4LineEditPrivate::_q_updateLineEditText()
{
    D_Q(DIpv4LineEdit);
    QString text;

    for (const QLineEdit *edit : editList) {
        text.append(".").append(edit->text());
    }

    QObject::disconnect(q, SIGNAL(textChanged(QString)), q, SLOT(_q_setIpLineEditText(QString)));

    if (text == "....") {
        if (!q->text().isEmpty()) {
            q->setText("");
            Q_EMIT q->textEdited(q->text());
        }
    } else {
        text = text.mid(1);

        if (text != q->text()) {
            q->setText(text);
            Q_EMIT q->textEdited(q->text());
        }
    }

    QObject::connect(q, SIGNAL(textChanged(QString)), q, SLOT(_q_setIpLineEditText(QString)), Qt::QueuedConnection);

    q->QLineEdit::setCursorPosition(q->cursorPosition());
}

void DIpv4LineEditPrivate::_q_setIpLineEditText(const QString &text)
{
    D_Q(DIpv4LineEdit);

    int pos = 0;

    if (q->validator()->validate(const_cast<QString &>(text), pos) != QValidator::Acceptable) {
        _q_updateLineEditText();
        return;
    }

    const QStringList text_list = text.split('.');

    int min_count = qMin(editList.count(), text_list.count());

    for (int i = 0; i < min_count; ++i) {
        QLineEdit *edit = editList[i];
        bool edit_blocked = edit->blockSignals(true);
        edit->setText(text_list[i]);
        edit->blockSignals(edit_blocked);
    }

    for (int i = min_count; i < editList.count(); ++i) {
        QLineEdit *edit = editList[i];
        bool edit_blocked = edit->blockSignals(true);
        edit->clear();
        edit->blockSignals(edit_blocked);
    }
}

/*!
  \class Dtk::Widget::DIpv4LineEdit
  \inmodule dtkwidget
  \brief 类DIpv4LineEdit是专门用于输入ipv4格式文本的编辑框.
  
  此类内部由四个 \l QLineEdit 组合而成，输入框之间可以通过Tab/Shift+Tab快捷切
  换光标，输入字符"."时会自动跳到下一个输入框。在使用左右方向键移动光标时会自动跨
  越输入框，和在同一个输入框内光标的没有区别。和 \l QLineEdit 一样也可以使用快捷
  键方便的复制粘贴文本，在粘贴时会自动根据ipv4的规则过滤掉非法字符。每个输入框的文
  本内容必须要满足以下的正则规则：
  \code{.unparsed}
  ^(2[0-4]\d|25[0-5]|[01]?\d\d?)?$
  \endcode
  \warning 不能使用DLineEdit和QLineEdit中的功能性函数
 */

/*!
  \property DIpv4LineEdit::selectedText
  \brief 选中的文本内容
  \note 只读
  \sa QLineEdit::selectedText
  */

/*!
  \property DIpv4LineEdit::placeholderText
  \brief 输入框未输入内容时的占位文本，通常显示为灰色，当输入任意字符后就会被隐藏
  \note 只读。默认为空字符串
  \sa QLineEdit::placeholderText
  */

/*!
  \property DIpv4LineEdit::clearButtonEnabled
  \brief 是否在输入框上显示清理按钮
  \note 只读。默认为 false
  \sa QLineEdit::clearButtonEnabled
  */

/*!
  \brief 构造DIpv4LineEdit类，和普通 \l QWidget 一样，可以传入一个QWidget指
  针对象作为其父对象
  管理的控件
  \a parent
 */
DIpv4LineEdit::DIpv4LineEdit(QWidget *parent)
    : QLineEdit(parent)
    , DObject(*new DIpv4LineEditPrivate(this))
{
    d_func()->init();
}

/*!
  \property DIpv4LineEdit::displayText
  \brief 输入框显示的文本内容
  
  和 QLineEdit::displayText 不同，无论 \l echoMode 值为多少，都返回和 \l text 同样的内容
  
  \note 可读可写
  \sa QLineEdit::echoMode QLineEdit::text
 */

/*!
  \return
 */
QString DIpv4LineEdit::displayText() const
{
    return text();
}

/*!
  \property DIpv4LineEdit::cursorPosition
  \brief 当前光标所在位置
  
  \note 可读可写
  \sa QLineEdit::cursorPosition
 */

/*!
  \return
 */
int DIpv4LineEdit::cursorPosition() const
{
    D_DC(DIpv4LineEdit);

    int cursorPosition = 0;

    for (const QLineEdit *edit : d->editList) {
        if (edit->hasFocus()) {
            cursorPosition += edit->cursorPosition();
            break;
        } else {
            cursorPosition += edit->text().count() + 1;
        }
    }

    return cursorPosition;
}

/*!
  \property DIpv4LineEdit::alignment
  \brief 文本内容对齐方式
  
  固定为 \l Qt::AlignHCenter
  
  \note 只读
  \sa QLineEdit::alignment
 */

/*!
  \return
 */
Qt::Alignment DIpv4LineEdit::alignment() const
{
    return Qt::AlignHCenter;
}

/*!
  \property DIpv4LineEdit::acceptableInput
  \note 只读
  \sa QLineEdit::acceptableInput
 */

/*!
  \return
 */
bool DIpv4LineEdit::hasAcceptableInput() const
{
    D_DC(DIpv4LineEdit);

    bool has = true;

    for (const QLineEdit *edit : d->editList) {
        has = has && edit->hasAcceptableInput();
    }

    return has;
}

/*!
  \property DIpv4LineEdit::readOnly
  \note 可读可写
  \sa QLineEdit::readOnly
 */

/*!
  \return
 */
bool DIpv4LineEdit::isReadOnly() const
{
    return d_func()->editList.first()->isReadOnly();
}

/*!
  \a cursorPosition
  \sa cursorPosition
 */
void DIpv4LineEdit::setCursorPosition(int cursorPosition)
{
    QLineEdit::setCursorPosition(cursorPosition);

    D_D(DIpv4LineEdit);

    for (QLineEdit *edit : d->editList) {
        if (cursorPosition > edit->text().count()) {
            cursorPosition -= edit->text().count();
            --cursorPosition;
        } else {
            edit->setCursorPosition(cursorPosition);
            edit->setFocus();

            break;
        }
    }
}

/*!
  \a readOnly
  \sa readOnly
 */
void DIpv4LineEdit::setReadOnly(bool readOnly)
{
    D_D(DIpv4LineEdit);

    for (QLineEdit *edit : d->editList) {
        edit->setReadOnly(readOnly);
    }
}

/*!
  \brief 使用此方法选中文本时会把四个输入框当做一个看待
  \a start
  \a length
  \sa QLineEdit::setSelection
 */
void DIpv4LineEdit::setSelection(int start, int length)
{
    D_D(DIpv4LineEdit);

    for (QLineEdit *edit : d->editList) {
        if (edit->text().count() > start) {
            if (edit->text().count() < length + start) {
                int tmp_length = edit->text().count() - start;

                edit->setSelection(start, tmp_length);

                length -= tmp_length;
            } else {
                edit->setSelection(start, length);
                break;
            }
        } else {
            edit->setSelection(edit->cursorPosition(), 0);
        }

        start -= edit->text().count();
    }

    QLineEdit::setSelection(start, length);
}

/*!
  \brief 选中所有输入框的全部内容
  \sa QLineEdit::selectAll
 */
void DIpv4LineEdit::selectAll()
{
    D_D(DIpv4LineEdit);

    for (QLineEdit *edit : d->editList) {
        edit->selectAll();
    }

    QLineEdit::selectAll();
}

bool DIpv4LineEdit::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::KeyPress) {
        QLineEdit *edit = qobject_cast<QLineEdit *>(obj);

        if (edit) {
            QKeyEvent *event = static_cast<QKeyEvent *>(e);

            if (event) {
                // 按住shlft+left/right 时选中当前的edit文字，而非移动光标
                if (event->modifiers() == Qt::ShiftModifier &&
                    (event->key() == Qt::Key_Left ||
                     event->key() == Qt::Key_Right)) {
                  return QLineEdit::eventFilter(obj, e);
                }

                D_D(DIpv4LineEdit);
                if (event->key() <= Qt::Key_9 && event->key() >= Qt::Key_0) {
                    if (edit->cursorPosition() == edit->text().count()) {
                        QRegularExpression rx(RX_PATTERN_IP);

                        const QString number = QString::number(event->key() - Qt::Key_0);

                        if (!rx.match(edit->text().append(number)).hasMatch()) {
                            int index = d->editList.indexOf(edit) + 1;

                            if (index < d->editList.count()) {
                                d->editList[index]->setFocus();

                                if (d->editList[index]->text().isEmpty()) {
                                    d->editList[index]->setText(number);
                                    d->_q_updateLineEditText();
                                }
                            }

                            return true;
                        }
                    }
                } else {
                    if (event->key() == Qt::Key_Backspace) {
                        bool accept = false;

                        for (QLineEdit *edit : d->editList) {
                            if (!edit->selectedText().isEmpty()) {
                                edit->setText(edit->text().remove(edit->selectedText()));
                                d->_q_updateLineEditText();
                                accept = true;
                            }
                        }

                        if (accept)
                            return true;
                    }

                    if (event->key() == Qt::Key_Left
                            || (event->key() == Qt::Key_Backspace
                                && edit->cursorPosition() == 0)) {
                        setCursorPosition(cursorPosition() - 1);

                        return true;
                    }

                    if (event->key() == Qt::Key_Right) {
                        setCursorPosition(cursorPosition() + 1);

                        return true;
                    }

                    if (event->key() == Qt::Key_Period || event->key() == Qt::Key_Space) {
                        int index = d->editList.indexOf(edit) + 1;

                        if (index < d->editList.count()) {
                            d->editList[index]->setFocus();
                        }

                        return true;
                    }

                    if (event->modifiers() == Qt::ControlModifier) {
                        if (event->key() == Qt::Key_V) {
                            QString clipboarg_text = qApp->clipboard()->text();
                            QString text = edit->text().insert(edit->cursorPosition(), clipboarg_text);

                            QRegularExpression rx(RX_PATTERN_IP);

                            if (rx.match(text).hasMatch()) {
                                edit->setText(text);
                                d->_q_updateLineEditText();
                            } else {
                                int pos = 0;

                                if (validator()->validate(clipboarg_text, pos) == QValidator::Acceptable)
                                    d->_q_setIpLineEditText(clipboarg_text);
                            }

                            return true;
                        } else if (event->key() == Qt::Key_A) {
                            selectAll();
                        } else if (!selectedText().isEmpty() && event->matches(QKeySequence::Cut))  {
                            cut();
                        } else if (!selectedText().isEmpty() && event->matches(QKeySequence::Copy)) {
                            copy();
                        } else {
                            return QLineEdit::eventFilter(obj, e);
                        }

                        return true;
                    }
                }
            }
        }
    } else if (e->type() == QEvent::FocusIn) {
        QLineEdit *edit = qobject_cast<QLineEdit *>(obj);

        if (edit) {
            // focusProxy 只能设置一个， 所以每次focusIn时设置当前这个edit为FocusProxy吧
            setFocusProxy(edit);
            QLineEdit::setCursorPosition(cursorPosition());
        }

        d_func()->setFocus(true);

    } else if (e->type() == QEvent::FocusOut || e->type() == QEvent::MouseButtonPress) {
        D_D(DIpv4LineEdit);

        bool focus = false;

        for (QLineEdit *edit : d->editList) {
            edit->setSelection(edit->cursorPosition(), 0);

            focus = edit->hasFocus() | focus;
        }

        if (!focus)
            d_func()->setFocus(false);
    }

    return QLineEdit::eventFilter(obj, e);
}

DIpv4LineEdit::DIpv4LineEdit(DIpv4LineEditPrivate &q, QWidget *parent)
    : QLineEdit(parent)
    , DObject(q)
{

}

void DIpv4LineEdit::resizeEvent(QResizeEvent *event)
{
    D_D(DIpv4LineEdit);
    QSize s = event->size();
    QStyleOptionFrame opt;
    initStyleOption(&opt);
    int radius = DStyle::pixelMetric(style(), DStyle::PM_FrameRadius, &opt, this);
    s -= {radius, radius};
    d->editMainWidget->resize(s);
    d->editMainWidget->move(radius / 2, radius / 2);
}

DWIDGET_END_NAMESPACE

#include "moc_dipv4lineedit.cpp"
