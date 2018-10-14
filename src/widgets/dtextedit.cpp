/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dtextedit.h"
#include "dthememanager.h"
#include "private/dtextedit_p.h"
#include "dscrollbar.h"

#include <QResizeEvent>

DWIDGET_BEGIN_NAMESPACE

DTextEditPrivate::DTextEditPrivate(DTextEdit *qq) :
    DObjectPrivate(qq)
{

}

void DTextEditPrivate::init()
{
    D_Q(DTextEdit);

    insideFrame = new QFrame(q);
    insideFrame->setObjectName("TextEditInsideFrame");
    insideFrame->setFocusProxy(q);
    insideFrame->setAttribute(Qt::WA_TransparentForMouseEvents);

    q->setContextMenuPolicy(Qt::NoContextMenu);
    q->setVerticalScrollBar(new DScrollBar);
    q->setHorizontalScrollBar(new DScrollBar);
}

/*!
 * \~chinese \brief DTextEdit
 * \~chinese DTextEdit 提供 deepin 风格的多行编辑控件，是 QTextEdit 的子类。
 */

DTextEdit::DTextEdit(QWidget *parent) :
    QTextEdit(parent),
    DObject(*new DTextEditPrivate(this))
{
    DThemeManager::registerWidget(this);

    d_func()->init();
}

void DTextEdit::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);

    D_D(DTextEdit);

    const QSize &size = event->size();

    d->insideFrame->setFixedHeight(size.height() - 1);
    d->insideFrame->setFixedWidth(size.width());
}

DWIDGET_END_NAMESPACE
