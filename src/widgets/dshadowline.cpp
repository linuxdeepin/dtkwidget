// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dshadowline.h"

#include <DGuiApplicationHelper>
#include <DObjectPrivate>

#include <QEvent>
#include <QIcon>
#include <QPainter>

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DShadowLinePrivate : public DCORE_NAMESPACE::DObjectPrivate
{
public:
    DShadowLinePrivate(DShadowLine *qq)
        : DObjectPrivate(qq)
    {

    }

    QPixmap shadow;
};

/*!
  \brief DShadowLine::DShadowLine 提供一个以titlebar_shadow为主题背景的线条形状的控件
  \a parent
 */
DShadowLine::DShadowLine(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DShadowLinePrivate(this))
{
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [this] {
        D_D(DShadowLine);
        d->shadow = QPixmap();
        update();
    });

    setAttribute(Qt::WA_TransparentForMouseEvents);
    setFocusPolicy(Qt::NoFocus);
}

QSize DShadowLine::sizeHint() const
{
    return QSize(20, 20);
}

void DShadowLine::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    D_D(DShadowLine);

    if (d->shadow.isNull()) {
        d->shadow = QIcon::fromTheme("titlebar_shadow").pixmap(sizeHint());
    }

    QPainter pa(this);
    pa.drawPixmap(contentsRect(), d->shadow);
}

DWIDGET_END_NAMESPACE
