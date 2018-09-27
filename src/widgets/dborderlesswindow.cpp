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

#include "dborderlesswindow.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QMenu>
#include <DApplication>

#include "dtitlebar.h"
#include "private/dborderlesswidget_p.h"

DWIDGET_BEGIN_NAMESPACE

class DBorderlessWindowPrivate: public DBorderlessWidgetPrivate
{
    D_DECLARE_PUBLIC(DBorderlessWindow)

public:
    explicit DBorderlessWindowPrivate(DBorderlessWindow *q);
    void init();

    QMenu   *m_menu     = nullptr;
    QWidget *m_parent   = nullptr;
};

DBorderlessWindowPrivate::DBorderlessWindowPrivate(DBorderlessWindow *q): DBorderlessWidgetPrivate(q)
{

}

void DBorderlessWindowPrivate::init()
{
    D_Q(DBorderlessWindow);

    m_menu = new QMenu(q);
    q->setTitlebarMenu(m_menu);

    const DApplication *dapp = qobject_cast<DApplication*>(qApp);
    if (dapp) {
        q->setWindowTitle(dapp->productName());
    } else {
        q->setWindowTitle(qApp->applicationDisplayName());
    }
}

/*!
 * \~chinese @class DBorderlessWindow
 * \~chinese @brief DBorderlessWindow 是DBorderlessWidget的派生类，提供了简化的接口
 *
 * \~chinese @param parent
 */

DBorderlessWindow::DBorderlessWindow(QWidget *parent): DBorderlessWidget(*(new DBorderlessWindowPrivate(this)))
{
    D_D(DBorderlessWindow);

    setParent(parent);

    d->init();
}

/*!
 * \~chinese @brief 修改当前的parent
 *
 * \~chinese @param parent
 */
void DBorderlessWindow::setParent(QWidget *parent)
{
    D_D(DBorderlessWindow);
    d->m_parent = parent;
    QObject::setParent(parent);
}

/*!
 * \~chinese @brief 获取当前的父窗口对象
 *
 * \~chinese @return QWidget* 父窗口对象
 */
QWidget *DBorderlessWindow::parentWidget() const
{
    D_DC(DBorderlessWindow);
    return d->m_parent;
}

/*!
 * \~chinese @brief 设置内部的布局
 *
 * \~chinese @param l 布局对象
 */
void DBorderlessWindow::setContentLayout(QLayout *l)
{
    DBorderlessWidget::setLayout(l);
}

/*!
 * \~chinese @brief 设置内部控件
 *
 * \~chinese @param w
 */
void DBorderlessWindow::setContentWidget(QWidget *w)
{
    QHBoxLayout *l = new QHBoxLayout;
    l->setSpacing(0);
    l->setMargin(0);
    l->addWidget(w);
    l->setAlignment(w, Qt::AlignCenter);
    setContentLayout(l);
}

DWIDGET_END_NAMESPACE
