/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     wangpeng <wangpeng@uniontech.com>
*
* Maintainer: wangpeng <wangpeng@uniontech.com>
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
#include "dtoolbutton.h"

#include <QStyleOptionButton>
#include <QStylePainter>

DWIDGET_BEGIN_NAMESPACE

DToolButton::DToolButton(QWidget *parent)
    : QToolButton(parent)
{

}

void DToolButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStylePainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    p.drawComplexControl(QStyle::CC_ToolButton, opt);
}

void DToolButton::initStyleOption(QStyleOptionToolButton *option) const
{
    QToolButton::initStyleOption(option);
    //判断条件不用Qt::ToolButtonTextBesideIcon原因
    //会强制居中，大小不受sizeHint（）控制
    if (!option->icon.isNull() && !option->text.isEmpty()) {
        option->toolButtonStyle = Qt::ToolButtonTextBesideIcon;
    }
}

QSize DToolButton::sizeHint() const
{
    QSize size = QToolButton::sizeHint();
    //计算图标文字同时存在时的宽度
    if (!icon().isNull() && !text().isEmpty()) {
        QFontMetrics fm = fontMetrics();
        int textWidth = fm.size(Qt::TextShowMnemonic, text()).width();
        size.setWidth(size.width() + textWidth);
    }
    return size;
}

DWIDGET_END_NAMESPACE
