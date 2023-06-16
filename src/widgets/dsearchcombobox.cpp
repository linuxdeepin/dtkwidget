// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dsearchcombobox.h"
#include "private/dsearchcombobox_p.h"

#ifndef emit
#define emit Q_EMIT
#include <private/qcombobox_p.h>
#undef emit
#else
#include <private/qcombobox_p.h>
#endif
#include <private/qcombobox_p.h>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QLayout>

DWIDGET_BEGIN_NAMESPACE

enum layoutMargin
{
    layoutMargin = 10
};

/*!
  \class Dtk::Widget::DSearchComboBox
  \inmodule dtkwidget
  \brief DSearchComboBox一个聚合 QComboBox 搜索控件.

  DSearchComboBox提供了在下拉列表中搜索功能
  DSearchComboBox该控件适用于在大段列表中快速查找
  \image html DSearchComboBox.png
 */
DSearchComboBox::DSearchComboBox(QWidget *parent)
    : DComboBox(*new DSearchComboBoxPrivate(this), parent)
{
}

void DSearchComboBox::setEditable(bool editable)
{
    Q_D(DSearchComboBox);
    DComboBox::setEditable(editable);
    if (editable && !d->completer) {
        d->completer = new QCompleter(lineEdit());
        d->completer->setModel(model());
        lineEdit()->setCompleter(d->completer);
    }
}

void DSearchComboBox::showPopup()
{
    Q_D(DSearchComboBox);
    QComboBoxPrivate *dd = reinterpret_cast<QComboBoxPrivate *>(qGetPtrHelper(d_ptr));
    DComboBox::showPopup();
    if(auto layout = static_cast<QBoxLayout*>(dd->container->layout())) {
        if (!isEditable() && !d->searchEdit) {
            //因为dd->container容器在QComboBox::showPopup()
            //中才被new 所以布局等在此实现
            d->searchEdit = new DSearchEdit;
            d->searchEdit->setFixedHeight(d->searchEdit->sizeHint().height());

            QHBoxLayout *searlayout = new QHBoxLayout;
            searlayout->setContentsMargins(layoutMargin, layoutMargin, layoutMargin, 0);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
            searlayout->addWidget(d->searchEdit);
            layout->insertLayout(0, searlayout);
            dd->container->setFixedHeight(dd->container->height() + d->searchEdit->height() + layout->spacing() + layout->contentsMargins().top() + layoutMargin);
            d->proxyModel = new QSortFilterProxyModel(this);
            d->proxyModel->setSourceModel(model());

            //Qt源码中modle的父对象为this就会delete
            view()->model()->setParent(view());
            setModel(d->proxyModel);

            connect(d->searchEdit, &DSearchEdit::textChanged, this, [ = ] (const QString & text){
                d->proxyModel->setFilterRegularExpression(QRegularExpression(QRegularExpression::escape(text),QRegularExpression::CaseInsensitiveOption));
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
    : DComboBoxPrivate (q)
    , searchEdit(nullptr)
    , completer(nullptr)
    , proxyModel(nullptr)
    , height(0)
{
}

DWIDGET_END_NAMESPACE
