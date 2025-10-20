// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <DHorizontalLine>

#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>
#include <QDebug>
#include <QCloseEvent>
#include <QApplication>
#include <QSpacerItem>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QDesktopWidget>
#endif
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
#include "dsizemode.h"

DWIDGET_BEGIN_NAMESPACE

DDialogPrivate::DDialogPrivate(DDialog *qq)
    : DAbstractDialogPrivate(qq)
    , messageLabel(nullptr)
    , titleLabel(nullptr)
    , iconLayout(nullptr)
    , contentLayout(nullptr)
    , buttonLayout(nullptr)
    , clickedButtonIndex(-1)
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
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar, 0, Qt::AlignTop);
    mainLayout->addWidget(contentWidget);
    mainLayout->setContentsMargins(QMargins(0, 0, 0, 0));

    // MainLayout--ButtonLayout
    buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(5);
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

    const QScreen *screen = q->screen();

    if (screen)
        return screen;

    screen = QGuiApplication::screenAt(QCursor::pos());

    return screen;
}

QString DDialogPrivate::trimTag(QString origin) const
{
    return origin.replace(QRegularExpression("<.*?>"), "");
}

QMap<int, QString> DDialogPrivate::scanTags(QString origin) const
{
    QMap<int, QString> result;

    QRegularExpression re("<.*?>");
    QRegularExpressionMatch match;
    int index = origin.indexOf(re, 0,&match);
    while (match.hasMatch()) {
        result[index] = match.captured();
        index = origin.indexOf(re, index + match.capturedLength(),&match);
    }

    return result;
}

// It's not a perfect solution to elide rich text, but it should be sufficient
// for working with rich text only change text token color.
QString DDialogPrivate::elideString(QString str, const QFontMetrics &fm, int width) const
{
    QString trimmed = trimTag(str);
    if (fm.horizontalAdvance(trimmed) > width) {
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
        clickedButtonIndex = index;

        if (onButtonClickedClose) {
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
@~english
    @class Dtk::Widget::DDialog
    \inmodule dtkwidget

    @brief Use DDialog class to create quick dialog window matched DDE style.

    @ingroup dialog-classes
    @ingroup abstractwidgets

    \section1 differences Differences with QDialog

    DDialog is designed to provide a easy way to create information dialog which include title, dialog content,
    dialog icon, buttons. You can use addButton() , insertButton() and related functions to insert buttons to the dialog
    easily, and use addContent(), insertContent() and related functions to manage the dialog's built-in content layout.

    DDialog also provided some extra function which maybe useful for some common use case, for example, you can
    set setOnButtonClickedClose() to \c true , then once user clicked a button, the dialog will got closed.

    @sa DAbstractDialog, QDialog
*/

/*!
@~english
  @brief Construct a DDialog dialog box

  \a parent Parent control pointer
 */
DDialog::DDialog(QWidget *parent) :
    DAbstractDialog(*new DDialogPrivate(this), parent)
{
    d_func()->init();
}

/*!
@~english
  @brief Construct a DDialog dialog box
  
  \a title Title
  \a message Dialog box messages
  \a parent Parent control pointer
 */
DDialog::DDialog(const QString &title, const QString &message, QWidget *parent) :
    DAbstractDialog(*new DDialogPrivate(this), parent)
{
    d_func()->init();

    setTitle(title);
    setMessage(message);
}

/*!
@~english
  @brief Gets the button index from its text
  
  \a text The text of the button
  
  @return The index of the button
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
@~english
  @brief Gets the number of buttons that the dialog box contains.
 */
int DDialog::buttonCount() const
{
    D_DC(DDialog);

    return d->buttonList.count();
}

/*!
@~english
  @brief Gets the number of all content controls that the dialog box contains.
 */
int DDialog::contentCount() const
{
    D_DC(DDialog);

    return d->contentList.count();
}

/*!
@~english
  @brief Gets a list of buttons for the dialog box
 */
QList<QAbstractButton *> DDialog::getButtons() const
{
    D_DC(DDialog);

    return d->buttonList;
}

/*!
@~english
  @brief Gets a list of all content controls that the dialog box contains
 */
QList<QWidget *> DDialog::getContents() const
{
    D_DC(DDialog);

    return d->contentList;
}

/*!
@~english
  @brief Gets the button corresponding to the specified index
  
  \a index The index of the button
  @return Returns the button with the specified index
 */
QAbstractButton *DDialog::getButton(int index) const
{
    D_DC(DDialog);

    return d->buttonList.at(index);
}

/*!
@~english
  @brief Gets the content control corresponding to the specified subscript
  \a index The index of the control
  @return Returns the corresponding content control
 */
QWidget *DDialog::getContent(int index) const
{
    D_DC(DDialog);

    return d->contentList.at(index);
}

/*!
@~english
  @brief Returns the dialog box title

  @return Returns the dialog box title
 */
QString DDialog::title() const
{
    D_DC(DDialog);

    return d->title;
}

/*!
@~english
  @brief Returns the dialog box message text.

  @return Returns the dialog box message text
 */
QString DDialog::message() const
{
    D_DC(DDialog);

    return d->message;
}

/*!
@~english
  @brief Return to the dialog box icon

  @return  Return to the dialog box icon
 */
QIcon DDialog::icon() const
{
    D_DC(DDialog);

    return d->icon;
}

/*!
@~english
  @brief Returns the QPixmap object of the dialog icon
  @return Returns the QPixmap object of the dialog icon
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
QPixmap DDialog::iconPixmap() const
{
    D_DC(DDialog);

    if (d->icon.isNull())  {
        return QPixmap();
    } else {
        return d->icon.pixmap(d->icon.availableSizes().at(0));
    }
}
#endif

/*!
@~english
  @brief Returns the text format of the dialog box

  @return Returns the text format of the dialog box
 */
Qt::TextFormat DDialog::textFormat() const
{
    D_DC(DDialog);

    return d->textFormat;
}

/*!
@~english
  @brief Check to see if the dialog closes after clicking any button

  @return Returns true if the dialog box is closed, false otherwise
 */
bool DDialog::onButtonClickedClose() const
{
    D_DC(DDialog);

    return d->onButtonClickedClose;
}

/*!
@~english
  @brief Sets the content margin for the content layout
  \a margins Specific margins
 */
void DDialog::setContentLayoutContentsMargins(const QMargins &margins)
{
    D_D(DDialog);

    d->contentLayout->setContentsMargins(margins);
}

/*!
@~english
  @brief Returns the margins for the content layout

  @return Returns the content margin of the content layout
 */
QMargins DDialog::contentLayoutContentsMargins() const
{
    D_DC(DDialog);

    return d->contentLayout->contentsMargins();
}

/*!
@~english
  @brief Turns off the visibility property of the button

  @return Returns the bool value of whether the close button is visible
 */
bool DDialog::closeButtonVisible() const
{
    return windowFlags().testFlag(Qt::WindowCloseButtonHint);
}

/*!
@~english
  @brief Adds a button to the dialog box
  
  \a text The text of button
  \a isDefault Default button or not
  \a type Types of button
  
  @return The index of the added button
 */
int DDialog::addButton(const QString &text, bool isDefault, ButtonType type)
{
    int index = buttonCount();

    insertButton(index, text, isDefault, type);

    return index;
}

/*!
@~english
  @brief Adds a button to the dialog box
  
  \a text The text of button
  
  @return The index of the added button
 */
int DDialog::addButtons(const QStringList &text)
{
    int index = buttonCount();

    insertButtons(index, text);

    return index;
}

/*!
@~english
  @brief Adds a button to the dialog box
  
  \a index The index of the button to add
  \a text The text of button
  \a isDefault Default button or not
  \a type Types of buttons
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
@~english
  @brief Inserts a button into the dialog box.
  
  \a index The index of the button to add
  \a button Button to insert
  \a isDefault Default button or not
 */
void DDialog::insertButton(int index, QAbstractButton *button, bool isDefault)
{
    D_D(DDialog);

    DVerticalLine *line = new DVerticalLine;
    line->setObjectName("VLine");
    line->setFixedHeight(DSizeModeHelper::element(20, 30));

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

    if (text.size() == 2) {
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
@~english
  @brief Inserts a button into the dialog box
  
  \a index The index of the button to add
  \a text The text of button
 */
void DDialog::insertButtons(int index, const QStringList &text)
{
    for (int i = 0; i < text.count(); ++i) {
        insertButton(index + i, text[i]);
    }
}

/*!
@~english
  @brief Removes the button from the dialog box
  
  \a index The index of the button to remove
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
@~english
  @brief Removes the button from the dialog box
  
  \a button The button to remove
 */
void DDialog::removeButton(QAbstractButton *button)
{
    removeButton(getButtons().indexOf(button));
}

/*!
@~english
  @brief Removes the button from the dialog box
  
  \a text The text of the button to remove
 */
void DDialog::removeButtonByText(const QString &text)
{
    int index = getButtonIndexByText(text);

    if (index >= 0)
        removeButton(index);
}

/*!
@~english
  @brief Clear all buttons
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
@~english
  @brief Setting the default button
  
  \a index The index of the default button to set
  @return Returns true on success and false otherwise
 */
bool DDialog::setDefaultButton(int index)
{
    if (index < 0)
        return false;

    setDefaultButton(getButton(index));

    return true;
}

/*!
@~english
  @brief Setting the default button
  
  \a str The text content of the default button to set
  @sa default
 */
bool DDialog::setDefaultButton(const QString &str)
{
    return setDefaultButton(getButtonIndexByText(str));
}

/*!
@~english
  @brief Setting the default button
  
  \a button Default button to set
  @sa default
 */
void DDialog::setDefaultButton(QAbstractButton *button)
{
    D_D(DDialog);

    d->defaultButton = button;
}

/*!
@~english
  @brief Add controls to the dialog content layout.
  
  \a widget Controls to add
  \a alignment alignment
 */
void DDialog::addContent(QWidget *widget, Qt::Alignment alignment)
{
    D_DC(DDialog);

    int index = d->contentLayout->count();

    insertContent(index, widget, alignment);
}

/*!
@~english
  @brief Inserts a control at the location specified in the dialog box content layout
  
  \a index The index of the position to insert
  \a widget Control to insert
  \a alignment alignment
 */
void DDialog::insertContent(int index, QWidget *widget, Qt::Alignment alignment)
{
    D_D(DDialog);
# if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    d->contentLayout->insertWidget(index + DIALOG::CONTENT_INSERT_OFFSET,
                                   widget, 0, alignment);
#else
    d->contentLayout->insertWidget(index, widget, 0, alignment); //TODO: remove offset temporarily, this modification may cause some errors to occur
#endif
    d->contentList << widget;
}

/*!
@~english
  @brief Removes the specified control from the dialog box content layout.
  
  \a widget Control to remove
  \a isDelete Whether to delete
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
@~english
  @brief Clear everything in the content layout of the dialog box.
  
  \a isDelete Whether to delete
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
@~english
  @brief Sets the dialog content interval.
  
  \a spacing Spacing size for the content layout of the dialog box
  
  @sa QBoxLayout::setSpacing
 */
void DDialog::setSpacing(int spacing)
{
    D_D(DDialog);

    d->contentLayout->setSpacing(spacing);
}

/*!
@~english
  @brief Append dialog box content intervals
  
  Append an inelastic interval of size \a spacing after the content layout of the dialog box (a QSpacerItem)
  
  @sa QBoxLayout::addSpacing
 */
void DDialog::addSpacing(int spacing)
{
    D_D(DDialog);

    d->contentLayout->addSpacing(spacing);
}

/*!
@~english
  @brief Insert dialog box content intervals.
  
  Inserts an inelastic interval of size spacing (a QSpacerItem) at the specified position in the content layout of the dialog box.
  \a index The index position at which the interval is inserted.
  
  @sa QBoxLayout::insertSpacing
 */
void DDialog::insertSpacing(int index, int spacing)
{
    D_D(DDialog);

    d->contentLayout->insertSpacing(index, spacing);
}

/*!
@~english
  @brief Clear content intervals.
  
  Clear all QSpacerItems contained in the dialog content layout.
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
@~english
  @brief Setting button text.
  
  \a index The index of the button that requires the text to be set
  \a text Text to set
 */
void DDialog::setButtonText(int index, const QString &text)
{
    QAbstractButton *button = getButton(index);

    button->setText(text);
}

/*!
@~english
  @brief Set button icon.
  \a index The index of the button that needs to be set for the icon
  \a icon Icon to set
 */
void DDialog::setButtonIcon(int index, const QIcon &icon)
{
    QAbstractButton *button = getButton(index);

    button->setIcon(icon);
}

/*!
@~english
  @brief Sets the dialog box title.

  \a title Dialog box title.
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
@~english
  @brief Specifies whether the content of the title Label can be truncated.

  \a wordWrap true can wrap a line. false can't wrap a line
 */
void DDialog::setWordWrapTitle(bool wordWrap)
{
    D_D(DDialog);
    d->titleLabel->setWordWrap(wordWrap);
}

/*!
@~english
  @brief Sets the dialog box message content.

  \a message Dialog box messages
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
@~english
  @brief Set the dialog icon
  \a icon Dialog icon
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
@~english
  @brief Set the dialog icon.
  \overload

  Sets the icon for the dialog box and specifies a desired icon size.

  \a icon Dialog icon \a expectedSize Expected size.
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
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
#endif

/*!
@~english
  @brief Sets the dialog box bitmap icon.

  \a iconPixmap icon of pixmap type.
 */
#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0)
void DDialog::setIconPixmap(const QPixmap &iconPixmap)
{
    setIcon(QIcon(iconPixmap));
}
#endif

/*!
@~english
  @brief Formatting text.
  \a textFormat Text format.
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
@~english
  @brief Sets whether to close the dialog box when the button is clicked.
  
  When \a onButtonClickedClose is set to true, whatever button is clicked will close the dialog box when clicked.
 */
void DDialog::setOnButtonClickedClose(bool onButtonClickedClose)
{
    D_D(DDialog);

    d->onButtonClickedClose = onButtonClickedClose;
}

/*!
@~english
  @brief Displays the current dialog box as a modal.
  
  Displaying the current dialog as {QDialog#Modal Dialogs}{modal boxes} will block until the user closes the dialog box.
  
  If \a onButtonClickedClose() is true, it returns the Index of the button that is currently clicked; otherwise, it returns the result.
  @return Returns the result of the modal dialog processing.

  @sa QDialog::exec()
 */
int DDialog::exec()
{
    D_D(DDialog);

    d->clickedButtonIndex = -1;
    return DAbstractDialog::exec();
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
    Q_D(DDialog);

    Q_EMIT aboutToClose();

    done(d->clickedButtonIndex);

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

void DDialog::changeEvent(QEvent *event)
{
    Q_D(DDialog);
    if (event->type() == QEvent::StyleChange) {
        for (int i = 0; i < d->buttonLayout->count(); ++i) {
            if (auto line = qobject_cast<DVerticalLine *>(d->buttonLayout->itemAt(i)->widget())) {
                line->setFixedHeight(DSizeModeHelper::element(20, 30));
            }
        }
        d->updateSize();
    } else if (event->type() == QEvent::FontChange) {
        d->updateSize();
    }
    return DAbstractDialog::changeEvent(event);
}

DWIDGET_END_NAMESPACE

#include "moc_ddialog.cpp"
