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

#include <DSettings>
#include <DSettingsGroup>
#include <DSettingsOption>
#include <DSuggestButton>

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
    QVBoxLayout         *contentLayout;

    QMap<QString, QWidget *> titles;
    QList<QWidget *> sortTitles;

    DSettingsWidgetFactory       *widgetFactory;

    Content *q_ptr;
    Q_DECLARE_PUBLIC(Content)
};

Content::Content(QWidget *parent) :
    QFrame(parent), d_ptr(new ContentPrivate(this))
{
    Q_D(Content);

    setObjectName("RightFrame");
    setContentsMargins(0, 0, 0, 0);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    d->contentArea = new QScrollArea;
    d->contentArea->setContentsMargins(0, 0, 0, 0);
    d->contentArea->setWidgetResizable(true);
    d->contentArea->setStyleSheet("QScrollArea{border: none; background-color: transparent; }");
//    d->contentArea->setStyleSheet("QFrame{border: 1px solid red;}");

    d->contentFrame = new QWidget(this);
    d->contentFrame->setObjectName("SettingsContent");
    d->contentFrame->setStyleSheet("#SettingsContent{border: none; background-color: transparent; }");
    d->contentLayout = new QVBoxLayout(d->contentFrame);
    d->contentLayout->setContentsMargins(20, 0, 0, 0);
    d->contentLayout->setSpacing(0);
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
    for (auto groupKey : settings->groupKeys()) {
        auto group = settings->group(groupKey);
        if (group->isHidden()) {
            continue;
        }
        auto trName = translateContext.isEmpty() ? QObject::tr(group->name().toStdString().c_str())
                      : qApp->translate(translateContext.constData(), group->name().toStdString().c_str());
        auto title = new ContentTitle;
        title->setTitle(trName);
        title->setProperty("key", groupKey);
//        d->contentLayout->addSpacing(10);
        d->contentLayout->addWidget(title);
        d->contentLayout->addSpacing(8);
        d->sortTitles.push_back(title);
        d->titles.insert(groupKey, title);

        for (auto subgroup : settings->group(groupKey)->childGroups()) {
            if (subgroup->isHidden()) {
                continue;
            }

            if (!subgroup->name().isEmpty()) {
                auto title = new QLabel;
                title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                title->setObjectName("ContentSubTitleText");
                title->setFixedHeight(20);
                auto trName = translateContext.isEmpty() ? QObject::tr(subgroup->name().toStdString().c_str())
                              : qApp->translate(translateContext.constData(), subgroup->name().toStdString().c_str());
                title->setText(trName);
                title->setProperty("key", subgroup->key());
                title->setStyleSheet("#ContentSubTitleText{font-weight: 520; "
//                                     "border: 1px solid red; "
                                     "}");
                d->contentLayout->addWidget(title);
                d->contentLayout->addSpacing(10);
                d->sortTitles.push_back(title);
                d->titles.insert(subgroup->key(), title);
            }

            for (auto option : subgroup->childOptions()) {
                if (option->isHidden()) {
                    continue;
                }

                auto widget = d->widgetFactory->createWidget(translateContext, option);
                if (widget) {
                    d->contentLayout->addWidget(widget);
                    widget->setParent(d->contentFrame);
                }
            }

            d->contentLayout->addSpacing(4);
        }
    }

    auto resetBt = new DSuggestButton(QObject::tr("Restore Defaults"));
    resetBt->setObjectName("SettingsContentReset");
    resetBt->setFixedSize(310, 36);

    d->contentLayout->addSpacing(40);
    d->contentLayout->addWidget(resetBt, 0, Qt::AlignCenter);
    d->contentLayout->addSpacing(40);
    d->contentLayout->addStretch();

    connect(resetBt, &QPushButton::released,
    this, [ = ]() {
        settings->reset();
    });
}

DWIDGET_END_NAMESPACE
