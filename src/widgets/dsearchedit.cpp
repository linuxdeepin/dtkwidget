/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include "dsearchedit.h"
#include "private/dsearchedit_p.h"

#include <QAction>
#include <QPainter>

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \class DSearchEdit
 * \~chinese \brief DSearchEdit 类提供了专门用来进行搜索的输入框控件。
 * \~chinese
 * \~chinese 相比于一般的输入框控件，DSearchEdit 提供了搜索按钮指示，用户使用起来会更加自然。
 * \~chinese 如图示：
 * \~chinese \image html searchedit.png
 * \~chinese
 * \~chinese \warning DSearchEdit 与 QLineEdit、DLineEdit 没有继承关系，功能不兼容。
 */

/*!
 * \~chinese \brief DSearchEdit::DSearchEdit 是 DSearchEdit 类的构造函数。
 * \~chinese \param parent 指定了控件的父控件。
 */
DSearchEdit::DSearchEdit(QWidget *parent)
    : DLineEdit(*new DSearchEditPrivate(this), parent)
{
    D_D(DSearchEdit);

    d->init();
}

DSearchEdit::~DSearchEdit()
{

}

void DSearchEdit::setPlaceHolder(QString placeHolder)
{
    Q_D(DSearchEdit);

    d->placeHolder = placeHolder;
}

QString DSearchEdit::placeHolder() const
{
    D_DC(DSearchEdit);

    return d->placeHolder;
}

void DSearchEdit::paintEvent(QPaintEvent *event)
{
    Q_D(DSearchEdit);

    DLineEdit::paintEvent(event);

    if (hasFocus())
        return;

    QPainter p(this);
    QPalette pal = palette();
    QColor col = pal.text().color();

    col.setAlpha(128);
    p.setPen(col);

    p.drawText(this->rect(), Qt::AlignCenter, d->placeHolder);
}

DSearchEditPrivate::DSearchEditPrivate(DSearchEdit *q)
    : DLineEditPrivate(q)
{
}

void DSearchEditPrivate::init()
{
    D_Q(DSearchEdit);

    placeHolder = DSearchEdit::tr("Search");

    q->setFocusPolicy(Qt::ClickFocus);

    q->connect(q, SIGNAL(focusChanged(bool)), q, SLOT(_q_toEditMode()));
}

void DSearchEditPrivate::_q_toEditMode()
{
    D_Q(DSearchEdit);

    if (q->hasFocus()) {
        action = new QAction(QIcon(":/images/light/images/search.svg"), QString());
        q->addAction(action, QLineEdit::LeadingPosition);
    } else {
        q->removeAction(action);
    }
}

DWIDGET_END_NAMESPACE

#include "moc_dsearchedit.cpp"
