/*
 * Copyright (C) 2016 ~ 2017 Deepin Technology Co., Ltd.
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

#include "content.h"

#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QScrollBar>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QScroller>
#include <QMouseEvent>
#include <QFormLayout>

#include <DSettings>
#include <DSettingsGroup>
#include <DSuggestButton>
#include <DPushButton>
#include <DFontSizeManager>
#include <DBackgroundGroup>

#include "dsettingswidgetfactory.h"

#include "contenttitle.h"

DWIDGET_BEGIN_NAMESPACE

class ContentPrivate
{
public:
    ContentPrivate(Content *parent)
        : q_ptr(parent)
    {
        widgetFactory = new DSettingsWidgetFactory(parent);
    }

    QScrollArea *contentArea = nullptr;
    QWidget *contentFrame = nullptr;
    QVBoxLayout *contentLayout = nullptr;

    QMap<QString, QWidget *> titles = {};
    QList<QWidget *> sortTitles = {};

    DSettingsWidgetFactory *widgetFactory = nullptr;

    Content *q_ptr;
    Q_DECLARE_PUBLIC(Content)
};

Content::Content(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new ContentPrivate(this))
{
    Q_D(Content);

    setObjectName("RightFrame");

    auto layout = new QVBoxLayout(this);
    auto margins = layout->contentsMargins();
    margins.setRight(0);
    layout->setContentsMargins(margins);

    d->contentArea = new QScrollArea;
    d->contentArea->setContentsMargins(0, 0, 0, 0);
    d->contentArea->setWidgetResizable(true);
    d->contentArea->setFrameShape(QFrame::NoFrame);
    d->contentArea->setAccessibleName("ContentScrollArea");
    d->contentArea->viewport()->setAccessibleName("ContentScrollAreaViewPort");

    // 设置列表支持触屏滚动
    QScroller::grabGesture(d->contentArea->viewport());

    connect(QScroller::scroller(d->contentArea->viewport()), &QScroller::stateChanged, this, [d](QScroller::State newstate) {
        // fix bug-44587 防止滑动时(DKeyWidget)鼠标事件导致viewport位置发生偏移。
        bool isDragging = newstate == QScroller::Dragging;
        d->contentArea->viewport()->setAttribute(Qt::WA_TransparentForMouseEvents, isDragging);
    });
    d->contentFrame = new QWidget(this);
    d->contentFrame->setObjectName("SettingsContent");
    d->contentFrame->setAccessibleName("ContentSettingsFrame");
    d->contentLayout = new QVBoxLayout(d->contentFrame);
    d->contentLayout->setAlignment(Qt::AlignLeft);
    d->contentLayout->setContentsMargins(0, 0, 10, 0);
    layout->addWidget(d->contentArea);

    d->contentArea->setWidget(d->contentFrame);

    connect(d->contentArea->verticalScrollBar(), &QScrollBar::valueChanged,
    this, [ = ](int value) {
        Q_D(Content);

        // 当前显示的Title才参与滚动条的计算
        QList<QWidget *> visableSortTitles;
        for (auto idx = 0; idx < d->sortTitles.length(); ++idx) {
            auto title = d->sortTitles[idx];
            if (title->isVisible())
                visableSortTitles.push_back(title);
        }
        if (visableSortTitles.empty())
            return;

        auto currentTitle = visableSortTitles.first();
        auto viewHeight = d->contentArea->height();

        QList<QWidget *> visableTitleList;

        for (auto idx = 0; idx < visableSortTitles.length(); ++idx) {
            auto title = visableSortTitles[idx];
            if (title->y() <= value) {
                if (idx < visableSortTitles.length() - 1) {
                    auto nextTitle = visableSortTitles[idx + 1];
                    if (nextTitle->y() >= value) {
                        visableTitleList.push_back(title);
                    }
                }
            } else if (title->y() < (value + viewHeight)) {
                visableTitleList.push_back(title);
            }
        }

        if (!visableTitleList.isEmpty()) {
            auto lastTitle = visableSortTitles.last();
            if (value + viewHeight - 180 >= lastTitle->y()) {
                currentTitle = visableTitleList.last();
            } else {
                currentTitle = visableTitleList.first();
            }
        }

        if (value >= visableSortTitles.last()->y())
            currentTitle = visableSortTitles.last();
        if (value <= visableSortTitles.first()->y())
            currentTitle = visableSortTitles.first();

        if (currentTitle) {
            Q_EMIT scrollToGroup(currentTitle->property("key").toString());
        }
    });
}

Content::~Content()
{

}

DSettingsWidgetFactory *Content::widgetFactory() const
{
    Q_D(const Content);
    return d->widgetFactory;
}

bool Content::groupIsVisible(const QString &key) const
{
    Q_D(const Content);

    QWidget *title = d->titles.value(key);

    return title && title->isVisible();
}

void Content::setGroupVisible(const QString &key, bool visible)
{
    Q_D(Content);

    QSet<QString> keys = {key}; // 需要改变visible的key集合
    for (QObject *obj : d->contentFrame->children()) {
        auto parentKey = obj->property("_d_dtk_group_key").toString();
        auto currKey = obj->property("key").toString();

        // 若父组或者当前组为需要改变visible,则加入到keys集合中
        if (parentKey == key || currKey == key) {
            keys << currKey;
        }

        if (keys.contains(currKey)) {
            if (QWidget *w = qobject_cast<QWidget *>(obj)) {
                if (!visible || w->parentWidget()) // 无父控件时禁止其显示
                    w->setVisible(visible);
            }
        }
    }
}

void Content::onScrollToGroup(const QString &key)
{
    Q_D(Content);

    if (!d->titles.contains(key)) { return; }

    auto title = d->titles.value(key);

    this->blockSignals(true);
    d->contentArea->verticalScrollBar()->setValue(title->y());
    this->blockSignals(false);
//    qDebug() << "onScrollToGroup" << key;
}

void Content::updateSettings(const QByteArray &translateContext, QPointer<DTK_CORE_NAMESPACE::DSettings> settings)
{
    Q_D(Content);

    QString current_groupKey;
    QString current_subGroupKey;

    for (auto groupKey : settings->groupKeys()) {
        current_groupKey = groupKey;

        auto group = settings->group(groupKey);
        if (group->isHidden()) {
            continue;
        }
        auto trName = translateContext.isEmpty() ? QObject::tr(group->name().toStdString().c_str())
                      : qApp->translate(translateContext.constData(), group->name().toStdString().c_str());
        auto title = new ContentTitle;
        title->setTitle(trName);
        title->label()->setForegroundRole(QPalette::BrightText);
        title->setAccessibleName(QString("ContentTitleWidgetFor").append(current_groupKey));
        DFontSizeManager::instance()->bind(title, DFontSizeManager::T4, QFont::Medium);

        DWidget *widTile = new DWidget();
        widTile->setProperty("key", groupKey);
        widTile->setAccessibleName(QString("ContentWidTileFor").append(groupKey));
        QHBoxLayout *hLayTile = new QHBoxLayout(widTile);
        hLayTile->addWidget(title);
        d->contentLayout->addWidget(widTile);

        d->sortTitles.push_back(widTile);
        d->titles.insert(groupKey, widTile);
        d->contentLayout->setSpacing(10);

        for (auto subgroup : settings->group(groupKey)->childGroups()) {
            if (subgroup->isHidden()) {
                continue;
            }

            current_subGroupKey = subgroup->key();

            if (!subgroup->name().isEmpty()) {
                auto trName = translateContext.isEmpty() ? QObject::tr(subgroup->name().toStdString().c_str())
                              : qApp->translate(translateContext.constData(), subgroup->name().toStdString().c_str());
                auto title = new ContentTitle;
                title->setAccessibleName(QString("ContentTitleWidgetFor").append(current_subGroupKey));
                title->setTitle(trName);
                DFontSizeManager::instance()->bind(title, DFontSizeManager::T5, QFont::Medium);

                DWidget *wid = new DWidget();
                wid->setProperty("key", subgroup->key());
                wid->setProperty("_d_dtk_group_key", current_groupKey);
                wid->setAccessibleName(QString("ContentWidgetFor").append(current_subGroupKey));
                QHBoxLayout *hLay = new QHBoxLayout(wid);
                hLay->setContentsMargins(10, 0, 0, 0);
                hLay->addWidget(title);

                d->contentLayout->addWidget(wid);

                d->sortTitles.push_back(wid);
                d->titles.insert(subgroup->key(), wid);
            }

            QVBoxLayout *bgGpLayout = new QVBoxLayout;
            DBackgroundGroup *bgGroup = new DBackgroundGroup(bgGpLayout);
            bgGroup->setProperty("key", subgroup->key());
            bgGroup->setProperty("_d_dtk_group_key", current_groupKey);
            bgGroup->setItemSpacing(1);
            bgGroup->setItemMargins(QMargins(0, 0, 0, 0));
            bgGroup->setBackgroundRole(QPalette::Window);
            d->contentLayout->addWidget(bgGroup);

            for (auto option : subgroup->childOptions()) {
                if (option->isHidden()) {
                    continue;
                }

                QWidget *wrapperWidget = new QWidget();
                QHBoxLayout *hLay = new QHBoxLayout(wrapperWidget);
                hLay->setContentsMargins(10, 6, 10, 6);
                auto widget = d->widgetFactory->createItem(translateContext, option);

                // 先尝试创建item
                if (widget.first || widget.second) {
                    if (QLabel *label = qobject_cast<QLabel *>(widget.first)) {
                        if (widget.second)
                            label->setBuddy(widget.second);
                    }

                    if (widget.first) {
                        QWidget *container = new QWidget;
                        QHBoxLayout *hLayout = new QHBoxLayout(container);
                        hLayout->setSpacing(0);
                        hLayout->setMargin(0);
                        hLayout->addWidget(widget.first);
                        hLayout->addStretch(1);
                        hLay->addWidget(container, 2);
                    }
                    if (widget.second) {
                        hLay->addWidget(widget.second, 3);
                    }
                    wrapperWidget->setAccessibleName(QString("CustomWidgetAtContentRow%1BackgroundRow%2").arg(d->contentLayout->count()).arg(bgGpLayout->count()));

                    if (widget.first) {
                        widget.first->setProperty("_d_dtk_group_key", current_subGroupKey);
                    }

                    if (widget.second) {
                        widget.second->setProperty("_d_dtk_group_key", current_subGroupKey);
                    }
                } else {
                    QWidget *widget = d->widgetFactory->createWidget(translateContext, option);

                    if (widget) {
                        widget->setProperty("_d_dtk_group_key", current_subGroupKey);
                        hLay->addWidget(widget);
                        wrapperWidget->setAccessibleName(QString("DefaultWidgetAtContentRow%1BackgroundRow%2").arg(d->contentLayout->count()).arg(bgGpLayout->count()));
                    }
                }
                bgGpLayout->addWidget(wrapperWidget);
            }
        }
        QSpacerItem *spaceItem = new QSpacerItem(0, 20,QSizePolicy::Minimum,QSizePolicy::Expanding);
        d->contentLayout->addItem(spaceItem);
    }

    QWidget *box = new QWidget();
    QHBoxLayout *box_layout = new QHBoxLayout(box);
    box_layout->setContentsMargins(0, 30, 0, 0);
    auto resetBt = new DPushButton(QObject::tr("Restore Defaults"), box);
    resetBt->setObjectName("SettingsContentReset");
    resetBt->setMaximumWidth(300);
    resetBt->setAutoDefault(false);
    resetBt->setAccessibleName("ContentSettingsResetButton");
    box_layout->addWidget(resetBt);
    box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    box->setAccessibleName("ContentBottomWidget");

    d->contentLayout->addWidget(box);

    connect(resetBt, &QPushButton::released,
    this, [ = ]() {
        settings->reset();
    });
}

void Content::mouseMoveEvent(QMouseEvent *event)
{
    // 事件来源为 MouseEventSynthesizedByQt 时认为此事件为Touch事件合成而来
    // 由于支持触屏下对视图的滚动，所以此处接收没有被处理的move事件，防止事件泄露
    // 到主窗口后触发窗口移动动作
    if (event->source() == Qt::MouseEventSynthesizedByQt) {
        event->accept();

        return;
    }
}

void Content::resizeEvent(QResizeEvent *event)
{
    Q_D(Content);
    d->contentFrame->setMaximumWidth(d->contentArea->width());

    return QWidget::resizeEvent(event);
}

DWIDGET_END_NAMESPACE
