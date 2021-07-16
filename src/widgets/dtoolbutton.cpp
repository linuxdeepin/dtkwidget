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
    return QToolButton::sizeHint();
}

/*!
  \brief DToolButton::setAlignment 设置DToolButton的对齐方式
  \a flag Qt::AlignCenter居中对齐　Qt::AlignLeft左对齐　Qt::AlignRight右对齐
 */
void DToolButton::setAlignment(Qt::Alignment flag)
{
    this->setProperty("_d_dtk_toolButtonAlign", QVariant(flag));
}

/*!
  \brief DToolButton::alignment 返回DToolButton当前的对齐方式
  \return 如果setAlignment设置成功则返回当前对齐方式，否则返回Qt::AlignLeft
 */
Qt::Alignment DToolButton::alignment() const
{
    if(this->property("_d_dtk_toolButtonAlign").isValid())
        return static_cast<Qt::Alignment>(this->property("_d_dtk_toolButtonAlign").toInt());
    else
        return Qt::AlignLeft;
}

DWIDGET_END_NAMESPACE
