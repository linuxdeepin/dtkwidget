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
#include "dsearchcombobox.h"
#include "private/dsearchcombobox_p.h"

#ifndef emit
#define emit Q_EMIT
#include <private/qcombobox_p.h>
#endif

#include <QLayout>

DWIDGET_BEGIN_NAMESPACE

enum layoutMargin
{
    layoutMargin = 10
};

/*!
 * \~chinese \class DSearchComboBox
 * \~chinese \brief DSearchComboBox一个聚合 QComboBox 搜索控件
 * \~chinese \li DSearchComboBox提供了在下拉列表中搜索功能
 * \~chinese \li DSearchComboBox该控件适用于在大段列表中快速查找
 */
DSearchComboBox::DSearchComboBox(QWidget *parent)
    : DComboBox(parent)
    , DObject(*new DSearchComboBoxPrivate(this))
{
}

void DSearchComboBox::setEditable(bool editable)
{
    Q_D(DSearchComboBox);
    DComboBox::setEditable(editable);
    if (editable && !d->completer) {
        d->completer = new QCompleter;
        d->completer->setModel(model());
        lineEdit()->setCompleter(d->completer);
    }
}

void DSearchComboBox::showPopup()
{
    Q_D(DSearchComboBox);
    QComboBoxPrivate *dd = reinterpret_cast<QComboBoxPrivate *>(qGetPtrHelper(d_ptr));
    QComboBox::showPopup();

    if(auto layout = static_cast<QBoxLayout*>(dd->container->layout())) {
        if (!isEditable() && !d->searchEdit) {
            //因为dd->container容器在QComboBox::showPopup()
            //中才被new 所以布局等在此实现
            d->searchEdit = new DSearchEdit;
            d->searchEdit->setFixedHeight(d->searchEdit->sizeHint().height());

            QHBoxLayout *searlayout = new QHBoxLayout;
            searlayout->setContentsMargins(layoutMargin, layoutMargin, layoutMargin, 0);
            layout->setMargin(0);
            layout->setSpacing(0);
            searlayout->addWidget(d->searchEdit);
            layout->insertLayout(0, searlayout);

            d->proxyModel = new QSortFilterProxyModel;
            d->proxyModel->setSourceModel(model());

            //Qt源码中modle的父对象为this就会delete
            view()->model()->setParent(view());
            setModel(d->proxyModel);

            connect(d->searchEdit, &DSearchEdit::textChanged, this, [ = ] (const QString & text){
                d->proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive,
                                                              QRegExp::FixedString));
            });
        }
    }

    if (d->searchEdit &&!isEditable()) {
        //可编辑状态隐藏DSearchEdit
        if (d->searchEdit->isVisible() != isEditable())
            d->searchEdit->setVisible(!isEditable());
        d->height = qMax(dd->container->geometry().height() + layoutMargin, d->height);
        QRect containerRect = dd->container->geometry();
        containerRect.setHeight(d->height);
        dd->container->setGeometry(containerRect);
    }
}

DSearchComboBoxPrivate::DSearchComboBoxPrivate(DSearchComboBox *q)
    : DObjectPrivate(q)
    , searchEdit(nullptr)
    , completer(nullptr)
    , proxyModel(nullptr)
    , height(0)
{
}

DWIDGET_END_NAMESPACE
