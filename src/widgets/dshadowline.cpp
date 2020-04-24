/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
 * \~chinese \brief DShadowLine::DShadowLine 提供一个以titlebar_shadow为主题背景的线条形状的控件
 * \~chinese \param parent
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
