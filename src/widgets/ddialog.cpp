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

#include "dialog_constants.h"
#include "ddialog.h"
#include "dthememanager.h"
#include "dboxwidget.h"
#include "anchors.h"

DWIDGET_BEGIN_NAMESPACE

DialogButton::DialogButton(const QString &text, QWidget *parent)
    :QPushButton(text, parent)
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
    DAbstractDialogPrivate(qq),
    fixedStyle(Q_NULLPTR)
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
    closeButton = new QPushButton(q);
    closeButton->setObjectName("CloseButton");
    closeButton->setFixedSize(DIALOG::CLOSE_BUTTON_WIDTH, DIALOG::CLOSE_BUTTON_HEIGHT);
    closeButton->setAttribute(Qt::WA_NoMousePropagation);

    AnchorsBase::setAnchor(closeButton, Qt::AnchorRight, q, Qt::AnchorRight);

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

    if(screen)
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

    QAbstractButton *button = qobject_cast<QAbstractButton*>(q->sender());

    if(button) {
        clickedButtonIndex = buttonList.indexOf(button);
        q->buttonClicked(clickedButtonIndex, button->text());

        if(onButtonClickedClose)
            q->done(clickedButtonIndex);
    }
}

void DDialogPrivate::_q_defaultButtonTriggered()
{
    D_QC(DDialog);

    QAbstractButton *button = qobject_cast<QAbstractButton*>(q->focusWidget());

    if (button)
        button->click();
    else if (defaultButton)
        defaultButton->click();
}

DDialog::DDialog(QWidget *parent) :
    DAbstractDialog(*new DDialogPrivate(this), parent)
{
    D_THEME_INIT_WIDGET(dialogs/DDialog);

    d_func()->init();
}

DDialog::DDialog(const QString &title, const QString &message, QWidget *parent) :
    DAbstractDialog(*new DDialogPrivate(this), parent)
{
    D_THEME_INIT_WIDGET(dialogs/DDialog);

    d_func()->init();

    setTitle(title);
    setMessage(message);
}

int DDialog::getButtonIndexByText(const QString &text) const
{
    int i = -1;

    for(const QAbstractButton *button : getButtons()) {
        ++i;

        if(button->text() == text)
            return i;
    }

    return i;
}

int DDialog::buttonCount() const
{
    D_DC(DDialog);

    return d->buttonList.count();
}

int DDialog::contentCount() const
{
    D_DC(DDialog);

    return d->contentList.count();
}

QList<QAbstractButton*> DDialog::getButtons() const
{
    D_DC(DDialog);

    return d->buttonList;
}

QList<QWidget *> DDialog::getContents() const
{
    D_DC(DDialog);

    return d->contentList;
}

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

int DDialog::addButton(const QString &text, bool isDefault, ButtonType type)
{
    int index = buttonCount();

    insertButton(index, text, isDefault, type);

    return index;
}

int DDialog::addButtons(const QStringList &text)
{
    int index = buttonCount();

    insertButtons(index, text);

    return index;
}

void DDialog::insertButton(int index, const QString &text, bool isDefault, ButtonType type)
{
    DialogButton *button = new DialogButton(text);
    button->setObjectName("ActionButton");
    button->setButtonType(type);
    button->setAttribute(Qt::WA_NoMousePropagation);
    button->setFixedHeight(DIALOG::BUTTON_HEIGHT);

    insertButton(index, button, isDefault);
}

void DDialog::insertButton(int index, QAbstractButton *button, bool isDefault)
{
    D_D(DDialog);

    QLabel* label = new QLabel;
    label->setObjectName("VLine");
    label->setFixedWidth(1);
    label->hide();

    if(index > 0 && index >= buttonCount()) {
        QLabel *label = qobject_cast<QLabel*>(d->buttonLayout->itemAt(d->buttonLayout->count() - 1)->widget());
        if(label)
            label->show();
    }

    d->buttonLayout->insertWidget(index * 2, button);
    d->buttonList << button;
    d->buttonLayout->insertWidget(index * 2 + 1, label);

    connect(button, SIGNAL(clicked(bool)), this, SLOT(_q_onButtonClicked()));

    if(isDefault) {
        setDefaultButton(button);
    }
}

void DDialog::insertButtons(int index, const QStringList &text)
{
    for(int i = 0; i < text.count(); ++i) {
        insertButton(index + i, text[i]);
    }
}

void DDialog::removeButton(int index)
{
    D_D(DDialog);

    QLabel *label = qobject_cast<QLabel*>(d->buttonLayout->itemAt(index * 2 + 1)->widget());
    QAbstractButton * button = qobject_cast<QAbstractButton*>(d->buttonLayout->itemAt(index * 2)->widget());

    if(label) {
        d->buttonLayout->removeWidget(label);
        label->deleteLater();
    }

    if(button) {
        d->buttonLayout->removeWidget(button);
        button->deleteLater();
    }

    if(index > 0 && index == buttonCount() - 1) {
        QLabel *label = qobject_cast<QLabel*>(d->buttonLayout->itemAt(d->buttonLayout->count() - 1)->widget());

        if(label)
            label->hide();
    }

    d->buttonList.removeAt(index);
}

void DDialog::removeButton(QAbstractButton *button)
{
    removeButton(getButtons().indexOf(button));
}

void DDialog::removeButtonByText(const QString &text)
{
    int index = getButtonIndexByText(text);

    if(index >= 0)
        removeButton(index);
}

void DDialog::clearButtons()
{
    D_D(DDialog);

    d->buttonList.clear();

    while(d->buttonLayout->count()) {
        QLayoutItem *item = d->buttonLayout->takeAt(0);

        item->widget()->deleteLater();
        delete item;
    }
}

bool DDialog::setDefaultButton(int index)
{
    if(index < 0)
        return false;

    setDefaultButton(getButton(index));

    return true;
}

bool DDialog::setDefaultButton(const QString &str)
{
    return setDefaultButton(getButtonIndexByText(str));
}

void DDialog::setDefaultButton(QAbstractButton *button)
{
    D_D(DDialog);

    d->defaultButton = button;
}

void DDialog::addContent(QWidget *widget, Qt::Alignment alignment)
{
    D_DC(DDialog);

    int index = d->contentLayout->count();

    insertContent(index, widget, alignment);
}

void DDialog::insertContent(int index, QWidget *widget, Qt::Alignment alignment)
{
    D_D(DDialog);

    d->contentLayout->insertWidget(index + DIALOG::CONTENT_INSERT_OFFSET,
                                         widget, 0, alignment);
    d->contentList << widget;
}

void DDialog::removeContent(QWidget *widget, bool isDelete)
{
    D_D(DDialog);

    d->contentLayout->removeWidget(widget);

    if(isDelete)
        widget->deleteLater();

    d->contentList.removeOne(widget);
}

void DDialog::clearContents(bool isDelete)
{
    D_D(DDialog);

    for(QWidget *widget : d->contentList)
        d->contentLayout->removeWidget(widget);

    if(isDelete) {
        qDeleteAll(d->contentList);
    }

    d->contentList.clear();
}

void DDialog::setSpacing(int spacing)
{
    D_D(DDialog);

    d->contentLayout->setSpacing(spacing);
}

void DDialog::addSpacing(int spacing)
{
    D_D(DDialog);

    d->contentLayout->addSpacing(spacing);
}

void DDialog::insertSpacing(int index, int spacing)
{
    D_D(DDialog);

    d->contentLayout->insertSpacing(index, spacing);
}

void DDialog::clearSpacing()
{
    D_D(DDialog);

    for(int i = 0; i < d->contentLayout->count(); ++i) {
        QLayoutItem *item = d->contentLayout->itemAt(i);

        if(item->spacerItem()) {
            delete d->contentLayout->takeAt(i);
        }
    }
}

void DDialog::setButtonText(int index, const QString &text)
{
    QAbstractButton *button = getButton(index);

    button->setText(text);
}

void DDialog::setButtonIcon(int index, const QIcon &icon)
{
    QAbstractButton *button = getButton(index);

    button->setIcon(icon);
}

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

void DDialog::setIcon(const QIcon &icon)
{
    D_D(DDialog);

    d->icon = icon;

    if(!icon.isNull()) {
        const QList<QSize> &sizes = icon.availableSizes();
        setIconPixmap(icon.pixmap(sizes.first()));
        if(!sizes.isEmpty())
            setIconPixmap(icon.pixmap(sizes.first()));
        else
            setIconPixmap(icon.pixmap(64, 64));
    }
}

void DDialog::setIconPixmap(const QPixmap &iconPixmap)
{
    D_D(DDialog);

    d->iconLabel->setPixmap(iconPixmap);
    if(iconPixmap.isNull()) {
        d->iconLabel->hide();
    } else {
        d->iconLabel->show();
    }
}

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

void DDialog::setOnButtonClickedClose(bool onButtonClickedClose)
{
    D_D(DDialog);

    d->onButtonClickedClose = onButtonClickedClose;
}

int DDialog::exec()
{
    D_D(DDialog);

    d->clickedButtonIndex = -1;

    int code = DAbstractDialog::exec();

    return d->clickedButtonIndex >= 0 ? d->clickedButtonIndex : code;
}

DDialog::DDialog(DDialogPrivate &dd, QWidget *parent) :
    DAbstractDialog(dd, parent)
{
    D_THEME_INIT_WIDGET(dialogs/DDialog);

    d_func()->init();
}

void DDialog::showEvent(QShowEvent *event)
{
    D_D(DDialog);

    DAbstractDialog::showEvent(event);

    setAttribute(Qt::WA_Resized, false);
    d->updateSize();

    Q_EMIT visibleChanged(isVisible());
}

void DDialog::hideEvent(QHideEvent *event)
{
    Q_EMIT aboutToClose();

    DAbstractDialog::hideEvent(event);

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

        QStyle *style = d->fixedStyle;
        if (!style) {
            style = QStyleFactory::create("dlight");
            if (style) {
                d->fixedStyle = style;
                style->setParent(this);
            }
        }


        QWidget *child = qobject_cast<QWidget*>(event->child());
        if (child) {
            if (style) {
                child->setStyle(style);
            }

            // TODO(hualet): apply the rule to all dwidgets.
            // Just tried with no luck, DPsswordWidget's style goes wrong,
            // no time to deal with this detail, leave it alone for now.
            if (child->inherits("Dtk::Widget::DLineEdit")) {
                DThemeManager *dtm = DThemeManager::instance();
                QString qss = dtm->getQssForWidget("DLineEdit", "light");

                child->setStyleSheet(qss);
                dtm->disconnect(child);
            }
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
        d->messageLabel->setFixedHeight(d->messageLabel->sizeHint().height());
    }
}

DWIDGET_END_NAMESPACE

#include "moc_ddialog.cpp"
