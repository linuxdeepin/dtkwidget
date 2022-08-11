// SPDX-FileCopyrightText: 2016 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "contenttitle.h"

#include <QHBoxLayout>
#include <QLabel>

class ContentTitlePrivate
{
public:
    ContentTitlePrivate(ContentTitle *parent) : q_ptr(parent) {}

    QLabel          *titleLabel = nullptr;

    ContentTitle *q_ptr;
    Q_DECLARE_PUBLIC(ContentTitle)
};

ContentTitle::ContentTitle(QWidget *parent) :
    QWidget(parent), d_ptr(new ContentTitlePrivate(this))
{
    Q_D(ContentTitle);

    setObjectName("ContentTitle");

    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    d->titleLabel = new QLabel;
    d->titleLabel->setObjectName("ContentTitleText");
    layout->addWidget(d->titleLabel, 0, Qt::AlignLeft);

    auto lineLabel = new QLabel;
    lineLabel->setObjectName("ContentTitleLine");
    layout->addWidget(lineLabel);
    lineLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

ContentTitle::~ContentTitle()
{

}

void ContentTitle::setSpacing(int width)
{
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout *>(layout());
    hLayout->insertSpacing(0, width);
}

QLabel *ContentTitle::label() const
{
    Q_D(const ContentTitle);

    return d->titleLabel;
}

void ContentTitle::setTitle(const QString &title)
{
    Q_D(ContentTitle);
    d->titleLabel->setText(title);
}
