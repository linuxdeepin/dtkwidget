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
#include "dpalette.h"
#include "dstyle.h"
#include "private/dsearchedit_p.h"

#include <QAction>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>

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

DSearchEditPrivate::DSearchEditPrivate(DSearchEdit *q)
    : DLineEditPrivate(q)
    , action(nullptr)
{
}

void DSearchEditPrivate::init()
{
    D_Q(DSearchEdit);

    label = new QLabel;
    placeHolder = DSearchEdit::tr("Search");

    action = new QAction;

    action->setIcon(DStyleHelper(q->style()).standardIcon(DStyle::SP_IndicatorSearch, nullptr));

    DPalette pe;
    QStyleOption opt;
    QColor color = DStyleHelper(q->style()).getColor(&opt, pe, DPalette::TextTips);
    pe.setColor(DPalette::TextTips, color);

    label->setPalette(pe);
    label->setText(placeHolder);

    q->setFocusPolicy(Qt::ClickFocus);

    q->connect(q, SIGNAL(focusChanged(bool)), q, SLOT(_q_toEditMode(bool)));

    QHBoxLayout *layout = new QHBoxLayout(q->lineEdit());

    layout->addWidget(label, 0, Qt::AlignCenter);
}

void DSearchEditPrivate::_q_toEditMode(bool focus)
{
    D_Q(DSearchEdit);

    if (focus) {
        q->lineEdit()->addAction(action, QLineEdit::LeadingPosition);
        label->setVisible(false);
    } else {
        q->lineEdit()->removeAction(action);

        if (!q->lineEdit()->text().isEmpty()) {
            label->setVisible(false);
        } else {
            label->setVisible(true);
        }
    }
}

DWIDGET_END_NAMESPACE

#include "moc_dsearchedit.cpp"
