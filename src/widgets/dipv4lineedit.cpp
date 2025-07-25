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

    // Keep the consistency with the child edit and do not display the menu.
    q->setContextMenuPolicy(Qt::NoContextMenu);
    // A frame with rounded corners is a self-drawn background that covers the text of the edit control behind it.
    DFrame *frame = new DFrame(q);
    frame->setAccessibleName("DIpv4LineEditFrame");
    frame->setFrameRounded(true);
    frame->setFrameShape(QFrame::NoFrame);
    editMainWidget = frame;

    editMainWidget->setBackgroundRole(QPalette::Button);
    editMainWidget->setAutoFillBackground(true);

    QHBoxLayout *hbox_layout = new QHBoxLayout;

    hbox_layout->setSpacing(0);
    hbox_layout->setContentsMargins(0, 0, 0, 0);
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
    // Prohibit drawing the focus box of sub edit
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

    // Redraw the border when the focus changes
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
@~english
  @class Dtk::Widget::DIpv4LineEdit
  \inmodule dtkwidget
  @brief The class DIpv4LineEdit is specifically designed as an input box for entering text in IPv4 format.

  This class is composed of four @ref`QLineEdit` objects. The user can switch the cursor between input boxes by using the `Tab/Shift+Tab` shortcut keys, and typing a "." will automatically move the cursor to the next input box. When moving the cursor using the left and right arrow keys, it will automatically cross input boxes, just like in a single input box. Similar to @ref `QLineEdit`, this class also supports shortcut keys for copying and pasting text, and illegal characters will be automatically filtered according to the rules for IPv4 addresses when pasting. The text in each input box must meet the following regular expression rules:
  @code{.unparsed}
  ^(2[0-4]\d|25[0-5]|[01]?\d\d?)?$
  @endcode
  @warning functional functions of DLineEdit and QLineEdit cannot be used.
 */

/*!
@~english
  @property DIpv4LineEdit::selectedText
  @brief Selected Text Content
  @note readOnly
  @sa QLineEdit::selectedText
  */

/*!
@~english
  @property DIpv4LineEdit::placeholderText
  @brief The placeholder text in the input box is typically displayed in gray when there is no input, and will be hidden once any character is entered.
  @note readOnly. The default is an empty string
  @sa QLineEdit::placeholderText
  */

/*!
@~english
  @property DIpv4LineEdit::clearButtonEnabled
  @brief Whether to display the cleaning button on the input box
  @note read only。Default is false.
  @sa QLineEdit::clearButtonEnabled
  */

/*!
@~english
  @brief To construct the DIpv4LineEdit class, like any regular @ref QWidget, a pointer to a QWidget object can be passed in as its parent object to manage the widget.
  @a parent
 */
DIpv4LineEdit::DIpv4LineEdit(QWidget *parent)
    : QLineEdit(parent)
    , DObject(*new DIpv4LineEditPrivate(this))
{
    d_func()->init();
}

/*!
@~english
  @property DIpv4LineEdit::displayText
  @brief The text content displayed in the input box
  
  Unlike QLineEdit::displayText, this method returns the same content as @ref text, regardless of the value of @ref echoMode.
  
  @note Read-write
  @sa QLineEdit::echoMode QLineEdit::text
 */

/*!
  @return
 */
QString DIpv4LineEdit::displayText() const
{
    return text();
}

/*!
@~english
  @property DIpv4LineEdit::cursorPosition
  @brief Current cursor position
  
  @note Read-write
  @sa QLineEdit::cursorPosition
 */

/*!
  @return
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
            cursorPosition += edit->text().size() + 1;
        }
    }

    return cursorPosition;
}

/*!
@~english
  @property DIpv4LineEdit::alignment
  @brief Text Content Alignment
  
  Fixed as @ref Qt:: AlignHCenter
  
  @note readOnly
  @sa QLineEdit::alignment
 */

/*!
  @return
 */
Qt::Alignment DIpv4LineEdit::alignment() const
{
    return Qt::AlignHCenter;
}

/*!
@~english
  @property DIpv4LineEdit::acceptableInput
  @brief Check if all QLineEdit objects in the DIPv4LineEdit class have acceptable inputs
  @note readOnly
  @sa QLineEdit::acceptableInput
 */

/*!
  @return
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
@~english
  @property DIpv4LineEdit::isReadOnly
  @brief Check if the first QLineEdit object in the DIPv4LineEdit class is in read-only mode
  @note Read-write
  @sa QLineEdit::readOnly
 */

/*!
  @return
 */
bool DIpv4LineEdit::isReadOnly() const
{
    return d_func()->editList.first()->isReadOnly();
}

/*!
@~english
  @property DIpv4LineEdit::setCursorPosition
  @brief Move the cursor to the specified position and set the focus to the QLineEdit object containing that position
  @a cursorPosition
  @sa cursorPosition
 */
void DIpv4LineEdit::setCursorPosition(int cursorPosition)
{
    QLineEdit::setCursorPosition(cursorPosition);

    D_D(DIpv4LineEdit);

    for (QLineEdit *edit : d->editList) {
        if (cursorPosition > edit->text().size()) {
            cursorPosition -= edit->text().size();
            --cursorPosition;
        } else {
            edit->setCursorPosition(cursorPosition);
            edit->setFocus();

            break;
        }
    }
}

/*!
@~english
  @property DIpv4LineEdit::setReadOnly
  @brief set whether all QLineEdit objects in the DIPv4LineEdit object are in read-only mode
  @a readOnly
  @sa readOnly
 */
void DIpv4LineEdit::setReadOnly(bool readOnly)
{
    D_D(DIpv4LineEdit);

    for (QLineEdit *edit : d->editList) {
        edit->setReadOnly(readOnly);
    }
}

/*!
@~english
  @property DIpv4LineEdit::setSelection
  @brief When using this method to select text, the four input boxes will be treated as one
  @a start
  @a length
  @sa QLineEdit::setSelection
 */
void DIpv4LineEdit::setSelection(int start, int length)
{
    D_D(DIpv4LineEdit);

    for (QLineEdit *edit : d->editList) {
        if (edit->text().size() > start) {
            if (edit->text().size() < length + start) {
                int tmp_length = edit->text().size() - start;

                edit->setSelection(start, tmp_length);

                length -= tmp_length;
            } else {
                edit->setSelection(start, length);
                break;
            }
        } else {
            edit->setSelection(edit->cursorPosition(), 0);
        }

        start -= edit->text().size();
    }

    QLineEdit::setSelection(start, length);
}

/*!
@~english
  @property DIpv4LineEdit::selectAll
  @brief Select all contents of all input boxes
  @sa QLineEdit::selectAll
 */
void DIpv4LineEdit::selectAll()
{
    D_D(DIpv4LineEdit);

    for (QLineEdit *edit : d->editList) {
        edit->selectAll();
    }

    QLineEdit::selectAll();
}

/*!
@~english
  @property DIpv4LineEdit::eventFilter
  @brief Handle keyboard, mouse, and other events in input boxes to correctly respond to user input and actions.
  @param obj The target object of the event.
  @param e The event object.
  @return bool Returns true if the event has been handled; otherwise returns false.
 */

bool DIpv4LineEdit::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::KeyPress) {
        QLineEdit *edit = qobject_cast<QLineEdit *>(obj);

        if (edit) {
            QKeyEvent *event = static_cast<QKeyEvent *>(e);

            if (event) {
                // Select the current edit text while holding down shlft+left/right, instead of moving the cursor
                if (event->modifiers() == Qt::ShiftModifier &&
                    (event->key() == Qt::Key_Left ||
                     event->key() == Qt::Key_Right)) {
                  return QLineEdit::eventFilter(obj, e);
                }

                D_D(DIpv4LineEdit);
                if (event->key() <= Qt::Key_9 && event->key() >= Qt::Key_0) {
                    if (edit->cursorPosition() == edit->text().size()) {
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
            // focusProxy can only be set to one, so set the current edit control as the focusProxy each time focusIn is triggered.
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

/*!
@~english
  @property DIpv4LineEdit::DIpv4LineEdit
  @brief Constructor of DIpv4LineEdit class, used to create a DIpv4LineEdit object.
  @param q A pointer to the internal data of the DIpv4LineEdit object.
  @param parent The parent window object of the DIpv4LineEdit.
 */
DIpv4LineEdit::DIpv4LineEdit(DIpv4LineEditPrivate &q, QWidget *parent)
    : QLineEdit(parent)
    , DObject(q)
{

}

/*!
@~english
  @property DIpv4LineEdit::resizeEvent
  @brief Override the resizeEvent function to adjust the size and position of the input box.
  @param event A QResizeEvent object that contains information about the window size change.
 */
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
