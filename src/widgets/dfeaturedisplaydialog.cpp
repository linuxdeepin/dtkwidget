// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dfeaturedisplaydialog.h"
#include "private/dfeaturedisplaydialog_p.h"
#include "dcommandlinkbutton.h"

#include <DFontSizeManager>
#include <DGuiApplicationHelper>

#include <QApplication>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QUrl>
#include <QtMath>

DWIDGET_BEGIN_NAMESPACE

static constexpr int DefaultTextLineWidth = 410;
static constexpr int DefaultIconWidth = 48;
static constexpr int DefaultIconHeight = 48;

DFeatureItemWidget::DFeatureItemWidget(const QIcon &icon, const QString &name,
                                       const QString &description, QWidget *parent)
    : QWidget(parent)
    , m_iconLabel(new QLabel)
    , m_featureNameLabel(new QLabel(name))
    , m_featureDescriptionLabel(new QLabel(description))
{
    m_featureDescriptionLabel->setWordWrap(true);
    DFontSizeManager *fontManager =  DFontSizeManager::instance();
    fontManager->bind(m_featureNameLabel, DFontSizeManager::T5, QFont::DemiBold);
    fontManager->bind(m_featureDescriptionLabel, DFontSizeManager::T6, QFont::Medium);

    m_iconLabel->setFixedSize(DefaultIconWidth, DefaultIconHeight);
    m_iconLabel->setPixmap(icon.pixmap(DefaultIconWidth, DefaultIconHeight));
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(10, 10, 10, 10);
    hLayout->setSpacing(0);
    hLayout->addWidget(m_iconLabel);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(10, 10, 10, 10);
    vLayout->setSpacing(0);
    vLayout->addStretch(0);
    vLayout->addWidget(m_featureNameLabel, 0, Qt::AlignVCenter);
    vLayout->addWidget(m_featureDescriptionLabel, 0, Qt::AlignVCenter);
    vLayout->addStretch(0);
    QHBoxLayout *mLayout = new QHBoxLayout(this);
    mLayout->setContentsMargins(0, 0, 0, 0);
    mLayout->setSpacing(0);
    mLayout->addLayout(hLayout);
    mLayout->addSpacing(2);
    mLayout->addLayout(vLayout);

    setMinimumWidth(360);
    auto fontMetrics = m_featureDescriptionLabel->fontMetrics();
    auto size = fontMetrics.size(Qt::TextShowMnemonic, description);
    setMinimumHeight(size.width() <= DefaultTextLineWidth ? 66 : 86);
}

DFeatureItemWidget::~DFeatureItemWidget()
{
}

void DFeatureItemWidget::setDescriptionLabelWidth(const int width)
{
    m_featureDescriptionLabel->setFixedWidth(width);
}

int DFeatureItemWidget::descriptionLabelWidth()
{
    auto fontMetrics = m_featureDescriptionLabel->fontMetrics();
    auto size = fontMetrics.size(Qt::TextShowMnemonic, m_featureDescriptionLabel->text());
    return size.width();
}

DFeatureItemPrivate::DFeatureItemPrivate(Core::DObject *qq, const QIcon &icon,
                                         const QString &name, const QString &description)
    : Core::DObjectPrivate(qq)
    , m_icon(icon)
    , m_name(name)
    , m_description(description)
{
}

DFeatureItemPrivate::~DFeatureItemPrivate()
{
}

DFeatureItem::DFeatureItem(const QIcon &icon, const QString &name,
                           const QString &description, QObject *parent)
    : QObject(parent)
    , DObject(*new DFeatureItemPrivate(this, icon, name, description))
{
}

DFeatureItem::~DFeatureItem()
{
}

QIcon DFeatureItem::icon() const
{
    Q_D(const DFeatureItem);
    return d->m_icon;
}

void DFeatureItem::setIcon(const QIcon &icon)
{
    Q_D(DFeatureItem);
    d->m_icon = icon;
}

QString DFeatureItem::name() const
{
    Q_D(const DFeatureItem);
    return d->m_name;
}

void DFeatureItem::setName(const QString &name)
{
    Q_D(DFeatureItem);
    d->m_name = name;
}

QString DFeatureItem::description() const
{
    Q_D(const DFeatureItem);
    return d->m_description;
}

void DFeatureItem::setDescription(const QString &description)
{
    Q_D(DFeatureItem);
    d->m_description = description;
}

DFeatureDisplayDialogPrivate::DFeatureDisplayDialogPrivate(DFeatureDisplayDialog *qq)
    : DDialogPrivate(qq)
{
}

void DFeatureDisplayDialogPrivate::init()
{
    Q_Q(DFeatureDisplayDialog);
    q->setMinimumSize(660, 620);
    q->setMaximumHeight(720);
    q->setWindowFlags(q->windowFlags() | Qt::CustomizeWindowHint);
    q->addButton(QObject::tr("Continue"), true, DDialog::ButtonRecommend);
    q->getButton(0)->setFixedSize(256, 36);
    q->setModal(true);

    m_title = new QLabel;
    DFontSizeManager *fontManager =  DFontSizeManager::instance();
    fontManager->bind(m_title, DFontSizeManager::T2, QFont::DemiBold);

    QWidget *itemWidget = new QWidget;
    itemWidget->setMinimumSize(360, 66);
    itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_vBoxLayout = new QVBoxLayout(itemWidget);
    m_vBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_vBoxLayout->setSpacing(12);
    QScrollArea *scrollWidget = new QScrollArea;
    scrollWidget->setWidget(itemWidget);
    scrollWidget->setWidgetResizable(true);
    scrollWidget->setMaximumHeight(490);
    scrollWidget->setAutoFillBackground(false);
    scrollWidget->setFrameShape(QFrame::NoFrame);
    QPalette pt = scrollWidget->palette();
    pt.setBrush(QPalette::Window, Qt::transparent);
    scrollWidget->setPalette(pt);

    m_linkBtn = new DCommandLinkButton(QObject::tr("Learn More") + " >");
    m_linkBtn->setVisible(false);

    QWidget *contentWidget = new QWidget;
    QVBoxLayout *vContentLayout = new QVBoxLayout(contentWidget);
    vContentLayout->setContentsMargins(150, 0, 150, 0);
    vContentLayout->setSpacing(0);
    vContentLayout->addWidget(m_title, 0, Qt::AlignCenter);
    vContentLayout->addSpacing(30);
    vContentLayout->addWidget(scrollWidget);
    vContentLayout->addWidget(m_linkBtn);
    vContentLayout->addStretch(0);
    vContentLayout->setSizeConstraint(QLayout::SetFixedSize);

    q->insertContent(0, contentWidget, Qt::AlignTop | Qt::AlignHCenter);
}

void DFeatureDisplayDialogPrivate::addFeatureItem(const QIcon &icon, const QString &name, const QString &description)
{
    m_vBoxLayout->addWidget(new DFeatureItemWidget(icon, name, description));
}

int DFeatureDisplayDialogPrivate::getDescriptionMaxWidth()
{
    int maxWidth = 0;
    for (int i = 0; i < m_vBoxLayout->count(); ++i) {
        QWidget* widget = m_vBoxLayout->itemAt(i)->widget();
        if (widget == nullptr)
            continue;
        DFeatureItemWidget* w = qobject_cast<DFeatureItemWidget*>(widget);
        if (w == nullptr)
            continue;
        maxWidth = maxWidth > w->descriptionLabelWidth() ? maxWidth : w->descriptionLabelWidth();
    }
    return maxWidth;
}

void DFeatureDisplayDialogPrivate::updateItemWidth()
{
    int maxWidth = getDescriptionMaxWidth();
    if (maxWidth > DefaultTextLineWidth) {
        maxWidth = (maxWidth / 2 < DefaultTextLineWidth) ? DefaultTextLineWidth: qCeil(maxWidth / 2.0);
    }
    for (int i = 0; i < m_vBoxLayout->count(); ++i) {
        QWidget* widget = m_vBoxLayout->itemAt(i)->widget();
        if (widget == nullptr)
            continue;
        DFeatureItemWidget* w = qobject_cast<DFeatureItemWidget*>(widget);
        if (w == nullptr)
            continue;
        w->setDescriptionLabelWidth(maxWidth);
    }
}

void DFeatureDisplayDialogPrivate::createWidgetItems()
{
    clearLayout();
    for (auto item : m_featureItems) {
        if (item == nullptr)
            continue;
        addFeatureItem(item->icon(), item->name(), item->description());
    }
}

void DFeatureDisplayDialogPrivate::deleteItems()
{
    for (auto item : m_featureItems) {
        if (item)
            item->deleteLater();
    }
    m_featureItems.clear();
}

void DFeatureDisplayDialogPrivate::clearLayout()
{
    QLayoutItem *item;
    while((item = m_vBoxLayout->takeAt(0))) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void DFeatureDisplayDialogPrivate::_q_toggleLinkBtn()
{
    DGuiApplicationHelper::openUrl(m_linkUrl);
}

DFeatureDisplayDialog::DFeatureDisplayDialog(QWidget *parent)
    : DDialog(*new DFeatureDisplayDialogPrivate(this), parent)
{
    Q_D(DFeatureDisplayDialog);
    d->init();
}

DFeatureDisplayDialog::~DFeatureDisplayDialog()
{
    D_D(DFeatureDisplayDialog);
    d->deleteItems();
}

void DFeatureDisplayDialog::setTitle(const QString &title)
{
    Q_D(DFeatureDisplayDialog);
    d->m_title->setText(title);
}

void DFeatureDisplayDialog::addItem(DFeatureItem *item)
{
    Q_D(DFeatureDisplayDialog);
    d->m_featureItems.append(item);
}

void DFeatureDisplayDialog::removeItem(DFeatureItem *item)
{
    Q_D(DFeatureDisplayDialog);
    d->m_featureItems.removeOne(item);
}

void DFeatureDisplayDialog::addItems(QList<DFeatureItem *> items)
{
    Q_D(DFeatureDisplayDialog);
    for (auto item : items) {
        d->m_featureItems.append(item);
    }
}

void DFeatureDisplayDialog::clearItems()
{
    Q_D(DFeatureDisplayDialog);
    d->deleteItems();
}

void DFeatureDisplayDialog::setLinkButtonVisible(bool isVisible)
{
    Q_D(DFeatureDisplayDialog);
    d->m_linkBtn->setVisible(isVisible);
    if (isVisible) {
        connect(d->m_linkBtn, SIGNAL(clicked()), this, SLOT(_q_toggleLinkBtn()), Qt::UniqueConnection);
    }
}

void DFeatureDisplayDialog::setLinkUrl(const QString &url)
{
    Q_D(DFeatureDisplayDialog);
    d->m_linkUrl = url;
}

void DFeatureDisplayDialog::show()
{
    Q_D(DFeatureDisplayDialog);
    d->createWidgetItems();
    d->updateItemWidth();
    DDialog::show();
    if (QWidget* window = qApp->activeWindow()) {
        const auto point(window->mapToGlobal(window->rect().topLeft()));
        moveToCenterByRect(QRect(point.x(), point.y(), window->rect().width(), window->rect().height()));
    }
}

bool DFeatureDisplayDialog::isEmpty() const
{
    Q_D(const DFeatureDisplayDialog);
    return d->m_featureItems.isEmpty();
}

DWIDGET_END_NAMESPACE
#include "moc_dfeaturedisplaydialog.cpp"
