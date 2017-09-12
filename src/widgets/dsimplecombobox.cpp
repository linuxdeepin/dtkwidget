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

#include "dsimplecombobox.h"
#include "private/dcombobox_p.h"

DWIDGET_BEGIN_NAMESPACE

class SimpleDelegateItem : public DComboBoxItem
{
    Q_OBJECT

public:
    explicit SimpleDelegateItem(QWidget *parent = 0);
};

class DComboBoxSimpleDelegate : public DAbstractComboBoxDelegate
{
public:
    explicit DComboBoxSimpleDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
};

SimpleDelegateItem::SimpleDelegateItem(QWidget *parent) :
    DComboBoxItem(parent)
{

}

DComboBoxSimpleDelegate::DComboBoxSimpleDelegate(QObject *parent) : DAbstractComboBoxDelegate(parent)
{

}

QWidget * DComboBoxSimpleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    SimpleDelegateItem * editor = new SimpleDelegateItem(parent);
    editor->setFixedHeight(MENU_ITEM_HEIGHT);

    return editor;
}

void DComboBoxSimpleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QJsonObject dataObj = index.model()->data(index, Qt::DisplayRole).toJsonValue().toObject();

    if (dataObj.isEmpty())
        return;

    SimpleDelegateItem *simpleItem = static_cast<SimpleDelegateItem *>(editor);
    QString title = "";

    if (simpleItem && dataObj.contains("itemText"))
        title = dataObj.value("itemText").toString();

    simpleItem->setText(title);

}


DSimpleComboBox::DSimpleComboBox(QWidget *parent) : DComboBox(parent)
{

    DComboBoxSimpleDelegate *dbfb = new DComboBoxSimpleDelegate(this);
    setItemDelegate(dbfb);

    setModel(new DComboBoxModel(this));

    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChange(int)));
}

void DSimpleComboBox::addItem(const QString &text)
{
    QJsonObject nameObj;
    nameObj.insert("itemText", QJsonValue(text));

    dcomboBoxModel()->append(nameObj);
    // Make the combo boxes always displayed.
    view()->openPersistentEditor(dcomboBoxModel()->getModelIndex(dcomboBoxModel()->count() - 1));
}

void DSimpleComboBox::addItems(const QStringList &texts)
{
    Q_FOREACH (QString text, texts) {
        this->addItem(text);
    }
}

void DSimpleComboBox::onCurrentIndexChange(int index)
{
    QJsonObject nameObj = dcomboBoxModel()->getJsonData(index);

    Q_EMIT currentTextChanged(nameObj["itemText"].toString());
}

DWIDGET_END_NAMESPACE

#include "dsimplecombobox.moc"
