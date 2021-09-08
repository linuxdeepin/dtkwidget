/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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
#include <QRegularExpression>
#include <DFontSizeManager>
#include <DApplication>

#include "private/ddialog_p.h"

#include "ddialogclosebutton.h"
#include "dialog_constants.h"
#include "ddialog.h"
#include "dboxwidget.h"
#include "DAnchors"
#include "dtitlebar.h"
#include "dwarningbutton.h"
#include "dsuggestbutton.h"

DWIDGET_BEGIN_NAMESPACE

DDialogPrivate::DDialogPrivate(DDialog *qq)
    : DAbstractDialogPrivate(qq)
    , messageLabel(nullptr)
    , titleLabel(nullptr)
    , iconLayout(nullptr)
    , contentLayout(nullptr)
    , buttonLayout(nullptr)
    , clickedButtonIndex(0)
{

}

QBoxLayout *DDialogPrivate::getContentLayout()
{
    return contentLayout;
}

static void palrtteTransparency(QWidget *widget, qint8 alphaFloat)
{
    QPalette palette = widget->palette();
    QColor color = DGuiApplicationHelper::adjustColor(palette.color(QPalette::Active, QPalette::BrightText), 0, 0, 0, 0, 0, 0, alphaFloat);
    palette.setColor(QPalette::WindowText, color);
    widget->setPalette(palette);
}

void DDialogPrivate::init()
{
    D_Q(DDialog);

    // TopLayout--TextLabel
    titleLabel = new QLabel;
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setAccessibleName("DDialogTitleLabel");
    titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    titleLabel->setWordWrap(true);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    titleLabel->installEventFilter(q);
    DFontSizeManager *fontManager =  DFontSizeManager::instance();
    fontManager->bind(titleLabel, DFontSizeManager::T5, QFont::Medium);
    palrtteTransparency(titleLabel, -10);
    titleLabel->hide();

    messageLabel = new QLabel;
    fontManager->bind(messageLabel, DFontSizeManager::T6, QFont::Medium);
    messageLabel->setObjectName("MessageLabel");
    messageLabel->setAccessibleName("DDialogMessageLabel");
    messageLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    messageLabel->setWordWrap(true);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    messageLabel->installEventFilter(q);
    palrtteTransparency(messageLabel, -30);
    messageLabel->hide();

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(5);
    textLayout->addWidget(titleLabel, 0);
    textLayout->addWidget(messageLabel, 1, Qt::AlignTop);

    // TopLayout--ContentLayout
    contentLayout = new QVBoxLayout;
    contentLayout->setContentsMargins(10, 0, 10, 0);
    contentLayout->setSpacing(0);
    contentLayout->addLayout(textLayout);

    contentWidget = new QWidget;
    contentWidget->setLayout(contentLayout);
    DApplication *dapp = qobject_cast<DApplication*>(qApp);

    if (dapp) {
        contentWidget->setAttribute(Qt::WA_LayoutOnEntireRect, false);
        contentWidget->setAttribute(Qt::WA_ContentsMarginsRespectsSafeArea, false);
        contentWidget->setProperty("_dtk_NoTopLevelEnabled", true);
        qApp->acclimatizeVirtualKeyboard(contentWidget);
    }

    titleBar = new DTitlebar();
    titleBar->setAccessibleName("DDialogTitleBar");
    titleBar->setIcon(icon); //设置标题icon
    titleBar->setMenuVisible(false);
    titleBar->setBackgroundTransparent(true);
    titleBar->setTitle(q->windowTitle());

    QObject::connect(q, &DDialog::windowTitleChanged, titleBar, &DTitlebar::setTitle);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    spacer = new QSpacerItem(1, 0);

    // MainLayout--TopLayout
    mainLayout->addWidget(titleBar, 0, Qt::AlignTop);
    mainLayout->addWidget(contentWidget);
    mainLayout->setContentsMargins(QMargins(0, 0, 0, 0));

    // MainLayout--ButtonLayout
    buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(0);
    buttonLayout->setContentsMargins(DIALOG::BUTTON_LAYOUT_LEFT_MARGIN,
                                     DIALOG::BUTTON_LAYOUT_TOP_MARGIN,
                                     DIALOG::BUTTON_LAYOUT_RIGHT_MARGIN,
                                     DIALOG::BUTTON_LAYOUT_BOTTOM_MARGIN);
    mainLayout->addSpacerItem(spacer);
    mainLayout->addLayout(buttonLayout);

    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, q, [this]() {
        palrtteTransparency(titleLabel, -10);
        palrtteTransparency(messageLabel, -30);
    });
    q->setLayout(mainLayout);
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

void DDialogPrivate::setSpacer(int height)
{
    spacer->changeSize(1, height);
}

void DDialogPrivate::_q_onButtonClicked()
{
    D_Q(DDialog);

    QAbstractButton *button = qobject_cast<QAbstractButton *>(q->sender());

    if (button) {
        int index = buttonList.indexOf(button);
        q->buttonClicked(index, button->text());

        if (onButtonClickedClose) {
            clickedButtonIndex = index;
            q->done(clickedButtonIndex);
        }
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
    \class Dtk::Widget::DDialog
    \inmodule dtkwidget

    \brief 可以使用 DDialog 类快速创建符合 DDE 风格的简要对话框窗口.
    \brief Use DDialog class to create quick dialog window matched DDE style.

    \ingroup dialog-classes
    \ingroup abstractwidgets

    DDialog 旨在提供简要的讯问式对话框的快速实现。提供了包含标题，对话框内容，默认图标，用以添加按钮的布局和一个可以自由添加内容的内容布局。
    可以使用 addButton() , insertButton(), setDefaultButton() 等函数方便的给对话框插入按钮并进行管理，可以使用 addContent(), insertContent()
    等函数操作内容布局。

    此外， DDialog 还提供了一些额外的函数以供实现一些常见的需求。如，可以通过设置 setOnButtonClickedClose() 为 true 来使得用
    户在点击任何对话框上的按钮后关闭对话框。

    当你需要快速构建较为简单结构的对话框，你应当使用 DDialog ，对于较为复杂的需求，请参阅 DAbstractDialog 或 QDialog 相关文档。

    \section1 differences Differences with QDialog

    DDialog is designed to provide a easy way to create information dialog which include title, dialog content,
    dialog icon, buttons. You can use addButton() , insertButton() and related functions to insert buttons to the dialog
    easily, and use addContent(), insertContent() and related functions to manage the dialog's built-in content layout.

    DDialog also provided some extra function which maybe useful for some common use case, for example, you can
    set setOnButtonClickedClose() to \c true , then once user clicked a button, the dialog will got closed.

    \sa DAbstractDialog, QDialog
*/

/*!
  \brief 构造一个 DDialog 对话框.

  \a parent 父控件指针.
 */
DDialog::DDialog(QWidget *parent) :
    DAbstractDialog(*new DDialogPrivate(this), parent)
{
    d_func()->init();
}

/*!
  \brief 构造一个 DDialog 对话框。
  
  \a title 标题
  \a message 对话框消息
  \a parent 父控件
 */
DDialog::DDialog(const QString &title, const QString &message, QWidget *parent) :
    DAbstractDialog(*new DDialogPrivate(this), parent)
{
    d_func()->init();

    setTitle(title);
    setMessage(message);
}

/*!
  \brief 通过按钮文字获取按钮下标.
  
  \a text 按钮文字
  
  \return 按钮下标
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
  \brief 获得对话框包含的按钮数量.
 */
int DDialog::buttonCount() const
{
    D_DC(DDialog);

    return d->buttonList.count();
}

/*!
  \brief 获得对话框所含的所有内容控件的数量.
 */
int DDialog::contentCount() const
{
    D_DC(DDialog);

    return d->contentList.count();
}

/*!
  \brief 获得对话框的按钮列表
 */
QList<QAbstractButton *> DDialog::getButtons() const
{
    D_DC(DDialog);

    return d->buttonList;
}

/*!
  \brief 获得对话框所含的所有内容控件列表
 */
QList<QWidget *> DDialog::getContents() const
{
    D_DC(DDialog);

    return d->contentList;
}

/*!
  \brief 获得指定下标所对应的按钮.
  
  \a index 按钮下标
  \return 对应的按钮
 */
QAbstractButton *DDialog::getButton(int index) const
{
    D_DC(DDialog);

    return d->buttonList.at(index);
}

/*!
  \brief 获取指定下标对应的内容控件.
  \a index　控件下标
  \return 返回对应的内容控件
 */
QWidget *DDialog::getContent(int index) const
{
    D_DC(DDialog);

    return d->contentList.at(index);
}

/*!
  \brief 返回对话框标题.

  \return 返回对话框的标题内容
 */
QString DDialog::title() const
{
    D_DC(DDialog);

    return d->title;
}

/*!
  \brief 返回对话框消息文本.

  \return 返回对话框的显示信息
 */
QString DDialog::message() const
{
    D_DC(DDialog);

    return d->message;
}

/*!
  \brief 返回对话框图标.

  \return  返回对话框的icon
 */
QIcon DDialog::icon() const
{
    D_DC(DDialog);

    return d->icon;
}

/*!
  \brief 返回对话框图标的 QPixmap 对象.
  \return 返回ICON的pixmap
 */
QPixmap DDialog::iconPixmap() const
{
    D_DC(DDialog);

    if (d->icon.isNull())  {
        return QPixmap();
    } else {
        return d->icon.pixmap(d->icon.availableSizes().at(0));
    }
}

/*!
  \brief 返回对话框的文本格式.

  \return 返回设定的文本格式
 */
Qt::TextFormat DDialog::textFormat() const
{
    D_DC(DDialog);

    return d->textFormat;
}

/*!
  \brief 检查在点击任何按钮后是否都会关闭对话框.

  \return 关闭对话框返回 true , 否则返回 false.
 */
bool DDialog::onButtonClickedClose() const
{
    D_DC(DDialog);

    return d->onButtonClickedClose;
}

/*!
  \brief 设定内容布局的内容 margin .
  \a margins 具体的 margins
 */
void DDialog::setContentLayoutContentsMargins(const QMargins &margins)
{
    D_D(DDialog);

    d->contentLayout->setContentsMargins(margins);
}

/*!
  \brief 返回内容布局的边距.

  \return 返回内容布局的内容margin
 */
QMargins DDialog::contentLayoutContentsMargins() const
{
    D_DC(DDialog);

    return d->contentLayout->contentsMargins();
}

/*!
  \brief 关闭按钮的可见属性.

  \return 返回关闭按钮是否可见的bool值
 */
bool DDialog::closeButtonVisible() const
{
    return windowFlags().testFlag(Qt::WindowCloseButtonHint);
}

/*!
  \brief 向对话框添加按钮.
  
  \a text 按钮文字
  \a isDefault 是否默认按钮
  \a type 按钮类型
  
  \return 所添加的按钮的下标
 */
int DDialog::addButton(const QString &text, bool isDefault, ButtonType type)
{
    int index = buttonCount();

    insertButton(index, text, isDefault, type);

    return index;
}

/*!
  \brief 向对话框添加按钮.
  
  \a text 按钮文字
  
  \return 所添加的按钮的下标
 */
int DDialog::addButtons(const QStringList &text)
{
    int index = buttonCount();

    insertButtons(index, text);

    return index;
}

/*!
  \brief 向对话框插入按钮.
  
  \a index 下标
  \a text 按钮文字
  \a isDefault 是否是默认按钮
  \a type 按钮类型
 */
void DDialog::insertButton(int index, const QString &text, bool isDefault, ButtonType type)
{
    QAbstractButton *button;

    switch (type) {
    case ButtonWarning:
        button = new DWarningButton(this);
        break;
    case ButtonRecommend:
        button = new DSuggestButton(this);
        break;
    default:
        button = new QPushButton(this);
        break;
    }

    button->setText(text);
    button->setObjectName("ActionButton");
    button->setAccessibleName(text);
    button->setAttribute(Qt::WA_NoMousePropagation);

    insertButton(index, button, isDefault);
}

/*!
  \brief 向对话框插入按钮.
  
  \a index 下标
  \a button 待插入的按钮
  \a isDefault 是否是默认按钮
 */
void DDialog::insertButton(int index, QAbstractButton *button, bool isDefault)
{
    D_D(DDialog);

    DVerticalLine *line = new DVerticalLine;
    line->setObjectName("VLine");
    line->setFixedHeight(30);

    d->buttonLayout->insertWidget(index * 2 , line);
    d->buttonLayout->insertWidget(index * 2 + 1, button);
    d->buttonList << button;
    if (d->buttonList.count() == 1) {
        int spacing = DStyleHelper(style()).pixelMetric(DStyle::PM_ContentsSpacing);
        d->setSpacer(spacing);
    }
    line->show();
    d->buttonLayout->itemAt(0)->widget()->hide();

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
  \brief 向对话框插入按钮.
  
  \a index 下标
  \a text 按钮文字
 */
void DDialog::insertButtons(int index, const QStringList &text)
{
    for (int i = 0; i < text.count(); ++i) {
        insertButton(index + i, text[i]);
    }
}

/*!
  \brief 从对话框移除按钮.
  
  \a index 待移除按钮的下标
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

    if (d->buttonList.isEmpty()) {
        d->setSpacer(0);
    }
}

/*!
  \brief 从对话框移除按钮.
  
  \a button 待移除的按钮
 */
void DDialog::removeButton(QAbstractButton *button)
{
    removeButton(getButtons().indexOf(button));
}

/*!
  \brief 从对话框移除按钮.
  
  \a text 待移除按钮的文本内容
 */
void DDialog::removeButtonByText(const QString &text)
{
    int index = getButtonIndexByText(text);

    if (index >= 0)
        removeButton(index);
}

/*!
  \brief 清除所有按钮.
 */
void DDialog::clearButtons()
{
    D_D(DDialog);

    d->buttonList.clear();
    d->setSpacer(0);

    while (d->buttonLayout->count()) {
        QLayoutItem *item = d->buttonLayout->takeAt(0);

        item->widget()->deleteLater();
        delete item;
    }
}

/*!
  \brief 设置默认按钮.
  
  \a index 要设置的默认按钮的下标.
  \return 设置成功返回 true，否则返回false.
 */
bool DDialog::setDefaultButton(int index)
{
    if (index < 0)
        return false;

    setDefaultButton(getButton(index));

    return true;
}

/*!
  \brief 设置默认按钮.
  
  \a str 要设置的默认按钮的文本内容
  \sa default
 */
bool DDialog::setDefaultButton(const QString &str)
{
    return setDefaultButton(getButtonIndexByText(str));
}

/*!
  \brief 设置默认按钮
  
  \a button 要设置的默认按钮
  \sa default
 */
void DDialog::setDefaultButton(QAbstractButton *button)
{
    D_D(DDialog);

    d->defaultButton = button;
}

/*!
  \brief 添加控件到对话框内容布局.
  
  \a widget 待添加的控件
  \a alignment 对齐方式
 */
void DDialog::addContent(QWidget *widget, Qt::Alignment alignment)
{
    D_DC(DDialog);

    int index = d->contentLayout->count();

    insertContent(index, widget, alignment);
}

/*!
  \brief 在对话框内容布局指定位置插入控件.
  
  \a index 待插入的位置下标
  \a widget 待插入的控件
  \a alignment 对齐方式
 */
void DDialog::insertContent(int index, QWidget *widget, Qt::Alignment alignment)
{
    D_D(DDialog);

    d->contentLayout->insertWidget(index + DIALOG::CONTENT_INSERT_OFFSET,
                                   widget, 0, alignment);
    d->contentList << widget;
}

/*!
  \brief 从对话框内容布局中移除指定控件.
  
  \a widget 待移除的控件
  \a isDelete 是否执行删除
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
  \brief 清空对话框内容布局中的所有内容.
  
  \a isDelete 是否删除
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
  \brief 设置对话框内容间隔.
  
  设置对话框的内容布局的间隔 \a spacing 大小
  
  \sa QBoxLayout::setSpacing
 */
void DDialog::setSpacing(int spacing)
{
    D_D(DDialog);

    d->contentLayout->setSpacing(spacing);
}

/*!
  \brief 追加对话框内容间隔.
  
  在对话框的内容布局后追加一个非弹性，大小为 \a spacing 的间隔（一个 QSpacerItem ）。
  
  \sa QBoxLayout::addSpacing
 */
void DDialog::addSpacing(int spacing)
{
    D_D(DDialog);

    d->contentLayout->addSpacing(spacing);
}

/*!
  \brief 插入对话框内容间隔.
  
  在对话框的内容布局的指定位置插入一个非弹性，大小为 \a spacing 的间隔（一个 QSpacerItem ）。
  \a index 插入间隔的索引位置.
  
  \sa QBoxLayout::insertSpacing
 */
void DDialog::insertSpacing(int index, int spacing)
{
    D_D(DDialog);

    d->contentLayout->insertSpacing(index, spacing);
}

/*!
  \brief 清除内容间隔.
  
  清除对话框内容布局中包含的所有 QSpacerItem 。
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
  \brief 设置按钮文字.
  
  \a index 需要设置文字的按钮的下标
  \a text 所需要设置的文字
 */
void DDialog::setButtonText(int index, const QString &text)
{
    QAbstractButton *button = getButton(index);

    button->setText(text);
}

/*!
  \brief 设置按钮图标.
  \a index 需要设置图标的按钮的下标
  \a icon 所需要设置的图标
 */
void DDialog::setButtonIcon(int index, const QIcon &icon)
{
    QAbstractButton *button = getButton(index);

    button->setIcon(icon);
}

/*!
  \brief 设置对话框标题.

  \a title 对话框标题.
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
  \brief 设定标题Label内容是否可截断换行.

  \a wordWrap true可换行　false不可以换行
 */
void DDialog::setWordWrapTitle(bool wordWrap)
{
    D_D(DDialog);
    d->titleLabel->setWordWrap(wordWrap);
}

/*!
  \brief 设置对话框消息内容.

  \a message 对话框消息.
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

void DDialog::setWordWrapMessage(bool wordWrap)
{
    D_D(DDialog);
    d->messageLabel->setWordWrap(wordWrap);
}

/*!
  \brief 设置对话框图标.
  \a icon 对话框图标.
 */
void DDialog::setIcon(const QIcon &icon)
{
    D_D(DDialog);

    d->icon = icon;

    if (!icon.isNull()) {
        d->titleBar->setIcon(d->icon);
    }
}

/*!
  \brief 设置对话框图标.
  \overload

  为对话框设置图标，同时可以指定一个期望的图标大小。

  \a icon 对话框图标 \a expectedSize 期望大小.
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
  \brief 设置对话框位图图标.

  \a iconPixmap pixmap类型图标.
 */
void DDialog::setIconPixmap(const QPixmap &iconPixmap)
{
    setIcon(QIcon(iconPixmap));
}

/*!
  \brief 设置文字格式.
  \a textFormat 文字格式.
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
  \brief 设置是否在点击按钮后关闭对话框.
  
  当 \a onButtonClickedClose 设置为 true 后，无论点击什么按钮，都会在点击后关闭对话框。
 */
void DDialog::setOnButtonClickedClose(bool onButtonClickedClose)
{
    D_D(DDialog);

    d->onButtonClickedClose = onButtonClickedClose;
}

/*!
  \brief 以模态框形式显示当前对话框.
  
  以 \l{QDialog#Modal Dialogs}{模态框} 形式显示当前对话框，将会阻塞直到用户关闭对话框。
  
  onButtonClickedClose()为 true 时返回当前点击按钮的Index，否则返回 结果。
  \return 返回模态对话框处理的结果.

  \sa QDialog::exec()
 */
int DDialog::exec()
{
    D_D(DDialog);

    d->clickedButtonIndex = -1;
    int clickedIndex = d->clickedButtonIndex;

    if (d->onButtonClickedClose) {
        // 如果设置了WA_DeleteOnClose属性，那么在exec()中将直接delete this
        // d->clickedButtonIndex中记录的数据失效，这里通过信号槽更新正确的数据
        connect(this, &DDialog::buttonClicked, this, [ &clickedIndex ] (int index, const QString &) {
                clickedIndex = index;
        });
    }

    int code = DAbstractDialog::exec();

    return clickedIndex >= 0 ? clickedIndex : code;
}

void DDialog::setCloseButtonVisible(bool closeButtonVisible)
{
    bool visible = isVisible();
    // This function calls setParent() when changing the flags for a window, causing the widget to be hidden.
    setWindowFlag(Qt::WindowCloseButtonHint, closeButtonVisible);
    // So we need call show() or so to make the widget visible again.
    this->setVisible(visible);
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
        if (d->titleBar) {
            d->titleBar->raise();
        }
    }
}

void DDialog::resizeEvent(QResizeEvent *event)
{
    return DAbstractDialog::resizeEvent(event);
}

void DDialog::keyPressEvent(QKeyEvent *event)
{
    D_D(DDialog);
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        d->_q_defaultButtonTriggered();
    } else {
        DAbstractDialog::keyPressEvent(event);
    }
}

bool DDialog::eventFilter(QObject *watched, QEvent *event)
{
    Q_D(DDialog);

    if (watched == d->messageLabel || watched == d->titleLabel) {
        if (event->type() == QEvent::FontChange) {
            QLabel *label = qobject_cast<QLabel *>(watched);

            if (label && !label->text().isEmpty() && label->wordWrap()) {
                QSize sz = style()->itemTextRect(label->fontMetrics(), label->rect(), Qt::TextWordWrap, false, label->text()).size();

                label->setMinimumHeight(qMax(sz.height(), label->sizeHint().height()));
            }
        }

        return false;
    }

    return DAbstractDialog::eventFilter(watched, event);
}

DWIDGET_END_NAMESPACE

#include "moc_ddialog.cpp"
