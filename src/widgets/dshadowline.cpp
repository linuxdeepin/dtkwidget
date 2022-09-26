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
    D_D(DShadowLine);
    // 在非变色龙主题下，无法通过QIcon::fromtheme访问这个图标资源
    // 为了让软件在其他非变色龙的主题上正常显示，这里不使用图标引擎，而直接使用图标
    auto getPixmap = [=](DGuiApplicationHelper::ColorType themeType) {
        return themeType == DGuiApplicationHelper::LightType ?
                   QPixmap::fromImage(QImage(":/icons/deepin/builtin/light/texts/titlebar_shadow_20px.svg")) :
                   QPixmap::fromImage(QImage(":/icons/deepin/builtin/dark/texts/titlebar_shadow_20px.svg"));
    };
    d->shadow = getPixmap(DGuiApplicationHelper::instance()->themeType());
    connect(DGuiApplicationHelper::instance(),
            &DGuiApplicationHelper::themeTypeChanged,
            this,
            [=](DGuiApplicationHelper::ColorType themeType) {
                d->shadow = getPixmap(themeType);
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

    QPainter pa(this);
    pa.drawPixmap(contentsRect(), d->shadow);
}

DWIDGET_END_NAMESPACE
