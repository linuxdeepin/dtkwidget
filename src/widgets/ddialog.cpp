/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include <DHorizontalLine>

#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>
#include <QDebug>
#include <QCloseEvent>
#include <QApplication>
#include <QSpacerItem>
#include <QDesktopWidget>
#include <QScreen>
#include <QAction>
#include <QRegExp>
#include <QRegularExpression>

#include <QStyle>
#include <QStyleFactory>

#include "private/ddialog_p.h"

#include "ddialogclosebutton.h"
#include "dialog_constants.h"
#include "ddialog.h"
#include "dboxwidget.h"
#include "DAnchors"

DWIDGET_BEGIN_NAMESPACE

DialogButton::DialogButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{

}

int DialogButton::buttonType() const
{
    return m_buttonType;
}

void DialogButton::setButtonType(int buttonType)
{
    if (m_buttonType == buttonType)
        return;

    m_buttonType = buttonType;
    Q_EMIT buttonTypeChanged(buttonType);
}

DDialogPrivate::DDialogPrivate(DDialog *qq) :
    DAbstractDialogPrivate(qq)
{

}

QBoxLayout *DDialogPrivate::getContentLayout()
{
    return contentLayout;
}

void DDialogPrivate::init()
{
    D_Q(DDialog);

    // TopLayout
    topLayout = new QHBoxLayout;
    topLayout->setContentsMargins(DIALOG::ICON_LAYOUT_LEFT_MARGIN,
                                  DIALOG::ICON_LAYOUT_TOP_MARGIN,
                                  DIALOG::ICON_LAYOUT_RIGHT_MARGIN,
                                  DIALOG::ICON_LAYOUT_BOTTOM_MARGIN);
    topLayout->setSpacing(DIALOG::ICON_LAYOUT_SPACING);


    // TopLayout--Icon
    iconLabel = new QLabel;
    iconLabel->hide();
    iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    topLayout->addWidget(iconLabel);

    // TopLayout--TextLabel
    titleLabel = new QLabel;
    titleLabel->setObjectName("TitleLabel");
    titleLabel->hide();
    titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    messageLabel = new QLabel;
    messageLabel->setObjectName("MessageLabel");
    messageLabel->hide();
    messageLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(5);
    textLayout->addStretch();
    textLayout->addWidget(titleLabel, 0, Qt::AlignLeft);
    textLayout->addWidget(messageLabel, 0, Qt::AlignLeft);
    textLayout->addStretch();

    // TopLayout--ContentLayout
    contentLayout = new QVBoxLayout;
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    contentLayout->addLayout(textLayout);

    topLayout->addLayout(contentLayout);

    // TopLayout--Close button
    closeButton = new DDialogCloseButton(q);
    closeButton->setObjectName("CloseButton");
    closeButton->setIconSize(QSize(DIALOG::CLOSE_BUTTON_WIDTH, DIALOG::CLOSE_BUTTON_HEIGHT));
    closeButton->adjustSize();
    closeButton->setAttribute(Qt::WA_NoMousePropagation);

    DAnchors<DIconButton> anchors_close_button(closeButton);
    anchors_close_button.setMargins(10);
    anchors_close_button.setAnchor(Qt::AnchorRight, q, Qt::AnchorRight);
    anchors_close_button.setAnchor(Qt::AnchorTop, q, Qt::AnchorTop);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // MainLayout--TopLayout
    mainLayout->addLayout(topLayout);

    // MainLayout--ButtonLayout
    buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(0);
    buttonLayout->setContentsMargins(DIALOG::BUTTON_LAYOUT_LEFT_MARGIN,
                                     DIALOG::BUTTON_LAYOUT_TOP_MARGIN,
                                     DIALOG::BUTTON_LAYOUT_RIGHT_MARGIN,
                                     DIALOG::BUTTON_LAYOUT_BOTTOM_MARGIN);
    mainLayout->addLayout(buttonLayout);


    QAction *button_action = new QAction(q);

    button_action->setShortcuts(QKeySequence::InsertParagraphSeparator);
    button_action->setAutoRepeat(false);

    QObject::connect(closeButton, SIGNAL(clicked()), q, SLOT(close()));
    QObject::connect(button_action, SIGNAL(triggered(bool)), q, SLOT(_q_defaultButtonTriggered()));

    q->setLayout(mainLayout);
    q->addAction(button_action);
    q->setFocusPolicy(Qt::ClickFocus);
    q->setFocus();
}

const QScreen *DDialogPrivate::getScreen() const
{
    D_QC(DDialog);

    const QScreen *screen = qApp->screens()[qApp->desktop()->screenNumber(q)];

    if (screen)
        return screen;

    screen = qApp->screens()[qApp->desktop()->screenNumber(QCursor::pos())];

    return screen;
}

QString DDialogPrivate::trimTag(QString origin) const
{
    return origin.replace(QRegularExpression("<.*?>"), "");
}

QMap<int, QString> DDialogPrivate::scanTags(QString origin) const
{
    QMap<int, QString> result;

    QRegExp re("<.*?>");
    int index = origin.indexOf(re, 0);
    int matchLength = 0;
    while (index >= 0) {
        result[index] = re.cap();
        matchLength = re.matchedLength();
        index = origin.indexOf(re, index + matchLength);
    }

    return result;
}

// It's not a perfect solution to elide rich text, but it should be sufficient
// for working with rich text only change text token color.
QString DDialogPrivate::elideString(QString str, const QFontMetrics &fm, int width) const
{
    QString trimmed = trimTag(str);
    if (fm.width(trimmed) > width) {
        QMap<int, QString> info = scanTags(str);
        QString elided = fm.elidedText(trimmed, Qt::ElideMiddle, width);
        int elideStart = elided.indexOf("…");
        int elideLength = trimmed.length() - elided.length();
        QList<int> indexes = info.keys();
        std::sort(indexes.begin(), indexes.end());
        for (int index : indexes) {
            if (index <= elideStart) {
                elided.insert(index, info[index]);
            } else if (elideStart < index && index <= elideStart + elideLength) {
                elided.insert(elideStart, info[index]);
                elideStart += 1;
            } else {
                elided.insert(index - elideLength + 1, info[index]);
            }
        }
        return elided;
    } else {
        return str;
    }
}

void DDialogPrivate::updateSize()
{
    D_Q(DDialog);

    if (!q->testAttribute(Qt::WA_Resized)) {
        QSize size = q->sizeHint();

        size.setWidth(qMax(size.width(), DIALOG::DEFAULT_WIDTH));
        size.setHeight(qMax(size.height(), DIALOG::DEFAULT_HEIGHT));

        q->resize(size);
        q->setAttribute(Qt::WA_Resized, false);
    }
}

void DDialogPrivate::_q_onButtonClicked()
{
    D_Q(DDialog);

    QAbstractButton *button = qobject_cast<QAbstractButton *>(q->sender());

    if (button) {
        clickedButtonIndex = buttonList.indexOf(button);
        q->buttonClicked(clickedButtonIndex, button->text());

        if (onButtonClickedClose)
            q->done(clickedButtonIndex);
    }
}

void DDialogPrivate::_q_defaultButtonTriggered()
{
    D_QC(DDialog);

    QAbstractButton *button = qobject_cast<QAbstractButton *>(q->focusWidget());

    if (button)
        button->click();
    else if (defaultButton)
        defaultButton->click();
}

/*!
    \~chinese \class DDialog
    \~chinese \brief 可以使用 DDialog 类快速创建符合 DDE 风格的简要对话框窗口。

    \~chinese \ingroup dialog-classes
    \~chinese \ingroup abstractwidgets

    \~chinese DDialog 旨在提供简要的讯问式对话框的快速实现。提供了包含标题，对话框内容，默认图标，用以添加按钮的布局和一个可以自由添加内容的内容布局。
    可以使用 addButton() , insertButton(), setDefaultButton() 等函数方便的给对话框插入按钮并进行管理，可以使用 addContent(), insertContent()
    等函数操作内容布局。

    \~chinese 此外， DDialog 还提供了一些额外的函数以供实现一些常见的需求。如，可以通过设置 setOnButtonClickedClose() 为 true 来使得用
    户在点击任何对话框上的按钮后关闭对话框。

    \~chinese 当你需要快速构建较为简单结构的对话框，你应当使用 DDialog ，对于较为复杂的需求，请参阅 DAbstractDialog 或 QDialog 相关文档。

    \~chinese \sa DAbstractDialog, QDialog
*/

/*!
    \~english \class DDialog
    \~english \brief Use DDialog class to create quick dialog window matched DDE style.

    \~english \ingroup dialog-classes
    \~english \ingroup abstractwidgets

    \~english \section differences Differences with QDialog

    \~english DDialog is designed to provide a easy way to create information dialog which include title, dialog content,
    dialog icon, buttons. You can use addButton() , insertButton() and related functions to insert buttons to the dialog
    easily, and use addContent(), insertContent() and related functions to manage the dialog's built-in content layout.

    \~english DDialog also provided some extra function which maybe useful for some common use case, for example, you can
    set setOnButtonClickedClose() to \c true , then once user clicked a button, the dialog will got closed.

    \~english \sa DAbstractDialog, QDialog
*/

/*!
 * \~chinese \brief 构造一个 DDialog 对话框。
 */
DDialog::DDialog(QWidget *parent) :
    DAbstractDialog(*new DDialogPrivate(this), parent)
{
    d_func()->init();
}

/*!
 * \~chinese \brief 构造一个 DDialog 对话框。
 *
 * \~chinese \param title 标题
 * \~chinese \param message 对话框消息
 * \~chinese \param parent 父控件
 */
DDialog::DDialog(const QString &title, const QString &message, QWidget *parent) :
    DAbstractDialog(*new DDialogPrivate(this), parent)
{
    d_func()->init();

    setTitle(title);
    setMessage(message);
}

/*!
 * \~chinese \brief 通过按钮文字获取按钮下标
 *
 * \~chinese \param text 按钮文字
 *
 * \~chinese \return 按钮下标
 */
int DDialog::getButtonIndexByText(const QString &text) const
{
    int i = -1;

    for (const QAbstractButton *button : getButtons()) {
        ++i;

        if (button->text() == text)
            return i;
    }

    return i;
}

/*!
 * \~chinese \brief 获得对话框包含的按钮数量
 */
int DDialog::buttonCount() const
{
    D_DC(DDialog);

    return d->buttonList.count();
}

/*!
 * \~chinese \brief 获得对话框所含的所有内容控件的数量
 */
int DDialog::contentCount() const
{
    D_DC(DDialog);

    return d->contentList.count();
}

/*!
 * \~chinese \brief 获得对话框的按钮列表
 */
QList<QAbstractButton *> DDialog::getButtons() const
{
    D_DC(DDialog);

    return d->buttonList;
}

/*!
 * \~chinese \brief 获得对话框所含的所有内容控件列表
 */
QList<QWidget *> DDialog::getContents() const
{
    D_DC(DDialog);

    return d->contentList;
}

/*!
 * \~chinese \brief 获得指定下标所对应的按钮
 *
 * \~chinese \param index 按钮下标
 * \~chinese \return 对应的按钮
 */
QAbstractButton *DDialog::getButton(int index) const
{
    D_DC(DDialog);

    return d->buttonList.at(index);
}

QWidget *DDialog::getContent(int index) const
{
    D_DC(DDialog);

    return d->contentList.at(index);
}

QString DDialog::title() const
{
    D_DC(DDialog);

    return d->title;
}

QString DDialog::message() const
{
    D_DC(DDialog);

    return d->message;
}

QIcon DDialog::icon() const
{
    D_DC(DDialog);

    return d->icon;
}

QPixmap DDialog::iconPixmap() const
{
    D_DC(DDialog);

    return *d->iconLabel->pixmap();
}

Qt::TextFormat DDialog::textFormat() const
{
    D_DC(DDialog);

    return d->textFormat;
}

/*!
 * \~chinese \brief 检查在点击任何按钮后是否都会关闭对话框。
 */
bool DDialog::onButtonClickedClose() const
{
    D_DC(DDialog);

    return d->onButtonClickedClose;
}

void DDialog::setContentLayoutContentsMargins(const QMargins &margins)
{
    D_D(DDialog);

    d->topLayout->setContentsMargins(margins);
}

QMargins DDialog::contentLayoutContentsMargins() const
{
    D_DC(DDialog);

    return d->topLayout->contentsMargins();
}

bool DDialog::closeButtonVisible() const
{
    D_DC(DDialog);

    return d->closeButton->isVisible();
}

/*!
 * \~chinese \brief 向对话框添加按钮
 *
 * \~chinese \param text 按钮文字
 * \~chinese \param isDefault 是否默认按钮
 * \~chinese \param type 按钮类型
 *
 * \~chinese \return 所添加的按钮的下标
 */
int DDialog::addButton(const QString &text, bool isDefault, ButtonType type)
{
    int index = buttonCount();

    insertButton(index, text, isDefault, type);

    return index;
}

/*!
 * \~chinese \brief 向对话框添加按钮
 *
 * \~chinese \param text 按钮文字
 *
 * \~chinese \return 所添加的按钮的下标
 */
int DDialog::addButtons(const QStringList &text)
{
    int index = buttonCount();

    insertButtons(index, text);

    return index;
}

/*!
 * \~chinese \brief 向对话框插入按钮
 *
 * \~chinese \param index 下标
 * \~chinese \param text 按钮文字
 * \~chinese \param isDefault 是否是默认按钮
 * \~chinese \param type 按钮类型
 */
void DDialog::insertButton(int index, const QString &text, bool isDefault, ButtonType type)
{
    DialogButton *button = new DialogButton(text);
    button->setObjectName("ActionButton");
    button->setButtonType(type);
    button->setAttribute(Qt::WA_NoMousePropagation);

    insertButton(index, button, isDefault);
}

/*!
 * \~chinese \brief 向对话框插入按钮
 *
 * \~chinese \param index 下标
 * \~chinese \param button 待插入的按钮
 * \~chinese \param isDefault 是否是默认按钮
 */
void DDialog::insertButton(int index, QAbstractButton *button, bool isDefault)
{
    D_D(DDialog);

    DVerticalLine *line = new DVerticalLine;
    line->setObjectName("VLine");
    line->hide();

    if (index > 0 && index >= buttonCount()) {
        DVerticalLine *label = qobject_cast<DVerticalLine *>(d->buttonLayout->itemAt(d->buttonLayout->count() - 1)->widget());
        if (label)
            line->show();
    }

    d->buttonLayout->insertWidget(index * 2, button);
    d->buttonList << button;
    d->buttonLayout->insertWidget(index * 2 + 1, line);


    connect(button, SIGNAL(clicked(bool)), this, SLOT(_q_onButtonClicked()));

    if (isDefault) {
        setDefaultButton(button);
    }

    const QString &text = button->text();

    if (text.count() == 2) {
        for (const QChar &ch : text) {
            switch (ch.script()) {
            case QChar::Script_Han:
            case QChar::Script_Katakana:
            case QChar::Script_Hiragana:
            case QChar::Script_Hangul:
                break;
            default:
                return;
            }
        }

        button->setText(QString().append(text.at(0)).append(QChar::Nbsp).append(text.at(1)));
    }
}

/*!
 * \~chinese \brief 向对话框插入按钮
 *
 * \~chinese \param index 下标
 * \~chinese \param text 按钮文字
 */
void DDialog::insertButtons(int index, const QStringList &text)
{
    for (int i = 0; i < text.count(); ++i) {
        insertButton(index + i, text[i]);
    }
}

/*!
 * \~chinese \brief 从对话框移除按钮
 *
 * \~chinese \param index 待移除按钮的下标
 */
void DDialog::removeButton(int index)
{
    D_D(DDialog);

    DVerticalLine *label = qobject_cast<DVerticalLine *>(d->buttonLayout->itemAt(index * 2 + 1)->widget());
    QAbstractButton *button = qobject_cast<QAbstractButton *>(d->buttonLayout->itemAt(index * 2)->widget());

    if (label) {
        d->buttonLayout->removeWidget(label);
        label->deleteLater();
    }

    if (button) {
        d->buttonLayout->removeWidget(button);
        button->deleteLater();
    }

    if (index > 0 && index == buttonCount() - 1) {
        DVerticalLine *label = qobject_cast<DVerticalLine *>(d->buttonLayout->itemAt(d->buttonLayout->count() - 1)->widget());

        if (label)
            label->hide();
    }

    d->buttonList.removeAt(index);
}

/*!
 * \~chinese \brief 从对话框移除按钮
 *
 * \~chinese \param button 待移除的按钮
 */
void DDialog::removeButton(QAbstractButton *button)
{
    removeButton(getButtons().indexOf(button));
}

/*!
 * \~chinese \brief 从对话框移除按钮
 *
 * \~chinese \param text 待移除按钮的文本内容
 */
void DDialog::removeButtonByText(const QString &text)
{
    int index = getButtonIndexByText(text);

    if (index >= 0)
        removeButton(index);
}

/*!
 * \~chinese \brief 清除所有按钮
 */
void DDialog::clearButtons()
{
    D_D(DDialog);

    d->buttonList.clear();

    while (d->buttonLayout->count()) {
        QLayoutItem *item = d->buttonLayout->takeAt(0);

        item->widget()->deleteLater();
        delete item;
    }
}

/*!
 * \~chinese \brief 设置默认按钮
 *
 * \~chinese \param index 要设置的默认按钮的下标
 *
 * \~chinese \sa \ref default "默认按钮"
 */
bool DDialog::setDefaultButton(int index)
{
    if (index < 0)
        return false;

    setDefaultButton(getButton(index));

    return true;
}

/*!
 * \~chinese \brief 设置默认按钮
 *
 * \~chinese \param str 要设置的默认按钮的文本内容
 *
 * \sa \ref default "默认按钮"
 */
bool DDialog::setDefaultButton(const QString &str)
{
    return setDefaultButton(getButtonIndexByText(str));
}

/*!
 * \~chinese \brief 设置默认按钮
 *
 * \~chinese \param button 要设置的默认按钮
 *
 * \sa \ref default "默认按钮"
 */
void DDialog::setDefaultButton(QAbstractButton *button)
{
    D_D(DDialog);

    d->defaultButton = button;
}

/*!
 * \~chinese \brief 添加控件到对话框内容布局
 *
 * \~chinese \param widget 待添加的控件
 * \~chinese \param alignment 对齐方式
 */
void DDialog::addContent(QWidget *widget, Qt::Alignment alignment)
{
    D_DC(DDialog);

    int index = d->contentLayout->count();

    insertContent(index, widget, alignment);
}

/*!
 * \~chinese \brief 在对话框内容布局指定位置插入控件
 *
 * \~chinese \param index 待插入的位置下标
 * \~chinese \param widget 待插入的控件
 * \~chinese \param alignment 对齐方式
 */
void DDialog::insertContent(int index, QWidget *widget, Qt::Alignment alignment)
{
    D_D(DDialog);

    d->contentLayout->insertWidget(index + DIALOG::CONTENT_INSERT_OFFSET,
                                   widget, 0, alignment);
    d->contentList << widget;
}

/*!
 * \~chinese \brief 从对话框内容布局中移除指定控件
 *
 * \~chinese \param widget 待移除的控件
 * \~chinese \param isDelete 是否执行删除
 */
void DDialog::removeContent(QWidget *widget, bool isDelete)
{
    D_D(DDialog);

    d->contentLayout->removeWidget(widget);

    if (isDelete)
        widget->deleteLater();

    d->contentList.removeOne(widget);
}

/*!
 * \~chinese \brief 清空对话框内容布局中的所有内容
 *
 * \~chinese \param isDelete 是否删除
 */
void DDialog::clearContents(bool isDelete)
{
    D_D(DDialog);

    for (QWidget *widget : d->contentList)
        d->contentLayout->removeWidget(widget);

    if (isDelete) {
        qDeleteAll(d->contentList);
    }

    d->contentList.clear();
}

/*!
 * \~chinese \brief 设置对话框内容间隔
 *
 * \~chinese 设置对话框的内容布局的间隔大小
 *
 * \sa QBoxLayout::setSpacing
 */
void DDialog::setSpacing(int spacing)
{
    D_D(DDialog);

    d->contentLayout->setSpacing(spacing);
}

/*!
 * \~chinese \brief 追加对话框内容间隔
 *
 * \~chinese 在对话框的内容布局后追加一个非弹性，大小为 \a spacing 的间隔（一个 QSpacerItem ）。
 *
 * \sa QBoxLayout::addSpacing
 */
void DDialog::addSpacing(int spacing)
{
    D_D(DDialog);

    d->contentLayout->addSpacing(spacing);
}

/*!
 * \~chinese \brief 插入对话框内容间隔
 *
 * \~chinese 在对话框的内容布局的指定位置插入一个非弹性，大小为 \a spacing 的间隔（一个 QSpacerItem ）。
 *
 * \sa QBoxLayout::insertSpacing
 */
void DDialog::insertSpacing(int index, int spacing)
{
    D_D(DDialog);

    d->contentLayout->insertSpacing(index, spacing);
}

/*!
 * \~chinese \brief 清除内容间隔
 *
 * \~chinese 清除对话框内容布局中包含的所有 QSpacerItem 。
 */
void DDialog::clearSpacing()
{
    D_D(DDialog);

    for (int i = 0; i < d->contentLayout->count(); ++i) {
        QLayoutItem *item = d->contentLayout->itemAt(i);

        if (item->spacerItem()) {
            delete d->contentLayout->takeAt(i);
        }
    }
}

/*!
 * \~chinese \brief 设置按钮文字
 *
 * \~chinese \param index 需要设置文字的按钮的下标
 * \~chinese \param text 所需要设置的文字
 */
void DDialog::setButtonText(int index, const QString &text)
{
    QAbstractButton *button = getButton(index);

    button->setText(text);
}

/*!
 * \~chinese \brief 设置按钮图标
 * \~chinese \param index 需要设置图标的按钮的下标
 * \~chinese \param icon 所需要设置的图标
 */
void DDialog::setButtonIcon(int index, const QIcon &icon)
{
    QAbstractButton *button = getButton(index);

    button->setIcon(icon);
}

/*!
 * \~chinese \brief 设置对话框标题
 */
void DDialog::setTitle(const QString &title)
{
    D_D(DDialog);

    if (d->title == title)
        return;

    d->title = title;
    d->titleLabel->setText(title);
    d->titleLabel->setHidden(title.isEmpty());

    Q_EMIT titleChanged(title);
}

/*!
 * \~chinese \brief 设置对话框消息内容
 */
void DDialog::setMessage(const QString &message)
{
    D_D(DDialog);

    if (d->message == message)
        return;

    d->message = message;
    d->messageLabel->setText(message);
    d->messageLabel->setHidden(message.isEmpty());

    Q_EMIT messageChanged(message);
}

/*!
 * \~chinese \brief 设置对话框图标
 *
 * \~chinese 注意：该接口将会在未来版本中废弃，
 * 请使用 setIcon(const QIcon &icon, const QSize &expectedSize) 这个重载。
 */
void DDialog::setIcon(const QIcon &icon)
{
    D_D(DDialog);

    d->icon = icon;

    if (!icon.isNull()) {
        auto size = QSize(64, 64);
        size = icon.availableSizes().value(0, size);
        auto pixmap = icon.pixmap(size);
        setIconPixmap(pixmap);
    }
}

/*!
 * \~chinese \brief 设置对话框图标
 *
 * \~chinese 为对话框设置图标，同时可以指定一个期望的图标大小。
 */
void DDialog::setIcon(const QIcon &icon, const QSize &expectedSize)
{
    D_D(DDialog);

    d->icon = icon;

    if (icon.isNull()) {
        return;
    }

    QSize actualSize = expectedSize.isValid() ? expectedSize : QSize(64, 64);
    QPixmap pixmap = icon.pixmap(actualSize);
    setIconPixmap(pixmap);
}

/*!
 * \~chinese \brief 设置对话框位图图标
 */
void DDialog::setIconPixmap(const QPixmap &iconPixmap)
{
    D_D(DDialog);

    d->iconLabel->setPixmap(iconPixmap);
    if (iconPixmap.isNull()) {
        d->iconLabel->hide();
    } else {
        d->iconLabel->show();
    }
}

/*!
 * \~chinese \brief 设置文字格式
 */
void DDialog::setTextFormat(Qt::TextFormat textFormat)
{
    D_D(DDialog);

    if (d->textFormat == textFormat)
        return;

    d->textFormat = textFormat;
    d->titleLabel->setTextFormat(textFormat);
    d->messageLabel->setTextFormat(textFormat);

    Q_EMIT textFormatChanged(textFormat);
}

/*!
 * \~chinese \brief 设置是否在点击按钮后关闭对话框
 *
 * \~chinese 当设置为 true 后，无论点击什么按钮，都会在点击后关闭对话框。
 */
void DDialog::setOnButtonClickedClose(bool onButtonClickedClose)
{
    D_D(DDialog);

    d->onButtonClickedClose = onButtonClickedClose;
}

/*!
 * \~chinese \brief 以模态框形式显示当前对话框
 *
 * \~chinese 以 \l{QDialog#Modal Dialogs}{模态框} 形式显示当前对话框，将会阻塞直到用户关闭对话框，并返回 \DialogCode 结果。
 *
 * \sa open(), show(), result(), setWindowModality()
 */
int DDialog::exec()
{
    D_D(DDialog);

    d->clickedButtonIndex = -1;

    int code = DAbstractDialog::exec();

    return d->clickedButtonIndex >= 0 ? d->clickedButtonIndex : code;
}

void DDialog::setCloseButtonVisible(bool closeButtonVisible)
{
    D_D(DDialog);

    d->closeButton->setVisible(closeButtonVisible);
}

DDialog::DDialog(DDialogPrivate &dd, QWidget *parent) :
    DAbstractDialog(dd, parent)
{
    d_func()->init();
}

/*!\reimp */
void DDialog::showEvent(QShowEvent *event)
{
    D_D(DDialog);

    DAbstractDialog::showEvent(event);

    setAttribute(Qt::WA_Resized, false);
    d->updateSize();

    Q_EMIT visibleChanged(isVisible());
}

/*!\reimp */
void DDialog::hideEvent(QHideEvent *event)
{
    DAbstractDialog::hideEvent(event);

    Q_EMIT visibleChanged(isVisible());
}

void DDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)

    Q_EMIT aboutToClose();

    done(-1);

    Q_EMIT visibleChanged(isVisible());
    Q_EMIT closed();
}

void DDialog::childEvent(QChildEvent *event)
{
    DAbstractDialog::childEvent(event);

    D_D(DDialog);

    if (event->added()) {
        if (d->closeButton) {
            d->closeButton->raise();
        }
    }
}

void DDialog::resizeEvent(QResizeEvent *event)
{
    DAbstractDialog::resizeEvent(event);

    D_D(DDialog);


    d->titleLabel->setWordWrap(false);
    int labelMaxWidth = maximumWidth() - d->closeButton->width() - d->titleLabel->x();

    if (d->titleLabel->sizeHint().width() > labelMaxWidth) {
        d->titleLabel->setFixedWidth(labelMaxWidth);
        d->titleLabel->setWordWrap(true);
        d->titleLabel->setFixedHeight(d->titleLabel->sizeHint().height());
    }

    d->messageLabel->setWordWrap(false);
    labelMaxWidth = maximumWidth() - d->closeButton->width() - d->messageLabel->x();

    if (d->messageLabel->sizeHint().width() > labelMaxWidth) {
        d->messageLabel->setFixedWidth(labelMaxWidth);
        d->messageLabel->setWordWrap(true);
    }
}

DWIDGET_END_NAMESPACE

#include "moc_ddialog.cpp"
