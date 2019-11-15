/*
 * Copyright (C) 2016 ~ 2017 Deepin Technology Co., Ltd.
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
#include <DSettingsOption>
#include <DSuggestButton>
#include <DFontSizeManager>

#include "dsettingswidgetfactory.h"

#include "contenttitle.h"

DWIDGET_BEGIN_NAMESPACE

class ContentPrivate
{
public:
    ContentPrivate(Content *parent) : q_ptr(parent)
    {
        widgetFactory = new DSettingsWidgetFactory;
    }


    QScrollArea         *contentArea;
    QWidget             *contentFrame;
    QFormLayout         *contentLayout;

    QMap<QString, QWidget *> titles;
    QList<QWidget *> sortTitles;

    DSettingsWidgetFactory       *widgetFactory;

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

    d->contentArea = new QScrollArea;
    d->contentArea->setContentsMargins(0, 0, 0, 0);
    d->contentArea->setWidgetResizable(true);
    d->contentArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    d->contentArea->setFrameShape(QFrame::NoFrame);

    // 设置列表支持触屏滚动
    QScroller::grabGesture(d->contentArea);

    d->contentFrame = new QWidget(this);
    d->contentFrame->setObjectName("SettingsContent");
    d->contentLayout = new QFormLayout(d->contentFrame);
    d->contentLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    d->contentLayout->setLabelAlignment(Qt::AlignLeft);
    d->contentLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    layout->addWidget(d->contentArea);

    d->contentArea->setWidget(d->contentFrame);

    connect(d->contentArea->verticalScrollBar(), &QScrollBar::valueChanged,
    this, [ = ](int value) {
        Q_D(Content);
        auto viewHeight = d->contentArea->height();
        auto currentTitle = d->sortTitles.first();

        QList<QWidget *> visableTitleList;

        for (auto idx = 0; idx < d->sortTitles.length(); ++idx) {
            auto title = d->sortTitles[idx];
            if (title->y() <= value) {
                if (idx < d->sortTitles.length() - 1) {
                    auto nextTitle = d->sortTitles[idx + 1];
                    if (nextTitle->y() >= value) {
                        visableTitleList.push_back(title);
                    }
                }
            } else if (title->y() < (value + viewHeight)) {
                visableTitleList.push_back(title);
            }
        }

        if (!visableTitleList.isEmpty()) {
            auto lastTitle = d->sortTitles.last();
            if (value + viewHeight + 180 >= lastTitle->y()) {
                currentTitle = visableTitleList.last();
            } else {
                currentTitle = visableTitleList.first();
            }
        }

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

    if (!d->titles.contains(key))
        return;

    auto title = d->titles.value(key);
    title->setVisible(visible);

    for (QObject *obj : d->contentFrame->children()) {
        if (obj->property("_d_dtk_group_key").toString() == key) {
            if (ContentTitle *title = qobject_cast<ContentTitle*>(obj)) {
                const QString &key = title->property("key").toString();

                if (d->titles.contains(key)) {
                    setGroupVisible(key, visible);
                    continue;
                }
            }

            if (QWidget *w = qobject_cast<QWidget*>(obj)) {
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
        title->setProperty("key", groupKey);
        title->label()->setForegroundRole(QPalette::BrightText);

        DFontSizeManager::instance()->bind(title, DFontSizeManager::T4, QFont::Medium);
        d->contentLayout->setWidget(d->contentLayout->rowCount(), QFormLayout::LabelRole, title);
        d->sortTitles.push_back(title);
        d->titles.insert(groupKey, title);

        for (auto subgroup : settings->group(groupKey)->childGroups()) {
            if (subgroup->isHidden()) {
                continue;
            }

            current_subGroupKey = subgroup->key();

            if (!subgroup->name().isEmpty()) {
                auto trName = translateContext.isEmpty() ? QObject::tr(subgroup->name().toStdString().c_str())
                              : qApp->translate(translateContext.constData(), subgroup->name().toStdString().c_str());
                auto title = new ContentTitle;

                title->setTitle(trName);
                DFontSizeManager::instance()->bind(title, DFontSizeManager::T5, QFont::Medium);
                title->setProperty("key", subgroup->key());
                title->setProperty("_d_dtk_group_key", current_groupKey);

                d->contentLayout->setWidget(d->contentLayout->rowCount(), QFormLayout::LabelRole, title);
                d->sortTitles.push_back(title);
                d->titles.insert(subgroup->key(), title);
            }

            for (auto option : subgroup->childOptions()) {
                if (option->isHidden()) {
                    continue;
                }

                auto widget = d->widgetFactory->createItem(translateContext, option);

                // 先尝试创建item
                if (widget.first || widget.second) {
                    if (QLabel *label = qobject_cast<QLabel*>(widget.first)) {
                        if (widget.second)
                            label->setBuddy(widget.second);
                    }

                    d->contentLayout->addRow(widget.first, widget.second);

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
                        d->contentLayout->setWidget(d->contentLayout->rowCount(), QFormLayout::SpanningRole, widget);
                    }
                }
            }
        }
    }

    QWidget *box = new QWidget();
    QHBoxLayout *box_layout = new QHBoxLayout(box);
    box_layout->setContentsMargins(0, 30, 0, 0);
    auto resetBt = new DSuggestButton(QObject::tr("Restore Defaults"), box);
    resetBt->setObjectName("SettingsContentReset");
    resetBt->setMaximumWidth(300);
    box_layout->addWidget(resetBt);
    box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    d->contentLayout->setWidget(d->contentLayout->rowCount(), QFormLayout::SpanningRole, box);

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
